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

#include "terrainproceduraldialog.h"

#include "terraincontrol.h"

TerrainProceduralDialog::TerrainProceduralDialog(QObject *parent):
    Dialog(parent),m_model(new QStandardItemModel)
{
    m_project=NULL;
    m_imageProvider=NULL;

    m_maxHeight=0;
    m_minHeight=0;
    m_mapDepth=0;
    m_mapWidth=0;

    QHash<int, QByteArray> roleNames;
    roleNames[TerrainProceduralDialog::TextRole] =  "modelData";
    roleNames[TerrainProceduralDialog::MaterialIdRole] = "id";
    m_model->setItemRoleNames(roleNames);
    m_horizontalTexture=0;
    m_verticalTexture=0;
    data=NULL;
    m_working=false;
}

TerrainProceduralDialog::~TerrainProceduralDialog()
{
    delete [] data;
    delete m_model;
}

void TerrainProceduralDialog::opened()
{
    setMapWidth(terrain->regionsX()*terrain->regionSize());
    setMapDepth(terrain->regionsZ()*terrain->regionSize());
    setHeightLimit(terrain->regionsY()*terrain->regionSize());
    setMaxHeight(m_heightLimit);
    setMinHeight(0);

    m_imageProvider->setImage(QImage(m_mapWidth,m_mapDepth,QImage::Format_RGB888));

    QFileInfo info;

    m_model->clear();
    for(int i=0;i<terrain->materialCount();i++)
    {
        TerrainMaterial * material=terrain->getMaterial(i);
        if(material!=NULL && material->enabled())
        {
            QStandardItem * item=new QStandardItem;
            info.setFile(material->texture_path.c_str());
            if(material->name.empty())
                item->setData(info.fileName(),TextRole);
            else
                item->setData(material->name.c_str(),TextRole);
            item->setData(material->id(),MaterialIdRole);

            m_model->appendRow(item);

        }
    }
    setWorking(false);
    emit modelChanged();
}

QStandardItemModel *TerrainProceduralDialog::model() const
{
    return m_model;
}

bool TerrainProceduralDialog::working() const
{
    return m_working;
}

void TerrainProceduralDialog::setModel(QStandardItemModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged();
    }
}

void TerrainProceduralDialog::setWorking(bool arg)
{
    if (m_working != arg) {
        m_working = arg;
        emit workingChanged(arg);
    }
}

void TerrainProceduralDialog::updateImage()
{
    if(m_imageProvider==NULL) return;

    QImage image(m_mapWidth,m_mapDepth,QImage::Format_RGB888);
    for(int i=0;i<m_mapWidth;i++)
        for(int j=0;j<m_mapDepth;j++)
        {
            float value=data[i*m_mapWidth+j];
            value+=1.0;
            value/=2.0;
            value=255-(value)*255.0;
            image.setPixel(i,j, qRgb(value,value,value));
        }
    m_imageProvider->setImage(image);
}

void TerrainProceduralDialog::makePerlinNoise(float seed, float frequency)
{
    setWorking(true);

    PerlinNoise perlin;
    perlin.setSeed(seed);
    perlin.initGradients();

    for(int i=0; i<m_mapWidth;i++)
        for(int j=0; j<m_mapDepth;j++)
        {
            float value=perlin.noise(frequency * i / (float)m_mapWidth, frequency * j / (float)m_mapDepth, 0);

            data[i*m_mapWidth+j]=value;
        }
    updateImage();
    setWorking(false);
}

void TerrainProceduralDialog::makeDiamondNoise(float seed, float frequency)
{
    setWorking(true);
    DiamondSquare diamod;
    diamod.run(seed,data,m_mapWidth,m_mapDepth);
/*
    PerlinNoise perlin;
    perlin.setSeed(seed);
    perlin.initGradients();

    for(int i=0; i<m_mapWidth;i++)
        for(int j=0; j<m_mapDepth;j++)
        {
            float value=perlin.noise(frequency * i / (float)m_mapWidth, frequency * j / (float)m_mapDepth, 0);

            data[i*m_mapWidth+j]=value;
        }
  */
    updateImage();
    setWorking(false);
}

void TerrainProceduralDialog::makePertrub(float frequency, float distance)
{
    setWorking(true);
    PerlinNoise perlin;
    perlin.setSeed(1);
    perlin.initGradients();

    int u, v;
    float * temp = new float[m_mapWidth*m_mapDepth];
    for (int i = 0; i < m_mapWidth; ++i)
         for (int j = 0; j < m_mapDepth; ++j)
         {
              u = i + (int)(perlin.noise(frequency * i / (float)m_mapWidth, frequency * j / (float)m_mapDepth, 0) * distance);
              v = j + (int)(perlin.noise(frequency * i / (float)m_mapWidth, frequency * j / (float)m_mapDepth, 1) * distance);
              if (u < 0) u = 0; if (u >= m_mapWidth) u = m_mapWidth - 1;
              if (v < 0) v = 0; if (v >= m_mapDepth) v = m_mapDepth - 1;
              temp[i*m_mapWidth+j] = data[u*m_mapWidth+v];
         }
    for(int i=0;i<m_mapWidth*m_mapDepth;i++)
        data[i]=temp[i];

    delete temp;
    updateImage();
    setWorking(false);
}

void TerrainProceduralDialog::makeSmooth(float step)
{
    setWorking(true);
    int s=step;
    for (int i = s; i < m_mapWidth - s; ++i)
        for (int j = s; j < m_mapDepth - s; ++j)
        {
            float total = 0.0f;
            for (int u = -s; u <= s; u++)
            {
                for (int v = -s; v <= s; v++)
                {
                    total += data[(u+i)*m_mapWidth+j+v];
                }
            }

            data[i*m_mapWidth+j] = total / ((s*2+1)*(s*2+1));
        }
    updateImage();
    setWorking(false);
}

