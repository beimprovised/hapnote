#ifndef RECORDTABLESCREEN_H_
#define RECORDTABLESCREEN_H_

#include <QWidget>
#include <QListView>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QByteArray>
#include <QItemSelection>
#include <QLabel>
#include "models/tree/TreeItem.h"


class RecordController;
class VerticalScrollArea;
class FlatToolButton;
class TreeItem;

namespace browser {
    class ToolbarSearch;
}

class RecordScreen : public QWidget {
    Q_OBJECT

public:
    RecordScreen(QString object_name
                 // , boost::intrusive_ptr<TreeItem> _shadow_branch_root
                 , QWidget *parent = 0);
    virtual ~RecordScreen();

    int     first_selection_pos(void);
    QString first_selection_id(void);
    void    select_pos(int pos);
    void    select_id(QString id);

    void tree_path(QString path);
    QString tree_path(void);

    inline RecordController *
    table_controller()
    {
        return _table_controller;
    }

    // The steps used on the toolbar and in the context menu entries    // Действия, используемые как на тулбаре, так и в контекстном меню списка записей
    QAction *_save_in_new_branch;
    QAction *_pin;
    QAction *_addnew_to_end;
    QAction *_addnew_before;
    QAction *_addnew_after;
    QAction *_edit_field;
    QAction *_delete;
    QAction *_cut;
    QAction *_copy;
    QAction *_paste;
    QAction *_settings;
    QAction *_back;
    QAction *_find_in_base;
    QAction *_sort;
    QAction *_print;

public slots:

    // Обновление панели инструментов
    void tools_update(void);


protected:
    void resizeEvent(QResizeEvent *e);
private slots:

    void on_syncro_click(void);
    void on_walkhistory_previous_click(void);
    void on_walkhistory_next_click(void);
    void on_back_click(void);

private:
    QToolBar            *_toolsline;
    QToolBar            *_extra_toolsline;

    QLabel              *_treepathlabel;
    //    FlatToolButton  *_treepath_button;
    QString             _treepath;

    VerticalScrollArea  *_vertical_scrollarea;
    RecordController     *_table_controller;

    QHBoxLayout         *_recordtable_toolslayout;
    //    browser::ToolbarSearch  *_recordtree_search;
    //    QHBoxLayout             *_recordtree_searchlayout;
    QVBoxLayout         *_recordtable_screenlayout;

    QAction *_action_move_up;
    QAction *_action_move_dn;
    QAction *_action_syncro;
    QAction *_action_walk_history_previous;
    QAction *_action_walk_history_next;

    void setup_ui(void);
    void setup_signals(void);
    void setup_actions(void);
    void assembly(void);

    void disable_all_actions(void);

};

#endif /* RECORDTABLESCREEN_H_ */