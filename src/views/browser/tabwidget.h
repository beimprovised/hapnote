/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TABWIDGET_H
#define TABWIDGET_H


#include "boost/format.hpp"
#include <exception>

#include <utility>
#include "utility/util.hxx"
#include "utility/delegate.h"

#include <QtWebEngineWidgets/QWebEngineFullScreenRequest>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QShortcut>
#include <QProxyStyle>

#include "models/record_table/record_index.hxx"
// #include "models/record_table/Record.h"
// #include "models/record_table/ItemsFlat.h"
// #include "models/tree/TreeItem.h"
// #include "models/record_table/RecordModel.h"
// #include "controllers/record_table/RecordController.h"
// #include "views/record_table/RecordScreen.h"




#include "libraries/global_parameters.h"
#include "libraries/disk_helper.h"

#include "views/browser/history.h"

#include "views/browser/webview.h"


// #include "models/tree/TreeModel.h"

// #include "views/tree/KnowView.h"
#include "models/tree/tree_know_model.h"

#include "views/tree/tree_screen.h"


#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif


class gl_para;
extern gl_para			globalparameters;
extern QMap<Qt::CheckState, QString>	_string_from_check_state;
extern QMap<QString, Qt::CheckState>	_state_check_from_string;
extern QString				get_unical_id(void);


QT_BEGIN_NAMESPACE
class QWebEngineDownloadItem;
class QWebEngineProfile;
class QWebEngineView;
QT_END_NAMESPACE

#include <QWebEnginePage>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

#include <QtCore/QUrl>
#include <QtWidgets/QTabWidget>

#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE

class QComboBox;
class QStackedWidget;
class QVBoxLayout;
class QHBoxLayout;
class QButtonGroup;

// QT_END_NAMESPACE

// QT_BEGIN_NAMESPACE

class QCompleter;
class QLineEdit;
class QMenu;
class QStackedWidget;
class TreeItem;
class tm_t;
class RecordModel;
class rv_t;
class tv_t;
class Editentry;




QT_END_NAMESPACE

namespace browser {
    class WebView;
    class TabWidget;
}

// extern boost::intrusive_ptr<Record> request_record(
// const QUrl &_url
// , std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, boost::intrusive_ptr<Record>>>
// , std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, boost::intrusive_ptr<Record>>>
// );

// QT_BEGIN_NAMESPACE

namespace browser {
    class Browser;
    class FullScreenNotification;
    class Profile;



