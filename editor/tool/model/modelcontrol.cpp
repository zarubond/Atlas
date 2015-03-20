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

#include "modelcontrol.h"

ModelControl::ModelControl(QQuickItem *parent) :
    ToolControl(parent), m_treeModel(new QStandardItemModel)
{
    left_press=false;
    mouse_x=0;
    mouse_y=0;

    connect(m_treeModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(modelChanged(QStandardItem*)));

    QHash<int, QByteArray> roleNames;
    roleNames[ModelControl::TextRole] =  "title";
    roleNames[ModelControl::ElementRole] = "elements";
    m_treeModel->setItemRoleNames(roleNames);

    m_mapWidth=0;
    m_mapHeight=0;
    m_mapDepth=0;

    m_modelOperation=POSITION;

    axis_select=-1;

    connect(&m_model,SIGNAL(nameChanged(QString)),this,SLOT(updateModelName(QString)));
    connect(&m_mesh,SIGNAL(nameChanged(QString)),this, SLOT(updateMeshName(QString)));

    connect(&m_model,SIGNAL(positionXChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(positionYChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(positionZChanged(float)),this,SLOT(modelChanged()));

    connect(&m_model,SIGNAL(rotationXChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(rotationYChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(rotationZChanged(float)),this,SLOT(modelChanged()));

    connect(&m_model,SIGNAL(scaleXChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(scaleYChanged(float)),this,SLOT(modelChanged()));
    connect(&m_model,SIGNAL(scaleZChanged(float)),this,SLOT(modelChanged()));

}

ModelControl::~ModelControl()
{
    delete m_treeModel;
}

void ModelControl::setup(Instrument * navig)
{
    this->navig=navig;
}

void ModelControl::mousePress(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton && left_press!=true)
    {
        EditorGraphics::Axis axis=EditorGraphics::Axis::NONE;
        if(m_model.model()!=NULL)
        {
            Vector3f dir=camera.unproject(event->x(), event->y(), 0.0);
            dir.normalize();
            dir*=3000;
            Vector3d end(camera.position());
            end+=dir;

            axis=editor->collision(camera.position(), end);
            switch(axis)
            {
            case EditorGraphics::Axis::X: axis_select=0; break;
            case EditorGraphics::Axis::Y: axis_select=1; break;
            case EditorGraphics::Axis::Z: axis_select=2; break;
            case EditorGraphics::Axis::NONE: axis_select=-1; break;
            }

            if(axis_select!=-1)
            {
                if(editor->axis.type()==EditorPositioning::POSITION)
                {
                    if(axis_select==1)
                    {
                        Vector3f dir0=camera.position();
                        float t=(m_model.model()->position()[0]-dir0[0])/dir[0];
                        pos_dif[0]=m_model.model()->position()[0]-(dir0[0]+dir[0]*t);
                        pos_dif[1]=m_model.model()->position()[1]-(dir0[1]+dir[1]*t);
                        pos_dif[2]=m_model.model()->position()[2]-(dir0[2]+dir[2]*t);
                    }
                    else
                    {
                        Vector3f dir0=camera.position();
                        float t=(m_model.model()->position()[1]-dir0[1])/dir[1];
                        pos_dif[0]=m_model.model()->position()[0]-(dir0[0]+dir[0]*t);
                        pos_dif[1]=m_model.model()->position()[1]-(dir0[1]+dir[1]*t);
                        pos_dif[2]=m_model.model()->position()[2]-(dir0[2]+dir[2]*t);
                    }
                }
                else if(editor->axis.type()==EditorPositioning::SCALE)
                {
                    orig_scale=m_model.model()->scale();
                    model_center=m_model.model()->center();
                    axis_dist=model_center.distance(end);
                }
            }
        }

        if(axis==EditorGraphics::Axis::NONE)
        {
            Model * model=findModel(event->x(),event->y());

            if(model!=NULL)
            {
                selectModel(model);

                QStandardItem* imodel,* imesh;

                for(int i=0;i<m_treeModel->rowCount();i++)
                {
                    imesh=m_treeModel->item(i);
                    for( int row = 0; row < imesh->rowCount(); row++ )
                    {
                        imodel=imesh->child(row,0);
                        Model *tmp=(Model*)imodel->data(ModelControl::ElementRole).value<void*>();
                        if(tmp==model)
                        {
                            //little hack dont know how to pass selected item in better way
                            emit selectModel(imesh->row(),imodel->row());
                            break;
                        }
                    }
                }

                Vector3f dir0=camera.position();
                Vector3f dir;
                dir=camera.unproject(event->x(),event->y(),0.0);
                float t=(m_model.model()->position()[1]-dir0[1])/dir[1];
                pos_dif[0]=m_model.model()->position()[0]-(dir0[0]+dir[0]*t);
                pos_dif[1]=m_model.model()->position()[1]-(dir0[1]+dir[1]*t);
                pos_dif[2]=m_model.model()->position()[2]-(dir0[2]+dir[2]*t);
            }
            else
            {
                deselectModel();
                QStandardItem* imesh;
                for(int i=0;i<m_treeModel->rowCount();i++)
                {
                    imesh=m_treeModel->item(i);
                    if(m_mesh.mesh()==(ModelMesh*)imesh->data(ModelControl::ElementRole).value<void*>())
                    {
                        emit selectMesh(imesh->row());
                        break;
                    }
                }
            }
        }

        left_press=true;
        mouse_x=event->x();
        mouse_y=event->y();
    }
}

void ModelControl::mouseRelease(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        axis_select=-1;
        left_press=false;
        mouse_x=event->x();
        mouse_y=event->y();
    }
}

void ModelControl::mouseDoubleClick(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(m_mesh.mesh()!=NULL)
        {
            Vector3f dir=camera.unproject(event->x(),event->y(),0.0);
            dir.normalize();
            dir*=1000;
            Vector3f end(camera.position());
            end+=dir;
            Collision col =map->collisionVisible(camera.position(),end,camera);
            if(col.object!=NULL)
            {
                Model * new_model=new Model(base);
                new_model->setName("model");
                new_model->setScale(Vector3f(m_mesh.mesh()->default_scale, m_mesh.mesh()->default_scale,
                                             m_mesh.mesh()->default_scale));
                new_model->setPosition(col.collision);
                new_model->setMesh(m_mesh.mesh());

                if(map->insertObject(new_model))
                {
                    QStandardItem * item=new QStandardItem();
                    item->setData(new_model->getName(), ModelControl::TextRole);
                    QVariant tmp=qVariantFromValue((void *)new_model);
                    item->setData(tmp,ModelControl::ElementRole);
                    QStandardItem * it=m_treeModel->itemFromIndex(m_meshIndex);
                    it->appendRow(item);
                    selectModel(new_model);
                    emit selectModel(m_meshIndex.row(),item->row());
                }
                else
                {
                    delete new_model;
                }
            }
        }
    }
}

void ModelControl::mouseMove(QMouseEvent * event)
{
    if(m_model.model()!=NULL && left_press && axis_select!=-1)
    {
            Vector3f dir0=camera.position();
            Vector3f pos,dir;
            dir=camera.unproject(event->x(),event->y(),0.0);


        if(editor->axis.type()==EditorPositioning::POSITION)
        {
            if(axis_select==0)
            {
                float t=(m_model.model()->position()[1]-dir0[1])/dir[1];
                pos[0]=dir0[0]+dir[0]*t+pos_dif[0];
                pos[1]=dir0[1]+dir[1]*t+pos_dif[1];
                pos[2]=m_model.model()->position()[2];
            }
            else if(axis_select==1)
            {
                float t=(m_model.model()->position()[0]-dir0[0])/dir[0];
                pos[0]=m_model.model()->position()[0];
                pos[1]=dir0[1]+dir[1]*t+pos_dif[1];
                pos[2]=m_model.model()->position()[2];
            }
            else if(axis_select==2)
            {
                float t=(m_model.model()->position()[1]-dir0[1])/dir[1];
                pos[0]=m_model.model()->position()[0];
                pos[1]=dir0[1]+dir[1]*t+pos_dif[1];
                pos[2]=dir0[2]+dir[2]*t+pos_dif[2];
            }

            if(pos[0]<0)
                pos[0]=0;
            if(pos[1]<0)
                pos[1]=0;
            if(pos[2]<0)
                pos[2]=0;

            if(pos[0] > map->width())
                pos[0] = map->width();
            if(pos[1] > map->height())
                pos[1] = map->height();
            if(pos[2] > map->depth())
                pos[2] = map->depth();

            m_model.setPosition(pos);
        }
        else if(editor->axis.type()==EditorPositioning::SCALE)
        {

            if(axis_select==1)
            {
                float t=(model_center[0]-dir0[0])/dir[0];
                pos[0]=dir0[0]+dir[0]*t;
                pos[1]=dir0[1]+dir[1]*t;
                pos[2]=dir0[2]+dir[2]*t;
            }
            else
            {
                float t=(model_center[1]-dir0[1])/dir[1];
                pos[0]=dir0[0]+dir[0]*t;
                pos[1]=dir0[1]+dir[1]*t;
                pos[2]=dir0[2]+dir[2]*t;
            }

            float dist=(model_center.distance(pos))/axis_dist;

            if(axis_select==0)
            {
                m_model.setScaleX(dist*orig_scale[0]);
            }
            else if(axis_select==1)
            {
                m_model.setScaleY(dist*orig_scale[1]);
            }
            else if(axis_select==2)
            {
                m_model.setScaleZ(dist*orig_scale[2]);
            }
        }
    }
}

void ModelControl::loadProject(Project *project)
{
    base=&project->world.model;
    map=&project->world.map;
    model_io=&project->io->model_io;
    editor=&project->world.editor;

    setMapWidth(project->world.map.width());
    setMapHeight(project->world.map.height());
    setMapDepth(project->world.map.depth());

    reloadTree();
}
void ModelControl::unloadProject()
{
    m_treeModel->clear();
}

void ModelControl::onEnter()
{
    navig->hide();
}

void ModelControl::onLeave()
{
    deselectModel();
}

void ModelControl::update(int elapsed_time, const Player &player)
{
    camera=player.getCamera();
}

QStandardItemModel *ModelControl::treeModel() const
{
    return m_treeModel;
}

int ModelControl::mapWidth() const
{
    return m_mapWidth;
}

int ModelControl::mapHeight() const
{
    return m_mapHeight;
}

int ModelControl::mapDepth() const
{
    return m_mapDepth;
}

void ModelControl::selectModel(Model *model)
{
    deselectModel();
    if(map->removeObject(model))
    {
        map->insertOutOfTree(model);
        base->highlightModel(model);

        editor->axis.setVisible(true);
        editor->axis.setPosition(model->center());

        m_model.setModel(model);

    }
}

MeshDataModel *ModelControl::mesh()
{
    return &m_mesh;
}

ModelDataModel *ModelControl::model()
{
    return &m_model;
}

QModelIndex ModelControl::meshIndex() const
{
    return m_meshIndex;
}

QModelIndex ModelControl::modelIndex() const
{
    return m_modelIndex;
}

ModelControl::ModelOperation ModelControl::modelOperation() const
{
    return m_modelOperation;
}

void ModelControl::importMesh(QUrl path)
{
    std::string file_name=path.toLocalFile().toStdString();
    if(file_name.empty())
        return;
    ModelMesh * new_mesh=model_io->importMesh(file_name);

    if(new_mesh!=NULL)
    {
        QStandardItem * item=new QStandardItem();
        item->setData(new_mesh->getName().c_str(), ModelControl::TextRole);
        QVariant tmp=qVariantFromValue((void *)new_mesh);
        item->setData(tmp,ModelControl::ElementRole);
        m_treeModel->appendRow(item);
        emit selectMesh(item->row());
    }
    else
    {
        //error
    }
}

void ModelControl::modelChanged(QStandardItem *item)
{
    if(item!=NULL)
    {
        if(item->parent()==NULL)
        {
            ModelMesh * mesh=(ModelMesh *)item->data(ModelControl::ElementRole).value<void *>();
            mesh->setName(item->data(ModelControl::TextRole).toString().toUtf8().constData());
        }
        else
        {
            Model * model=(Model *)item->data(ModelControl::ElementRole).value<void *>();
            model->setName(item->data(ModelControl::TextRole).toString().toUtf8().constData());
        }
    }
}

void ModelControl::setMapWidth(int arg)
{
    if (m_mapWidth != arg) {
        m_mapWidth = arg;
        emit mapWidthChanged(arg);
    }
}

void ModelControl::setMapHeight(int arg)
{
    if (m_mapHeight != arg) {
        m_mapHeight = arg;
        emit mapHeightChanged(arg);
    }
}

void ModelControl::setMapDepth(int arg)
{
    if (m_mapDepth != arg) {
        m_mapDepth = arg;
        emit mapDepthChanged(arg);
    }
}

void ModelControl::setMeshIndex(const QModelIndex &arg)
{
    if (m_meshIndex == arg)
        return;

    m_meshIndex = arg;
    if(m_meshIndex.isValid())
    {
        QStandardItem * item=m_treeModel->itemFromIndex(arg);
        if(item!=NULL)
        {
            ModelMesh * mesh=static_cast<ModelMesh *>(item->data(ModelControl::ElementRole).value<void *>());
            m_mesh.setMesh(mesh);
        }
    }
    emit meshIndexChanged(arg);
}

void ModelControl::setModelIndex(const QModelIndex & arg)
{
    if (m_modelIndex == arg)
        return;

    m_modelIndex = arg;
    if(m_modelIndex.isValid())
    {
        QStandardItem * item=m_treeModel->itemFromIndex(m_modelIndex);
        if(item!=NULL)
        {
            Model * model=static_cast<Model*>(item->data(ModelControl::ElementRole).value<void *>());
            m_model.setModel(model);
        }
    }
    emit modelIndexChanged(arg);
}

void ModelControl::updateMeshName(const QString &name)
{
    QStandardItem * item=m_treeModel->itemFromIndex(m_meshIndex);
    if(item!=NULL)
    {
        item->setData(name, ModelControl::TextRole);
    }
}

void ModelControl::updateModelName(const QString &name)
{
    QStandardItem * item=m_treeModel->itemFromIndex(m_modelIndex);
    if(item!=NULL)
    {
        item->setData(name, ModelControl::TextRole);
    }
}

void ModelControl::setModelOperation(ModelControl::ModelOperation arg)
{
    if (m_modelOperation == arg)
        return;

    switch(arg)
    {
    case ModelControl::POSITION: editor->axis.setType(EditorPositioning::POSITION); break;
    case ModelControl::ROTATION: editor->axis.setType(EditorPositioning::ROTATION); break;
    case ModelControl::SCALE: editor->axis.setType(EditorPositioning::SCALE); break;
    }

    m_modelOperation = arg;
    emit modelOperationChanged(arg);
}

void ModelControl::modelChanged()
{
    if(m_model.model()!=NULL)
    {
        editor->axis.setPosition(m_model.model()->center());
        map->setChanged(true);
    }
}

Model *ModelControl::findModel(int x, int y)
{
    Vector3f dir=camera.unproject(x,y,0.0);
    dir.normalize();
    dir*=3000;
    Vector3f end(camera.position());
    end+=dir;
    Collision col =map->collisionVisible(camera.position(),end,camera);

    if(col.object!=NULL && col.object->type()==base->type())
    {
        return (Model*)col.object;
    }

    return NULL;
}

void ModelControl::deselectModel()
{
    if(m_model.model()!=NULL)
    {
        map->removeOutOfTree(m_model.model());
        map->insertObject(m_model.model());

        editor->axis.setVisible(false);
        base->highlightModel(NULL);
        m_model.setModel(NULL);
    }
}

void ModelControl::deleteMesh(const QModelIndex &index)
{
    if(!index.isValid()) return;

    QStandardItem * item=m_treeModel->itemFromIndex(index);
    if(item->parent()==NULL)
    {
        ModelMesh * mesh=(ModelMesh *)item->data(ModelControl::ElementRole).value<void *>();
        deselectModel();

        m_treeModel->removeRow(item->row());
        model_io->removeMesh(mesh);
    }
}

void ModelControl::duplicateModel()
{
    if(m_model.model()!=NULL)
    {
        Model * new_model=new Model(base);
        new_model->setName("model");
        new_model->setMesh(m_model.model()->getMesh());
        map->insertObject(new_model);
        QStandardItem * item=new QStandardItem();
        item->setData(new_model->getName(), ModelControl::TextRole);
        QVariant tmp=qVariantFromValue((void *)new_model);
        item->setData(tmp,ModelControl::ElementRole);
        QStandardItem * it=m_treeModel->itemFromIndex(m_modelIndex);
        it->parent()->appendRow(item);
        selectModel(new_model);
    }
}

void ModelControl::deleteModel(const QModelIndex &index)
{
    if(!index.isValid()) return;
    QStandardItem * item=m_treeModel->itemFromIndex(index);
    if(item->parent()!=NULL)
    {
        Model * model=(Model *)item->data(ModelControl::ElementRole).value<void *>();
        map->removeObject(model);
        item->parent()->removeRow(index.row());
        deselectModel();
        delete model;
    }
}

void ModelControl::newModel()
{/*
    QModelIndex index=ui->treeView->currentIndex();
    if(!index.isValid())
        return;
    QStandardItem * item=tree_model.itemFromIndex(index);
    if(item->parent()==NULL)
    {
        ModelMesh * mesh=(ModelMesh *)item->data().value<void *>();
        Model * new_model=new Model(base);
        new_model->setMesh(mesh);

        Vertex3f dir=canvas->getCamera()->unproject(canvas->width()/2,canvas->height()/2,0.0);
        dir.normalize();
        Vertex3f start(canvas->getCamera()->getX(),canvas->getCamera()->getY(),canvas->getCamera()->getZ()),
                end(canvas->getCamera()->getX() + 400*dir[2],canvas->getCamera()->getY() + 400*dir[1],canvas->getCamera()->getZ() + 400*dir[0]);

        Vertex3f c=map->collision(start,end).collision;
        new_model->setPosition(c);

        map->insertObject(new_model);

        QStandardItem * model_item=new QStandardItem("model");
        QVariant tmp=qVariantFromValue((void *)base);
        model_item->setData(tmp);

        item->appendRow(model_item);
        selectModel(new_model);
    }*/
}


void ModelControl::reloadTree()
{
    const std::set<ModelMesh *> & meshes=base->getMeshes();
    QVariant tmp;
    m_treeModel->clear();
    int items=0;

    for(std::set<ModelMesh *>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
    {
        ModelMesh* mesh=(*it);
        QStandardItem * item=new QStandardItem();
        item->setData(mesh->getName().c_str(), ModelControl::TextRole);
        tmp=qVariantFromValue((void *)mesh);
        item->setData(tmp,ModelControl::ElementRole);

        for(std::set<Model *>::const_iterator mod = mesh->getModels().begin(); mod != mesh->getModels().end(); ++mod)
        {
            Model* model=(*mod);
            QStandardItem * ite=new QStandardItem();
            if(strlen(model->getName())==0)
                model->setName("???");
            ite->setData(model->getName(), ModelControl::TextRole);
            tmp=qVariantFromValue((void *)model);
            ite->setData(tmp,ModelControl::ElementRole);

            item->appendRow(ite);
        }

        m_treeModel->setItem(items++,0,item);
    }
}

void ModelControl::addMesh(ModelMesh *mesh)
{/*
    ModelMesh* mesh=(*it);
    QStandardItem * item=new QStandardItem(mesh->getName());
    tmp=qVariantFromValue((void *)mesh);
    item->setData(tmp);
    tree_model.appendRow();*/
}
