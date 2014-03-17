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
#include "terrainio.h"

TerrainIO::TerrainIO(TerrainGraphics *base)
    : base(*base)
{
}

bool TerrainIO::load(const QDir &dir, const QDomElement &root, Map *map)
{
    part_dir=dir;
    QElapsedTimer timer;
    timer.start();

    QDomElement el;
    base.setMap(map);
    base.build(ceil(map->width()/map->nodeSize()),
                               ceil(map->height()/map->nodeSize()),
                               ceil(map->depth()/map->nodeSize()),map->nodeSize());

    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="textures")
        {
            base.atlas.load(256,256,8,GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT);


            QDomNodeList textures=el.elementsByTagName("texture");
            for(int j=0;j<textures.count();j++)
            {
                el = textures.at(j).toElement();
                int id=el.attribute("id").toInt()-1;

                loadTexture(part_dir.absoluteFilePath(el.text()),id);
            }
        }
        else if(el.tagName()=="normalmaps")
        {
            base.normalmap.load(256,256,8,GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT);

            QDomNodeList normalmaps=el.elementsByTagName("normalmap");
            for(int j=0;j<normalmaps.count();j++)
            {
                el = normalmaps.at(j).toElement();
                int id=el.attribute("id").toInt()-1;

                loadNormalMap(part_dir.absoluteFilePath(el.text()),id);
            }
        }
        else if(el.tagName()=="voxelmap" && !el.text().isEmpty())
        {
            timer.restart();
            if(!el.text().isEmpty())
                loadVoxel(part_dir.filePath(el.text()));
            qDebug()<<"Loading time: "<<timer.elapsed()<<"ms";
        }
        child = child.nextSibling();
    }

    timer.restart();

    base.rebuid();

    qDebug()<<"Voxel build time: "<<timer.elapsed()<<"ms";

    return true;
}

bool TerrainIO::save(QXmlStreamWriter &xml)
{
    xml.writeStartElement("terrain");
    xml.writeStartElement("textures");
    for(int i=0;i<base.maxTextures();i++)
    {
        if(!texture_file[i].isEmpty())
        {
            xml.writeStartElement("texture");
            xml.writeAttribute("id",QString::number(i+1));
            xml.writeCharacters(part_dir.relativeFilePath(texture_file[i]));
            xml.writeEndElement();
        }
    }
    xml.writeEndElement();

    xml.writeStartElement("normalmaps");
    for(int i=0;i<base.maxTextures();i++)
    {
        if(!normalmap_file[i].isEmpty())
        {
            xml.writeStartElement("normalmap");
            xml.writeAttribute("id",QString::number(i+1));
            xml.writeCharacters(part_dir.relativeFilePath(normalmap_file[i]));
            xml.writeEndElement();
        }
    }

    xml.writeEndElement();
    xml.writeTextElement("voxelmap",part_dir.relativeFilePath(voxel_file));
    xml.writeEndElement();

    saveVoxel(part_dir.absoluteFilePath(voxel_file));

    return true;
}

bool TerrainIO::create(const QDir &dir, Map *map)
{
    part_dir=dir;
    voxel_file=part_dir.absoluteFilePath("terrain/terrain.bin");

    base.setMap(map);
    base.build(ceil(map->width()/map->nodeSize()),
                        ceil(map->height()/map->nodeSize()),
                        ceil(map->depth()/map->nodeSize()),map->nodeSize());
    //insert(map);

    return true;
}

bool TerrainIO::insert(Map *map)
{
    for(int i=0; i< base.regionsX();i++)
        for(int j=0; j< base.regionsY();j++)
            for(int k=0; k< base.regionsZ();k++)
            {
                map->insertObject(base.regionAtIndex(i,j,k));
            }
    return true;
}

