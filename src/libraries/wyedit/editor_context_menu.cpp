
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QAction>
#include <QDebug>

#include "editor_context_menu.h"


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(EditorContextMenu)
#endif


EditorContextMenu::EditorContextMenu(QWidget *parent) : QMenu(parent)
{
    setup_actions();
    setup_signals();
    setup_menu();
}


EditorContextMenu::~EditorContextMenu(void)
{

}


void EditorContextMenu::setup_actions(void)
{
    actionUndo = new QAction(tr("Undo"), this);

    actionRedo = new QAction(tr("Redo"), this);

    actionCut = new QAction(tr("Cut"), this);

    actionCopy = new QAction(tr("Copy"), this);

    actionPaste = new QAction(tr("Paste"), this);

    actionSelectAll = new QAction(tr("Select All"), this);

    actionEditImageProperties = new QAction(tr("Edit image properties"), this);
}


// Показывать или нет пункт редактирования свойств изображения
void EditorContextMenu::set_edit_image_properties(bool is_image_select)
{
    qDebug() << "In EditorContextMenu::set_edit_image_properties() " << is_image_select;

    actionEditImageProperties->setVisible(is_image_select);
}


void EditorContextMenu::setup_signals(void)
{
    connect(actionUndo, &QAction::triggered, this, &EditorContextMenu::undo);
    connect(actionRedo, &QAction::triggered, this, &EditorContextMenu::redo);

    connect(actionCut, &QAction::triggered, this, &EditorContextMenu::cut);
    connect(actionCopy, &QAction::triggered, this, &EditorContextMenu::copy);
    connect(actionPaste, &QAction::triggered, this, &EditorContextMenu::paste);

    connect(actionSelectAll, &QAction::triggered, this, &EditorContextMenu::selectAll);

    connect(actionEditImageProperties, &QAction::triggered, this, &EditorContextMenu::contextMenuEditImageProperties);
}


void EditorContextMenu::setup_menu(void)
{
    this->addAction(actionUndo);
    this->addAction(actionRedo);

    this->addSeparator();

    this->addAction(actionCut);
    this->addAction(actionCopy);
    this->addAction(actionPaste);

    this->addSeparator();

    this->addAction(actionSelectAll);
    this->addAction(actionEditImageProperties);
}

