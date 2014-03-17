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
#include "modeltool.h"
#include "ui_modeltool.h"

ModelTool::ModelTool(QWidget *parent) :
    EditorTool(parent),
    ui(new Ui::ModelTool)
{
    ui->setupUi(this);
    select_model=NULL;
    left_press=false;
    mouse_x=0;
    mouse_y=0;

    items=0;

    connect(&tree_model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(modelChanged(QStandardItem*)));
}

ModelTool::~ModelTool()
{
    delete ui;
}

void ModelTool::setup(Canvas *space,Navigator * navig,Assets * assets)
{
    this->canvas=space;
    this->navig=navig;
    this->assets=assets;
    ui->treeView->setModel(&tree_model);
}

void ModelTool::mousePress(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton && left_press!=true)
    {
        Model * model=findModel(event->x(),event->y());

        if(model!=NULL)
        {
            selectModel(model);
            Camera * camera=canvas->getCamera();
            Vertex3f dir0=camera->getPosition();
            Vertex3f pos,dir;
            dir=camera->unproject(event->x(),event->y(),0.0);
            float t=(select_model->getPosition()[1]-dir0[1])/dir[1];
            pos_dif[0]=select_model->getPosition()[0]-(dir0[0]+dir[0]*t);
            pos_dif[1]=select_model->getPosition()[1]-(dir0[1]+dir[1]*t);
            pos_dif[2]=select_model->getPosition()[2]-(dir0[2]+dir[2]*t);
        }
        else
        {
            deselectModel();
        }

        left_press=true;
        mouse_x=event->x();
        mouse_y=event->y();
    }
}

void ModelTool::mouseRelease(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        left_press=false;
        mouse_x=event->x();
        mouse_y=event->y();
    }
}

void ModelTool::wheel(QWheelEvent *event)
{
    if(select_model!=NULL)
    {
        float val=event->delta()*0.001;
        map->removeObject(select_model);
        select_model->translate(0,val,0);
        map->insertObject(select_model);

        ui->position_y->setValue(select_model->getPosition()[1]);
    }
}

void ModelTool::loadProject(Project *project)
{
    base=&project->model;
    model_io=&project->model_io;
    map=&project->map;

    ui->position_x->setMaximum(map->width());
    ui->position_y->setMaximum(map->height());
    ui->position_z->setMaximum(map->depth());

    reloadTree();

}
void ModelTool::unloadProject()
{
    tree_model.clear();
}


void ModelTool::onEnter()
{
    navig->hide();
}

void ModelTool::onLeave()
{
    deselectModel();
}

QString ModelTool::getName()
{
    return tr("Model");
}

