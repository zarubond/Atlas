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
    char * text;
    text=readFile(vertex_file);
    if(text==NULL)
        return false;
    GLuint vertexShader   = createShader ( text , GL_VERTEX_SHADER  );     // load vertex shader
    delete [] text;

    text=readFile(fragment_file);
    if(text==NULL)
        return false;
    GLuint fragmentShader = createShader ( text , GL_FRAGMENT_SHADER );  // load fragment shader
    delete [] text;

    program  = glCreateProgram ();
    if(program==0)
    {
        cerr<<"Unable to create shader program!"<<endl;
        return false;
    }
    glAttachShader ( program, vertexShader );
    glAttachShader ( program, fragmentShader );

    glLinkProgram ( program );
    //cout<<program<<" "<<vertex_file<<endl;
    setlocale(LC_ALL, "");
    return true;
}

char * Shader::readFile(const char * file_name)
{
    File file(file_name);
    char *buf;
    if(file.open(static_cast<File::Mode>(File::READ | File::TEXT)))
    {
        long len=file.size();
        buf=new char[len];
        memset(buf,0,len);
        file.read(buf,len,1);
        buf[len-1]=0;
        file.close();
    }
    else
    {
        cerr<<"Unable to load shader file:"<<file_name<<endl;
        return NULL;
    }

    return buf;
}

Attribute Shader::getAttribute(const char *name)
{
    GLint att= glGetAttribLocation(program, name);
    if(att==-1)
    {
        cerr<<"Unable to obtain attribute: "<<name<<endl;
    }
    return att;
}

void Shader::bind()
{
    glUseProgram(program);
}

void Shader::release()
{
    glUseProgram(0);
}

void Shader::printUniforms()
{
    cout<<"Active uniforms in shader: "<< program<<endl;
    int total = -1;
    glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &total );
    for(int i=0; i<total; ++i)  {
        int name_len=-1, num=-1;
        GLenum type = 0;
        char name[100];
        glGetActiveUniform( program, GLuint(i), sizeof(name)-1,
            &name_len, &num, &type, name );
        name[name_len] = 0;
        cout<<"    Uniform: "<<name<<endl;
    }
}

GLuint Shader::createShader(const char *bytes, GLenum type)
{
     GLuint shader = glCreateShader(type);
    const char *src[3];
    GLint srclen[3];

    if(glVersion()==OPENGL_ES_2_0)
    {
        src[0]= "#version 100\n";
        src[1]= type == GL_FRAGMENT_SHADER ?
                    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
                    "precision highp float;\n"
                    "#else\n"
                    "precision mediump float;\n"
                    "#endif\n"
                    : "";
        src[2]=bytes;
    }
    else if(glVersion()==OPENGL_ES_3_0)/// TODO
    {
        src[0]= "#version 300 es\n";
        src[1]= type == GL_FRAGMENT_SHADER ?
                    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
                    "precision highp float;\n"
                    "#else\n"
                    "precision mediump float;\n"
                    "#endif\n"
                    "#define varying in\n"
                    "#define texture2D texture\n"
                    "#define texture3D texture\n"
                    "#define texture2DArray texture\n"
                    "#define gl_FragData fragData\n"
                    "out vec4 fragData[3];\n"
                    :
                    "#define attribute in\n"
                    "#define varying out\n";
        src[2]=bytes;
    }
    else if(glVersion()==OPENGL_3_0)
    {
        src[0]= "#version 130\n"
                "#extension GL_EXT_texture_array : enable\n";
                //"#pragma optimize(off)\n";
        src[1]="";
        src[2]=bytes;
    }
    else
    {
        cerr<<"Undefined opengl version"<<endl;
        return 0;
    }

    srclen[0]=strlen(src[0]);
    srclen[1]=strlen(src[1]);
    srclen[2]=strlen(src[2]);

    glShaderSource(shader, 3, src, srclen);
    glCompileShader(shader);

    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        cerr<<"Vendor: "<<(const char *)glGetString(GL_VENDOR)<<endl;
        cerr<<"Renderer: "<<(const char *)glGetString(GL_RENDERER)<<endl;
        cerr<<"Version: "<<(const char *)glGetString(GL_VERSION)<<endl;
        //std::cerr<<src[0]<<src[1]<<src[2]<<std::endl;
        int line=printSource(src[0])+1;
        line=printSource(src[1],line)+1;
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
    cout<<line<<". ";
    while(str[i]!='\0')
    {
        if(str[i]=='\n')
        {
            line++;
            cout<<endl<<line<<". ";
        }
        else
            cout.putchar(str[i]);
        i++;
    }
    cout<<endl;
    return line;
}

void Shader::printLog(GLuint object) {
    GLint logLength = 0;
    if (glIsShader(object))
    {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
    }
    else if (glIsProgram(object))
    {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
    }
    else {
        cerr << "printLog: Not a shader or a program"<<endl;
        return;
    }
    char * log = new char[logLength];
    if (glIsShader(object))
        glGetShaderInfoLog(object, logLength, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, logLength, NULL, log);

    if(logLength>0)
        cerr<<log<<endl;
    else
        cerr<<"Unable to get shader log"<<endl;
    delete [] log;
}

Uniform Shader::getUniform(const char *name)
{
    GLint uni=glGetUniformLocation(program, name);
    if(uni==-1)
    {
        cerr<<"Unable to obtain uniform: "<<name<<endl;
        printUniforms();
    }
    return uni;
}

void Shader::enableAttribute(Attribute attrib)
{
    glEnableVertexAttribArray(attrib);
}

void Shader::disableAttribute(Attribute attrib)
{
    glDisableVertexAttribArray(attrib);
}

void Shader::setAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, GLuint offset)
{
    glVertexAttribPointer(index, size, type, GL_FALSE, stride,OPENGL_OFFSET(offset));
}

void Shader::uniform(Uniform location,const GLfloat v0)
{
    glUniform1f(location,v0);
}

void Shader::uniform(Uniform location, const GLint v0)
{
    glUniform1i(location,v0);
}

void Shader::uniform(Uniform location, const GLuint v0)
{
    if(glVersion()==OPENGL_ES_2_0)
        glUniform1i(location,v0);
    else
       glUniform1ui(location,v0);
}

void Shader::uniform(Uniform location, GLfloat v0, GLfloat v1)
{
    glUniform2f(location,v0,v1);
}

void Shader::uniform(Uniform location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(location,v0,v1,v2);
}

void Shader::uniform(Uniform location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(location,v0,v1,v2,v3);
}

void Shader::uniform(Uniform location, const Vector2f &vert)
{
    glUniform2fv(location,1,vert.array());
}

void Shader::uniform(Uniform location, const Vector3f &vert)
{
    //glUniform3f(location,vert[0],vert[1],vert[2]);
    glUniform3fv(location,1,vert.array());
}
void Shader::uniform(Uniform location,const Vector4f &vert)
{
    //glUniform4f(location,vert[0],vert[1],vert[2],vert[3]);
    glUniform4fv(location,1,vert.array());
}

void Shader::uniform(Uniform location, const ColorRGBA &color)
{
    glUniform4fv(location,1,color.array());
}

void Shader::uniform(Uniform location, GLsizei count, GLint *value)
{
    glUniform1iv(location,count,value);
}

void Shader::uniformMatrix(Uniform location, const Matrix4f &matrix)
{
    glUniformMatrix4fv(location,1,GL_FALSE,matrix.array());
}

void Shader::uniformMatrix(Uniform location,const Matrix3f & matrix)
{
    glUniformMatrix3fv(location,1,GL_FALSE,matrix.array());
}
