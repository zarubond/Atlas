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

#include "terrainio.h"

TerrainIO::TerrainIO():base(NULL)
{
}

void TerrainIO::setModel(WorldGraphics *model)
{
    this->map = &model->map;
    this->base = &model->terrain;
}

bool TerrainIO::load(const QDir &dir, const QDomElement &root)
{
    part_dir=dir;
    ElapsedCPU timer;
    timer.start();

    QDomElement el;
    base->setMap(map);
    base->build(ceil(map->width()/map->nodeSize()),
                               ceil(map->height()/map->nodeSize()),
                               ceil(map->depth()/map->nodeSize()),map->nodeSize());

    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="materials")
        {
            sendProgress("Loading terrain materials...",40);
            loadMaterials(el.toElement());
        }
        else if(el.tagName()=="voxelmap" && !el.text().isEmpty())
        {
            timer.restart();
            if(!el.text().isEmpty())
            {
                loadVoxel(base,part_dir.filePath(el.text()).toUtf8().constData());
            }
            cerr<<"Loading time: "<<timer.elapsed()<<"ms"<<endl;
        }
        child = child.nextSibling();
    }

    sendProgress("Building terrain...",40);

    timer.restart();
    base->rebuid();

    cerr<<"Voxel build time: "<<timer.elapsed()<<"ms"<<endl;

    return true;
}

bool TerrainIO::load(const QDir &dir, const QDomElement &root, Map *map, TerrainBase * base)
{
    part_dir=dir;
    QElapsedTimer timer;
    timer.start();

    QDomElement el;
    base->setMap(map);
    base->build(ceil(map->width()/map->nodeSize()),
                               ceil(map->height()/map->nodeSize()),
                               ceil(map->depth()/map->nodeSize()),map->nodeSize());

    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="voxelmap" && !el.text().isEmpty())
        {
            timer.restart();
            if(!el.text().isEmpty())
                loadVoxel(base,part_dir.filePath(el.text()).toUtf8().constData());
            cerr<<"Loading time: "<<timer.elapsed()<<"ms"<<FILEINFO<<endl;
        }
        child = child.nextSibling();
    }    

    timer.restart();

    return true;
}

bool TerrainIO::save(QXmlStreamWriter &xml)
{
    xml.writeStartElement("terrain");

    xml.writeStartElement("materials");
    for(int i=0; i<base->maxMaterials(); i++)
    {
        TerrainMaterial * tmp=base->getMaterial(i);
        if(tmp!=NULL && tmp->enabled())
        {
            xml.writeStartElement("material");
            xml.writeAttribute("id",QString::number(tmp->id()));

            xml.writeTextElement("name",tmp->name.c_str());
            xml.writeTextElement("texture",part_dir.relativeFilePath(tmp->texture_path.c_str()));
            xml.writeTextElement("normalmap",part_dir.relativeFilePath(tmp->normal_map_path.c_str()));

            xml.writeEndElement();
        }
    }
    xml.writeEndElement();

    xml.writeTextElement("voxelmap",part_dir.relativeFilePath(voxel_file.c_str()));
    xml.writeEndElement();

    saveVoxel(part_dir.absoluteFilePath(voxel_file.c_str()).toUtf8().constData());

    return true;
}

bool TerrainIO::create(const QDir &dir, WorldGraphics *world)
{
    part_dir=dir;
    voxel_file=part_dir.absoluteFilePath("terrain/terrain.bin").toUtf8().constData();
    base=&world->terrain;
    base->setMap(&world->map);
    map=&world->map;
    base->build(ceil(map->width()/map->nodeSize()),
                        ceil(map->height()/map->nodeSize()),
                        ceil(map->depth()/map->nodeSize()),map->nodeSize());

    return true;
}

