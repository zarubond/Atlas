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

#include "meshobj.h"
#include "mesh.h"

MeshObj::MeshObj()
{
}

bool MeshObj::load(const char *file_name,Mesh * mesh)
{
    this->mesh=mesh;
    File file(file_name);
    if(!file.open(static_cast<File::Mode>(File::READ | File::TEXT)))
    {
        cerr<<"Unable to open model:"<<file_name<<endl;
        return false;
    }
    mesh->file=file_name;

    std::string folder,tmp_name(file_name);
    size_t slash=tmp_name.find_last_of("/\\");
    folder=tmp_name.substr(0,slash);
    folder+="/";

    //std::string line;
    char line[300];
    int max_v=5000,max_v_i=5000,max_vt_i=5000,max_vt=5000,max_vn=5000,max_vn_i=5000;

    float * tmp_v=new float[max_v];
    unsigned short * tmp_v_i=new unsigned short[max_v_i];
    //texture coordinates indicies
    uint * tmp_vt_i=new uint[max_vt_i];
    //vt-texture coordinates
    float * tmp_vt=new float[max_vt];
    //vn-vertex normals
    float * tmp_vn=new float[max_vn];
    uint * tmp_vn_i=new uint[max_vn_i];

    //counters
    int v_c=0,v_c_i=0,vt_c=0,vt_c_i=0,vn_c=0,vn_c_i=0;
    int n;

    char type[50];
    char tmp[50];
    char * str;

    MeshPart * use_mesh=NULL;
    setlocale(LC_NUMERIC, "C");
    while(file.gets(line,300) != NULL)
    {
        n=0;
        str=line;
        if(strlen(str)<=0)
            continue;

        if(sscanf(str,"%s %n",type,&n)!=1)
            continue;
        str+=n;

        resize(tmp_v    ,max_v      ,v_c);
        resize(tmp_v_i  ,max_v_i    ,v_c_i);
        resize(tmp_vt_i ,max_vt_i   ,vt_c_i);
        resize(tmp_vt   ,max_vt     ,vt_c);
        resize(tmp_vn   ,max_vn     ,vn_c);
        resize(tmp_vn_i ,max_vn_i   ,vn_c_i);

        if(strcmp(type,"v")==0)//vertex
        {
            sscanf(str,"%f %f %f %n",&tmp_v[v_c],&tmp_v[v_c+1],&tmp_v[v_c+2],&n);
            v_c+=3;
            str+=n;
        }
        else if(strcmp(type,"f")==0)
        {
            int v;
            sscanf(str,"%d %n",&v,&n);//v1
            str+=n;
            tmp_v_i[v_c_i++]=v-1;

            if(str[0]=='/')
            {
                str++;
                if(str[0]=='/')//f v1//vn1 v2//vn2 v3//vn3
                {
                    cout<<"No texture coord"<<endl;
                    str++;
                }
                else // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                {
                    sscanf(str,"%d %n",&v,&n);//vt1
                    str+=n;
                    tmp_vt_i[vt_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn1
                    str+=n;
                    tmp_vn_i[vn_c_i++]=v-1;

                    //str++;
                    sscanf(str,"%d %n",&v,&n);//v2
                    str+=n;
                    tmp_v_i[v_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vt2
                    str+=n;
                    tmp_vt_i[vt_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn2
                    str+=n;
                    tmp_vn_i[vn_c_i++]=v-1;

                    //str++;
                    sscanf(str,"%d %n",&v,&n);//v3
                    str+=n;
                    tmp_v_i[v_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vt3
                    str+=n;
                    tmp_vt_i[vt_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn3
                    str+=n;
                    tmp_vn_i[vn_c_i++]=v-1;
                    //four is quad

                    //std::cout<<tmp_vt_i[vt_i_c-3]<<" "<<tmp_vt_i[vt_i_c-2]<<" "<<tmp_vt_i[vt_i_c-1]<<std::endl;

                    use_mesh->index_count+=3;
                }
                /*else //f v1/vt1 v2/vt2 v3/vt3
                {

                }*/

            }
            else //f v1 v2 v3 v4
            {

            }

        }
        else if(strcmp(type,"vt")==0)
        {
            sscanf(str,"%f %f %n",&tmp_vt[vt_c],&tmp_vt[vt_c+1],&n);
            vt_c+=2;
            str+=n;
        }
        else if(strcmp(type,"vn")==0)
        {
            sscanf(str,"%f %f %f %n",&tmp_vn[vn_c],&tmp_vn[vn_c+1],&tmp_vn[vn_c+2],&n);
            vn_c+=3;
            str+=n;
        }
        else if(strcmp(type,"o")==0)
        {
            sscanf(str,"%s %n",type,&n);
            str+=n;
        }
        else if(strcmp(type,"mtllib")==0)
        {
            sscanf(str,"%s %n",tmp,&n);
            str+=n;

            tmp_name.clear();
            tmp_name=folder;
            if(tmp[0]=='.' && tmp[1]=='/')
                tmp_name+=&tmp[2];
            else
                tmp_name+=tmp;

            loadMTL(tmp_name,folder);

            use_mesh=mesh->createPart(mesh->materials.back(),v_c_i,0);
        }
        else if(strcmp(type,"usemtl")==0)
        {
            sscanf(str,"%s %n",tmp,&n);
            str+=n;

            for (std::vector<MeshMaterial*>::iterator it = mesh->materials.begin() ; it != mesh->materials.end(); ++it)
            {
                if((*it)->name==tmp)
                {
                    use_mesh=mesh->createPart((*it),v_c_i,0);
                    break;
                }
            }
        }
        else if(strcmp(type,"#")==0)
        {

        }
        else
        {
          //  std::cout<<type<<std::endl;
        }
    }
    setlocale(LC_ALL, "");
    file.close();

    float * ogl_normal=new float[v_c_i*3];
    float * ogl_tex=new float[v_c_i*2];
    float * ogl_vec=new float[v_c_i*3];

    for(int i=0;i<v_c_i;i++)
    {
        /*
        ogl_normal[tmp_v_i[i]*3  ]=tmp_vn[tmp_vn_i[i]*3  ];
        ogl_normal[tmp_v_i[i]*3+1]=tmp_vn[tmp_vn_i[i]*3+1];
        ogl_normal[tmp_v_i[i]*3+2]=tmp_vn[tmp_vn_i[i]*3+2];

        ogl_tex[tmp_v_i[i]*2     ]=tmp_vt[tmp_vt_i[i]*2  ];
        ogl_tex[tmp_v_i[i]*2+1   ]=tmp_vt[tmp_vt_i[i]*2+1];*/

        ogl_normal[i*3  ]=tmp_vn[tmp_vn_i[i]*3  ]*-1;
        ogl_normal[i*3+1]=tmp_vn[tmp_vn_i[i]*3+1]*-1;
        ogl_normal[i*3+2]=tmp_vn[tmp_vn_i[i]*3+2]*-1;

        ogl_vec[i*3  ]=tmp_v[tmp_v_i[i]*3  ];
        ogl_vec[i*3+1]=tmp_v[tmp_v_i[i]*3+1];
        ogl_vec[i*3+2]=tmp_v[tmp_v_i[i]*3+2];

        ogl_tex[i*2     ]=tmp_vt[tmp_vt_i[i]*2];
        ogl_tex[i*2+1   ]=tmp_vt[tmp_vt_i[i]*2+1];

        tmp_v_i[i]=i;
    }


    mesh->setVertices(ogl_vec,v_c_i);
    mesh->setNormals(ogl_normal,v_c_i);
    mesh->setTexturesUV(ogl_tex, v_c_i);
    mesh->setIndices(tmp_v_i,v_c_i);

    delete [] tmp_v;
    //delete [] tmp_v_i;
    delete [] tmp_vt;
    delete [] tmp_vt_i;
    delete [] tmp_vn;
    delete [] tmp_vn_i;

    //delete [] ogl_normal;
    //delete [] ogl_tex;
    //delete [] ogl_vec;

    return true;
}

void MeshObj::loadMTL(const std::string & file_name,const std::string & folder)
{
    File file(file_name);
    if(!file.open(static_cast<File::Mode>(File::READ| File::TEXT)))
        cerr<<"Unable to open mtl:"<<file_name<<endl;

    char line[300];
    std::stringstream str;
    std::string tmp;
    std::string tmp_name;

    MeshMaterial * material=NULL;

    while(file.gets(line,300) != NULL)//getline(file,line))
    {
        str.clear();
        str.str("");
        str<<line;

        str>>tmp;
        //std::cout<<tmp<<std::endl;
        if(tmp.compare("newmtl")==0)
        {
            material=mesh->createMaterial();

            str>>material->name;
            material->index_count=0;
            material->start_index=0;
            material->diffuse.set(1,1,1);
            material->ambient.set(1,1,1);
            material->specular.set(1,1,1);
        }
        else if(material!=NULL)
        {
            if(tmp.compare("map_Kd")==0)
            {
                str>>tmp;
                tmp_name=folder;
                if(tmp[0]=='.' && tmp[1]=='/')
                    tmp_name+=tmp.erase(0,2);
                else
                    tmp_name+=tmp;
                //cout<<tmp_name<<endl;
                material->setTexture(tmp_name.c_str());
            }
            else if(tmp.compare("map_d")==0)
            {

            }
            else if(tmp.compare("Kd")==0)
            {
                float r=0,g=0,b=0;
                str>>r>>g>>b;
                material->diffuse.set(r,g,b);
            }
            else if(tmp.compare("Ks")==0)
            {

                float r=0,g=0,b=0;
                str>>r>>g>>b;
                material->specular.set(r,g,b);
            }
            else if(tmp.compare("Ka")==0)
            {

                float r=0,g=0,b=0;
                str>>r>>g>>b;
                material->ambient.set(r,g,b);
            }
            /*
            else if(tmp.compare("bump") || tmp.compare("map_bump"))
            {
                str>>tmp;
                tmp_name=folder;
                if(tmp[0]=='.' && tmp[1]=='/')
                    tmp_name+=tmp.erase(0,2);
                else
                    tmp_name+=tmp;

                material->bump.load(tmp_name.c_str());
            }*/
            else
            {
                //std::cout<<tmp<<std::endl;
            }
        }

    }
    file.close();
}

void MeshObj::resize(float *&array, int &max, int size)
{
    if(size>max-10)
    {
        float * tmp = new float[max*2];
        for(int i=0;i<size;i++)
        {
            tmp[i]=array[i];
        }
        delete [] array;
        array=tmp;
        max=max*2;
    }
}

void MeshObj::resize(uint *&array, int &max, int size)
{
    if(size>max-10)
    {
        uint * tmp = new uint[max*2];
        for(int i=0;i<size;i++)
        {
            tmp[i]=array[i];
        }
        delete [] array;
        array=tmp;
        max=max*2;
    }
}

void MeshObj::resize(unsigned short *&array, int &max, int size)
{
    if(size>max-10)
    {
        unsigned short * tmp = new unsigned short[max*2];
        for(int i=0;i<size;i++)
        {
            tmp[i]=array[i];
        }
        delete [] array;
        array=tmp;
        max=max*2;
    }
}

std::vector<std::string> MeshObj::meshFiles(const char * file_name)
{
    File file(file_name);

    std::vector<std::string> list;

    if(!file.open(static_cast<File::Mode>(File::READ| File::TEXT)))
    {
        cerr<<"Unable to open mesh:"<<file_name<<endl;
        return list;
    }
    list.push_back(file_name);

    std::string folder,tmp_name(file_name);
    size_t slash=tmp_name.find_last_of("/\\");
    folder=tmp_name.substr(0,slash);
    folder+="/";
    char line[300];
    char type[50];
    char tmp[50];
    char * str;

    setlocale(LC_NUMERIC, "C");
    while(file.gets(line,300) != NULL)
    {
        int n=0;
        str=line;
        if(strlen(str)<=0)
            continue;

        if(sscanf(str,"%s %n",type,&n)!=1)
            continue;
        str+=n;

        if(strcmp(type,"mtllib")==0)
        {
            sscanf(str,"%s %n",tmp,&n);
            str+=n;

            tmp_name.clear();
            tmp_name=folder;
            if(tmp[0]=='.' && tmp[1]=='/')
                tmp_name+=&tmp[2];
            else
                tmp_name+=tmp;
//////////////////////

            File mtl_file(tmp_name);
            if(!mtl_file.open(static_cast<File::Mode>(File::READ| File::TEXT)))
            {
                list.clear();
                cerr<<"Unable to open mtl:"<<tmp_name<<endl;
                return list;
            }

            list.push_back(tmp_name);

            char line2[300];
            std::stringstream str;
            std::string tmp;
            std::string img_name;

            while(mtl_file.gets(line2,300) != NULL)
            {
                str.clear();
                str.str("");
                str<<line2;

                str>>tmp;
                if(tmp.compare("map_Kd")==0)
                {
                    str>>tmp;
                    img_name=folder;
                    if(tmp[0]=='.' && tmp[1]=='/')
                        img_name+=tmp.erase(0,2);
                    else
                        img_name+=tmp;
                    list.push_back(img_name);
                }

            }

            ////////////////////////
            break;
        }
    }
    file.close();
    return list;
}

std::vector<std::string> MeshObj::meshParts(const char *file_name)
{
    std::vector<std::string> files;
    return files;
}
