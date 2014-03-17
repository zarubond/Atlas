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
#include "objloader.h"

ObjLoader::ObjLoader()
{
    tex=false;
    material_count=0;
    materials=NULL;
    meshes=NULL;
    scale=1;
    pos_x=0;
    pos_y=0;
    pos_z=0;
}

ObjLoader::~ObjLoader()
{
    Material * tmp_mat;
    while(materials!=NULL)
    {
        tmp_mat=materials->next;
        delete materials;
        materials=tmp_mat;
    }

    Mesh * tmp_mesh;
    while(meshes!=NULL)
    {
        tmp_mesh=meshes->next;
        delete meshes;
        meshes=tmp_mesh;
    }

    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_tex);
    glDeleteBuffers(1, &ibo_elements);
}

bool ObjLoader::load(const char *file_name)
{
    std::ifstream file;
    file.open(file_name);
    if(!file.is_open())
        std::cerr<<"Unable to open model:"<<file_name<<std::endl;

    std::string folder,tmp_name(file_name);
    size_t slash=tmp_name.find_last_of("/\\");
    folder=tmp_name.substr(0,slash);
    folder+="/";

    std::string line;

    GLfloat * tmp_v=new GLfloat[400000];
    GLuint * tmp_v_i=new GLuint[400000];
    //texture coordinates indicies
    GLuint * tmp_vt_i=new GLuint[400000];
    //vt-texture coordinates
    GLfloat * tmp_vt=new GLfloat[400000];
    //vn-vertex normals
    GLfloat * tmp_vn=new GLfloat[400000];
    GLuint * tmp_vn_i=new GLuint[400000];

    //counters
    int v_c=0,v_c_i=0,vt_c=0,vt_i_c=0,vn_c=0,vn_i_c=0;
    std::stringstream str;
    std::string tmp;
    char tmp_char;
    float a;

    Mesh * use_mesh=NULL;

    while(getline(file,line))
    {
        if(line.size()<=1)
            continue;
        str.clear();
        str.str("");
        str<<line;

        str>>tmp;
        if(tmp.compare("v")==0)
        {
            str >> tmp_v[v_c++];
            str >> tmp_v[v_c++];
            str >> tmp_v[v_c++];
        }
        else if(tmp.compare("f")==0)
        {
            str>> a;//v1
            str.get(tmp_char);
            if(tmp_char=='/')
            {
                str.get(tmp_char);
                if(tmp_char=='/') //f v1//vn1 v2//vn2 v3//vn3
                {
                }
                else
                {
                    str.putback(tmp_char);
                    tmp_v_i[v_c_i++]=a-1;
                    str >> a;//vt1
                    tmp_vt_i[vt_i_c++]=a-1;
                    str.get(tmp_char);
                    if(tmp_char=='/') // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                    {
                        str>>a;//vn1
                        tmp_vn_i[vn_i_c++]=a-1;
                        str.get(tmp_char);
                        str>>a;//v2
                        tmp_v_i[v_c_i++]=a-1;
                        str.get(tmp_char);
                        str>>a;//vt2
                        tmp_vt_i[vt_i_c++]=a-1;
                        str.get(tmp_char);
                        str>>a;//vn2
                        tmp_vn_i[vn_i_c++]=a-1;
                        str.get(tmp_char);
                        str>>a;//v3
                        tmp_v_i[v_c_i++]=a-1;
                        str.get(tmp_char);
                        str>>a;//vt3
                        tmp_vt_i[vt_i_c++]=a-1;
                        str.get(tmp_char);
                        str>>a;//vn3
                        tmp_vn_i[vn_i_c++]=a-1;
                        use_mesh->index_count+=3;
                    }
                    else //f v1/vt1 v2/vt2 v3/vt3
                    {
                        str.putback(tmp_char);
                        use_mesh->index_count+=2;
                    }
                }
            }
            else //f v1 v2 v3 v4
            {
                str.putback(tmp_char);
            }

        }
        else if(tmp.compare("vt")==0)
        {
            str >> tmp_vt[vt_c++];
            str >> tmp_vt[vt_c++];
        }
        else if(tmp.compare("vn")==0)
        {
            str >> tmp_vn[vn_c++];
            str >> tmp_vn[vn_c++];
            str >> tmp_vn[vn_c++];
        }
        else if(tmp.compare("o")==0)
        {
           // if(name.length()>0)
             //   break;
            str>>name;
        }
        else if(tmp.compare("mtllib")==0)
        {
            str>>tmp;
            tmp_name.clear();
            tmp_name=folder;
            if(tmp[0]=='.' && tmp[1]=='/')
                tmp_name+=tmp.erase(0,2);
            else
                tmp_name+=tmp;

            loadMTL(tmp_name,folder);

            Material * mat_ptr=materials;
            while(mat_ptr!=NULL)
            {
                if(mat_ptr->next==NULL)
                {
                    use_mesh=new Mesh;
                    use_mesh->material=mat_ptr;
                    use_mesh->next=meshes;
                    use_mesh->start_index=v_c_i;
                    use_mesh->index_count=0;
                    meshes=use_mesh;
                    break;
                }
                mat_ptr=mat_ptr->next;
            }
        }
        else if(tmp.compare("usemtl")==0)
        {
            str>>tmp;
            Material * tmp_mat=materials;
            while(tmp_mat!=NULL)
            {
                if(tmp_mat->name==tmp)
                {
                    use_mesh=new Mesh;
                    use_mesh->start_index=v_c_i;
                    use_mesh->index_count=0;
                    use_mesh->material=tmp_mat;
                    use_mesh->next=meshes;
                    meshes=use_mesh;
                    break;
                }
                tmp_mat=tmp_mat->next;
            }
        }
        else if(tmp.compare("#")==0)
        {

        }
        else
        {
            //std::cout<<tmp<<std::endl;
        }
    }

    GLfloat * ogl_normal=new GLfloat[v_c_i*3];
    GLfloat * ogl_tex=new GLfloat[v_c_i*2];

    for(int i=0;i<v_c_i;i++)
    {
        ogl_normal[tmp_v_i[i]*3  ]=tmp_vn[tmp_vn_i[i]*3  ];
        ogl_normal[tmp_v_i[i]*3+1]=tmp_vn[tmp_vn_i[i]*3+1];
        ogl_normal[tmp_v_i[i]*3+2]=tmp_vn[tmp_vn_i[i]*3+2];

        ogl_tex[tmp_v_i[i]*2     ]=tmp_vt[tmp_vt_i[i]*2  ];
        ogl_tex[tmp_v_i[i]*2+1   ]=tmp_vt[tmp_vt_i[i]*2+1];
    }