bool TerrainIO::importTexture(unsigned short id,const QString &path)
{
    QFileInfo info(path);
    QDir t_dir=part_dir;
    t_dir.cd("terrain");
    QString name="texture";
    name+=QString::number(id+1);
    name+=".png";
    QString new_path=t_dir.absoluteFilePath(name);
    QImage img(path);
    if(info.exists() && img.save(new_path,"PNG"))
    {
        texture_file[id]=new_path;
        base.atlas.loadSub(new_path.toUtf8().constData(),id);

        emit newMaterial(id+1,img);
    }
    else
    {
        qDebug()<<"Unable to save image"<<new_path;
    }
    return false;
}

bool TerrainIO::importNormalMap(unsigned short id, const QString &path)
{
    QFileInfo info(path);
    QDir t_dir=part_dir;
    t_dir.cd("terrain");
    QString name="texture";
    name+=QString::number(id+1);
    name+="-normal.png";
    QString new_path=t_dir.absoluteFilePath(name);
    QImage img(path);
    if(info.exists() && img.save(new_path,"PNG"))
    {
        normalmap_file[id]=new_path;
        base.normalmap.loadSub(new_path.toUtf8().constData(),id);

       // emit newMaterial(id+1,img);
    }
    else
    {
        qDebug()<<"Unable to save image"<<new_path;
    }
    return false;
}

bool TerrainIO::loadVoxel(const QString & file_name)
{
    QFileInfo info(file_name);
    if(!info.exists())
    {
        part_dir.mkpath(file_name);
    }

    FILE *file = fopen(file_name.toUtf8().constData(), "rb");
    if(file==NULL)
    {
        qDebug()<<"Unable to load voxel map file: "+file_name;
        return false;
    }

    voxel_file=file_name.toUtf8().constData();

    setlocale(LC_NUMERIC, "C");
    Patch patch;
    TerrainHeader header;

    TerrainSurface * surface;

    if(fread((void*)&header,sizeof(header),1,file)<=0)
    {
        setlocale(LC_ALL, "");
        fclose(file);
        return false;
    }

    if(header.id!=42)
    {
        std::cerr<<"Unknown voxel format"<<std::endl;
        return false;
    }

    int node_size=header.node_size;
    unsigned int s=node_size,s_2=node_size*node_size,s_3=node_size*node_size*node_size;

    signed char * voxel_data=new signed char[s_3*3];

    while(fread((void*)&patch,sizeof(patch),1,file) > 0)
    {
        if(fread((void*)voxel_data,patch.size,1,file) <=0)
                break;//hack

        int cnt=0;
        signed char texture;

        for(unsigned int i=0; i<patch.size; i++)//patch.size cnt<s_3
        {
            texture=voxel_data[i++];
            if(texture<0)
            {
                texture=-(texture+1);
              //  std::cout<<(int)texture<<std::endl;
                unsigned char tex_count=reinterpret_cast<unsigned char&>(voxel_data[i]);

                for(int j=0;j<=tex_count;j++)
                {
                  //  std::cout<<(int)voxel_data[i]<<std::endl;
                    i++;
                  //  std::cout<<(int)voxel_data[i]<<std::endl;
                    if(voxel_data[i]==-128)
                    {
                        i++;
                        j++;
                        unsigned char val_count=reinterpret_cast<unsigned char&>(voxel_data[i]);
                        for(int k=0;k<val_count;k++)
                        {
                            base.setVoxel(cnt/s_2 + patch.x, (cnt/s)%s+patch.y, cnt%s+patch.z,
                                                                  voxel_data[i-2], texture);
                           // std::cout<<cnt/s_2<<" "<<(cnt/s)%s<<" "<<cnt%s<<" "<<(int)voxel_data[i-2]<<" "<<(int)texture<<" /"<<std::endl;
                            cnt++;
                        }
                        //std::cout<<"val cnt:"<<(int)voxel_data[i]<<std::endl;

                    }
                    else
                    {
                        base.setVoxel(cnt/s_2 + patch.x, (cnt/s)%s+patch.y, cnt%s+patch.z,
                                                              voxel_data[i], texture);
                      //  std::cout<<cnt/s_2<<" "<<(cnt/s)%s<<" "<<cnt%s<<" "<<(int)voxel_data[i]<<" "<<(int)texture<<" *"<<i<<std::endl;
                        cnt++;
                    }
                }

               // std::cout<<"tex:"<<(int)tex_count<<std::endl;
            }
            else
            {
                texture=texture-1;
                base.setVoxel(cnt/s_2 + patch.x, (cnt/s)%s + patch.y, cnt%s+patch.z,
                                                      voxel_data[i], texture);
               // std::cout<<cnt/s_2<<" "<<(cnt/s)%s<<" "<<cnt%s<<" "<<(int)voxel_data[i]<<" "<<(int)texture<<" -"<<std::endl;
                cnt++;
            }
        }
        //break;
    }


    delete [] voxel_data;
    setlocale(LC_ALL, "");
    fclose(file);

    return true;
}

