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
#include "shader.h"

Shader::Shader()
{
    program=0;
}

bool Shader::load(const char *vertex_src, const char *fragment_src)
{
    setlocale(LC_NUMERIC, "C");
    GLuint vertexShader   = createShader ( vertex_src , GL_VERTEX_SHADER  );
    GLuint fragmentShader = createShader ( fragment_src , GL_FRAGMENT_SHADER );
    program  = glCreateProgram ();
    glAttachShader ( program, vertexShader );
    glAttachShader ( program, fragmentShader );
    glLinkProgram ( program );

    setlocale(LC_ALL, "");
    return true;
}

bool Shader::loadFile(const char *vertex_file, const char *fragment_file)
{
    setlocale(LC_NUMERIC, "C");
#ifdef QT_VERSION

    QFile v_file(vertex_file),f_file(fragment_file);
    if(!v_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr<<"Cannot open file:"<<vertex_file<<std::endl;
        return false;
    }

    QString str;
    str=v_file.readAll();
    v_file.close();

    GLuint vertexShader   = createShader ( str.toUtf8().constData() , GL_VERTEX_SHADER  );

    if(!f_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr<<"Cannot open file:"<<fragment_file<<std::endl;
        return false;
    }
    str=f_file.readAll();
    f_file.close();
    GLuint fragmentShader = createShader ( str.toUtf8().constData() , GL_FRAGMENT_SHADER );

#else
    char * text;
    text=readFile(vertex_file);
    if(text==NULL)
        return false;
        GLuint vertexShader   = createShader ( text , GL_VERTEX_SHADER  );     // load vertex shader
        delete [] text;
    if(text==NULL)
        return false;
        GLuint fragmentShader = createShader ( text , GL_FRAGMENT_SHADER );  // load fragment shader
        delete [] text;
#endif
    program  = glCreateProgram ();
    if(program==0)
    {
        std::cerr<<"Unable to create shader program!"<<std::endl;
        return false;
    }
    glAttachShader ( program, vertexShader );
    glAttachShader ( program, fragmentShader );

    glLinkProgram ( program );
    setlocale(LC_ALL, "");
    return true;
}

char * Shader::readFile(const char * file_name)
{
    FILE *file;
    long len;
    char *buf;
    file=fopen(file_name,"r");
    if(file!=NULL)
    {
        fseek(file,0,SEEK_END);
        len=ftell(file);
        fseek(file,0,SEEK_SET);
        buf=new char[len];
        fread(buf,len,1,file);
        fclose(file);
    }
    else
    {
        std::cerr<<"Unable to load shader file:"<<file_name<<std::endl;
        return NULL;
    }

    return buf;
}

GLint Shader::getAttribute(const char *name)
{
    GLint att= glGetAttribLocation(program, name);
    if(att==-1)
    {
        std::cerr<<"Unable to obtain attribute: "<<name<<std::endl;
        OpenGL::Error();
    }
    return att;
}

void Shader::bind()
{
    glUseProgram(program);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::printUniforms()
{
    std::cout<<"Active uniforms in shader: "<< program<<std::endl;
    int total = -1;
    glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &total );
    for(int i=0; i<total; ++i)  {
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform( program, GLuint(i), sizeof(name)-1,
            &name_len, &num, &type, name );
        name[name_len] = 0;
        std::cout<<"    Uniform: "<<name<<std::endl;
    }
}

