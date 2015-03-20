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

#include "mesh3ds.h"
#include "mesh.h"

Mesh3ds::Mesh3ds()
{
}

bool Mesh3ds::load(const char *file_name, Mesh *mesh)
{
    file=fopen(file_name,"rb");
    if(file==NULL)
    {
        cerr<<"Unable to open mesh: "<<file_name<<endl;
        return false;
    }

    std::string tmp_name(file_name);
    size_t slash=tmp_name.find_last_of("/\\");
    folder=tmp_name.substr(0,slash);
    folder+="/";

    uv_count=0;
    vertex_count=0;
    normal_count=0;
    indices_count=0;

    vertices=NULL;
    uv=NULL;
    normals=NULL;
    indices=NULL;
    this->mesh=NULL;

    Chunk chunk;
    readChunk(&chunk);
    if (chunk.id != 0x4D4D) //MAIN CHUNK
    {
        cout<<"Invalid 3ds format"<<file_name;
        return false;
    }
    this->mesh=mesh;

    process(&chunk);
    return true;
}

std::vector<std::string> Mesh3ds::meshFiles(const char *file_name)
{
    std::vector<std::string> files;
    return files;
}

std::vector<std::string> Mesh3ds::meshParts(const char *file_name)
{
    std::vector<std::string> files;
    return files;
}

void Mesh3ds::readChunk(Mesh3ds::Chunk * chunk)
{
    chunk->read = 0;
    chunk->read = fread(&chunk->id, 1, 2, file);
    chunk->read += fread(&chunk->length, 1, 4, file);
}

void Mesh3ds::process(Mesh3ds::Chunk *parent)
{
    Chunk chunk;
    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0x0002: //VERSION
        {
            unsigned int version = 0;
            chunk.read += fread(&version, 1, chunk.length - chunk.read, file);
            if (version > 0x03)
                cerr << "This 3DS file is over version 3 so it may load incorrectly" << endl;

            break;
        }
        case 0x3D3D: //3D EDITOR CHUNK
        {
            processEditorChunk(&chunk);
            break;
        }
        case 0xB000://KEYFRAMER CHUNK
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }
        parent->read += chunk.read;
    }

    computeNormals();
    //make normals

    mesh->setVertices(vertices,vertex_count);
    mesh->setNormals(normals,normal_count);
    mesh->setTexturesUV(uv, uv_count);
    mesh->setIndices(indices,indices_count);
}

void Mesh3ds::processEditorChunk(Mesh3ds::Chunk *parent)
{
    Chunk chunk;
    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0xAFFF: //MATERIAL BLOCK
        {
            //new material
            MeshMaterial * material=mesh->createMaterial();
            processMaterialChunk(&chunk,material);

            break;
        }
        case 0x4000: //OBJECT BLOCK
        {
            //new object
            char name[255];
            readString(&chunk,name);
            MeshPart * part=mesh->createPart(NULL,0,0);
            processObjectChunk(&chunk,part);
            break;
        }
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }
        parent->read += chunk.read;
    }
}

void Mesh3ds::processMaterialChunk(Mesh3ds::Chunk *parent ,MeshMaterial * material)
{
    Chunk chunk;

    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0xA000: // MATERIAL NAME
            chunk.read += fread(buffer, 1, chunk.length - chunk.read, file);
            material->name=buffer;
            cout<<material->name<<endl;
            break;
        case 0xA200: // TEXTURE MAP 1
            processMappingChunk(&chunk, material);
            break;
        case 0xA010: // AMBIENT COLOR
        case 0xA020: // DIFFUSE COLOR
        case 0xA030: // SPECULAR COLOR
        case 0xA230: // BUMP MAP
        case 0xA220: // REFLECTION MAP
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }

        parent->read += chunk.read;
    }
}

void Mesh3ds::processMappingChunk(Mesh3ds::Chunk *parent ,MeshMaterial * material)
{
    Chunk chunk;

    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0xA300: //MAPPING FILENAME
        {
            cout<<chunk.length - chunk.read<<endl;
            chunk.read += fread(buffer, 1, chunk.length - chunk.read, file);
            cout<<buffer<<endl;

            std::string tmp_name=folder,tmp=buffer;
            if(tmp[0]=='.' && tmp[1]=='/')
                tmp_name+=tmp.erase(0,2);
            else
                tmp_name+=tmp;
            material->setTexture(tmp_name.c_str());
        }
            break;
        case 0xA351: //MAPPING PARAMETERS
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }

        parent->read += chunk.read;
    }
}

void Mesh3ds::processObjectChunk(Mesh3ds::Chunk *parent ,MeshPart * part)
{
    Chunk chunk;
    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0x4100: //TRIANGULAR MESH
            processMeshChunk(&chunk,part);
            break;
        case 0x4600: //LIGHT
        case 0x4700: //CAMERA
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }

        parent->read += chunk.read;
    }
}

