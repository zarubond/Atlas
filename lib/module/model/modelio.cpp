/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2015  Ondřej Záruba
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "modelio.h"

ModelIO::ModelIO()
{
}

void ModelIO::setModel(WorldGraphics *model)
{
    this->map=&model->map;
    this->base=&model->model;
}

bool ModelIO::load(const QDir &dir, const QDomElement & root)
{
    QDomElement el;
    QDomNode child=root.firstChild();
    part_dir=dir;
    part_dir.cd("model");
    std::map<int,ModelMesh*> mesh_map;

    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="file" && !el.text().isEmpty())
        {
            model_file=dir.absoluteFilePath(el.text());

            File file(model_file.toUtf8().constData());
            if(!file.open(File::READ))
            {
                cerr<<"Unable to load models from file: "<<model_file.toUtf8().constData()<<endl;
            }
            else
            {
                ModelHeader model;

                while(file.read((void*)&model,sizeof(model),1) >0)
                {
                    std::map<int,ModelMesh*>::iterator it=mesh_map.find(model.mesh_id);
                    if(it!=mesh_map.end())
                    {
                        Model * new_model=new Model(base);
                        new_model->setMesh(it->second);
                        new_model->setName(model.name);
                        new_model->setScale(Vector3f(model.scale[0],model.scale[1],model.scale[2]));
                        new_model->setPosition(Vector3f(model.position[0],model.position[1],model.position[2]));
                        new_model->setRotation(Quaternion(model.rotation[0],model.rotation[1],model.rotation[2],model.rotation[3]));

                        if(map->insertObject(new_model)==false)
                        {
                            delete new_model;
                            cout<<"Unable to fit model to map"<<endl;
                        }
                    }
                }

                file.close();
            }
        }
        else if(el.tagName()=="meshes")
        {
            QDomNodeList meshes=el.elementsByTagName("mesh");
            for(int j=0;j<meshes.count();j++)
            {
                el = meshes.at(j).toElement();
                int id=el.attribute("id").toInt();

                ModelMesh * mesh= new ModelMesh;
                mesh->setName(el.attribute("name").toUtf8().constData());

                std::string file=dir.path().toUtf8().constData(), file_name=el.text().toUtf8().constData();
                if(file[file.size()-1]=='/')
                    file.erase(file.size()-1);
                if(file_name[0]!='/')
                    file+="/";

                file+=file_name;
                if(mesh->load(file.c_str()))
                {
                    base->addMesh(mesh);
                    mesh_map.insert(std::pair<int,ModelMesh*>(id,mesh));
                }
                else
                    delete mesh;
            }
        }
        child=child.nextSibling();
    }
    part_dir.cdUp();

    return true;
}

bool ModelIO::save(QXmlStreamWriter &xml)
{
    QFileInfo info(model_file);
    if(!info.exists())
    {
        part_dir.mkpath(info.absolutePath());
    }

    xml.writeStartElement("model");

    FILE * file = fopen(model_file.toUtf8().constData(),"wb");
    if(file!=NULL)
    {
        xml.writeStartElement("meshes");
        const std::set<ModelMesh *> & meshes=base->getMeshes();
        int mesh_id=0;
        for(std::set<ModelMesh *>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
        {
            ModelMesh* mesh=(*it);

            xml.writeStartElement("mesh");
            xml.writeAttribute("name",mesh->getName().c_str());
            xml.writeAttribute("id",QString::number(mesh_id));
            xml.writeCharacters(part_dir.relativeFilePath(mesh->getFile()));
            xml.writeEndElement();

            for(std::set<Model *>::const_iterator mod = mesh->getModels().begin(); mod != mesh->getModels().end(); ++mod)
            {
                Model* model=(*mod);
                ModelHeader model_header;

                model_header.mesh_id=mesh_id;
                strcpy(model_header.name,model->getName());

                const Vector3f &pos=model->position();
                model_header.position[0]=pos[0];
                model_header.position[1]=pos[1];
                model_header.position[2]=pos[2];

                const Vector3f &scale=model->scale();
                model_header.scale[0]=scale[0];
                model_header.scale[1]=scale[1];
                model_header.scale[2]=scale[2];

                const Quaternion &rot=model->rotation();
                model_header.rotation[0]=rot[0];
                model_header.rotation[1]=rot[1];
                model_header.rotation[2]=rot[2];
                model_header.rotation[3]=rot[3];

                fwrite(&model_header,sizeof(ModelHeader),1,file);
            }
            mesh_id++;
        }

        fclose(file);

        xml.writeEndElement();
    }
    else
        cerr<<"Unable to open model file for save: "<<model_file<<endl;

    xml.writeTextElement("file",part_dir.relativeFilePath(model_file));
    xml.writeEndElement();
    return true;
}

bool ModelIO::create(const QDir &dir, WorldGraphics *base)
{
    part_dir=dir;
    this->base=&base->model;

    model_file=part_dir.absoluteFilePath("model/model.bin").toUtf8().constData();
    return true;
}

ModelMesh * ModelIO::importMesh(const std::string &file_name)
{
    QFileInfo file_info(file_name.c_str());
    QFileInfo new_info;

    std::vector<std::string> list=ModelMesh::meshFiles(file_name.c_str());
    if(list.size()>0)
    {
        QDir new_dir(part_dir);
        new_dir.cd("model");

        QDir orig_dir(file_info.absoluteDir());
        QDir file_dir;


        if(new_dir.mkdir(file_info.baseName()) && new_dir.cd(file_info.baseName()))
        {
            for(std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
            {
                file_info.setFile((*it).c_str());
                file_dir=new_dir;
                file_dir.cd(orig_dir.relativeFilePath(file_info.absolutePath()));

                file_dir.mkpath(file_dir.path());
                new_info.setFile(file_dir.absoluteFilePath(file_info.fileName()));
                if(!new_info.exists())
                {
                    if(!QFile::copy((*it).c_str(),file_dir.absoluteFilePath(file_info.fileName())))
                        cerr<<"Unable to copy mesh file from:"<<(*it).c_str()<<" to:"<<file_dir.absoluteFilePath(file_info.fileName())<<endl;
                }
            }

            file_info.setFile(file_name.c_str());
            new_dir.cdUp();
            QString fname=file_info.baseName()+QDir::separator()+file_info.fileName();

            ModelMesh * mesh= new ModelMesh;
            mesh->setName(file_info.baseName().toUtf8().constData());

            std::string file=new_dir.path().toStdString(), file_name=fname.toStdString();
            if(file[file.size()-1]=='/')
                file.erase(file.size()-1);
            if(file_name[0]!='/')
                file+="/";

            file+=file_name;
            if(mesh->load(file.c_str()))
            {
                base->addMesh(mesh);
                return mesh;
            }
            else
            {
                new_dir.cd(file_info.baseName());
                removeDir(new_dir);
                delete mesh;
            }
        }
    }
    return NULL;
}

void ModelIO::removeMesh(ModelMesh *mesh)
{
    for(std::set<Model *>::const_iterator mod = mesh->getModels().begin(); mod != mesh->getModels().end(); ++mod)
    {
        Model* model=(*mod);
        map->removeObject(model);
        delete model;
    }
    base->removeMesh(mesh);
    QFileInfo info(mesh->getFile());
    removeDir(info.dir().absolutePath());
    delete mesh;
}