bool TerrainIO::insert(Map *map)
{
    for(int i=0; i< base->regionsX();i++)
        for(int j=0; j< base->regionsY();j++)
            for(int k=0; k< base->regionsZ();k++)
            {
                map->insertObject(base->regionAtIndex(i,j,k));
            }
    return true;
}

bool TerrainIO::importTexture(unsigned short id, const std::string &path, const std::string & name)
{
    QFileInfo info(path.c_str());
    QDir t_dir=part_dir;
    t_dir.cd("terrain");
    QString file_name="texture";
    file_name+=QString::number(id);
    file_name+=".png";
    std::string new_path=t_dir.absoluteFilePath(file_name).toStdString();
    Image img(path);

    if(info.exists() && img.save(new_path,"PNG"))
    {
        TerrainMaterial * tmp=base->getMaterial(id);
        if(tmp!=NULL && tmp->loadTexture(new_path.c_str()))
        {
            if(tmp->name.empty())
                tmp->name=info.baseName().toUtf8().constData();
            if(!name.empty())
                tmp->name=name;

            tmp->texture_path=new_path.c_str();
            base->textures()->generateMipmap();
            return true;
        }
        else
            cerr<<"Unable to load texture"<<FILEINFO<<endl;
    }
    else
        cerr<<"Unable to save image"<<new_path<<FILEINFO<<endl;

    return false;
}

bool TerrainIO::importNormalMap(unsigned short id, const std::string &path)
{
    QFileInfo info(path.c_str());
    QDir t_dir=part_dir;
    t_dir.cd("terrain");
    QString name="texture";
    name+=QString::number(id);
    name+="-normal.png";
    QString new_path=t_dir.absoluteFilePath(name);
    Image img(path);

    if(info.exists() && img.save(new_path.toStdString(),"PNG"))
    {
        TerrainMaterial * tmp=base->getMaterial(id);
        if(tmp!=NULL &&  tmp->loadNormalMap(new_path.toUtf8().constData()))
        {
            tmp->normal_map_path=new_path.toUtf8().constData();
            base->normalMaps()->generateMipmap();
            return true;
        }
    }
    else
    {
        cerr<<"Unable to save image"<<FILEINFO<<endl;
    }
    return false;
}

