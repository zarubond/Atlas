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

#include "terrainimportdialog.h"

#include "terraincontrol.h"

TerrainImportDialog::TerrainImportDialog(QObject *parent):
    Dialog(parent),m_model(new QStandardItemModel)
{
    m_minHeight=0;
    m_maxHeight=0;
    m_verticalTexture=0;
    m_horizontalTexture=0;
    m_heightLimit=0;

    QHash<int, QByteArray> roleNames;
    roleNames[TerrainImportDialog::TextRole] =  "modelData";
    roleNames[TerrainImportDialog::MaterialIdRole] = "id";
    m_model->setItemRoleNames(roleNames);

    m_project=NULL;
    m_working=false;
    m_imageProvider=NULL;
}

TerrainImportDialog::~TerrainImportDialog()
{
    delete m_model;
}

void TerrainImportDialog::opened()
{
    QFileInfo info;

    for(int i=0;i<terrain->materialCount();i++)
    {
        TerrainMaterial * material=terrain->getMaterial(i);
        if(material!=NULL && material->enabled())
        {
            QStandardItem * item=new QStandardItem;
            info.setFile(material->texture_path.c_str());
            item->setData(material->name.c_str(),TextRole);
            item->setData(material->id(),MaterialIdRole);

            m_model->appendRow(item);
        }
    }
    emit modelChanged();

    m_supportedFormats=Image::supportedFormats().c_str();
    emit supportedFormatsChanged();

    setHeightLimit(terrain->regionsY()*terrain->regionSize());
    setMaxHeight(m_heightLimit);
    m_working=false;
    emit workingChanged(m_working);
}

QStandardItemModel *TerrainImportDialog::model() const
{
    return m_model;
}

Project *TerrainImportDialog::project() const
{
    return m_project;
}

bool TerrainImportDialog::working() const
{
    return m_working;
}

float TerrainImportDialog::minHeight() const
{
    return m_minHeight;
}

float TerrainImportDialog::maxHeight() const
{
    return m_maxHeight;
}

ImageProvider *TerrainImportDialog::imageProvider() const
{
    return m_imageProvider;
}

int TerrainImportDialog::verticalTexture() const
{
    return m_verticalTexture;
}

int TerrainImportDialog::horizontalTexture() const
{
    return m_horizontalTexture;
}

QString TerrainImportDialog::supportedFormats() const
{
    return m_supportedFormats;
}

float TerrainImportDialog::heightLimit() const
{
    return m_heightLimit;
}

void TerrainImportDialog::accept()
{
    if(image.isNull())
    {
        close();
        return;
    }
    terrain->clear();

    int width=terrain->regionsX()*terrain->regionSize();
    int depth=terrain->regionsZ()*terrain->regionSize();

    QImage img(image.scaled(width,depth));

    TerrainVoxel voxel;
    voxel.setTextureRawH(m_horizontalTexture,15);
    voxel.setTextureRawV(m_verticalTexture,15);

    int max=m_maxHeight-m_minHeight;
    float val, tmp;
    QRgb rgb;

    for(int i=0; i<width;i++)
        for(int j=0; j<depth;j++)
        {
            rgb=img.pixel(i,j);
            val=qGray(rgb);
            val/=255.0;
            val*=max;

            for(int h=0; h<m_minHeight; h++)
            {
                voxel.value=-127;
                terrain->setVoxel(i,h,j,&voxel);
            }

            for(int h=m_minHeight;h<max;h++)
            {
                tmp=float(h)-val;
                tmp*=32;

                if(tmp<-127)
                    tmp=-127;
                else if(tmp>127)
                    break;
                voxel.value=tmp;
                terrain->setVoxel(i,h,j,&voxel);

                if(tmp>0) break;
            }
        }
    terrain->rebuid();

    close();
}

void TerrainImportDialog::importImage(const QUrl &path)
{
    QUrl url(path);
    if(image.load(url.toLocalFile()) && !image.isNull())
    {
        QRgb col;
        int gray;
        int width = image.width();
        int height = image.height();
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                col = image.pixel(i, j);
                gray = qGray(col);
                image.setPixel(i, j, qRgb(gray, gray, gray));
            }
        }
        int w=terrain->regionsX()*terrain->regionSize();
        int h=terrain->regionsZ()*terrain->regionSize();
        m_imageProvider->setImage(image.scaled(w,h));
    }
}

void TerrainImportDialog::setModel(QStandardItemModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged();
    }
}

void TerrainImportDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        terrain=&m_project->world.terrain;
        emit projectChanged(arg);
    }
}

void TerrainImportDialog::setMinHeight(float arg)
{
    if (m_minHeight != arg) {
        m_minHeight = arg;
        emit minHeightChanged(arg);
    }
}

void TerrainImportDialog::setMaxHeight(float arg)
{
    if (m_maxHeight != arg) {
        m_maxHeight = arg;
        emit maxHeightChanged(arg);
    }
}

void TerrainImportDialog::setImageProvider(ImageProvider *arg)
{
    if (m_imageProvider != arg) {
        m_imageProvider = arg;
        emit imageProviderChanged(arg);
    }
}

void TerrainImportDialog::setVerticalTexture(int arg)
{
    if (m_verticalTexture != arg) {
        m_verticalTexture = arg;
        emit verticalTextureChanged(arg);
    }
}

void TerrainImportDialog::setHorizontalTexture(int arg)
{
    if (m_horizontalTexture != arg) {
        m_horizontalTexture = arg;
        emit horizontalTextureChanged(arg);
    }
}

void TerrainImportDialog::setHeightLimit(float arg)
{
    if (m_heightLimit != arg) {
        m_heightLimit = arg;
        emit heightLimitChanged(arg);
    }
}
