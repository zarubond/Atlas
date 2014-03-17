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
#include "meshobj.h"
#include "mesh.h"

MeshObj::MeshObj()
{
}

bool MeshObj::load(const char *file_name,Mesh * mesh)
{
    this->mesh=mesh;
    FILE * file=fopen(file_name,"r");
    if(file==NULL)
    {
        std::cerr<<"Unable to open model:"<<file_name<<std::endl;
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

    GLfloat * tmp_v=new GLfloat[max_v];
    GLuint * tmp_v_i=new GLuint[max_v_i];
    //texture coordinates indicies
    GLuint * tmp_vt_i=new GLuint[max_vt_i];
    //vt-texture coordinates
    GLfloat * tmp_vt=new GLfloat[max_vt];
    //vn-vertex normals
    GLfloat * tmp_vn=new GLfloat[max_vn];
    GLuint * tmp_vn_i=new GLuint[max_vn_i];

    //counters
    int v_c=0,v_c_i=0,vt_c=0,vt_i_c=0,vn_c=0,vn_i_c=0;
    int n;

    char type[50];
    char tmp[50];
    char * str;

    Mesh::Part * use_mesh=NULL;
    setlocale(LC_NUMERIC, "C");
    while(fgets(line,300,file) != NULL)
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
        resize(tmp_vt_i ,max_vt_i   ,vt_i_c);
        resize(tmp_vt   ,max_vt     ,vt_c);
        resize(tmp_vn   ,max_vn     ,vn_c);
        resize(tmp_vn_i ,max_vn_i   ,vn_i_c);

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
                    std::cout<<"No texture coord"<<std::endl;
                    str++;
                }
                else // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                {
                    sscanf(str,"%d %n",&v,&n);//vt1
                    str+=n;
                    tmp_vt_i[vt_i_c++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn1
                    str+=n;
                    tmp_vn_i[vn_i_c++]=v-1;

                    //str++;
                    sscanf(str,"%d %n",&v,&n);//v2
                    str+=n;
                    tmp_v_i[v_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vt2
                    str+=n;
                    tmp_vt_i[vt_i_c++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn2
                    str+=n;
                    tmp_vn_i[vn_i_c++]=v-1;

                    //str++;
                    sscanf(str,"%d %n",&v,&n);//v3
                    str+=n;
                    tmp_v_i[v_c_i++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vt3
                    str+=n;
                    tmp_vt_i[vt_i_c++]=v-1;
                    str++;
                    sscanf(str,"%d %n",&v,&n);//vn3
                    str+=n;
                    tmp_vn_i[vn_i_c++]=v-1;
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

            Mesh::Material * mat_ptr=mesh->getMaterials();
            while(mat_ptr!=NULL)
            {
                if(mat_ptr->next==NULL)
                {
                    use_mesh=new Mesh::Part;
                    use_mesh->material=mat_ptr;
                    use_mesh->start_index=v_c_i;
                    use_mesh->index_count=0;
                    mesh->addPart(use_mesh);
                    break;
                }
                mat_ptr=mat_ptr->next;
            }
        }
        else if(strcmp(type,"usemtl")==0)
        {
            sscanf(str,"%s %n",tmp,&n);
            str+=n;

            Mesh::Material * tmp_mat=mesh->getMaterials();
            while(tmp_mat!=NULL)
            {
                if(tmp_mat->name==tmp)
                {
                    use_mesh=new Mesh::Part;
                    use_mesh->start_index=v_c_i;
                    use_mesh->index_count=0;
                    use_mesh->material=tmp_mat;
                    mesh->addPart(use_mesh);
                    break;
                }
                tmp_mat=tmp_mat->next;
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

    GLfloat * ogl_normal=new GLfloat[v_c_i*3];
    GLfloat * ogl_tex=new GLfloat[v_c_i*2];
    GLfloat * ogl_vec=new GLfloat[v_c_i*3];

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

    mesh->vbo_vertices.create(v_c_i*3*sizeof(GLfloat),ogl_vec, GL_STATIC_DRAW);
    mesh->vbo_normals.create(v_c_i*3*sizeof(GLfloat),ogl_normal, GL_STATIC_DRAW);
    mesh->vbo_tex.create(v_c_i*2*sizeof(GLfloat),ogl_tex, GL_STATIC_DRAW);
    mesh->ibo_elements.create(v_c_i*sizeof(GLuint),tmp_v_i, GL_STATIC_DRAW);

    mesh->setData(ogl_vec,tmp_v_i,v_c_i);
    delete [] tmp_v;
    //delete [] tmp_v_i;
    delete [] tmp_vt;
    delete [] tmp_vt_i;
    delete [] tmp_vn;
    delete [] tmp_vn_i;

    delete [] ogl_normal;
    delete [] ogl_tex;
    //delete [] ogl_vec;

    return true;
}

void MeshObj::loadMTL(const std::string & file_name,const std::string & folder)
{
    std::ifstream file;
    file.open(file_name.c_str());
    if(!file.is_open())
        std::cerr<<"Unable to open mtl:"<<file_name<<std::endl;

    std::string line;
    std::stringstream str;
    std::string tmp;
    std::string tmp_name;

    Mesh::Material * material=NULL;


    while(getline(file,line))
    {
        str.clear();
        str.str("");
        str<<line;

        str>>tmp;
        //std::cout<<tmp<<std::endl;
        if(tmp.compare("newmtl")==0)
        {
            material=new Mesh::Material;
            mesh->addMaterial(material);

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
                //std::cout<<tmp_name<<std::endl;
                material->texture.load(tmp_name.c_str(),GL_LINEAR_MIPMAP_NEAREST,GL_REPEAT);
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
}

void MeshObj::resize(GLfloat *&array, int &max, int size)
{
    if(size>max-10)
    {
        GLfloat * tmp = new GLfloat[max*2];
        for(int i=0;i<size;i++)
        {
            tmp[i]=array[i];
        }
        delete [] array;
        array=tmp;
        max=max*2;
    }
}

void MeshObj::resize(GLuint *&array, int &max, int size)
{
    if(size>max-10)
    {
        GLuint * tmp = new GLuint[max*2];
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
    FILE * file=fopen(file_name,"r");

    std::vector<std::string> list;

    if(file==NULL)
    {
        std::cerr<<"Unable to open mesh:"<<file_name<<std::endl;
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
    while(fgets(line,300,file) != NULL)
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

            std::ifstream mtl_file;
            mtl_file.open(tmp_name.c_str());
            if(!mtl_file.is_open())
            {
                list.clear();
                std::cerr<<"Unable to open mtl:"<<tmp_name<<std::endl;
                return list;
            }

            list.push_back(tmp_name);

            std::string line;
            std::stringstream str;
            std::string tmp;
            std::string img_name;


            while(getline(mtl_file,line))
            {
                str.clear();
                str.str("");
                str<<line;

                str>>tmp;
                if(tmp.compare("map_Kd")==0)
                {
                    str>>tmp;
                    img_name=folder;
                    if(tmp[0]=='.' && tmp[1]=='/')
                        img_name+=tmp.erase(0,2);
                    else
                        img_name+=tmp;
                        //std::cout<<tmp_name<<std::endl;
                        //material->texture.load(img_name.c_str(),GL_LINEAR_MIPMAP_NEAREST,GL_REPEAT);
                    list.push_back(img_name);
                }

            }

            ////////////////////////
            break;
        }
    }
    return list;
}