bool TerrainIO::loadVoxel(TerrainBase * base, const std::string &file_name)
{
    QFileInfo info(file_name.c_str());
    if(!info.exists())
    {
        part_dir.mkpath(file_name.c_str());
    }


    //FILE *file = fopen(file_name.c_str(), "rb");
    File file(file_name);
    if(!file.open(File::READ))
    {
        cerr<<"Unable to load voxel map file: "<<file_name<<endl;
        return false;
    }

    voxel_file=file_name;

    sendProgress("Loading terrain data...",20);

    setlocale(LC_NUMERIC, "C");
    TerrainHeader header;

    if(file.read((void*)&header,sizeof(header),1)<=0)
    {
        setlocale(LC_ALL, "");
        file.close();
        return false;
    }

    TerrainCompress compress;

    int node_size=header.node_size;
    unsigned int s_3=node_size*node_size*node_size;

    signed char * voxel_data=new signed char[s_3*sizeof(TerrainVoxel)*2];
    signed char * temp_data =new signed char[s_3*sizeof(TerrainVoxel)*2];

    QByteArray array;

    if(header.id==44)
    {
        Patch3 patch;
        int compress_type;
        int size;
        file.read((void*)&compress_type,sizeof(compress_type),1);

        while(file.read((void*)&patch,sizeof(patch),1) > 0)
        {
            if(file.read((void*)voxel_data,patch.size,1) <= 0)
                    break;//hack

            TerrainRegion * region=
                    base->createRegion(patch.x/base->regionSize(),patch.y/base->regionSize(),patch.z/base->regionSize());
            if(patch.type == TerrainRegion::UNDERGROUND)
            {
                region->voxel_data=(TerrainVoxel*)voxel_data;

                signed char value=reinterpret_cast<signed char&>(region->voxel_data[0].value);

                for(unsigned int i=0; i<s_3; i++)
                {
                    region->voxel_data[i].value=-127;//value;
                    region->voxel_data[i].material[0]=255;
                    region->voxel_data[i].material[1]=0;
                    region->voxel_data[i].material[2]=0;
                    region->voxel_data[i].material[3]=0;
                }

                size=compress.encode(TerrainCompress::SPLIT_RLE,region,temp_data);
                signed char * tmp=new signed char[size];
                memcpy(tmp, temp_data, size);
                region->voxel_compressed=tmp;
                region->comressed_size=size;
                region->voxel_data=NULL;
                region->setRegionType(TerrainRegion::UNDERGROUND);
                region->dirty=false;

            }
            else if(patch.type == TerrainRegion::GROUND)
            {
                array = qUncompress((uchar*)voxel_data,patch.size);
                region->voxel_compressed=(signed char*)array.data();
                region->comressed_size=array.size();
                region->voxel_data=new TerrainVoxel[node_size*node_size*node_size];

                compress.decode(TerrainCompress::EXT_RLE_OLD,region,(signed char *)array.data());

                size=compress.encode(TerrainCompress::SPLIT_RLE,region,voxel_data);

                signed char * tmp=new signed char[size];
                memcpy(tmp,voxel_data,size);
                region->dirty=true;
                region->setRegionType(TerrainRegion::GROUND);
                region->voxel_compressed=tmp;
                region->comressed_size=size;

                delete [] region->voxel_data;
                region->voxel_data=NULL;
            }
        }
    }
    else if(header.id==45)
    {
        Patch3 patch;
        int compress_type;
        int size;
        file.read((void*)&compress_type,sizeof(compress_type),1);

        while(file.read((void*)&patch,sizeof(patch),1) > 0)
        {
            if(file.read((void*)voxel_data,patch.size,1) <= 0)
                    break;//hack

            TerrainRegion * region = base->createRegion(patch.x/base->regionSize(),patch.y/base->regionSize(),patch.z/base->regionSize());
            //TODO make one prototype and copy it
            if(patch.type == TerrainRegion::UNDERGROUND)
            {
                signed char value =reinterpret_cast<signed char&>(voxel_data[0]);
                unsigned char mat1=reinterpret_cast<unsigned char&>(voxel_data[1]);
                unsigned char mat2=reinterpret_cast<unsigned char&>(voxel_data[2]);
                unsigned char mat3=reinterpret_cast<unsigned char&>(voxel_data[3]);
                unsigned char mat4=reinterpret_cast<unsigned char&>(voxel_data[4]);

                region->voxel_data=(TerrainVoxel*)voxel_data;

                for(unsigned int i=0; i<s_3; i++)
                {
                    region->voxel_data[i].value      =value;
                    region->voxel_data[i].material[0]=mat1;
                    region->voxel_data[i].material[1]=mat2;
                    region->voxel_data[i].material[2]=mat3;
                    region->voxel_data[i].material[3]=mat4;
                }

                size=compress.encode(TerrainCompress::EXT_RLE,region,temp_data);
                signed char * tmp=new signed char[size];
                memcpy(tmp, temp_data, size);
                region->voxel_compressed=tmp;
                region->comressed_size=size;
                region->voxel_data=NULL;
                region->setRegionType(TerrainRegion::UNDERGROUND);
                region->dirty=false;
            }
            else if(patch.type == TerrainRegion::GROUND)
            {
                array = qUncompress((uchar*)voxel_data,patch.size);
                region->setRegionType(TerrainRegion::GROUND);
                signed char * tmp=new signed char[array.size()];
                memcpy(tmp,array.data(),array.size());
                region->voxel_compressed=tmp;
                region->comressed_size=array.size();
                region->voxel_data=NULL;
                region->dirty=true;
            }
        }
    }
    else if(header.id==46)
    {
        Patch3 patch;
        int compress_type;
        int size;
        file.read((void*)&compress_type,sizeof(compress_type),1);

        while(file.read((void*)&patch,sizeof(patch),1) > 0)
        {
            if(file.read((void*)voxel_data,patch.size,1) <= 0)
                    break;//hack

            TerrainRegion * region = base->createRegion(patch.x/base->regionSize(),patch.y/base->regionSize(),patch.z/base->regionSize());
            //TODO make one prototype and copy it
            if(patch.type == TerrainRegion::UNDERGROUND)
            {
                signed char value =reinterpret_cast<signed char&>(voxel_data[0]);
                unsigned char mat1=reinterpret_cast<unsigned char&>(voxel_data[1]);
                unsigned char mat2=reinterpret_cast<unsigned char&>(voxel_data[2]);
                unsigned char mat3=reinterpret_cast<unsigned char&>(voxel_data[3]);
                unsigned char mat4=reinterpret_cast<unsigned char&>(voxel_data[4]);

                region->voxel_data=(TerrainVoxel*)voxel_data;

                for(unsigned int i=0; i<s_3; i++)
                {
                    region->voxel_data[i].value      =value;
                    region->voxel_data[i].material[0]=mat1;
                    region->voxel_data[i].material[1]=mat2;
                    region->voxel_data[i].material[2]=mat3;
                    region->voxel_data[i].material[3]=mat4;
                }

                size=compress.encode(TerrainCompress::SPLIT_RLE,region,temp_data);
                signed char * tmp=new signed char[size];
                memcpy(tmp, temp_data, size);
                region->voxel_compressed=tmp;
                region->comressed_size=size;
                region->voxel_data=NULL;
                region->setRegionType(TerrainRegion::UNDERGROUND);
                region->dirty=false;
            }
            else if(patch.type == TerrainRegion::GROUND)
            {
                array = qUncompress((uchar*)voxel_data,patch.size);
                region->setRegionType(TerrainRegion::GROUND);
                signed char * tmp=new signed char[array.size()];
                memcpy(tmp,array.data(),array.size());
                region->voxel_compressed=tmp;
                region->comressed_size=array.size();
                region->voxel_data=NULL;
                region->dirty=true;
            }
        }
    }
    else
    {
        cerr<<"Unknown voxel format"<<FILEINFO<<endl;
        delete [] voxel_data;
        delete [] temp_data;
        setlocale(LC_ALL, "");
        file.close();
        return false;
    }


    delete [] voxel_data;
    delete [] temp_data;
    setlocale(LC_ALL, "");
    file.close();

    return true;
}