    namespace alter {
//	class TabBar : public QTabBar	// QWidget
//	{
//	    W_OBJECT(TabBar)
////	    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex STORED true NOTIFY currentIndexChanged)
////	    Q_PROPERTY(QStringList pageTitleList READ pageTitleList WRITE setPageTitleList STORED true)
////	    Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle STORED false NOTIFY pageTitleChanged)
////	    Q_PROPERTY(QStringList pageIconList READ pageIconList WRITE setPageIconList STORED true)
////	    Q_PROPERTY(QIcon pageIcon READ pageIcon WRITE setPageIcon STORED false NOTIFY pageIconChanged)

//	    public:
//		TabBar(TabWidget *parent = 0);

//		QSize sizeHint() const;

//		int	count() const;
//		int	currentIndex() const;
//		QWidget *widget(int index);
//		int	indexOf(QWidget *w);

//		QStringList	pageTitleList() const;
//		QString		pageTitle() const;

//		QStringList	pageIconList() const;
//		QIcon		pageIcon() const;

//		bool	setVisible(QWidget *w, bool b);
//		bool	setEnabled(QWidget *w, bool b);

//// For lazy programmers migrating from QTabWidget to this class
//		int	addTab(QWidget *page, const QString &title);
//		int	addTab(QWidget *page, const QIcon &icon = QIcon(), const QString &title = QString());

//		void setTabText(int index, const QString &title);

//	    public slots:
//		int	addPage(QWidget *page, const QIcon &icon = QIcon(), const QString &title = QString());
//		int	insertPage(int index, QWidget *page, const QIcon &icon = QIcon(), const QString &title = QString());
//		void	removePage(int index);
//		void	setCurrentIndex(int index);

//		void	setPageTitleList(QStringList const &newTitleList);
//		void	setPageTitle(QString const &newTitle);
//		void	setPageTitle(int index, QString const &newTitle);

//		void	setPageIconList(QStringList const &newIconList);
//		void	setPageIcon(QIcon const &newIcon);
//	    private slots:	// cloned from TabBar
//		void	selectTabAction();
//		void	cloneTab();
//		void	closeTab();
//		void	closeOtherTabs();
//		void	reloadTab();
//		void	contextMenuRequested(const QPoint &position);
//	    signals:	// cloned from TabBar
//		void	newTab();
//		void	cloneTabSignal(int index);
//		void	closeTabSignal(int index);
//		void	closeOtherTabsSignal(int index);
//		void	reloadTabSignal(int index);
//		void	reloadAllTabs();
//		void	tabMoveRequested(int fromIndex, int toIndex);
//	    signals:
//		void	currentIndexChanged(int index);
//		void	pageTitleChanged(const QString &title);
//		void	pageIconChanged(const QIcon &icon);

//	    protected:	// cloned from TabBar
//		void	mousePressEvent(QMouseEvent *event);
//		void	mouseMoveEvent(QMouseEvent *event);

//	    private:
//		QStringList titleList, iconList;

//// QStackedWidget *
//		TabWidget	*stackWidget;
//		QButtonGroup	*buttonGroup;
//		QHBoxLayout	*layout;
//		QVBoxLayout	*buttonLayout;
//	    private:	// cloned from TabBar
//		QList<QShortcut *> _tabshortcuts;
//		friend class browser::TabWidget;

//		QPoint	_dragstartpos;
//		int	_dragcurrentindex;

//		W_PROPERTY(int, currentIndex, &TabBar::currentIndex, &TabBar::setCurrentIndex, W_Notify, &TabBar::currentIndexChanged)	// , W_Stored, true
//		w_PROPERTY(QStringList, pageTitleList, &TabBar::pageTitleList, &TabBar::setPageTitleList)// , W_Stored, true
//		w_PROPERTY(QString, pageTitle, &TabBar::pageTitle, &TabBar::setPageTitle, W_Notify, &TabBar::pageTitleChanged)	// , W_Stored, false
//		w_PROPERTY(QStringList, pageIconList, &TabBar::pageIconList, &TabBar::setPageIconList)	// , W_Stored, true
//		w_PROPERTY(QIcon, pageIcon, &TabBar::pageIcon, &TabBar::setPageIcon, W_Notify, &TabBar::pageIconChanged)// , W_Stored, false
//	};
    }


	// deprecated:
    class CustomTabStyle : public QProxyStyle {
	public:
	    CustomTabStyle();

	    QSize SizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
	    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    };


    namespace origin {
	/*
	    Tab bar with a few more features such as a context menu and shortcuts
	 */
	class TabBar : public QTabBar {
#if QT_VERSION == 0x050600
	    W_OBJECT(TabBar)
#else
	    Q_OBJECT
#endif

	    signals:
		void newTab()
#if QT_VERSION == 0x050600
		W_SIGNAL(newTab)//
#else
		;
#endif
		void cloneTabSignal(int index)
#if QT_VERSION == 0x050600
		W_SIGNAL(cloneTabSignal, (int), index)	//
#else
		;
#endif
		void closeTabSignal(int index)
#if QT_VERSION == 0x050600
		W_SIGNAL(closeTabSignal, (int), index)	//
#else
		;
#endif
		void closeOtherTabsSignal(int index)
#if QT_VERSION == 0x050600
		W_SIGNAL(closeOtherTabsSignal, (int), index)	//
#else
		;
#endif
		void reloadTabSignal(int index)
#if QT_VERSION == 0x050600
		W_SIGNAL(reloadTabSignal, (int), index)	//
#else
		;
#endif
		void reloadAllTabs()
#if QT_VERSION == 0x050600
		W_SIGNAL(reloadAllTabs)	//
#else
		;
#endif
		void tabMoveRequested(int fromIndex, int toIndex)
#if QT_VERSION == 0x050600
		W_SIGNAL(tabMoveRequested, (int, int), fromIndex, toIndex)	//
#else
		;
#endif

