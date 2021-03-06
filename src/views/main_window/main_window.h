#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QDialog>
#include <QMainWindow>
#include <QWidget>
#include <QTextListFormat>
#include <QTextBlockFormat>

#include <QtXml>
#include <QtGui>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QItemSelectionModel>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QList>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSystemTrayIcon>

#include <QStatusBar>
#include "hidable_tabwidget.h"

// #include "libraries/GlobalParameters.h"
// #include "models/app_config/AppConfig.h"
// #include "models/database_config/DataBaseConfig.h"



#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif



extern const char *meta_editor_singleton_name;

namespace browser {
    class Entrance;
    class Browser;
    class DownloadManager;
    class HistoryMenu;
    class BookmarksMenu;
    class Profile;
}

class ts_t;
class MetaEditor;
class rs_t;
class FindScreen;
class WindowSwitcher;
class sapp_t;
class rctrl_t;
class gl_para;
class AppConfig;
class DataBaseConfig;
class tsv_t;
class Editentry;


class wn_t
    : public QMainWindow {
#if QT_VERSION == 0x050600
    W_OBJECT(wn_t)
#else
    Q_OBJECT
#endif

    public:
	wn_t(gl_para &_globalparameters
	    , AppConfig &_appconfig
	    , DataBaseConfig &_databaseconfig
	    , browser::Profile *_profile, QString style_source);

	~wn_t();

	void restore_geometry(void);
	void restore_tree_position(void);
//    void restore_recordtable_position(void);// too many _record_screen objects, deprecated
	void restore_editor_cursor_position(void);
	void restore_editor_scrollbar_position(void);
	void restore_find_in_base_visible(void);

	void set_tree_position(QString view_root_id, QStringList current_item_absolute_path);
	bool is_tree_position_crypt();

//    void select_id(QString id);// too many _record_screen objects, deprecated

	void synchronization(void);

	void go_walk_history_previous(void);
	void go_walk_history_next(void);

	void save_text_area(void);
	void save_all_state(void);

	HidableTabWidget	*vtab_record() const;
	ts_t			*tree_screen() const;

	QMenu			*file_menu() const;
	QMenu			*edit_menu() const;
	QMenu			*view_menu() const;
	browser::HistoryMenu	*histry_menu() const;
	browser::BookmarksMenu	*bookmark_menu() const;
	QMenu			*window_menu() const;
	QMenu			*tools_menu() const;
	QMenu			*help_menu() const;
	QSplitter		*find_splitter() const;
	QSplitter		*h_record_splitter() const;
	QSplitter		*h_tree_splitter() const;
//	std::vector<tsv_t *>	tree_viewers() const;
    public slots:
	void application_exit(void);
	void application_fast_exit(void);
	void commit_data(QSessionManager &manager);
	void editor_switch(void);

    private slots:
	void file_new(void);
	void file_open(void);
	bool file_save(void);
	bool file_save_as(void);
	void file_print(void);
	void file_print_preview(void);
	void file_print_pdf(void);
	void tools_find(void);
	void tools_preferences(void);
//    void editor_switch(void);


	void on_expand_edit_area(bool flag);

	void on_click_help_about_hapnote(void);
	void on_click_help_about_qt(void);

	void icon_activated(QSystemTrayIcon::ActivationReason reason);

	void on_focus_changed(QWidget *, QWidget *);

    private:

	void setup_ui(void);
	void setup_signals(void);
	void assembly(void);

	void init_file_menu(void);
	void append_quit_menu();
	void init_tools_menu(void);
	void init_preferences_menu(QMenu *menu);
	void init_help_menu(void);

	void init_itemsflat_actions(void);

	void setup_icon_actions(void);
	void create_tray_icon(void);
	void set_icon(void);

	void save_geometry(void);
	void save_tree_position(void);
//    void save_recordtable_position(void);// too many _record_screen objects, deprecated
	void save_editor_cursor_position(void);
	void save_editor_scrollbar_position(void);


	QString _style;

	gl_para	&_globalparameters;
	AppConfig		&_appconfig;
	DataBaseConfig		&_databaseconfig;



	QAction			*_action_tray_restore;
	QAction			*_action_tray_maximize;
	QAction			*_action_tray_minimize;
	QAction			*_action_tray_quit;

	QSystemTrayIcon		*_tray_icon;
	QMenu			*_tray_icon_menu;

	QSplitter		*_v_right_splitter;
	QSplitter		*_v_find_splitter;

//	HidableTabWidget	*_vtab_tree;
	QSplitter		*_h_record_splitter;
	QSplitter		*_h_tree_splitter;
//    QSplitter           *_h_splitter;


	QMenu				*_filemenu;
	QMenu				*_editmenu;
	QMenu				*_viewmenu;
	browser::HistoryMenu		*_histrymenu;
	browser::BookmarksMenu		*_bookmarkmenu;
	QMenu				*_windowmenu;
	QMenu				*_toolsmenu;
	QMenu				*_helpmenu;

	ts_t				*_tree_screen;
	FindScreen			*_find_screen;

	Editentry *_editenty;
	browser::Entrance		*_entrance;
//	browser::DownloadManager	*_download;
	HidableTabWidget	*_vtab_record;
	QStatusBar		*_statusbar;
	WindowSwitcher		*_switcher;



//    bool _treetable_hidden;       // = globalparameters.getTreeScreen()->isHidden();
//    bool _recordtable_hidden;     // = globalparameters.getRecordTableScreen()->isHidden();

    protected:



	void closeEvent(QCloseEvent *event);

	bool eventFilter(QObject *o, QEvent *e);	// Отслеживание прочих событий

	void go_walk_history(void);

	bool _enable_real_close;

	friend class browser::Browser;
};
#endif