//comprimation for the future
// http://opensource.imageworks.com/?p=field3d
bool TerrainIO::saveVoxel(const std::string &file_name)
{
    QFileInfo info(file_name.c_str());
    if(!info.exists())
    {
        part_dir.mkpath(info.absolutePath());
    }

    FILE *file = fopen(file_name.c_str(), "wb");
    if(file==NULL)
    {
        cerr<<"Unable to open terrain data for save: "<< file_name<<FILEINFO<<endl;
        return false;
    }
    QElapsedTimer timer;
    timer.start();

    voxel_file=file_name;

    //disable localization for dot problem 0.0 vs 0,0
    setlocale(LC_NUMERIC, "C");
    TerrainRegion * region;
    Patch3 patch;

    int node_size=base->regionSize();
    int s_3=node_size*node_size*node_size;
    signed char * voxel_data=new signed char[s_3*sizeof(TerrainVoxel)*2];
/*
 *low=input & 0xff
 *high=(input>>8) & 0xff
 *input=low | (high<<8)
 **/
    TerrainHeader2 header;
    header.id=46;
    header.node_size=base->regionSize();
    header.nodes_x=base->regionsX();
    header.nodes_y=base->regionsY();
    header.nodes_z=base->regionsZ();
    header.compress_type=TerrainCompress::DEFLATE;

    fwrite(&header,sizeof(header),1,file);
    TerrainCompress compress;

    QByteArray array;

    signed char * compress_data;
    int size;

    //better than any combination with 7z or other
    for(int i=0;i<base->regionsX();i++)
        for(int j=0;j<base->regionsY();j++)
            for(int k=0;k<base->regionsZ();k++)
    {
        region=base->regionAtIndex(i,j,k);

        if(region!=NULL)
        {
            if(region->regionType() == TerrainRegion::UNDERGROUND)
            {
                patch.x=region->position()[0];
                patch.y=region->position()[1];
                patch.z=region->position()[2];
                patch.type=TerrainRegion::UNDERGROUND;
                patch.size=sizeof(signed char)*5;

                region->decompress();
                voxel_data[0]=reinterpret_cast<signed char&>(region->voxel_data[0].value);
                voxel_data[1]=reinterpret_cast<signed char&>(region->voxel_data[0].material[0]);
                voxel_data[2]=reinterpret_cast<signed char&>(region->voxel_data[0].material[1]);
                voxel_data[3]=reinterpret_cast<signed char&>(region->voxel_data[0].material[2]);
                voxel_data[4]=reinterpret_cast<signed char&>(region->voxel_data[0].material[3]);

                fwrite(&patch,sizeof(patch),1,file);
                fwrite(voxel_data,patch.size,1,file);
            }
            else if(region->type() == TerrainRegion::GROUND)
            {/*
                if(region->voxel_data!=NULL)
                {
                    size=compress.encode(TerrainCompress::EXT_RLE,region,voxel_data);
                    compress_data=voxel_data;
                }
                else
                {
                    size=region->comressed_size;
                    compress_data=region->voxel_compressed;
                }*/
                //////

                region->decompress();
                size=compress.encode(TerrainCompress::SPLIT_RLE,region,voxel_data);
                compress_data=voxel_data;
                /////


                array=qCompress((uchar*)compress_data,size,9);

                patch.x=region->position()[0];
                patch.y=region->position()[1];
                patch.z=region->position()[2];
                patch.type=TerrainRegion::GROUND;
                patch.size=sizeof(signed char)*array.size();

                fwrite(&patch, sizeof(patch), 1, file);
                if(array.size() > 0)
                {
                    fwrite(array.data(), patch.size, 1, file);
                    array.clear();
                }
            }
        }
    }

    delete [] voxel_data;
    cout<<"Terrain export time "<<timer.elapsed()<<"ms"<<endl;
    fclose(file);
    setlocale(LC_ALL, "");
    return true;
}