QMenu *ModelTool::createMenu()
{
    QMenu * menu=new QMenu(getName(),this);
    QAction * action;
    action=new QAction(tr("Import File"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(importMesh()));
    menu->addAction(action);

    action=new QAction(tr("Import Asset"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(importAsset()));
    menu->addAction(action);

    return menu;
}

void ModelTool::importMesh()
{
    QString file_name = QFileDialog::getOpenFileName(this,
        tr("Import Mesh"), "~", tr("Mesh Files: obj (*.obj)"));// *.3ds

    if(file_name.isNull())
        return;
    ModelMesh * new_mesh=model_io->importMesh(file_name);

    if(new_mesh!=NULL)
    {
        QStandardItem * item=new QStandardItem(new_mesh->getName());
        QVariant tmp=qVariantFromValue((void *)new_mesh);
        item->setData(tmp);
        tree_model.appendRow(item);
        return;
    }

    QMessageBox messageBox;
    messageBox.critical(0,"Error","Unable to open mesh!");
    messageBox.exec();
}

void ModelTool::importAsset()
{
    MeshDialog dialog(assets);
    dialog.exec();
    if(!dialog.isSelected())
        return;

    ModelMesh * new_mesh=model_io->importMesh(dialog.file());

    if(new_mesh!=NULL)
    {
        QStandardItem * item=new QStandardItem(new_mesh->getName());
        QVariant tmp=qVariantFromValue((void *)new_mesh);
        item->setData(tmp);
        tree_model.appendRow(item);
        return;
    }

    QMessageBox messageBox;
    messageBox.critical(0,"Error","Unable to open mesh!");

}

void ModelTool::modelChanged(QStandardItem *item)
{
    if(item!=NULL)
    {
        if(item->parent()==NULL)
        {
            ModelMesh * mesh=(ModelMesh *)item->data().value<void *>();
            if(mesh==NULL)
                return;
            mesh->setName(item->text().toUtf8().constData());
        }
        else
        {
            Model * model=(Model *)item->data().value<void *>();
            if(select_model==NULL)
                return;
            model->setName(item->text().toUtf8().constData());
        }
    }
}

void ModelTool::mouseMove(QMouseEvent * event)
{
    if(select_model!=NULL && left_press)
    {
        Camera * camera=canvas->getCamera();
        Vertex3f dir0=camera->getPosition();
        Vertex3f pos,dir;
        dir=camera->unproject(event->x(),event->y(),0.0);
        float t=(select_model->getPosition()[1]-dir0[1])/dir[1];
        pos[0]=dir0[0]+dir[0]*t+pos_dif[0];
        pos[1]=dir0[1]+dir[1]*t+pos_dif[1];
        pos[2]=dir0[2]+dir[2]*t+pos_dif[2];

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


        map->removeObject(select_model);
        select_model->setPosition(pos);
        map->insertObject(select_model);

        ui->position_x->setValue(select_model->getPosition()[0]);
        ui->position_y->setValue(select_model->getPosition()[1]);
        ui->position_z->setValue(select_model->getPosition()[2]);
    }
}

Model *ModelTool::findModel(int x, int y)
{
    Camera * camera=canvas->getCamera();
    Vertex3f dir=camera->unproject(x,y,0.0);
    dir.normalize();
    dir*=300;
    Vertex3f end(camera->getPosition());
    end+=dir;
    Collision col =map->collisionVisible(camera->getPosition(),end,*camera);

    if(col.object!=NULL && col.object->type()==base->type())
    {
        return (Model*)col.object;
    }

    return NULL;
}

void ModelTool::selectModel(Model *model)
{
    deselectModel();

    ui->position_x->setValue(model->getPosition()[0]);
    ui->position_y->setValue(model->getPosition()[1]);
    ui->position_z->setValue(model->getPosition()[2]);
    ui->scale->setValue(model->getScale()[0]);

    Vertex3f q=model->getRotation().getAngle();
    o_x=q[0];
    o_y=q[1];
    o_z=q[2];

    ui->angle_x->setValue(o_x);
    ui->angle_y->setValue(o_y);
    ui->angle_z->setValue(o_z);

    ui->rotate_x->setValue(o_x*57.2957795);
    ui->rotate_y->setValue(o_y*57.2957795);
    ui->rotate_z->setValue(o_z*57.2957795);

    select_model=model;
    select_model->select();
}

void ModelTool::deselectModel()
{
    if(select_model!=NULL)
    {
        select_model->deselect();
        select_model=NULL;

        ui->angle_x->setValue(0);
        ui->angle_y->setValue(0);
        ui->angle_z->setValue(0);
        ui->scale->setValue(0);
        ui->position_x->setValue(0);
        ui->position_y->setValue(0);
        ui->position_z->setValue(0);
    }
}

void ModelTool::deleteMesh()
{
    QModelIndex index=ui->treeView->currentIndex();
    if(!index.isValid())
        return;

    QStandardItem * item=tree_model.itemFromIndex(index);
    if(item->parent()==NULL)
    {
        ModelMesh * mesh=(ModelMesh *)item->data().value<void *>();
        deselectModel();

        tree_model.removeRow(item->row());
        model_io->removeMesh(mesh);
    }
}

void ModelTool::on_position_x_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        map->removeObject(select_model);
        select_model->setPosX(arg1);
        map->insertObject(select_model);
    }
}

void ModelTool::on_position_y_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        map->removeObject(select_model);
        select_model->setPosY(arg1);
        map->insertObject(select_model);
    }
}

void ModelTool::on_position_z_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        map->removeObject(select_model);
        select_model->setPosZ(arg1);
        map->insertObject(select_model);
    }
}

void ModelTool::on_rotate_x_valueChanged(int value)
{
    if(select_model!=NULL)
    {
        float a=value*0.0174532925;
        rotateObject(a,o_y,o_z);
        ui->angle_x->setValue(a);
    }
}

void ModelTool::on_rotate_y_valueChanged(int value)
{
    if(select_model!=NULL)
    {
        float a=value*0.0174532925;
        rotateObject(o_x,a,o_z);
        ui->angle_y->setValue(a);
    }
}

void ModelTool::on_rotate_z_valueChanged(int value)
{
    if(select_model!=NULL)
    {
        float a=value*0.0174532925;
        rotateObject(o_x,o_y,a);
        ui->angle_z->setValue(a);
    }
}

void ModelTool::on_angle_x_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        rotateObject(arg1,o_y,o_z);
        ui->rotate_x->setValue(arg1*57.2957795);
    }
}