	    public:
		TabBar(QWidget *parent = 0);

	    protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	    private slots:
		void selectTabAction();
		void cloneTab();
		void closeTab();
		void closeOtherTabs();
		void reloadTab();
		void contextMenuRequested(const QPoint &position);

	    private:
		QList<QShortcut *> _tabshortcuts;
		friend class browser::TabWidget;

		QPoint _dragstartpos;
		int _dragcurrentindex;
	};
    }

// typedef alter::TabBar TabBar;
    using TabBar = origin::TabBar;

	// #include <QWebEnginePage>

	// QT_BEGIN_NAMESPACE
	// class QAction;
	// QT_END_NAMESPACE


	// template<typename T>
    class WebView;

	/*!
	    A proxy object that connects a single browser action
	    to one child webpage action at a time.

	    Example usage: used to keep the main window stop action in sync with
	    the current tabs webview's stop action.
	 */
    class WebActionMapper : public QObject {
#if QT_VERSION == 0x050600
	W_OBJECT(WebActionMapper)
#else
	Q_OBJECT
#endif

	public:
	    WebActionMapper(QAction *root, QWebEnginePage::WebAction webAction, QObject *parent);
	    QWebEnginePage::WebAction webAction() const;
	    void addChild(QAction *action);
	    void updateCurrent(QWebEnginePage *currentParent);

	private slots:
	    void rootTriggered();
	    void childChanged();
	    void rootDestroyed();
	    void currentDestroyed();

	private:
	    QWebEnginePage		*_currentparent;
	    QAction			*_root;
	    QWebEnginePage::WebAction _webaction;
    };

	// #include <QtCore/QUrl>
	// #include <QtWidgets/QTabWidget>
	// QT_BEGIN_NAMESPACE
	// class QCompleter;
	// class QLineEdit;
	// class QMenu;
	// class QStackedWidget;
	// QT_END_NAMESPACE




	/*!
	    TabWidget that contains WebViews and a stack widget of associated line edits.

	    Connects up the current tab's signals to this class's signal and uses WebActionMapper
	    to proxy the actions.
	 */

	// browsertabmanager
    class TabWidget : public QTabWidget {
#if QT_VERSION == 0x050600
	W_OBJECT(TabWidget)
#else
	Q_OBJECT
#endif

	signals:
		// tab widget signals
	    void loadPage(const QString &url)
#if QT_VERSION == 0x050600
	    W_SIGNAL(loadPage, (const QString &), url)	//
#else
	    ;
#endif
	    void tabsChanged()
#if QT_VERSION == 0x050600
	    W_SIGNAL(tabsChanged)	//
#else
	    ;
#endif
	    void lastTabClosed()
#if QT_VERSION == 0x050600
	    W_SIGNAL(lastTabClosed)	//
#else
	    ;
#endif

		// current tab signals
	    void setCurrentTitle(const QString &url)
#if QT_VERSION == 0x050600
	    W_SIGNAL(setCurrentTitle, (const QString &), url)	//
#else
	    ;
#endif
	    void showStatusBarMessage(const QString &message, int timeout = 0)
#if QT_VERSION == 0x050600
	    W_SIGNAL(showStatusBarMessage, (const QString &, int), message, timeout)	//
#else
	    ;
#endif
	    void linkHovered(const QString &link)
#if QT_VERSION == 0x050600
	    W_SIGNAL(linkHovered, (const QString &), link)	// ;	// , int timeout = 0)
#else
	    ;
#endif
	    void loadProgress(int progress)
#if QT_VERSION == 0x050600
	    W_SIGNAL(loadProgress, (int), progress)	//
#else
	    ;
#endif
	    void geometryChangeRequested(const QRect &geometry)
#if QT_VERSION == 0x050600
	    W_SIGNAL(geometryChangeRequested, (const QRect &), geometry)	//
#else
	    ;
#endif
	    void menuBarVisibilityChangeRequested(bool visible)
#if QT_VERSION == 0x050600
	    W_SIGNAL(menuBarVisibilityChangeRequested, (bool), visible)	//
#else
	    ;
#endif
	    void statusBarVisibilityChangeRequested(bool visible)
#if QT_VERSION == 0x050600
	    W_SIGNAL(statusBarVisibilityChangeRequested, (bool), visible)	//
#else
	    ;
#endif
	    void toolBarVisibilityChangeRequested(bool visible)
#if QT_VERSION == 0x050600
	    W_SIGNAL(toolBarVisibilityChangeRequested, (bool), visible)	//
#else
	    ;
#endif

#if defined(QWEBENGINEPAGE_PRINTREQUESTED)
	    void printRequested(QWebEngineFrame *frame);
#endif