void TerrainIO::loadMaterials(const QDomElement &root)
{
    QDomElement el;
    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName() == "material")
        {
            int id=el.attribute("id").toInt();
            loadMaterial(el.toElement(),id);
        }
        child=child.nextSibling();
    }
    base->textures()->generateMipmap();
    base->normalMaps()->generateMipmap();
}

void TerrainIO::loadMaterial(const QDomElement &root, int id)
{
    QDomElement el;
    QDomNode child=root.firstChild();
    QString name, texture, normalmap;

    while(!child.isNull())
    {
        el=child.toElement();
        //cout<<el.text()<<endl;
        if(el.tagName() == "name")
        {
            name = el.text();
        }
        else if(el.tagName() == "texture")
        {
            texture = el.text();
        }
        else if(el.tagName() == "normalmap")
        {
            normalmap = el.text();
        }

        child=child.nextSibling();
    }

    TerrainMaterial * material=base->getMaterial(id);
    if(!texture.isEmpty() && material!=NULL)
    {
        material->name=name.toStdString();
        material->loadTexture(part_dir.absoluteFilePath(texture).toUtf8().constData());

        if(!normalmap.isEmpty())
        {
            material->loadNormalMap(part_dir.absoluteFilePath(normalmap).toUtf8().constData());
        }
    }
}