void ModelTool::on_angle_y_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        rotateObject(o_x,arg1,o_z);
        ui->rotate_y->setValue(arg1*57.2957795);
    }
}

void ModelTool::on_angle_z_valueChanged(double arg1)
{
    if(select_model!=NULL)
    {
        rotateObject(o_x,o_y,arg1);
        ui->rotate_z->setValue(arg1*57.2957795);
    }
}

void ModelTool::rotateObject(float x, float y, float z)
{
    if(select_model==NULL)
        return;
    map->removeObject(select_model);

    o_x=x;
    o_y=y;
    o_z=z;
    Quaternion q=select_model->getRotation();
    q.rotate(x,y,z);
    select_model->setRotation(q);

    map->insertObject(select_model);
}

void ModelTool::on_treeView_customContextMenuRequested(const QPoint &)
{
    QModelIndex index=ui->treeView->currentIndex();
    if(!index.isValid())
        return;
    QStandardItem * item=tree_model.itemFromIndex(index);
    if(item!=NULL)
    {
        if(item->parent()==NULL)
        {
            QMenu *menu = new QMenu;
            menu->addAction(QString("New Model"), this, SLOT(newModel()));
            menu->addAction(QString("Delete"),this,SLOT(deleteMesh()));
            //menu->addAction(QString("Properties"), this, SLOT(test_slot()));
            menu->exec(QCursor::pos());
        }
        else
        {
            Model * model=(Model *)item->data().value<void *>();
            deselectModel();
            model->select();
            select_model=model;
            select_index=index;

            QMenu *menu = new QMenu;
            menu->addAction(QString("Duplicate"),this,SLOT(duplicateModel()));
            menu->addAction(QString("Delete"),this,SLOT(deleteModel()));
            //menu->addAction(QString("Properties"), this, SLOT(test_slot()));
            menu->exec(QCursor::pos());
        }
    }
}

void ModelTool::test_slot()
{
}

void ModelTool::duplicateModel()
{
    if(select_model!=NULL)
    {
        Model * new_model=new Model(base);
        map->insertObject(new_model);
        QStandardItem * item=new QStandardItem(new_model->getName());
        QVariant tmp=qVariantFromValue((void *)new_model);
        item->setData(tmp);
        QStandardItem * it=tree_model.itemFromIndex(select_index);
        it->parent()->appendRow(item);
        selectModel(new_model);
        ui->treeView->setCurrentIndex(it->index());
    }
}

void ModelTool::deleteModel()
{
    QModelIndex index=ui->treeView->currentIndex();
    if(!index.isValid())
        return;
    QStandardItem * item=tree_model.itemFromIndex(index);
    if(item->parent()!=NULL)
    {
        Model * model=(Model *)item->data().value<void *>();
        map->removeObject(model);
        item->parent()->removeRow(index.row());
        deselectModel();
        delete model;
    }
}

void ModelTool::newModel()
{
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
    }
}

void ModelTool::on_treeView_clicked(const QModelIndex &index)
{
    QStandardItem * item=tree_model.itemFromIndex(index);
    if(item!=NULL)
    {
        if(item->parent()==NULL)
        {
        }
        else
        {
            Model * model=(Model *)item->data().value<void *>();
            selectModel(model);
        }
    }
}

void ModelTool::on_treeView_activated(const QModelIndex &index)
{
    on_treeView_clicked(index);
}

void ModelTool::reloadTree()
{
    const std::set<ModelMesh *> & meshes=base->getMeshes();
    QVariant tmp;
    tree_model.clear();

    for(std::set<ModelMesh *>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
    {
        ModelMesh* mesh=(*it);
        QStandardItem * item=new QStandardItem(mesh->getName());
        tmp=qVariantFromValue((void *)mesh);
        item->setData(tmp);

        for(std::set<Model *>::const_iterator mod = mesh->getModels().begin(); mod != mesh->getModels().end(); ++mod)
        {
            Model* model=(*mod);
            QStandardItem * ite=new QStandardItem(model->getName());
            tmp=qVariantFromValue((void *)model);
            ite->setData(tmp);

            item->appendRow(ite);
        }

        tree_model.setItem(items++,0,item);
    }
}

void ModelTool::addMesh(ModelMesh *mesh)
{/*
    ModelMesh* mesh=(*it);
    QStandardItem * item=new QStandardItem(mesh->getName());
    tmp=qVariantFromValue((void *)mesh);
    item->setData(tmp);
    tree_model.appendRow();*/
}

void ModelTool::on_scale_valueChanged(double arg1)
{
    if(select_model==NULL)
        return;

    map->removeObject(select_model);
    select_model->setScale(Vertex3f(arg1));
    map->insertObject(select_model);
}