	public:
	    TabWidget(ts_t		*tree_screen_
		     , FindScreen	*find_screen_
		     , Editentry	*editentry_
		     , Browser		*browser_
		     , rs_t		*record_screen_
		     , Entrance		*entrance_
		     , wn_t		*main_window_
		     , Profile		*profile_);
		// , TableController *_page_controller
		// , boost::intrusive_ptr<TreeItem> _shadow_branch_root

		// typedef Binder CouplerDelegation;
		// typedef typename Binder::bind_interface      bind_interface;
		// typedef typename Binder::activate_interface  activate_interface;
		// typedef typename Binder::item_interface      item_interface;
		// typedef typename Binder::page_interface      page_interface;

		// typedef typename Binder::bind_helper     bind_helper;
		// typedef typename Binder::activate_helper activate_helper;
		// typedef typename Binder::item_helper     item_helper;
		// typedef typename Binder::page_helper     page_helper;

	    ~TabWidget();
	    void clear();
	    void addWebAction(QAction *action, QWebEnginePage::WebAction webAction);

	    QAction	*newTabAction() const;
	    QAction	*closeTabAction() const;
	    QAction	*recentlyClosedTabsAction() const;
	    QAction	*nextTabAction() const;
	    QAction	*previousTabAction() const;

		// QWidget *
		// void lineEditStack(QStackedWidget *lineedits);
	    QStackedWidget			*lineEditStack() const;
	    QLineEdit				*currentLineEdit() const;
	    WebView				*select_as_current(WebView *v);
	    boost::intrusive_ptr<TreeItem>	current_item() const;
	    WebView				*currentWebView() const;
	    WebView				*webView(int index) const;
	    QLineEdit				*lineEdit(int index) const;
	    int	webViewIndex(WebView *webView) const;

	    QByteArray saveState() const;
	    bool restoreState(const QByteArray &state);

	    void setProfile(Profile *profile);
	    WebView	*find(const std::function<bool (boost::intrusive_ptr<const ::Binder>)> &_equal) const;		//= [](boost::intrusive_ptr<TreeItem> it, const QUrl &_url) ->bool {return it->field("url") == _url.toString();}

		// WebView *find(boost::intrusive_ptr<const TreeItem> it_find)const;
	    WebView		*find_nopin() const;
	    Browser		*browser();







		// struct coupler : public std::enable_shared_from_this<coupler> { // boost::intrusive_ref_counter<Coupler, boost::thread_safe_counter>  //

		// TabWidget                       *_tabmanager;
		// boost::intrusive_ptr<TreeItem>  _item_link;
		// WebPage                         *_page_link;
		// bool                            _make_current;

		// coupler(TabWidget                           *_tabmanager
		// , boost::intrusive_ptr<TreeItem>    item_link_
		// , bool                              make_current_ = true
		// );

		////                : _tabmanager(_tabmanager), _bounded_item(_bounded_item), _bounded_page(nullptr), _make_current(_make_current)
		////            {
		////                _bounded_item->record_binder(std::make_shared<CouplerDelegation>(
		////                                                 std::make_shared<bounded_item_interface>("", &Coupler::bounded_item, shared_from_this())
		////                                                 , std::make_shared<bounded_page_interface>("", &Coupler::bounded_page, shared_from_this())
		////                                                 , std::make_shared<bind_interface>("", &Coupler::binder, shared_from_this())
		////                                                 , std::make_shared<activate_interface> ("", &Coupler::activator, shared_from_this())
		////                                             ));
		////            }