void TerrainProceduralDialog::makeErrode(float smoothness)
{
    setWorking(true);
    for (int i = 1; i < m_mapWidth - 1; i++)
        for (int j = 1; j < m_mapDepth - 1; j++)
        {
            float d_max = 0.0f;
            int match[] = { 0, 0 };

            for (int u = -1; u <= 1; u++)
            {
                for (int v = -1; v <= 1; v++)
                {
                    if(abs(u) + abs(v) > 0)
                    {
                        float d_i = data[i*m_mapWidth+j] -data[(u+i)*m_mapWidth+j+v];
                        if (d_i > d_max)
                        {
                            d_max = d_i;
                            match[0] = u; match[1] = v;
                        }
                    }
                }
            }

            if(0 < d_max && d_max <= (smoothness / (float)m_mapWidth))
            {
                float d_h = 0.5f * d_max;
                data[i*m_mapWidth+j]-= d_h;
                data[(i+match[0])*m_mapWidth+j+match[1]]+=d_h;
            }
        }

    updateImage();
    setWorking(false);
}

void TerrainProceduralDialog::setMinHeight(float arg)
{
    if (m_minHeight != arg) {
        m_minHeight = arg;
        emit minHeightChanged(arg);
    }
}

void TerrainProceduralDialog::setMaxHeight(float arg)
{
    if (m_maxHeight != arg) {
        m_maxHeight = arg;
        emit maxHeightChanged(arg);
    }
}

void TerrainProceduralDialog::setMapWidth(int arg)
{
    if (m_mapWidth != arg) {
        m_mapWidth = arg;

        delete [] data;
        data=new double[int(m_mapWidth*m_mapDepth)];
        for(int i=0;i<m_mapWidth*m_mapDepth;i++)
            data[i]=-1;
        updateImage();

        emit mapWidthChanged(arg);
    }
}

void TerrainProceduralDialog::setMapDepth(int arg)
{
    if (m_mapDepth != arg) {
        m_mapDepth = arg;

        delete [] data;
        data=new double[int(m_mapWidth*m_mapDepth)];
        for(int i=0;i<m_mapWidth*m_mapDepth;i++)
            data[i]=-1;
        updateImage();

        emit mapDepthChanged(arg);
    }
}

void TerrainProceduralDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        terrain=&m_project->world.terrain;
        emit projectChanged(arg);
    }
}

void TerrainProceduralDialog::setImageProvider(ImageProvider *arg)
{
    if (m_imageProvider != arg) {
        m_imageProvider = arg;
        emit imageProviderChanged(arg);
    }
}

void TerrainProceduralDialog::setVerticalTexture(int arg)
{
    if (m_verticalTexture != arg) {
        m_verticalTexture = arg;
        emit verticalTextureChanged(arg);
    }
}

void TerrainProceduralDialog::setHorizontalTexture(int arg)
{
    if (m_horizontalTexture != arg) {
        m_horizontalTexture = arg;
        emit horizontalTextureChanged(arg);
    }
}

void TerrainProceduralDialog::setHeightLimit(float arg)
{
    if (m_heightLimit != arg) {
        m_heightLimit = arg;
        emit heightLimitChanged(arg);
    }
}

void TerrainProceduralDialog::accept()
{
    setWorking(true);
    TerrainGraphics * terrain=&m_project->world.terrain;

    unsigned char texV=0,texH=0;
    QStandardItem * item;
    item=m_model->item(m_verticalTexture);
    if(item!=NULL)
        texV=item->data(MaterialIdRole).toInt();
    item=m_model->item(m_horizontalTexture);
    if(item!=NULL)
        texH=item->data(MaterialIdRole).toInt();

    TerrainVoxel voxel;
    voxel.setTextureRawH(texH,15);
    voxel.setTextureRawV(texV,15);

    terrain->clear();
    int max=m_maxHeight-m_minHeight;
    float height, tmp;
    for(int i=0; i<m_mapWidth;i++)
        for(int j=0; j<m_mapDepth;j++)
        {
            height=data[i*m_mapWidth+j];
            height+=1;
            height/=2;
            height*=max;

            for(int h=0; h<m_minHeight+floor(height); h++)
            {
                voxel.value=-127;
                terrain->setVoxel(i,h,j,&voxel);
            }

            tmp=float(floor(height))-height;
            voxel.value=tmp*64.0;
            terrain->setVoxel(i,m_minHeight+floor(height),j,&voxel);

            tmp=float(ceil(height))-height;
            voxel.value=tmp*64.0;
            terrain->setVoxel(i,m_minHeight+ceil(height),j,&voxel);
        }

    terrain->rebuid();
    setWorking(false);
    close();
}

float TerrainProceduralDialog::minHeight() const
{
    return m_minHeight;
}

float TerrainProceduralDialog::maxHeight() const
{
    return m_maxHeight;
}

int TerrainProceduralDialog::mapWidth() const
{
    return m_mapWidth;
}

int TerrainProceduralDialog::mapDepth() const
{
    return m_mapDepth;
}

Project *TerrainProceduralDialog::project() const
{
    return m_project;
}

ImageProvider *TerrainProceduralDialog::imageProvider() const
{
    return m_imageProvider;
}

int TerrainProceduralDialog::verticalTexture() const
{
    return m_verticalTexture;
}

int TerrainProceduralDialog::horizontalTexture() const
{
    return m_horizontalTexture;
}

float TerrainProceduralDialog::heightLimit() const
{
    return m_heightLimit;
}