//comprimation for the future
// http://opensource.imageworks.com/?p=field3d
bool TerrainIO::saveVoxel(const QString & file_name)
{
    QFileInfo info(file_name);
    if(!info.exists())
    {
        part_dir.mkpath(info.absolutePath());
    }

    FILE *file = fopen(file_name.toUtf8().constData(), "wb");
    if(file==NULL)
    {
        qDebug()<<"Unable to open terrain data for save: "<< file_name;
        return false;
    }

    voxel_file=file_name;

    //disable localization for dot problem 0.0 vs 0,0
    setlocale(LC_NUMERIC, "C");
    TerrainRegion * region;
    Patch patch;

    int node_size=base.regionSize();

    TerrainVoxel *tmp;
    int x,y,z;
    int s=node_size,s_2=node_size*node_size,s_3=node_size*node_size*node_size;

    signed char * voxel_data=new signed char[s_3*3];
/*
 *low=input & 0xff
 *high=(input>>8) & 0xff
 *input=low | (high<<8)
 **/
    TerrainHeader header;
    header.id=42;
    header.node_size=base.regionSize();
    header.nodes_x=base.regionsX();
    header.nodes_y=base.regionsY();
    header.nodes_z=base.regionsZ();

    fwrite(&header,sizeof(header),1,file);

    for(int i=0;i<base.regionsX();i++)
        for(int j=0;j<base.regionsY();j++)
            for(int k=0;k<base.regionsZ();k++)
    {
        region=base.regionAtIndex(i,j,k);
        if(region!=NULL && region->voxelField()!=NULL)
        {
            unsigned char tex_count=0,val_count=0;
            int size=1;
            int last_tex=0;
            voxel_data[last_tex]=-10;

            int last_val=420;

            for(int c=0;c<s_3;c++)
            {
                x=c/s_2;
                y=(c/s)%s;
                z=c%s;

                tmp=region->field(x,y,z);
                //tmp->texture_id=0;
                if(tmp->value==-128)
                    tmp->value=-127;

                if(tex_count<251 && voxel_data[last_tex]==tmp->texture_id)
                {
                    if(last_val==tmp->value && val_count<255)
                    {
                        val_count++;
                     //   std::cout<<x<<" "<<y<<" "<<z<<" "<<(int)tmp->value<<" "<<(int)tmp->texture_id<<std::endl;
                    }
                    else
                    {
                        if(val_count>0)
                        {
                            voxel_data[size++]=-128;
                            voxel_data[size++]=reinterpret_cast<signed char&>(val_count);
                   //         std::cout<<"val"<<(int)val_count<<std::endl;
                            tex_count+=2;
                            val_count=0;
                        }
                        voxel_data[size++]=tmp->value;
                        last_val=tmp->value;
                        tex_count++;
                   //      std::cout<<x<<" "<<y<<" "<<z<<" "<<(int)tmp->value<<" "<<(int)tmp->texture_id<<std::endl;
                    }
                }
                else
                {
                    if(val_count>0)
                    {
                        voxel_data[size++]=-128;
                        voxel_data[size++]=reinterpret_cast<signed char&>(val_count);
                 //       std::cout<<"val"<<(int)val_count<<std::endl;
                        tex_count+=2;
                        val_count=0;
                    }

                    if(tex_count>0)
                    {
                        voxel_data[last_tex]=-(voxel_data[last_tex]+1);
                        voxel_data[last_tex+1]=reinterpret_cast<signed char&>(tex_count);
                   //     std::cout<<"tex"<<(int)tex_count<<std::endl;
                    }
                    else
                    {
                        voxel_data[last_tex]=voxel_data[last_tex]+1;
                        size--;
                    }

                    last_tex=size;
                    last_val=tmp->value;

                    voxel_data[size++]=tmp->texture_id;
                    voxel_data[size++]=tmp->value;
                    voxel_data[size++]=tmp->value;
                    tex_count=0;
                   // std::cout<<x<<" "<<y<<" "<<z<<" "<<(int)tmp->value<<" "<<(int)tmp->texture_id<<std::endl;
                }
            }

            if(val_count>0)
            {
                voxel_data[size++]=-128;
                voxel_data[size++]=reinterpret_cast<signed char&>(val_count);
             //   std::cout<<"val"<<(int)val_count<<std::endl;
                tex_count+=2;
                val_count=0;
            }

            if(tex_count>0)
            {
                voxel_data[last_tex]=-(voxel_data[last_tex]+1);
                voxel_data[last_tex+1]=reinterpret_cast<signed char&>(tex_count);
              //  std::cout<<"tex"<<(int)tex_count<<std::endl;
            }
            else
            {
                voxel_data[last_tex]=voxel_data[last_tex]+1;
                size--;
            }

            //load patch information
            patch.x=region->pos_x;
            patch.y=region->pos_y;
            patch.z=region->pos_z;
            patch.size=size;
         //   std::cout<<size<<std::endl;
            fwrite(&patch,sizeof(patch),1,file);

            fwrite(voxel_data,sizeof(signed char)*size,1,file);
        }
        /*
        delete [] voxel_data;

        fclose(voxel_file);
        setlocale(LC_ALL, "");
        return true;*/
    }

    delete [] voxel_data;

    fclose(file);
    setlocale(LC_ALL, "");
    return true;
}