		// boost::intrusive_ptr<TreeItem> &item_link() {return _item_link;}
		// WebPage *&page_link() {return _page_link;}

		// WebView *binder();   // , boost::intrusive_ptr<TreeItem>(TreeItem::* _bind)(WebPage *)
		// WebView *activator();

		// };









		// void sychronize_metaeditor_to_record(boost::intrusive_ptr<Record> record);
		// RecordModel *source_model() {return _record_controller->source_model();}
		// void tree_item(boost::intrusive_ptr<TreeItem> item) {_shadow_branch_root = item;}

		// std::shared_ptr<RecordTable> table_data() {return _page_tree_item->record_table();}
		// void reset_tabledata(std::shared_ptr<RecordTable> table_data) {_page_tree_item->record_table(table_data);}

	    TabBar	*tabbar();
	    rctrl_t	*record_controller();




		// boost::intrusive_ptr<TreeItem> item_request_from_tree_fragment(const QUrl &_url);

		// boost::intrusive_ptr<TreeItem> item_bind(boost::intrusive_ptr<TreeItem> item);

	    RecordModel *source_model();
	    rv_t	*view();

		// void addnew_to_end(void);
		// void addnew_before(void);


		//// Слот для добавления новой записи после выделенной строки
		//// Slot to add a new record after the highlighted row
		// void addnew_after(void);
		// void on_edit_fieldcontext(void);

		//// Обработка клика по удалению записи в контекстном меню и по кнопке на панели
		// void close_context(void);

		//// Копирование отмеченных записей в буфер обмена с удалением
		//// из таблицы конечных записей
		// void cut(void);
		//// Копирование отмеченных записей в буфер обмена
		// void copy(void);
		//// Insert records from the clipboard    // Вставка записей из буфера обмена
		// void paste(void);
		//// Слот, срабатывающий при вызове настроек
		// void settings(void);

		//// Перемещение записи вверх
		// void move_up(void);
		//// Перемещение записи вниз
		// void move_dn(void);

		//// Клик по пункту "Сортировка" в контекстном меню
		// void on_sort_click(void);

		// void on_print_click(void);

// TabWidget *tabmanager() {
// return this;
// }

	    boost::intrusive_ptr<TreeItem>	sibling(boost::intrusive_ptr<TreeItem> it) const;
//	    void current_download_acceptance(std::pair<QUrl, bool> policy);
//	    std::pair<QUrl, bool>		current_download_acceptance() const;
	    WebView				*sibling(WebView *v) const;

// int insertTab(int index, QWidget *w, const QIcon& icon, const QString &label);
	protected:
	    void mouseDoubleClickEvent(QMouseEvent *event);
	    void contextMenuEvent(QContextMenuEvent *event);
	    void mouseReleaseEvent(QMouseEvent *event);
	    void resizeEvent(QResizeEvent *);	// Q_DECL_OVERRIDE;

	public slots:
	    void loadUrlInCurrentTab(const QUrl &_url);

		// void new_view_void() {newTab(false);}
		// BrowserView *new_dummy();

	    WebView *newTab(boost::intrusive_ptr<RecordIndex> record_index, bool make_current = true);

		// void new_view(bool make_current = false);    //true

		// void cloneTab(int index = -1);
	    void requestCloseTab(int index = - 1);
	    void closeTab(int _previous_index = - 1);	// , bool sychronize_to_record_view = true
	    void closeOtherTabs(int index);
	    void reloadTab(int index = - 1);
	    void reloadAllTabs();
	    void nextTab();
	    void previousTab();
	    void moveTab(int fromIndex, int toIndex);
//            void setCurrentIndex(int index);
	private slots:
	    void currentChanged(int index);
	    void aboutToShowRecentTabsMenu();
	    void aboutToShowRecentTriggeredAction(QAction *action);
	    void downloadRequested(QWebEngineDownloadItem *download);
	    void webViewLoadStarted();
	    void webViewIconChanged();
	    void webViewTitleChanged(const QString &title);
	    void webViewUrlChanged(const QUrl &url);
	    void lineEditReturnPressed();
	    void windowCloseRequested();
		// void moveTab(int fromIndex, int toIndex);
	    void onTabsChanged();
	    void fullScreenRequested(QWebEngineFullScreenRequest request);
	    WebView	*view_no_pinned();
	private:
	    ts_t		*_tree_screen;
	    Editentry		*_editentry;
	    Entrance		*_entrance;
	    Browser		*_browser;
	    rs_t		*_record_screen;
	    wn_t		*_main_window;
	    rctrl_t		*_record_controller;		// RecordScreen        *_record_screen;

