/*
Name: QtRpt
Version: 2.0.0
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2016 Aleksey Osipov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "UndoRedoCommands.h"
#include "RepScrollArea.h"

MoveGItemCommand::MoveGItemCommand(const ItemsAndParams &itm, QUndoCommand *parent)
    : QUndoCommand(parent), m_itm(itm)
{
    setText(QObject::tr("Moving box"));
}

void MoveGItemCommand::redo() {
    GraphicsBox *box = static_cast<GraphicsBox*>(m_itm.item);
    box->setPos(m_itm.newPos);
    box->setWidth(m_itm.newWidth);
    box->setHeight(m_itm.newHeight);
    box->setSelected(true);
}

void MoveGItemCommand::undo() {
    GraphicsBox *box = static_cast<GraphicsBox*>(m_itm.item);
    box->setPos(m_itm.oldPos);
    box->setWidth(m_itm.oldWidth);
    box->setHeight(m_itm.oldHeight);
    box->setSelected(true);
}


MoveLineCommand::MoveLineCommand(const ItemsAndParams &itm, QUndoCommand *parent)
    : QUndoCommand(parent), m_itm(itm)
{
    setText(QObject::tr("Moving line"));
}

void MoveLineCommand::redo() {
    GraphicsLine *line = static_cast<GraphicsLine*>(m_itm.item);
    line->setPos(m_itm.newPos);
    line->setPointList(m_itm.newPointList);
}

void MoveLineCommand::undo() {
    GraphicsLine *line = static_cast<GraphicsLine*>(m_itm.item);
    line->setPos(m_itm.oldPos);
    line->setPointList(m_itm.oldPointList);
}

//************************************************************************************************

DelItemCommand::DelItemCommand(GraphicsScene *scene, QUndoCommand *parent) : QUndoCommand(parent) {
    myGraphicsScene = scene;

    for(auto item : scene->items()) {
        bool isSelected = false;
        GraphicsBox *box = nullptr;
        if (item->type() == ItemType::GBand) {
            box = static_cast<GraphicsBox*>(item);
            isSelected = box->isSelected();
        }
        if (item->type() == ItemType::GBox) {
            box = static_cast<GraphicsBox*>(item);
            isSelected = box->isSelected();
        }
        GraphicsLine *line = nullptr;
        if (item->type() == ItemType::GLine) {
            line = static_cast<GraphicsLine*>(item);
            isSelected = line->isSelected();
        }
        if (isSelected)
            itemList << item;
    }

    setText(QObject::tr("Delete"));
}

void DelItemCommand::undo() {
    for (int i=0; i<itemList.size(); i++) {
        myGraphicsScene->addItem(itemList[i]);
        if (itemList[i]->type() == ItemType::GBox || itemList[i]->type() == ItemType::GLine) {
            itemList.at(i)->setParentItem(parentList[i]);

            RepScrollArea *area = qobject_cast<RepScrollArea*>(myGraphicsScene->parent());
            area->newFieldTreeItem(itemList.at(i));
        }
        if (itemList[i]->type() == ItemType::GBand) {
            RepScrollArea *area = qobject_cast<RepScrollArea*>(myGraphicsScene->parent());
            area->newFieldTreeItem(itemList.at(i));
        }
    }
    myGraphicsScene->update();
}

void DelItemCommand::redo() {
    for(auto cont1 : itemList) {
        parentList << cont1->parentItem();
        myGraphicsScene->removeItem(cont1);
    }
}

//************************************************************************************************

AddCommand::AddCommand(QGraphicsItem *item, GraphicsScene *scene, QGraphicsItem *p_item, QUndoCommand *parent) : QUndoCommand(parent) {
    static int itemCount = 0;

    myGraphicsScene = scene;
    mpItem = p_item;
    myDiagramItem = item;
    initialPosition = item->scenePos();
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add"));
}

AddCommand::~AddCommand() {
    if (!myDiagramItem)
        delete myDiagramItem;
}

void AddCommand::undo() {
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
}

void AddCommand::redo() {
    if (myDiagramItem->scene() == 0)
        myGraphicsScene->addItem(myDiagramItem);
	if (mpItem != 0) {
		myDiagramItem->setParentItem(mpItem);

        RepScrollArea *area = qobject_cast<RepScrollArea*>(myGraphicsScene->parent());
        area->newFieldTreeItem(myDiagramItem);
	}
    myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

//************************************************************************************************

ParamCommand::ParamCommand(QList<PairCont> list, GraphicsScene *scene, QUndoCommand *parent) : QUndoCommand(parent) {
    myGraphicsScene = scene;
    m_dataList = list;
    this->setText(QObject::tr("Changing Container's parameters"));
}

ParamCommand::~ParamCommand() {

}

void ParamCommand::undo() {
    for (int i=0; i<m_dataList.size(); i++) {
        PairCont pair = m_dataList[i];
        QDataStream in(pair.oldBArray);
        GraphicsHelperClass *second = qobject_cast<GraphicsHelperClass *>(pair.gHelper);
        if (second == 0) continue;
        in >> *second;
        pair.gHelper = second;

        /*if (pair.third != QtRptName::Line) {
            TContainerField *second = qobject_cast<TContainerField *>(pair.second);
            if (second == 0) continue;
            in >> *second;
            pair.second = second;
        } else {
            TContainerLine *second = qobject_cast<TContainerLine *>(pair.second);
            if (second == 0) continue;
            in >> *second;
            pair.second = second;
        }*/
        //pair.second->itemInTree->setText(0,pair.second->objectName());
    }
    myGraphicsScene->update();
}

