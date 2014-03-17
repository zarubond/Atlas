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
#include "dialogterrainprocedural.h"
#include "ui_dialogterrainprocedural.h"

#include "../terraintool.h"

DialogTerrainProcedural::DialogTerrainProcedural(TerrainIO *terrain_io, TerrainTool *parent) :
    QDialog(parent),
    ui(new Ui::DialogTerrainProcedural)
{
    ui->setupUi(this);

    this->tool=parent;
    width=parent->getTerrain()->regionsX()*parent->getTerrain()->regionSize();
    depth=parent->getTerrain()->regionsZ()*parent->getTerrain()->regionSize();
    max_height=parent->getTerrain()->regionsY()*parent->getTerrain()->regionSize();
    min_height=0;

    data=new double[width*depth];
    for(int i=0;i<width*depth;i++)
        data[i]=-1;

    updateImage();

    QFileInfo info;
    texture_vertical=1;

    for(int i=0;i<parent->getTerrain()->maxTextures();i++)
    {
        if(!terrain_io->getTexturePath(i)->isEmpty())
        {
            info.setFile(*terrain_io->getTexturePath(i));
            ui->combo_texture->addItem(info.fileName(),QVariant(i+1));
            ui->combo_texture2->addItem(info.fileName(),QVariant(i+1));
        }
    }
}

DialogTerrainProcedural::~DialogTerrainProcedural()
{
    delete ui;
}

void DialogTerrainProcedural::updateImage()
{
    QImage image(width, depth,QImage::Format_RGB888);

    for(int i=0;i<width;i++)
        for(int j=0;j<depth;j++)
        {
            float value=data[i*width+j];
            if(value>1.0 || value<-1.0)
                std::cout<<value<<std::endl;
            value+=1;
            value/=2;
            value=255-(value/1)*255;
            image.setPixel(i,j, qRgb(value,value,value));
        }

    ui->noise_image->setPixmap(QPixmap::fromImage(image));

}

void DialogTerrainProcedural::disable()
{
    ui->verticalLayout_2->setEnabled(false);
}

void DialogTerrainProcedural::enable()
{
    ui->verticalLayout_2->setEnabled(true);
}

void DialogTerrainProcedural::on_buttonBox_accepted()
{
    disable();
    TerrainGraphics * terrain=tool->getTerrain();
    int max=terrain->regionsY()*terrain->regionSize()-1;
    unsigned char tex=terrain->materialId(texture_vertical,texture_horizontal);
    for(int i=0; i<width;i++)
        for(int j=0; j<depth;j++)
        {
            float val=data[i*width+j];
            val+=1;
            val/=2;
            val*=max;

            float tmp;
            for(int h=0;h<max;h++)
            {
                tmp=float(h)-val;
                tmp*=16;
                if(tmp<-127)
                    tmp=-127;
                else if(tmp>127)
                    break;

                terrain->setVoxel(i,h,j,tmp,tex);
            }
        }
    terrain->rebuid();
    close();
}

void DialogTerrainProcedural::on_buttonBox_rejected()
{
    close();
}

void DialogTerrainProcedural::on_combo_texture_currentIndexChanged(int index)
{
    QVariant var=ui->combo_texture->itemData(index);
    texture_vertical=var.toInt();
}

void DialogTerrainProcedural::on_combo_texture2_currentIndexChanged(int index)
{
    QVariant var=ui->combo_texture2->itemData(index);
    texture_horizontal=var.toInt();
}

void DialogTerrainProcedural::on_pushNoise_clicked()
{
    disable();
    PerlinNoise perlin;
    int seed=ui->spinSeed->value();
    float freq=ui->doubleFrequency->value();
    perlin.setSeed(seed);
    perlin.initGradients();

    for(int i=0; i<width;i++)
        for(int j=0; j<depth;j++)
        {
            float value=perlin.noise(freq * i / (float)width, freq * j / (float)depth, 0);

            data[i*width+j]=value;
            if(value<-0.9)
                std::cout<<value<<std::endl;
        }
    updateImage();
    enable();
}

void DialogTerrainProcedural::perturb(float f, float d)
{
    PerlinNoise perlin;
    perlin.setSeed(ui->spinSeed->value());
    perlin.initGradients();

    int u, v;
    float * temp = new float[width*depth];
    for (int i = 0; i < width; ++i)
         for (int j = 0; j < depth; ++j)
         {
              u = i + (int)(perlin.noise(f * i / (float)width, f * j / (float)depth, 0) * d);
              v = j + (int)(perlin.noise(f * i / (float)width, f * j / (float)depth, 1) * d);
              if (u < 0) u = 0; if (u >= width) u = width - 1;
              if (v < 0) v = 0; if (v >= depth) v = depth - 1;
              temp[i*width+j] = data[u*width+v];
         }
    for(int i=0;i<width*depth;i++)
        data[i]=temp[i];

    delete temp;
}

void DialogTerrainProcedural::on_pushErode_clicked()
{
    disable();
    float smoothness=ui->spinErodeSmooth->value();
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < depth - 1; j++)
        {
            float d_max = 0.0f;
            int match[] = { 0, 0 };

            for (int u = -1; u <= 1; u++)
            {
                for (int v = -1; v <= 1; v++)
                {
                    if(abs(u) + abs(v) > 0)
                    {
                        float d_i = data[i*width+j] -data[(u+i)*width+j+v];
                        if (d_i > d_max)
                        {
                            d_max = d_i;
                            match[0] = u; match[1] = v;
                        }
                    }
                }
            }

            if(0 < d_max && d_max <= (smoothness / (float)width))
            {
                float d_h = 0.5f * d_max;
                data[i*width+j]-= d_h;
                data[(i+match[0])*width+j+match[1]]+=d_h;
            }
        }
    }

    updateImage();
    enable();
}


void DialogTerrainProcedural::on_pushSmooth_clicked()
{
    disable();
    for (int i = 1; i < width - 1; ++i)
    {
        for (int j = 1; j < depth - 1; ++j)
        {
            float total = 0.0f;
            for (int u = -1; u <= 1; u++)
            {
                for (int v = -1; v <= 1; v++)
                {
                    total += data[(u+i)*width+j+v];
                }
            }

            data[i*width+j] = total / 9.0f;
        }
    }
    updateImage();
    enable();
}

void DialogTerrainProcedural::on_pushButton_clicked()
{
    disable();
    float freq=ui->doublePerturbFreq->value();
    float distance=ui->spinPertrubDist->value();
    perturb(freq,distance);
    updateImage();
    enable();
}

void DialogTerrainProcedural::on_pushButton_2_clicked()
{
    disable();
    DiamondSquare noise;
    noise.run(ui->spinSeed->value(),data,width,depth);
    updateImage();
    enable();

}