/*
    if(scale!=1)
        for(int i=0;i<v_c;i++)
            tmp_v[i]*=scale;
*/

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, v_c*sizeof(GLfloat),tmp_v, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, v_c_i*3*sizeof(GLfloat),ogl_normal, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_tex);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glBufferData(GL_ARRAY_BUFFER, v_c_i*2*sizeof(GLfloat),ogl_tex, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_c_i*sizeof(GLuint),tmp_v_i, GL_STATIC_DRAW);

    delete [] tmp_v;
    delete [] tmp_v_i;
    delete [] tmp_vt;
    delete [] tmp_vt_i;
    delete [] tmp_vn;
    delete [] tmp_vn_i;

    delete [] ogl_normal;
    delete [] ogl_tex;


    loadShader();
    return true;
}

void ObjLoader::setScale(float scale)
{
    this->scale=scale;
}

void ObjLoader::render(const Matrix4f &mvp)
{
    shader.bind();

    Texture::active(GL_TEXTURE0);
    shader.uniformMatrix(uniform_mvp,mvp);

    glEnableVertexAttribArray(attribute_v_coord);
    glEnableVertexAttribArray(attribute_normal);
    glEnableVertexAttribArray(attribute_texcoord);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(attribute_v_coord,3, GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glVertexAttribPointer(attribute_normal, 3,GL_FLOAT, GL_FALSE,0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glVertexAttribPointer(attribute_texcoord,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);

    shader.uniform(uniform_scale,scale);
    shader.uniform(uniform_pos,pos_x,pos_y,pos_z);

    Mesh * mesh=meshes;
    while(mesh!=NULL)
    {
        if(mesh->index_count>0)
        {
            mesh->material->texture.bind();
            shader.uniform(uniform_texture,0);
            shader.uniform(uniform_diffuse, mesh->material->r,
                           mesh->material->g, mesh->material->b);

            glDrawElements(GL_TRIANGLES,mesh->index_count,
                           GL_UNSIGNED_INT, OPENGL_OFFSET(mesh->start_index*sizeof(GLuint)));
        }

        mesh=mesh->next;
    }

    glDisableVertexAttribArray(attribute_v_coord);
}

void ObjLoader::loadMTL(const std::string & file_name,const std::string & folder)
{
    std::ifstream file;
    file.open(file_name.c_str());
    if(!file.is_open())
        std::cerr<<"Unable to open mtl:"<<file_name<<std::endl;

    std::string line;
    std::stringstream str;
    std::string tmp;
    std::string tmp_name;

    Material * material=NULL;


    while(getline(file,line))
    {
        str.clear();
        str.str("");
        str<<line;

        str>>tmp;
        //std::cout<<tmp<<std::endl;
        if(tmp.compare("newmtl")==0)
        {
            material=new Material;
            material->next=materials;
            materials=material;
            material_count++;
            str>>material->name;
            material->index_count=0;
            material->start_index=0;
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
                std::cout<<tmp_name<<std::endl;
                material->texture.load(tmp_name.c_str(),GL_LINEAR_MIPMAP_NEAREST,GL_REPEAT);
            }
            else if(tmp.compare("map_d")==0)
            {

            }
            else if(tmp.compare("Kd")==0)
            {
                float r=0,g=0,b=0;
                str>>r>>g>>b;
                material->r=r*0.9;
                material->g=g*0.9;
                material->b=b*0.9;
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

void ObjLoader::loadObject()
{
}

bool ObjLoader::loadShader()
{
   // if(tex)
    //{
        const char * vertex_src =
            "attribute vec3 coord3d;\n"
            "attribute vec3 normal;\n"
            "attribute vec2 texcoord;\n"
            "uniform mat4 mvp;\n"
            "uniform float scale;\n"
            "uniform vec3 pos;\n"
            "varying vec3 f_normal;\n"
            "varying vec2 f_texcoord;\n"
            "void main(void)\n"
            "{\n"
            "   f_normal=normal;\n"
            "   f_texcoord=texcoord;\n"
            "   gl_Position = mvp * vec4(coord3d*scale+pos, 1.0);\n"
            "}\n";

        const char * fragment_src =
            "varying vec3 f_normal;\n"
            "varying vec2 f_texcoord;\n"
            "uniform sampler2D texture;\n"
            "uniform vec3 diffuse;\n"
            "void main(void) \n"
            "{\n"
            "   float fDiffuseIntensity = max(0.0, dot(normalize(f_normal), -vec3(0.5,0.5,0.5)) ); \n"
            "   vec4 vTexColor=texture2D(texture,f_texcoord);\n"
            "   gl_FragColor = vTexColor*vec4(diffuse*(0.5+fDiffuseIntensity), 1.0);\n"
            "}\n";
        if(!shader.load(vertex_src,fragment_src))
            return false;

        if((uniform_texture = shader.getUniform("texture"))==-1)
            return false;
 /*   }
    else
    {
        const char * vertex_src =
                "attribute vec3 coord3d;\n"
                "attribute vec3 normal;\n"
                "varying vec3 f_normal;\n"
                "uniform mat4 mvp;\n"
                "void main(void) {\n"
                "   f_normal=normal;\n"
                "   gl_Position = mvp * vec4(coord3d, 1.0);\n"
                "}\n";

        const char * fragment_src =
                "varying vec3 f_normal;\n"
                "void main(void) {\n"
                "   float fDiffuseIntensity = max(0.0, dot(normalize(f_normal), -vec3(0.5,0.5,0.5))); \n"
                "   vec4 vTexColor=vec4(0.5,0.5,0.5,1.0);\n"
                "   gl_FragColor = vTexColor*vec4(vec3(0.9,0.9,0.9)*(0.5+fDiffuseIntensity), 1.0);\n"
                "}\n";
        if(!shader.load(vertex_src,fragment_src))
            return false;
    }*/

    if((attribute_v_coord = shader.getAttribute("coord3d"))==-1)
        return false;
    if((attribute_normal = shader.getAttribute("normal"))==-1)
        return false;
    if((attribute_texcoord = shader.getAttribute("texcoord"))==-1)
        return false;


    if((uniform_mvp = shader.getUniform("mvp"))==-1)
        return false;

    if((uniform_scale = shader.getUniform("scale"))==-1)
        return false;

    if((uniform_pos = shader.getUniform("pos"))==-1)
        return false;
    if((uniform_diffuse = shader.getUniform("diffuse"))==-1)
        return false;
    return true;

}

void ObjLoader::setPosition(float x, float y, float z)
{
    pos_x=x;
    pos_y=y;
    pos_z=z;
}