GLuint Shader::createShader(const char *bytes, GLenum type)
{
    GLuint shader = glCreateShader(type);
#if OPENGL_VERSION == 2
    const char *src[] = {
    #ifdef GL_ES_VERSION_2_0
        "#version 100\n",
    #else
        "#version 120\n",
        //"#extension GL_EXT_texture_array : enable\n"
       // "#extension GL_EXT_gpu_shader4 : enable\n",//geometry shader
        //"#pragma optimize(off)\n",//to be removed
    #endif
    #ifdef GL_ES_VERSION_2_0
        type == GL_FRAGMENT_SHADER ?
        "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
        "precision highp float;\n"
        "#else\n"
        "precision mediump float;\n"
        "#endif\n"
        : "",
    #else
        "#define lowp\n"
        "#define mediump\n"
        "#define highp\n",
    #endif
        bytes
    };

    glShaderSource(shader, 3, src, NULL);
#else
    const char *src[] = {
        "#version 140\n"
        "#pragma optimize(off)\n",//to be removed
        bytes
    };

    glShaderSource(shader, 2, src, NULL);
#endif
    glCompileShader(shader);
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        std::cerr<<glGetString(GL_VENDOR)<<std::endl;
        //std::cerr<<src[0]<<src[1]<<src[2]<<std::endl;
        int line=printSource(src[0]);
        line=printSource(src[1],line);
        printSource(src[2],line);

        printLog(shader);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int Shader::printSource(const char * str,int line)
{
    int i=0;
    std::cout<<line<<". ";
    while(str[i]!='\0')
    {
        if(str[i]=='\n')
        {
            line++;
            putchar(str[i]);
            std::cout<<line<<". ";
        }
        else
            putchar(str[i]);
        i++;
    }
    std::cout<<std::endl;
    return line;
}

void Shader::printLog(GLuint object) {
    GLint logLength = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
    else {
        std::cerr << "printLog: Not a shader or a program\n";
        return;
    }
    char *log = (char *) malloc(logLength);
    if (glIsShader(object))
        glGetShaderInfoLog(object, logLength, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, logLength, NULL, log);
    std::cerr << log<<std::endl;
    free(log);
}

GLint Shader::getUniform(const char *name)
{
    GLint uni=glGetUniformLocation(program, name);
    if(uni==-1)
    {
        std::cerr<<"Unable to obtain uniform: "<<name<<std::endl;
        OpenGL::Error();
        printUniforms();
    }
    return uni;
}

bool Shader::getUniformLight(LightUniform &uniform, int id)
{
    char buffer[50];
    sprintf(buffer, "light[%d].pos",id);
    if((uniform.position = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].dir",id);
    if((uniform.direction = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].radius",id);
    if((uniform.radius = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].attenuation",id);
    if((uniform.attenuation = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].exponent",id);
    if((uniform.exponent = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].diffuse",id);
    if((uniform.diffuse = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].specular",id);
    if((uniform.specular = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].shininess",id);
    if((uniform.shininess = getUniform(buffer))==-1)
        return false;
    sprintf(buffer, "light[%d].type",id);
    if((uniform.type = getUniform(buffer))==-1)
        return false;

    return true;
}


void Shader::uniform(GLint location,const GLfloat v0)
{
    glUniform1f(location,v0);
}

void Shader::uniform(GLint location, GLint v0)
{
    glUniform1i(location,v0);
}

void Shader::uniform(GLint location, GLuint v0)
{
#ifdef OPENGL_ES
    glUniform1i(location,v0);
#else
    glUniform1ui(location,v0);
#endif
}

void Shader::uniform(GLint location, GLfloat v0, GLfloat v1)
{
    glUniform2f(location,v0,v1);
}

void Shader::uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(location,v0,v1,v2);
}

void Shader::uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(location,v0,v1,v2,v3);
}

void Shader::uniform(GLint location, const Vertex3f &vert) const
{
    glUniform3f(location,vert[0],vert[1],vert[2]);
}

void Shader::uniform(GLint location, Vertex3f vert)
{
    glUniform3f(location,vert[0],vert[1],vert[2]);
}

void Shader::uniform(GLint location,const Vertex4f &vert) const
{
    glUniform4f(location,vert[0],vert[1],vert[2],vert[3]);
}
void Shader::uniform(GLint location, Vertex4f vert)
{
    glUniform4f(location,vert[0],vert[1],vert[2],vert[3]);
}

void Shader::uniformMatrix(GLint location, const Matrix4f &matrix) const
{
    glUniformMatrix4fv(location,1,GL_FALSE,matrix.array());
}

void Shader::uniformMatrix(GLint location,const Matrix3f & matrix) const
{
    glUniformMatrix3fv(location,1,GL_FALSE,matrix.array());
}

void Shader::uniformLight(const LightUniform &location,const Light & light) const
{
    uniform(location.position       ,light.getPosition());
    uniform(location.direction      ,light.getDirection());
    glUniform1f(location.radius     ,light.getRadius());
    uniform(location.attenuation    ,light.getAttenuation());
    glUniform1f(location.exponent   ,light.getExponent());
    uniform(location.diffuse        ,light.getDiffuse());
    uniform(location.specular       ,light.getSpecular());
    glUniform1f(location.shininess  ,light.getShininess());
    glUniform1i(location.type       ,light.getType());
}