	    QAction		*_recentlyclosedtabsaction;
	    QAction		*_newtabaction;
	    QAction		*_closetabaction;
	    QAction		*_nexttabaction;
	    QAction		*_previoustabaction;

	    QMenu		*_recentlyclosedtabsmenu;
	    static const int _recentlyclosedtabssize = 10;

	    QList<QUrl>			_recentlyclosedtabs;
	    QList<WebActionMapper *>	_actions;

		// HistoryCompletionModel *_completionModel;// = new HistoryCompletionModel(this);
	    QCompleter			*_lineeditcompleter;	// = new QCompleter(_completionModel, this);
	    QStackedWidget		*_lineedits;
	    TabBar			*_tabbar;
		// QWebEngineProfile
	    Profile			*_profile;
	    QWebEngineView		*_fullscreenview;
	    FullScreenNotification	*_fullscreennotification;
	    std::pair<QUrl, bool>	_current_download_acceptance;
	    int _previous_index = - 1;

	protected:

		// active_record _active_record;
		// sd::_interface<sd::meta_info<void *>, WebView *, Record *const> _active;
		////        sd::method<sd::meta_info<void *const>> _active_r;
		// boost::intrusive_ptr<TreeItem> _shadow_branch_root;
		// TableModel              *_shadow_source_model;
		// std::shared_ptr<TableData>  _table_data;


	    friend class Browser;
	    friend class ToolbarSearch;
    };

	// class PopupWindow :
	////        public TabWidget {  //
	////        public QWidget {
	// public Browser {
	// Q_OBJECT
	// public:
	// PopupWindow(TabWidget *tabmanager, QWebEngineProfile *const setProfile, QUrl const &url, TableController *_record_controller
	// , TableController *_page_controller
	////                    , Browser *parent
	// );

	////        QWebEnginePage
	// WebPage *page() const;

	// private Q_SLOTS:
	// void setUrl(const QUrl &url);

	// void adjustGeometry(const QRect &newGeometry);

	// private:
	// QLineEdit   *_addressbar;
	// WebView     *_view;

	// struct ActiveRecordBinder {
	// PopupWindow         *_the;
	// TabWidget           *_tabmanager;
	// QWebEngineProfile   *_profile;
	// TableController     *_record_controller;
	// TableController     *_page_controller;
	// WebView             *_view;

	// ~ActiveRecordBinder() {delete _view;}
	// ActiveRecordBinder(
	// TabWidget *tabmanager
	// , PopupWindow *const the
	// , QWebEngineProfile *profile
	// , TableController *_record_controller
	// , TableController *_page_controller
	// ) :
	// _the(the)
	// , _tabmanager(tabmanager)
	// , _profile(profile)
	// , _record_controller(_record_controller)
	// , _page_controller(_page_controller)
	// , _view(nullptr)
	// {}

	// WebView *binder(boost::intrusive_ptr<Record> record)
	// {
	////            assert(!record->unique_page());   // assert by record it self

	////            if(!record->unique_page())
	// return  _view =
	// new WebView(record, _profile, _tabmanager, _the, _record_controller
	// , _page_controller
	// );
	////                _the->newTab(record, true, _the->_record_controller
	////                             , _the->_page_controller
	////                            );

	////            else
	////                return record->unique_page()->view();
	// }

	// WebView *activator(boost::intrusive_ptr<Record> record)
	// {
	// return record->unique_page()->active();
	// }

	// };

	// };
}

// QT_END_NAMESPACE

#endif	// TABWIDGET_H



