/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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

ModelIO::ModelIO(ModelGraphics *base):base(*base)
{
}

bool ModelIO::load(const QDir &dir, const QDomElement & root, Map *map)
{
    this->map=map;
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

            FILE *file = fopen(model_file.toUtf8().constData(), "rb");
            if(file==NULL)
            {
                std::cerr<<"Unable to load models from file: "<<model_file.toUtf8().constData()<<std::endl;
            }
            else
            {
                ModelHeader model;

                while(fread((void*)&model,sizeof(model),1,file) >0)
                {
                    std::map<int,ModelMesh*>::iterator it=mesh_map.find(model.mesh_id);
                    if(it!=mesh_map.end())
                    {
                        Model * new_model=new Model(&base);
                        new_model->setMesh(it->second);
                        new_model->setName(model.name);
                        new_model->setScale(Vertex3f(model.scale[0],model.scale[1],model.scale[2]));
                        new_model->setPosition(Vertex3f(model.position[0],model.position[1],model.position[2]));
                        new_model->setRotation(Quaternion(model.rotation[0],model.rotation[1],model.rotation[2],model.rotation[3]));

                        if(map->insertObject(new_model)==false)
                        {
                            delete new_model;
                            std::cout<<"Unable to fit model to map"<<std::endl;
                        }
                    }
                }

                fclose(file);
            }
        }
        else if(el.tagName()=="meshes")
        {
            QDomNodeList meshes=el.elementsByTagName("mesh");
            for(int j=0;j<meshes.count();j++)
            {
                el = meshes.at(j).toElement();
                int id=el.attribute("id").toInt();
                ModelMesh * mesh=base.newMesh(dir.path().toUtf8().constData(),el.text().toUtf8().constData());
                if(mesh!=NULL)
                {
                    mesh_map.insert(std::pair<int,ModelMesh*>(id,mesh));
                    mesh->setName(el.attribute("name").toStdString());
                }
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
        const std::set<ModelMesh *> & meshes=base.getMeshes();
        int mesh_id=0;
        for(std::set<ModelMesh *>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
        {
            ModelMesh* mesh=(*it);

            xml.writeStartElement("mesh");
            xml.writeAttribute("name",mesh->getName());
            xml.writeAttribute("id",QString::number(mesh_id));
            xml.writeCharacters(part_dir.relativeFilePath(mesh->getFile()));
            xml.writeEndElement();

            for(std::set<Model *>::const_iterator mod = mesh->getModels().begin(); mod != mesh->getModels().end(); ++mod)
            {
                Model* model=(*mod);
                ModelHeader model_header;

                model_header.mesh_id=mesh_id;
                strcpy(model_header.name,model->getName());

                const Vertex3f &pos=model->getPosition();
                model_header.position[0]=pos[0];
                model_header.position[1]=pos[1];
                model_header.position[2]=pos[2];

                const Vertex3f &scale=model->getScale();
                model_header.scale[0]=scale[0];
                model_header.scale[1]=scale[1];
                model_header.scale[2]=scale[2];

                const Quaternion &rot=model->getRotation();
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
        qDebug()<<"Unable to open model file for save: "<<model_file;

    xml.writeTextElement("file",part_dir.relativeFilePath(model_file));
    xml.writeEndElement();
    return true;
}

bool ModelIO::create(const QDir &dir, Map *map)
{
    part_dir=dir;

    model_file=part_dir.absoluteFilePath("model/model.bin").toUtf8().constData();
    return true;
}

ModelMesh * ModelIO::importMesh(const QString &file_name)
{
    QFileInfo file_info(file_name);
    QFileInfo new_info;

    std::vector<std::string> list=ModelMesh::meshFiles(file_name.toUtf8().constData());
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
                        qDebug()<<"Unable to copy mesh file from:"<<(*it).c_str()<<" to:"<<file_dir.absoluteFilePath(file_info.fileName());
                }
            }

            file_info=file_name;
            new_dir.cdUp();
            QString fname=file_info.baseName()+QDir::separator()+file_info.fileName();

            ModelMesh * new_mesh=base.newMesh(new_dir.absolutePath().toUtf8().constData()
                                                       ,fname.toUtf8().constData());
            if(new_mesh!=NULL)
            {
                new_mesh->setName(file_info.baseName().toUtf8().constData());
                return new_mesh;
            }
            else
            {
                new_dir.cd(file_info.baseName());
                removeDir(new_dir);
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
    base.removeMesh(mesh);
    QFileInfo info(mesh->getFile());
    removeDir(info.dir().absolutePath());
    delete mesh;
}