bool TerrainIO::loadTexture(const QString &file_name,int id)
{
    if(id>=0 && id<8)
    {
        //QFileInfo info;
        texture_file[id]=file_name;
        base.atlas.loadSub(file_name.toUtf8().constData(),id);
        /*
        info.setFile(file_name);
        QString normal=info.canonicalPath();
        normal+="/";
        normal+=info.baseName();
        normal+="-normal.png";

        base.normalmap.loadSub(normal.toUtf8().constData(),id);*/
        return true;
    }
    return false;
}

bool TerrainIO::loadNormalMap(const QString &file_name, int id)
{
    if(id>=0 && id<8)
    {
        normalmap_file[id]=file_name;
        base.normalmap.loadSub(file_name.toUtf8().constData(),id);
        return true;
    }
    return false;
}

bool TerrainIO::setTexture(int id, const char * file_name)
{
    //insert file to data
    if(id>0 && id<=base.maxTextures())
    {
        texture_file[id-1]=file_name;
        return true;
    }
    else
        std::cerr<<"Unable to set terrain texture id:"<<id<<" file:"<<file_name<<std::endl;
    return false;
}


QString *TerrainIO::getTexturePath(unsigned short id)
{
    if(id<base.maxTextures())
    {
        return &texture_file[id];
    }
    return NULL;
}

QString *TerrainIO::getNormalMapPath(unsigned short id)
{
    if(id<base.maxTextures())
    {
        return &normalmap_file[id];
    }
    return NULL;
}