void ParamCommand::redo() {
    /*if (m_create) {
        m_create = false;
        return;
    }*/
    for (int i=0; i<m_dataList.size(); i++) {
        PairCont pair = m_dataList[i];
        QDataStream in(pair.newBArray);
        GraphicsHelperClass *second = qobject_cast<GraphicsHelperClass *>(pair.gHelper);
        in >> *second;
        pair.gHelper = second;

        /*if (pair.third != QtRptName::Line) {
            TContainerField *second = qobject_cast<TContainerField *>(pair.second);
            in >> *second;
            pair.second = second;
        } else {
            TContainerLine *second = qobject_cast<TContainerLine *>(pair.second);
            in >> *second;
            pair.second = second;
        }*/
        //pair.second->itemInTree->setText(0,pair.second->objectName());
    }
    myGraphicsScene->update();
}

BArrayList ParamCommand::getBArrayFromContList(GraphicsHelperList contList) {
    BArrayList list;
    for(auto cont1 : contList) {
//        cont1->setProperties();
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out << *cont1;

//        TContainerField *contF = qobject_cast<TContainerField *>(cont1);
//        if (contF != 0)
//            out << *contF;
//        TContainerLine *contL = qobject_cast<TContainerLine *>(cont1);
//        if (contL != 0)
//            out << *contL;

        QPair<QByteArray, GraphicsHelperClass*> pair;
        pair.first = byteArray;
        pair.second = cont1;
        list.append(pair);
    }
    return list;
}

QList<PairCont> ParamCommand::compoundArrays(BArrayList oldList, BArrayList newList) {
    QList<PairCont> list;
    for (int i=0; i<oldList.size(); i++) {
        for (int j=0; j<newList.size(); j++) {
            if (oldList[i].second == newList[j].second) {
                PairCont pair;
                pair.oldBArray = oldList[i].first;
                pair.gHelper = oldList[i].second;
                pair.newBArray = newList[j].first;
                list.append(pair);
            }
        }
    }
    return list;
}

/*
 *
ParamsContainerCommand::ParamsContainerCommand(QList<PairCont> list, QUndoCommand *parent) : QUndoCommand(parent) {
    m_dataList = list;
    m_create = true;
    this->setText(QObject::tr("Changing Container's parameters"));
}

void ParamsContainerCommand::redo() {

}

void ParamsContainerCommand::undo() {

}





*/