void Mesh3ds::processMeshChunk(Mesh3ds::Chunk *parent, MeshPart * part)
{
    Chunk chunk;
    while (parent->read < parent->length)
    {
        readChunk(&chunk);

        switch (chunk.id)
        {
        case 0x4110: //VERTICES LIST
            readVertices(&chunk);
            break;
        case 0x4120: //FACES DESCRIPTION
            readIndices(&chunk,part);
            break;
        case 0x4130: //FACES MATERIAL
            readFaceMaterial(&chunk, part);
            break;
        case 0x4140: //MAPPING COORDINATES LIST
            readUVCoordinates(&chunk);
            break;
        default:
            fseek(file,chunk.length - chunk.read,SEEK_CUR);
            chunk.read += chunk.length - chunk.read;
            break;
        }
        parent->read += chunk.read;
    }
    //mesh

}

void Mesh3ds::processColorChunk(Mesh3ds::Chunk *parent)
{
    Chunk chunk;
    readChunk(&chunk);

    // Read in the R G B color (3 bytes - 0 through 255)
    chunk.read += fread(buffer, 1, chunk.length - chunk.read, file);

    // Add the bytes read to our chunk
    parent->read += chunk.read;
}

void Mesh3ds::readVertices(Mesh3ds::Chunk *parent)
{
    unsigned short num=0;
    parent->read += fread(&num, 1, sizeof(unsigned short), file);

    float * tmp = new float [(vertex_count+num)*3];
    for(int i=0; i<vertex_count; i++) tmp[i]=vertices[i];
    delete [] vertices;
    vertices=tmp;

    parent->read += fread(&vertices[vertex_count], 1, parent->length - parent->read, file);

    //swap -z and y
    float t;
    for(int i = 0; i < num; i++)
    {
        t = vertices[vertex_count*3+i*3+1];
        vertices[vertex_count*3+i*3+1] = vertices[vertex_count*3+i*3+2];
        vertices[vertex_count*3+i*3+2] = -t;
    }

    vertex_count+=num;
}

void Mesh3ds::readIndices(Mesh3ds::Chunk *parent,MeshPart * part)
{
    unsigned short index = 0;
    unsigned short num;
    parent->read += fread(&num, 1, 2, file);

    unsigned short * tmp = new unsigned short [num*3];
    for(int i=0;i<indices_count;i++) indices[i]=tmp[i];
    delete [] indices;
    indices=tmp;

    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            parent->read += fread(&index, 1, sizeof(index), file);
            indices[indices_count*3+i*3+j] = index;
        }
        parent->read += fread(&index, 1, sizeof(index), file);
    }

    part->start_index=indices_count;
    part->index_count=num*3;
    indices_count+=num*3;
}

void Mesh3ds::readUVCoordinates(Mesh3ds::Chunk *parent)
{
    unsigned short num;
    parent->read += fread(&num, 1, 2, file);

    float * tmp=new float[(uv_count+num)*2];
    for(int i=0;i<uv_count;i++) tmp[i]=uv[i];
    delete [] uv;
    uv=tmp;

    parent->read += fread(&uv[uv_count], 1, parent->length - parent->read, file);
    uv_count+=num;
}

void Mesh3ds::readFaceMaterial(Mesh3ds::Chunk *parent ,MeshPart * part)
{
    char mat_name[255];
    readString(parent, mat_name);

    for (std::vector<MeshMaterial*>::iterator it = mesh->materials.begin() ; it != mesh->materials.end(); ++it)
    {
        if(strcmp(mat_name, (*it)->name.c_str()) == 0)
        {
            part->material=(*it);
            /*
            // Now that we found the material, check if it's a texture map.
            // If the strFile has a string length of 1 and over it's a texture
            if(strlen(pModel->pMaterials[i].strFile) > 0) {

                // Set the object's flag to say it has a texture map to bind.
                pObject->bHasTexture = true;
            }*/
            break;
        }
        else
        {
            part->material=NULL;
        }
    }

    fseek(file,parent->length - parent->read,SEEK_CUR);
    parent->read += parent->length - parent->read;//some shared vertices
}

void Mesh3ds::readString(Mesh3ds::Chunk *parent, char * str)
{
    int index = 0;

    parent->read += fread(str, 1, 1, file);

    while (*(str + index++) != 0)
    {
        parent->read += fread(str + index, 1, 1,file);
    }
}

void Mesh3ds::computeNormals()
{
    normals=new float[vertex_count*3];
    normal_count=vertex_count;
    for(int i=0;i<vertex_count*3;i++)
    {
        normals[i]=0;
    }
    Triangle tri;
    for(int i=0;i<indices_count;i+=3)
    {
        tri.set(Vector3f(vertices[(indices[i+2])*3+0],vertices[(indices[i+2])*3+1],vertices[(indices[i+2])*3+2]),
                Vector3f(vertices[(indices[i+1])*3+0],vertices[(indices[i+1])*3+1],vertices[(indices[i+1])*3+2]),
                Vector3f(vertices[(indices[i+0])*3+0],vertices[(indices[i+0])*3+1],vertices[(indices[i+0])*3+2]));
        Vector3f normal=tri.normal();
        normals[(indices[i+0])*3+0]=normal[0];
        normals[(indices[i+0])*3+1]=normal[1];
        normals[(indices[i+0])*3+2]=normal[2];

        normals[(indices[i+1])*3+0]=normal[0];
        normals[(indices[i+1])*3+1]=normal[1];
        normals[(indices[i+1])*3+2]=normal[2];

        normals[(indices[i+2])*3+0]=normal[0];
        normals[(indices[i+2])*3+1]=normal[1];
        normals[(indices[i+2])*3+2]=normal[2];
    }
}
