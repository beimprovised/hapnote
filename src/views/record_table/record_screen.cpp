
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QObject>
#include <QMimeData>
#include <QProgressBar>
#include <QMap>
#include <QList>
#include <QFontMetrics>

#include "main.h"
#include "record_view.h"
#include "record_screen.h"
#include "views/record_table/vertical_scrollarea.h"
#include "views/main_window/main_window.h"
#include "views/record/meta_editor.h"
#include "models/app_config/app_config.h"
#include "views/find_in_base_screen/find_screen.h"
#include "libraries/window_switcher.h"
#include "libraries/global_parameters.h"
#include "controllers/record_table/record_controller.h"
#include "libraries/flat_control.h"
#include "views/browser/toolbarsearch.h"
#include "models/tree/tree_index.hxx"
#include "views/tree/tree_view.h"
#include "views/tree/tree_screen.h"
#include "views/browser/entrance.h"
#include "models/record_table/record_index.hxx"
#include "models/record_table/record_model.h"
#include "views/browser/browser.h"
#include "views/browser/tabwidget.h"
#include "models/record_table/linker.hxx"
#include "models/tree/tree_know_model.h"
#include "views/record/editentry.h"


extern gl_para globalparameters;
extern AppConfig	appconfig;
extern const char	*tree_screen_viewer_name;

// Виджет, который отображает список записей в ветке
// c кнопками управления


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(rs_t)
#endif


rs_t::rs_t(ts_t			*_tree_screen
	  , FindScreen          *_find_screen
	  , Editentry		*_editentry
	  , browser::Entrance   *_entrance	//	  , browser::Browser    *_browser
	  , wn_t		*_main_window
	  , const QString	&_style_source
	  , browser::Profile    *_profile)
    : QWidget(_main_window->vtab_record())	// , _browser(_browser)
      , _tree_screen(_tree_screen)
      , _main_window(_main_window)
//      , _record_hide(new QAction(tr("Hide record view"), this))	// move to main_window::_vtab_record->tabBar()->tabBarClicked// new QAction(_main_window->h_tree_splitter()->sizes()[0] == 0 ? tr("Show tree view") : tr("Hide tree view"), this)
//      , _save_in_new_branch(new QAction(tr("Save in new branch manually"), this))
#ifdef USE_BUTTON_PIN
      , _pin(new QAction(tr("Pin note"), this))
#endif
#ifdef USE_BLANK_ITEM
      , _addnew_to_end(new QAction(tr("Add note"), this))
      , _addnew_before(new QAction(tr("Add note before"), this))
      , _addnew_after(new QAction(tr("Add note after"), this))
#endif
      , _edit_field(new QAction(tr("Edit properties (name, url, tags...)"), this))
#ifdef USE_BUTTON_CLOSE
      , _close(new QAction(tr("Close note(s)"), this))
#endif
      , _delete(new QAction(tr("Delete note(s)"), this))
#ifdef USE_WITHOUT_REGISTERED_TREEITEM
      , _cut(new QAction(tr("&Cut note(s)"), this))
      , _copy(new QAction(tr("&Copy note(s)"), this))
      , _paste(new QAction(tr("&Paste note(s)"), this))
#endif
      , _editor(new QAction(tr("&Editor"), this))
      , _settings(new QAction(tr("&View settings"), this))
      , _action_move_top(new QAction(tr("Move to &Top"), this))
      , _action_move_up(new QAction(tr("Move &Up"), this))
      , _action_move_dn(new QAction(tr("Move &Down"), this))
      , _find_in_base(new QAction(tr("Find in base"), this))
      , _action_syncro(new QAction(tr("Synchronization"), this))
      , _action_walk_history_previous(new QAction(tr("Previous viewing note"), this))
      , _action_walk_history_next(new QAction(tr("Next viewing note"), this))
      , _back(new QAction(tr("Back to item tree"), this))
      , _sort(new QAction(tr("Toggle sorting"), this))
      , _print(new QAction(tr("Print table"), this))
      , _toolsline(new QToolBar(this))
      , _extra_toolsline(new QToolBar(this))
      , _treepathlabel(new QLabel(this))
      ,	_browser(new browser::Browser(_tree_screen, _find_screen, _editentry, this, _entrance, _main_window, _style_source, _profile, Qt::MaximizeUsingFullscreenGeometryHint))	// , _vtab_tree
      , _record_controller(_browser->tabmanager()->record_controller())	// , _tabmanager(_browser->tabmanager())
      , _vertical_scrollarea(new VerticalScrollArea(_record_controller->view(), this))	// std::make_shared<sd::_interface<void (QResizeEvent *), sd::meta_info<void *> > >(&RecordView::resizeEvent, _tabmanager->record_controller()->view())
      , _records_toolslayout(new QHBoxLayout())
      , _records_screenlayout(new QVBoxLayout()){
    assert(_record_controller);
	// , _recordtree_search(new browser::ToolbarSearch(this))
	// Инициализируется контроллер списка записей
	// recordTableController = new RecordTableController(this);

    setObjectName(record_screen_multi_instance_name);

	// _table_controller->setObjectName(object_name + "_controller");

    setup_actions();

	// _record_controller->init();

    setup_ui();
    setup_signals();
    assembly();
	// _inited = true;

    tools_update();



//    auto _vtab_record = _main_window->vtab_record();	// auto	vtab_tree	= _main_window->vtab_tree();
//    if(_vtab_record->indexOf(this) == - 1){
//	////        int index = vtab_tree->currentIndex();
////	    tsv_t			*tree_viewer_nullptr	= nullptr;
////	    std::vector<tsv_t *>	tree_viewers		= _main_window->tree_viewers();
//////	    int				tree_viewer_count	= tree_viewers.size();
//////            for(int i = 0; i < vtab_tree->count(); i ++){
//////                if(vtab_tree->widget(i)->objectName() == tree_screen_viewer_name){
//////                    tsvs.push_back(dynamic_cast<TreeScreenViewer *>(vtab_tree->widget(i)));
//////                    tree_viewer_count ++;
//////                }
//////            }
////	    for(auto tree_viewer : tree_viewers){
//////		if(1 == tree_viewer_count){
//////		tree_viewer = tree_viewers.back();
//////		if(tree_viewer){
//////		    rs_t *rs = dynamic_cast<rs_t *>(tree_viewer->widget_right());
////		if(! (dynamic_cast<rs_t *>(tree_viewer->widget_right()) || dynamic_cast<DownloadManager *>(tree_viewer->widget_right()))){
////		    tree_viewer_nullptr = tree_viewer;
////		    break;
//////		    vtab_tree->setUpdatesEnabled(false);

//////		    vtab_tree->insertTab(vtab_tree->indexOf(tree_viewer) + 1, new tsv_t(_tree_screen, _record_screen), QIcon(":/resource/pic/three_leaves_clover.svg"), QString("Browser"));			// QString("Browser ") + QString::number(tree_viewer_count)
//////			// vtab_tree->setCurrentIndex(index);
//////		    vtab_tree->setUpdatesEnabled(true);
//////		    }else if(){
//////			tree_viewer->widget_right(_record_screen);
////			// emit vtab_tree->tabBarClicked(vtab_tree->indexOf(tsv));    // vtab_tree->currentChanged(vtab_tree->indexOf(tsv));
////		}// else tree_viewer->widget_right(_record_screen);
//////		}
//////		}else{
//////		    vtab_tree->setUpdatesEnabled(false);

//////		    vtab_tree->insertTab(vtab_tree->indexOf(tree_viewers.back()) + 1, new tsv_t(_tree_screen, _record_screen), QIcon(":/resource/pic/three_leaves_clover.svg"), QString("Browser"));		// QString("Browser ") + QString::number(tree_viewer_count)
//////			// vtab_tree->setCurrentIndex(index);
//////		    vtab_tree->setUpdatesEnabled(true);
//////		}
////	    }
////	    if(tree_viewer_nullptr)tree_viewer_nullptr->widget_right(_record_screen);
////	    else{
////		vtab_tree->setUpdatesEnabled(false);

////		vtab_tree->insertTab(vtab_tree->indexOf(tree_viewers.back()) + 1, new tsv_t(_main_window, _tree_screen, _record_screen), QIcon(":/resource/pic/three_leaves_clover.svg"), QString("Browser"));			// QString("Browser ") + QString::number(tree_viewer_count)
////		// vtab_tree->setCurrentIndex(index);
////		vtab_tree->setUpdatesEnabled(true);
////	    }
//	// int index = vtab_record->currentIndex();
//	_vtab_record->setUpdatesEnabled(false);
//	_vtab_record->addTab(this, QIcon(":/resource/pic/three_leaves_clover.svg"), QString("Browser"));	// QString("Browser ") + QString::number(vtab_record->count())
//	auto _browsers = _vtab_record->browsers();

//	bool found = false;
//	for(auto i = _browsers.begin(); i != _browsers.end(); i ++){
//	    if(*i == this->_browser){
//		found = true;
//		break;
//	    }
//	}
//	if(! found)_browsers.insert(_browser);
//	_vtab_record->setUpdatesEnabled(true);
//	this->adjustSize();
//	// vtab_record->setCurrentIndex(index);
//    }
////    QSplitter *_h_right_splitter = _main_window->h_right_splitter();
////    if(_h_right_splitter->count() > 1){
////	for(int i = 0; i < _h_right_splitter->count(); i ++){
////	    auto wg = _h_right_splitter->widget(i);
////	    if(wg->objectName() == record_screen_multi_instance_name){wg->hide();wg->setParent(nullptr);}
////	}
////    }
////    if(_h_right_splitter->indexOf(this) == - 1) _h_right_splitter->insertWidget(0, _record_screen);	// vtab_record->addTab(_record_screen, QIcon(":/resource/pic/clover.svg"), QString("Browser ") + QString::number(vtab_record->count()));
//////        auto vtab_record = _main_window->vtab_tree();
////    QSplitter *_h_right_splitter = _main_window->h_right_splitter();
////    if(_h_right_splitter->count() > 1){
////	for(int i = 0; i < _h_right_splitter->count(); i ++){
////	    auto wg = _h_right_splitter->widget(i);
////	    if(wg->objectName() == record_screen_multi_instance_name){wg->hide();wg->parent(nullptr);}
////	}
////    }
////    if(_h_right_splitter->indexOf(_record_screen) == - 1){
////	_h_right_splitter->insertWidget(0, _record_screen);	// vtab_record->addTab(_record_screen, QIcon(":/resource/pic/clover.svg"), QString("Browser ") + QString::number(vtab_record->count()));
//////            vtab_record->setCurrentWidget(_record_screen);
////    }
}

rs_t::~rs_t(){
	// delete _recordtree_search;
	// delete
    if(_record_controller) _record_controller->deleteLater();
	// delete
    if(_browser){_browser->close();_browser->deleteLater();_browser = nullptr;}
    _vertical_scrollarea->deleteLater();
}

// void rs_t::save_in_new_branch(bool checked){
//    Q_UNUSED(checked)
//    ts_t * _tree_screen = globalparameters.tree_screen();											// find_object<TreeScreen>(tree_screen_singleton_name);
//    assert(_tree_screen);
//    auto _entrance = globalparameters.entrance();
//    assert(_entrance);
//    auto _tree_source_model = [&](){return _tree_screen->view()->source_model();};																						// static_cast<TreeKnowModel *>(tree_screen->tree_view()->model());

//    auto _index = _tree_screen->view()->current_index();
//    if(_index.isValid()){
//	if(_tree_screen && _entrance){
//		// assert(objectName() != "");

//	    QMap<QString, QString> data;

////	    QDateTime	ctime_dt	= QDateTime::currentDateTime();
//	    QString ctime = get_qtime();	// ctime_dt.toString("yyyyMMddhhmmss");
//		// _item->field("ctime", ctime);

//	    data["id"]		= get_unical_id();
//	    data["name"]	= ctime;
//	    data["ctime"]	= ctime;
//	    data["dir"]		= data["id"];
//	    data["file"]	= "text.html";

//	    boost::intrusive_ptr<TreeItem> _blank_header = TreeItem::dangle_instance(data);	// boost::intrusive_ptr<TreeItem>(new TreeItem(nullptr, data));
//		// = tree_screen->branch_add(tree_source_model, _index
//		// , objectName()    // tree_screen->know_branch()->root_item()   // ->field("name") // ""
//		// , [&](KnowModel * _current_model, QModelIndex _current_index, QString _id, QString _name) {
//		// boost::intrusive_ptr<TreeItem> result;
//		// auto parent = _current_model->item(_current_index)->parent();
//		// assert(parent);
//		// auto same = parent->child(parent->is_name_exists(_name));

//		// if(same) {  // && same->is_empty()
//		// assert(same->name() == _name);
//		// result = same;
//		////            children_transfer(_new_item, _current_model);
//		// } else {
//		//// Вставка новых данных в модель дерева записей
//		// result = _current_model->lock_sibling_add(_current_index, _id, _name);
//		// }

//		// return result;
//		// });
//		////            tree_item->field("name", tree_screen->_shadow_page_model->_root_item->field("name"));

//		////            assert(objectName() != "");
//		////            into_know_branch->field("name", objectName());

//		////            auto target = new_tree_item_in_treeknow_root;    // ->record_table();   // std::make_shared<RecordTable>(tree_item);
//	    auto	_record_model		= [&](){return this->record_controller()->source_model();};																																										// ->record_table();
//	    auto	know_model_board	= [&](){return _tree_screen->view()->know_model_board();};
//	    for(int i = 0; i < _record_model()->size(); i ++){
//		auto it = _record_model()->item(pos_source(i));
//		if(! know_model_board()->item([=](boost::intrusive_ptr<const TreeItem> t){
//			return t->id() == it->field<id_type>();
//		    })){																																																// source_model->item(i)->field("id")
//		    if(it->is_lite())it->to_fat();
//			// _record_model->item(i)->parent(_result_item);    // inside child_rent?
//		    _blank_header << it;																																																// _blank_header->child_rent(_record_model()->item(i));   // _result_item->work_pos(),
//		}
//	    }
//	    _tree_screen->view()->move_children(TreeIndex::instance(_tree_source_model, _tree_screen->view()->current_item()), _blank_header, [&](boost::intrusive_ptr<const Linker> target, boost::intrusive_ptr<const Linker> source) -> bool {
//		    return target->host()->field<url_type>() == source->host()->field<url_type>() && target->host()->field<name_type>() == source->host()->field<name_type>();
//		});
//		// new_tree_item_in_treeknow_root = target;
//	    _tree_screen->view()->synchronized(false);
//	    _tree_screen->view()->know_model_save();
//		// tree_screen->to_candidate_screen(entrance->shadow_branch()->index(tree_item));
//	}
//    }
// }

// Настройка возможных действий
void rs_t::setup_actions(void){
////    QList<int>	sizes		= _main_window->h_tree_splitter()->sizes();
//    QString _hide_tree_text = tr("Hide record view");	// sizes[0] == 0 ? tr("Show record view") : tr("Hide record view");
//    _record_hide->setText(_hide_tree_text);
//    _record_hide->setToolTip(_hide_tree_text);
//    _record_hide->setStatusTip(_hide_tree_text);
//    QIcon _hide_tree_icon = QIcon(":/resource/pic/butterfly-left.svg");	// sizes[0] == 0 ? QIcon(":/resource/pic/butterfly-right.svg") : QIcon(":/resource/pic/butterfly-left.svg");
//    _record_hide->setIcon(_hide_tree_icon);
//    assert(_main_window->h_tree_splitter());
//    connect(_main_window->h_tree_splitter(), &QSplitter::splitterMoved, [&, _hide_tree_text, _hide_tree_icon](int _tree_pos = 0, int index = 0) mutable {
//	    (void) _tree_pos;
//	    (void) index;
//	    auto old_tree_sizes = appconfig.h_tree_splitter_sizelist();
//	    auto h_record_splitter = _main_window->h_record_splitter();
//	    auto record_sizes = h_record_splitter->sizes();
//	    auto vtab_tree = _main_window->vtab_tree();
//	    auto bar_width = vtab_tree->tabBar()->geometry().width();
//	    if(_tree_pos <= bar_width){	// if(bar_width >= sizes[0]){																														// h_left_splitter->widget(0)->width()
//		_hide_tree_icon = QIcon(":/resource/pic/butterfly-right.svg");
//		_hide_tree_text = tr("Show tree view");
////		vtab_tree->resize(bar_width, vtab_tree->height());
////		vtab_tree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
//		vtab_tree->setMinimumWidth(bar_width);
////		vtab_tree->setMaximumWidth(bar_width);
//		if(! _toolsline->actions().contains(_tree_show))insert_action_as_button<QToolButton>(_toolsline, _pin, _tree_show);
//	    }else{
//		_hide_tree_icon = QIcon(":/resource/pic/butterfly-left.svg");
//		_hide_tree_text = tr("Hide tree view");
////		vtab_tree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//		vtab_tree->setMaximumWidth(_main_window->maximumWidth());
//		if(_toolsline->actions().contains(_tree_show))_toolsline->removeAction(_tree_show);
//	    }
//	    _tree_show->setIcon(_hide_tree_icon);
//	    _tree_show->setToolTip(_hide_tree_text);
//	    _tree_show->setStatusTip(_hide_tree_text);
//	    _tree_show->setText(_hide_tree_text);
//	    auto tree_siezes = _main_window->h_tree_splitter()->sizes();
////	    emit _tree_screen->_actionlist[action_show_hide_record_screen]->triggered();
//	    auto summary = record_sizes[0] + record_sizes[1];
//	    record_sizes[0] = record_sizes[0] + (tree_siezes[0] - old_tree_sizes[0]);
//	    record_sizes[1] = summary - record_sizes[0];
//	    h_record_splitter->setSizes(record_sizes);
//		//	    if(0 != pos){
////	    auto tree_siezes = _main_window->h_tree_splitter()->sizes();
//	    if(tree_siezes != appconfig.h_tree_splitter_sizelist())appconfig.h_tree_splitter_sizelist(tree_siezes);
//		//	    }
//	});
//    // move to main_window::_vtab_record->tabBar()->tabBarClicked
//    connect(_record_hide, &QAction::triggered, [&, _hide_tree_text, _hide_tree_icon]() mutable {
////	    auto h_tree_splitter = _main_window->h_tree_splitter();
//	    auto _h_record_splitter = _main_window->h_record_splitter();
////	    auto h_record_sizes = h_record_splitter->sizes();
//		////        if(_tree_screen->isHidden()) _tree_screen->show(); else _tree_screen->hide();
//		// if(h_left_splitter->width() != 0) h_left_splitter->resize(0, h_left_splitter->height());//adjustSize();
//		// else h_left_splitter->resize(h_left_splitter->sizeHint().width(), h_left_splitter->height());
//		// auto h_left_splitter = globalparameters.mainwindow()->h_left_splitter();
//		// auto h_right_splitter = globalparameters.mainwindow()->h_right_splitter();
//		// auto ll = h_left_splitter->geometry().left();   // 0 // width();  // 1366
//		// auto lr = h_left_splitter->handle(1)->geometry().right();  // 143
//		// auto rl = h_right_splitter->geometry().left();  // 142
//	    auto _vtab_record = _main_window->vtab_record();
//	    auto bar_width = _vtab_record->tabBar()->geometry().width();	// same as tabRect(0).width()
////            auto bar_width_ = _main_window->vtab_tree()->tabBar()->tabRect(0).width();	// width = large; minimumWidth() == 0;
//	    auto h_record_sizes = _h_record_splitter->sizes();	// auto h_tree_sizes = h_tree_splitter->sizes();
//////	    QList<int> delta;
////	    if(h_record_sizes[0] <= bar_width){	// show	// h_left_splitter->widget(0)->width()
////		auto vtab_tree_min_width = std::max(_vtab_record->minimumSizeHint().width(), _vtab_record->tabBar()->geometry().width());// _tree_screen->minimumSizeHint().width();     // globalparameters.entrance()->activated_browser()->record_screen()->minimumSizeHint().width();           // 6xx   // h_right_splitter->widget(0)->width();    // 0    // sizeHint().width();    // 23
////		// auto h = h_right_splitter->handle(1);
////		// h->move(lr + shw, h->rect().top());
////		auto tree_size_memory = appconfig.h_tree_splitter_sizelist();
////		auto tree_sum = tree_size_memory[0] + tree_size_memory[1];
////		h_record_sizes[0] = tree_size_memory[0] > vtab_tree_min_width ? tree_size_memory[0] < tree_sum ? tree_size_memory[0] : tree_sum * 15 / 100 : vtab_tree_min_width;
////		h_record_sizes[1] = tree_sum - h_record_sizes[0] > 0 ? tree_sum - h_record_sizes[0] : tree_sum * 85 / 100;
////		_vtab_record->setMaximumWidth(_main_window->maximumWidth());	// just a very big number
////		_vtab_record->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	// sizes[1] > size_memory[1] ? size_memory[1] : sizes[1];
//////		sizes[0] = size_memory[0] > vtab_tree_min_width ? size_memory[0] : vtab_tree_min_width;
//////		sizes[1] = size_memory[0] + size_memory[1] - sizes[0];		// sizes[1] > size_memory[1] ? size_memory[1] : sizes[1];
////////            h_left_splitter->moveSplitter(sizes[0], 1);   // protected member
//////		_hide_tree_icon = QIcon(":/resource/pic/butterfly-left.svg");
//////		_hide_tree_text = tr("Hide tree view");
////////            h_right_splitter->resize(h_right_splitter->sizeHint().width(), h_right_splitter->height());
////	    }else{	// hide
////			// h_right_splitter->resize(h_left_splitter->sizeHint().width(), h_right_splitter->height());
//	    h_record_sizes[1] = h_record_sizes[0] + h_record_sizes[1] - bar_width;
//	    h_record_sizes[0] = bar_width;	// 0;
////		vtab_tree->resize(bar_width, vtab_tree->height());
////		vtab_tree->setMaximumWidth(bar_width);
////	    for(int i = 0; i < _vtab_record->count(); i ++)_vtab_record->widget(i)->hide();
//	    emit _vtab_record->_hide_action->setChecked(true);	// _vtab_record->setMinimumWidth(bar_width);
//	    _vtab_record->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
//////		_hide_tree_icon = QIcon(":/resource/pic/butterfly-right.svg");
//////		_hide_tree_text = tr("Show tree view");
////	    }
//////	    delta << h_tree_splitter->sizes()[0] - h_tree_sizes[0];
////////	    delta[1] = h_tree_splitter->sizes()[1] - h_tree_splitter_sizes[1];
//	    if(h_record_sizes != _h_record_splitter->sizes()){
//		_h_record_splitter->setSizes(h_record_sizes);	//
//		emit _h_record_splitter->splitterMoved(h_record_sizes[0], 1);
//	    }
////	    h_record_sizes[0] = h_record_sizes[0] - delta[0];
////	    h_record_sizes[1] = h_record_sizes[1] + delta[0];
////	    if(h_record_sizes != h_record_splitter->sizes()){
////		h_record_splitter->setSizes(h_record_sizes);
////		emit h_record_splitter->splitterMoved(h_record_sizes[0], 1);
////	    }
//////	    _tree_hide->setIcon(_hide_tree_icon);
//////	    _tree_hide->setToolTip(_hide_tree_text);
//////	    _tree_hide->setStatusTip(_hide_tree_text);
//////	    _tree_hide->setText(_hide_tree_text);
////////	    emit _tree_screen->_actionlist[action_show_hide_record_screen]->triggered();
//	});
    if(_tree_screen->_actionlist[action_hide_tree_screen]->text() == tr("Show record screen")) emit _main_window->h_record_splitter()->splitterMoved(_main_window->h_record_splitter()->sizes()[0], 1);
// emit _tree_hide->triggered();

//	// _save_in_new_branch = new QAction(tr("Save in new branch"), this);
//    _save_in_new_branch->setStatusTip(tr("Save new records in new branch manually, not necessary"));
//    _save_in_new_branch->setIcon(QIcon(":/resource/pic/trace.svg"));

//    connect(_save_in_new_branch, &QAction::triggered, this, [&](bool checked = false) mutable {																					// &RecordScreen::save_in_new_branch
//	    Q_UNUSED(checked)

//	    ts_t * _tree_screen = globalparameters.tree_screen();																														// find_object<TreeScreen>(tree_screen_singleton_name);
//	    assert(_tree_screen);
//	    auto tree_view = _tree_screen->view();
//	    browser::Entrance *_entrance = globalparameters.entrance();
//	    assert(_entrance);
//	    auto know_model_board = [&](){
//		return _tree_screen->view()->know_model_board();
//	    };
//		// auto _index = _tree_screen->tree_view()->current_index();
//		// if(_index.isValid()) {
//	    if(_tree_screen && _entrance){
//		QMap<QString, QString> data;
//		auto _source_model = [&](){
//		    return tree_view->source_model();
//		};
//		// auto current_root_item = _source_model()->item(_index);

//		data["id"] = get_unical_id();																																							// current_root_item->id();     // source_model->root_item()->id();     //
//		data["name"] = "current session branch item";																																							// this->tabmanager()->webView(0)->page()->item_link()->name();    //current_root_item->name();   // source_model->root_item()->name();   //

//		boost::intrusive_ptr<TreeItem> branch_item = TreeItem::dangle_instance(data);	// boost::intrusive_ptr<TreeItem>(new TreeItem(nullptr, data));																																							// current_root_item


//		bool modified = false;
//		for(auto &browser : [&] {set<browser::Browser *> bs;for(auto rs : _main_window->vtab_record()->record_screens())bs.insert(rs->browser()); return bs;} ()){
//		    auto tabmanager = browser->tabmanager();																																																// record_controller()->source_model();  // ->record_table();
//		    for(int i = 0; i < tabmanager->count(); i ++){
//			auto page_item = tabmanager->webView(i)->page()->host();
//			if(! know_model_board()->item([=](boost::intrusive_ptr<const TreeItem> t){
//			    return t->id() == page_item->field<id_type>();
//			})){																																																										// item->field("id")
//			    if(page_item->is_lite())page_item->to_fat();
//				// page_item->parent(branch_item);
//				// _source_model()->model_move_as_child_impl(branch_item, page_item, branch_item->work_pos());  // new_branch_item->child_insert(new_branch_item->work_pos(), item);
//			    branch_item << page_item;
//			    modified = true;
//			}
//		    }
//		}
//		if(modified){
//		    tree_view->move_children(	// view_paste_sibling
//			TreeIndex::instance(_source_model, tree_view->session_root_auto(), tree_view->session_root_auto()->parent())	// _tree_screen->tree_view()->current_index() //,
//					    , branch_item
//					    , [&](boost::intrusive_ptr<const Linker> target, boost::intrusive_ptr<const Linker> source) -> bool {return target->host()->field<url_type>() == source->host()->field<url_type>() && target->host()->field<name_type>() == source->host()->field<name_type>();}
//			);

//			// _tree_screen->resetup_model(_source_model()->root_item());
//		    tree_view->synchronized(false);
//		    tree_view->know_model_save();
//		}
//		_source_model()->update_index(_source_model()->index(tree_view->session_root_auto()));
//	    }
//		// }
//	}

//	);
#ifdef USE_BUTTON_PIN
	// _pin = new QAction(tr("Pin note"), this);
    _pin->setStatusTip(tr("Pin a note"));
    _pin->setIcon(QIcon(":/resource/pic/pin.svg"));
    connect(_pin, &QAction::triggered
	   , [&](bool checked = false) mutable {
	    Q_UNUSED(checked)
		// Обновление инфополей в области редактирования записи
	    MetaEditor * metaeditor = globalparameters.meta_editor();																														// MetaEditor *metaEditor = find_object<MetaEditor>(meta_editor_singleton_name);
	    if(metaeditor) metaeditor->switch_pin();
	}
	);
#endif

#ifdef USE_BLANK_ITEM
	// Добавление записи
	// a->setShortcut(tr("Ctrl+X"));
	// _addnew_to_end = new QAction(tr("Add note"), this);
    _addnew_to_end->setStatusTip(tr("Add a new note"));
    _addnew_to_end->setIcon(QIcon(":/resource/pic/note_add.svg"));
	// setIcon(style()->standardIcon(QStyle::SP_FileIcon, 0, this));
    connect(_addnew_to_end, &QAction::triggered
	   , _record_controller																				// _tabmanager
	   , std::bind(&rctl_t::addnew_blank, _record_controller, add_new_record_to_end)																				// &RecordController::addnew_to_end  // &browser::TabWidget::addnew_to_end
	);

	// Добавление записи до
	// _addnew_before = new QAction(tr("Add note before"), this);
    _addnew_before->setStatusTip(tr("Add a note before selected"));
    connect(_addnew_before, &QAction::triggered
	   , _record_controller																				// _tabmanager
	   , std::bind(&rctl_t::addnew_blank, _record_controller, add_new_record_before)																				// &RecordController::addnew_before  // &browser::TabWidget::addnew_before
	);

	// Добавление записи после
	// _addnew_after = new QAction(tr("Add note after"), this);
    _addnew_after->setStatusTip(tr("Add a note after selected"));
    connect(_addnew_after, &QAction::triggered
	   , _record_controller																				// _tabmanager
	   , std::bind(&rctl_t::addnew_blank, _record_controller, add_new_record_after)																				// &RecordController::addnew_after   // &browser::TabWidget::addnew_after
	);
#endif

	// едактирование записи
	// _edit_field = new QAction(tr("Edit properties (pin, name, author, url, tags...)"), this);
    _edit_field->setStatusTip(tr("Edit note properties (pin, name, author, url, tags...)"));
    _edit_field->setIcon(QIcon(":/resource/pic/note_edit.svg"));
    connect(_edit_field, &QAction::triggered, _record_controller, &rctrl_t::on_edit_fieldcontext);										// _tabmanager
	// &browser::TabWidget::on_edit_fieldcontext

#ifdef USE_BUTTON_CLOSE
	// Удаление записи
	// _delete = new QAction(tr("Delete note(s)"), this);
    _close->setStatusTip(tr("Close note(s)"));
    _close->setIcon(QIcon(":/resource/pic/note_close.svg"));
    _close->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
    connect(_close, &QAction::triggered, _record_controller, &rctl_t::close_context);											// _tabmanager
	// &browser::TabWidget::close_context
#endif

    _delete->setStatusTip(tr("Delete note(s) for ever"));
    _delete->setIcon(QIcon(":/resource/pic/note_delete.svg"));
    connect(_delete, &QAction::triggered, _tree_screen, [&](bool checked = false) mutable -> void {
	    Q_UNUSED(checked);
	    auto _tree_view = _tree_screen->view();
	    auto _current_model = [&](){
		return _tree_view->source_model();
	    };
	    auto _item = _browser->tabmanager()->currentWebView()->page()->host();
	    _tree_view->delete_permanent(_current_model
					, QList<boost::intrusive_ptr<TreeItem> >() << _item
					, &tkm_t::delete_permanent
					, "cut"
					, false
	    );
	});


#ifdef USE_WITHOUT_REGISTERED_TREEITEM


	// Удаление записи с копированием в буфер обмена
	// _cut = new QAction(tr("&Cut note(s)"), this);
    _cut->setStatusTip(tr("Cut notes(s) to clipboard"));
    _cut->setIcon(QIcon(":/resource/pic/cb_cut.svg"));
    connect(_cut, &QAction::triggered, _record_controller, &rctl_t::cut);										// &browser::TabWidget::cut


	// Копирование записи (записей) в буфер обмена
	// _copy = new QAction(tr("&Copy note(s)"), this);
    _copy->setStatusTip(tr("Copy note(s) to clipboard"));
    _copy->setIcon(QIcon(":/resource/pic/cb_copy.svg"));
    connect(_copy, &QAction::triggered, _record_controller, &rctl_t::copy);											// &browser::TabWidget::copy


	// Вставка записи из буфера обмена
	// _paste = new QAction(tr("&Paste note(s)"), this);
    _paste->setStatusTip(tr("Paste note(s) from clipboard"));
    _paste->setIcon(QIcon(":/resource/pic/cb_paste.svg"));
    connect(_paste, &QAction::triggered, _record_controller, &rctl_t::paste);											// _tabmanager
	// &browser::TabWidget::paste

#endif
	// auto _editor = new QAction(tr("Editor"), this);
    _editor->setStatusTip(tr("Switch editor open / close"));
    _editor->setIcon(QIcon(":/resource/pic/attach_switch_to_editor.svg"));
    _editor->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(_editor, &QAction::triggered, _main_window, &wn_t::editor_switch);


	// Настройка внешнего вида таблицы конечных записей
	// _settings = new QAction(tr("&View settings"), this);
    _settings->setStatusTip(tr("Setup table view settins"));
    _settings->setIcon(QIcon(":/resource/pic/cogwheel.svg"));
    connect(_settings, &QAction::triggered, _record_controller, &rctrl_t::settings);											// &browser::TabWidget::settings

    assert(_record_controller);
	// Перемещение записи вверх
	// _action_move_up = new QAction(tr("&Move Up"), this);
    _action_move_top->setStatusTip(tr("Move note to top"));
    _action_move_top->setIcon(QIcon(":/resource/pic/pentalpha.svg"));
    connect(_action_move_top, &QAction::triggered
	   , [&]() mutable {
	    qDebug() << "In moveup(, 0)";

		// Получение номера первой выделенной строки
	    pos_proxy pos_proxy_ = _record_controller->view()->selection_first<pos_proxy>();

		// Выясняется ссылка на таблицу конечных данных
		// auto item = _source_model->browser_pages();
	    pos_proxy pos_target = pos_proxy(0);
		// Перемещение текущей записи вверх
	    _record_controller->source_model()->move(_record_controller->index<pos_source>(pos_proxy_), _record_controller->index<pos_source>(pos_target));

		// Установка засветки на перемещенную запись
	    _record_controller->select_as_current(pos_target);

		// Сохранение дерева веток
		// find_object<TreeScreen>(tree_screen_singleton_name)
	    globalparameters.tree_screen()->view()->know_model_save();
	});	// _record_controller, &RecordController::move_up

    assert(_record_controller);
	// Перемещение записи вверх
	// _action_move_up = new QAction(tr("&Move Up"), this);
    _action_move_up->setStatusTip(tr("Move note up"));
    _action_move_up->setIcon(QIcon(":/resource/pic/triangl_up.svg"));
    connect(_action_move_up, &QAction::triggered
	   , [&]() mutable {
	    qDebug() << "In moveup()";

		// Получение номера первой выделенной строки
	    pos_proxy pos_proxy_ = _record_controller->view()->selection_first<pos_proxy>();

		// Выясняется ссылка на таблицу конечных данных
		// auto item = _source_model->browser_pages();

		// Перемещение текущей записи вверх
	    _record_controller->source_model()->move(_record_controller->index<pos_source>(pos_proxy_));

		// Установка засветки на перемещенную запись
	    _record_controller->select_as_current(pos_proxy((int) pos_proxy_ - 1));

		// Сохранение дерева веток
		// find_object<TreeScreen>(tree_screen_singleton_name)
	    globalparameters.tree_screen()->view()->know_model_save();
	});	// _record_controller, &RecordController::move_up
	// connect(_action_move_up, &QAction::triggered, _tabmanager, &browser::TabWidget::move_up);

	// Перемещение записи вниз
	// _action_move_dn = new QAction(tr("&Move Down"), this);
    _action_move_dn->setStatusTip(tr("Move note down"));
    _action_move_dn->setIcon(QIcon(":/resource/pic/triangl_dn.svg"));
    connect(_action_move_dn, &QAction::triggered
	   , [&]() mutable {
	    qDebug() << "In movedn()";

		// Получение номера первой выделенной строки
	    pos_proxy pos_proxy_ = _record_controller->view()->selection_first<pos_proxy>();

		// Выясняется ссылка на таблицу конечных данных
		// auto item = _source_model->browser_pages();
	    pos_proxy target = static_cast<int>(pos_proxy_) < _record_controller->tabmanager()->count() - 1 ? pos_proxy((int) pos_proxy_ + 1) : pos_proxy(_record_controller->tabmanager()->count() - 1);
		// Перемещение текущей записи вниз
	    _record_controller->source_model()->move(_record_controller->index<pos_source>(pos_proxy_), _record_controller->index<pos_source>(target));

		// Установка засветки на перемещенную запись
	    _record_controller->select_as_current(pos_proxy((int) pos_proxy_ + 1));

		// Сохранение дерева веток
		// find_object<TreeScreen>(tree_screen_singleton_name)
	    globalparameters.tree_screen()->view()->know_model_save();
	});	// _record_controller, &RecordController::move_dn
		// connect(_action_move_dn, &QAction::triggered, _tabmanager, &browser::TabWidget::move_dn);

	// Поиск по базе (клик связывается с действием в MainWindow)
	// _find_in_base = new QAction(tr("Find in base"), this);
    _find_in_base->setStatusTip(tr("Find in base"));
    _find_in_base->setIcon(QIcon(":/resource/pic/find_in_base.svg"));

	// Синхронизация
	// _action_syncro = new QAction(tr("Synchronization"), this);
    _action_syncro->setStatusTip(tr("Run synchronization"));
    _action_syncro->setIcon(QIcon(":/resource/pic/synchronization.svg"));
    connect(_action_syncro, &QAction::triggered, this, &rs_t::on_syncro_click);

	// Перемещение по истории посещаемых записей назад
	// _action_walk_history_previous = new QAction(tr("Previous viewing note"), this);
    _action_walk_history_previous->setShortcut(tr("Ctrl+<"));
    _action_walk_history_previous->setStatusTip(tr("Previous note has been viewing"));
    _action_walk_history_previous->setIcon(QIcon(":/resource/pic/walk_history_previous.svg"));
    connect(_action_walk_history_previous, &QAction::triggered, this, &rs_t::on_walkhistory_previous_click);

	// Перемещение по истории посещаемых записей вперед
	// _action_walk_history_next = new QAction(tr("Next viewing note"), this);
    _action_walk_history_next->setShortcut(tr("Ctrl+>"));
    _action_walk_history_next->setStatusTip(tr("Next note has been viewing"));
    _action_walk_history_next->setIcon(QIcon(":/resource/pic/walk_history_next.svg"));
    connect(_action_walk_history_next, &QAction::triggered, this, &rs_t::on_walkhistory_next_click);

	// Кнопка Назад (Back) в мобильном интерфейсе
	// _back = new QAction(tr("Back to item tree"), this);
    _back->setStatusTip(tr("Back to item tree"));
    _back->setIcon(QIcon(":/resource/pic/mobile_back.svg"));
    connect(_back, &QAction::triggered, this, &rs_t::on_back_click);

	// Действия по сортировке
	// _sort = new QAction(tr("Toggle sorting"), this);
    _sort->setStatusTip(tr("Sorting by column"));// Enable/disable
    _sort->setIcon(QIcon(":/resource/pic/sort.svg"));
    connect(_sort, &QAction::triggered, [&](bool){
	    auto indicator = _sort->data().toInt();
	    auto horizontal_header = _record_controller->view()->horizontalHeader();
	    if(horizontal_header->sortIndicatorOrder() == Qt::AscendingOrder){
		horizontal_header->setSortIndicator(indicator, Qt::DescendingOrder);
		_record_controller->on_sort_request(indicator, Qt::DescendingOrder);
	    }else{
		horizontal_header->setSortIndicator(indicator, Qt::AscendingOrder);
		_record_controller->on_sort_request(indicator, Qt::AscendingOrder);
	    }
	});		// _record_controller, &rctrl_t::on_sort_click);


	// Кнопка вызова печати таблицы конечных записей
	// _print = new QAction(tr("Print table"), this);
    _print->setStatusTip(tr("Print current notes table"));
    _print->setIcon(QIcon(":/resource/pic/drops.svg"));											// actionPrint->setIcon(QIcon(":/resource/pic/print_record_table.svg"));
    connect(_print, &QAction::triggered, _record_controller, &rctrl_t::on_print_click);
	// _tabmanager
	// &browser::TabWidget::on_print_click


	// Сразу после создания все действия запрещены
    disable_all_actions();
}

void rs_t::setup_ui(void){
//    _toolsline = new QToolBar(this);
//    QSize toolBarIconSize(16, 16);
//    toolsLine->setIconSize(toolBarIconSize);
//    setStyleSheet("border : 0px;");
    if(appconfig.interface_mode() == "mobile"){
	append_action_as_button<QToolButton>(_toolsline, _back);
	_toolsline->addSeparator();
    }
//    append_action_as_button<QToolButton>(_toolsline, _record_hide);	// move to main_window::_vtab_record->tabBar()->tabBarClicked
#ifdef USE_BUTTON_PIN
    append_action_as_button<QToolButton>(_toolsline, _pin);
#endif

//    append_action_as_button<QToolButton>(_toolsline, _save_in_new_branch);
#ifdef USE_BLANK_ITEM
    append_action_as_button<QToolButton>(_toolsline, _addnew_to_end);
#endif
    if(appconfig.interface_mode() == "desktop"){
	append_action_as_button<QToolButton>(_toolsline, _edit_field);
#ifdef USE_BUTTON_CLOSE
	append_action_as_button<QToolButton>(_toolsline, _close);
#endif
    }
    append_action_as_button<QToolButton>(_toolsline, _action_walk_history_previous);
    append_action_as_button<QToolButton>(_toolsline, _action_walk_history_next);
    append_action_as_button<QToolButton>(_toolsline, _action_move_top);
    append_action_as_button<QToolButton>(_toolsline, _action_move_up);
    append_action_as_button<QToolButton>(_toolsline, _action_move_dn);

//    _toolsline->addSeparator();
#ifdef USE_WITHOUT_REGISTERED_TREEITEM
    append_action_as_button<QToolButton>(_toolsline, _cut);
    append_action_as_button<QToolButton>(_toolsline, _copy);
    append_action_as_button<QToolButton>(_toolsline, _paste);
#endif
    append_action_as_button<QToolButton>(_toolsline, _delete);

    _toolsline->addSeparator();

//    append_action_as_button<QToolButton>(_toolsline, _sort);
    append_action_as_button<QToolButton>(_toolsline, _print);
    append_action_as_button<QToolButton>(_toolsline, _settings);
	// _extra_toolsline = new QToolBar(this);
    if(appconfig.interface_mode() == "desktop"){
	append_action_as_button<QToolButton>(_extra_toolsline, _action_syncro);
	// insertActionAsButton(extraToolsLine, actionWalkHistoryPrevious);
	// insertActionAsButton(extraToolsLine, actionWalkHistoryNext);
    }
    append_action_as_button<QToolButton>(_extra_toolsline, _find_in_base);

    append_action_as_button<QToolButton>(_extra_toolsline, _tree_screen->_actionlist[action_main_menu]);
	// _treepathlabel = new QLabel(this);

	// _treepath_button = new FlatToolButton(this);

    _treepathlabel->setWordWrap(true);
	// treePathLabel->setLineWidth(contentsRect().width());
	// treePathLabel->setAlignment(Qt::AlignRight);
	// treePathLabel->baseSize();
	// int width = recordTableController->getView()->contentsRect().width();
	// treePathLabel->setMaximumWidth(contentsRect().width());
	// treePathLabel->setMinimumWidth(contentsRect().width());
    if(appconfig.interface_mode() == "desktop") _treepathlabel->hide();
	// _vertical_scrollarea = new VerticalScrollArea(
	// std::make_shared<sd::_interface<sd::meta_info<void *>, void, QResizeEvent *>>("", &RecordView::resizeEvent, _record_controller->view())
	// , this
	// );
}

void rs_t::setup_signals(void){
	// connect(this, &RecordTableScreen::resizeEvent, _treepath_button
	//// , &QLabel::resizeEvent
	// , [this](QResizeEvent * e) {
	////        int width = recordTableController->getView()->contentsRect().width();
	// _treepath_button->setMaximumWidth(
	// e->size().width()   //
	////            contentsRect().width()
	// );
	// _treepath_button->setMinimumWidth(
	// e->size().width()   //
	////            contentsRect().width()
	// );

	// }
	// );
    connect(this->_find_in_base, &QAction::triggered, globalparameters.window_switcher(), &WindowSwitcher::find_in_base_click);
}

void rs_t::assembly(void){
	// _recordtable_toolslayout = new QHBoxLayout();




    {
	_records_toolslayout->setSpacing(0);
	_records_toolslayout->setMargin(0);
	_records_toolslayout->setContentsMargins(0, 1, 0, 1);

	_toolsline->setContentsMargins(0, 0, 0, 0);
	_records_toolslayout->addWidget(_toolsline);

	_records_toolslayout->addStretch();

	_extra_toolsline->setContentsMargins(0, 0, 0, 0);
	_records_toolslayout->addWidget(_extra_toolsline);

// _tree_screen->menubar()->setContentsMargins(0,0,0,0);
// _records_toolslayout->addWidget(_tree_screen->menubar());
//// _recordtree_searchlayout = new QHBoxLayout();
//// _recordtree_searchlayout->addWidget(_recordtree_search);
//// _recordtable_screenlayout = new QVBoxLayout();
	_records_screenlayout->setObjectName(objectName() + "_qvboxlayout");



	////    QHBoxLayout *_treepath_button_layout = new QHBoxLayout();

	////    _treepath_button_layout->addWidget(_treepath_button);
	////    _treepath_button_layout->setMargin(0);
	////    _treepath_button_layout->setSpacing(0);
	//////    _treepath_button_layout->setSizeConstraint();

	////    if(appconfig.getInterfaceMode() == "mobile") {
	// _recordtable_screenlayout->addWidget(
	////        _treepath_button_layout    //
	// _treepath_button
	// );
	////    }

	// _recordtable_screenlayout->addLayout(_recordtree_searchlayout);




	_records_screenlayout->addLayout(_records_toolslayout);
    }




	//// how to use VerticalScrollArea class:
	// RecordView *record_view = _record_controller->view(); //    auto record_view = new QWidget(this);

	// record_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	// _vertical_scrollarea->setWidget(record_view);
	// record_view->viewport()->installEventFilter(_vertical_scrollarea);
    QVBoxLayout *_vertical_scrollarea_layout = new QVBoxLayout();

    _vertical_scrollarea_layout->setSpacing(0);
    _vertical_scrollarea_layout->setMargin(0);
    _vertical_scrollarea_layout->setContentsMargins(0, 0, 0, 0);

    _vertical_scrollarea->setContentsMargins(0, 0, 0, 0);
    _vertical_scrollarea_layout->addWidget(_vertical_scrollarea);
	// _verticalscrollarea->viewport()->installEventFilter(rtview);


    _records_screenlayout->addLayout(_vertical_scrollarea_layout);										//

	// _recordtable_screenlayout->addWidget(_table_controller->view());

	// recordTableScreenLayout->addWidget(_verticalscrollarea);

	// recordTableController->getView()->viewport()->installEventFilter(someFilterWidget);

    _records_screenlayout->setSpacing(0);
    _records_screenlayout->setMargin(0);
    _records_screenlayout->setContentsMargins(0, 0, 0, 0);





	// _recordtable_screenlayout->setSizeConstraint(QLayout::SetNoConstraint);

    setLayout(_records_screenlayout);

	// Границы убираются, так как данный объект будет использоваться как виджет
    QLayout *lt;
    lt = layout();
    lt->setMargin(0);
    lt->setContentsMargins(0, 0, 0, 0);											// setContentsMargins(0, 2, 0, 0);
    lt->setSpacing(0);
}

void rs_t::resizeEvent(QResizeEvent *e){
    _record_controller->view()->resizeEvent(e);

    QWidget::resizeEvent(e);

	// int rw = contentsRect().width();
	// int vw = recordTableController->getView()->contentsRect().width();
	// int vwe = recordTableController->getView()->width();
	// int w = width();
	// int bw = this->baseSize().width();
	// int border = this->geometry().width();

	// _treepath_button->setMaximumWidth(
	////        e->size().width() - 10 // contentsMargins().left() - contentsMargins().right() //
	// contentsRect().width() - 1
	// );

	// _treepath_button->setMinimumWidth(
	////        e->size().width() - 10//contentsMargins().left() - contentsMargins().right() //
	// contentsRect().width() - 1
	// );
}

// Отключение всех действий над записями
// (но не всех действий на панели инструментов, так как на панели инструментов есть действия, не оказывающие воздействия на записи)
void rs_t::disable_all_actions(void){
	// _save_in_new_branch->setEnabled(false);
#ifdef USE_BUTTON_PIN
    _pin->setEnabled(false);
#endif

#ifdef USE_BLANK_ITEM
    _addnew_to_end->setEnabled(false);
    _addnew_before->setEnabled(false);
    _addnew_after->setEnabled(false);
#endif
    _edit_field->setEnabled(false);
#ifdef USE_BUTTON_CLOSE
    _close->setEnabled(false);
#endif
    _delete->setEnabled(false);
#ifdef USE_WITHOUT_REGISTERED_TREEITEM
    _cut->setEnabled(false);
    _copy->setEnabled(false);
    _paste->setEnabled(false);
#endif
    _action_move_top->setEnabled(false);
    _action_move_up->setEnabled(false);
    _action_move_dn->setEnabled(false);
}

// void RecordScreen::restore_menubar(){
// auto menubar = _tree_screen->menubar();
// if(_records_toolslayout->indexOf(menubar) == -1)
// {_records_toolslayout->addWidget(menubar); }
// }

void rs_t::tools_update(){
    qDebug() << "RecordScreen::tools_update()";

	// Отключаются все действия
    disable_all_actions();

	// if(!_table_controller->is_tree_item_exists())
	// return;

	// Выясняется, содержит ли текущая ветка подчиненные ветки
	/*
	   QModelIndex index = find_object<TreeScreen>(tree_screen_singleton_name)->get_selection_model()->currentIndex();
	   TreeItem *item = find_object<TreeScreen>(tree_screen_singleton_name)->kntrmodel->getItem(index);
	   int branch_have_children=0;
	   if(item->childCount()>0)branch_have_children=1;
	 */

	// Включаются те действия которые разрешены
#ifdef USE_BUTTON_PIN
    _pin->setEnabled(true);
#endif
    rv_t		*_view			= _record_controller->view();
    QItemSelectionModel *item_selection_model	= _view->selectionModel();

//    int		selected_rows	= item_selection_model->selectedIndexes().size();	// (item_selection_model->selectedRows()).size();// always crash because tabmanager inaccessible
    bool	has_selection	= item_selection_model->hasSelection();
    bool	sorting_enabled = _view->isSortingEnabled();
#ifdef USE_BLANK_ITEM
	// Добавление записи
	// Добавлять можно к любой ветке
    _addnew_to_end->setEnabled(true);
	// Добавление записи до
	// Добавлять "до" можно только тогда, когда выбрана только одна строка
	// и не включена сортировка
    if(  has_selection
      && 1 == selected_rows
      && sorting_enabled == false
	){
	_addnew_before->setEnabled(true);
	_addnew_after->setEnabled(true);
    }
 #endif
	//// Добавление записи после
	//// Добавлять "после" можно только тогда, когда выбрана только одна строка
	//// и не включена сортировка
	// if(item_selection_model->hasSelection()
	// && (item_selection_model->selectedRows()).size() == 1
	// && view->isSortingEnabled() == false
	// ) {
	// _addnew_after->setEnabled(true);
	// }
	// едактирование записи
	// едактировать можно только тогда, когда выбрана только одна строка
    if(has_selection	// item_selection_model->hasSelection()
//      && 1 == selected_rows										// (item_selection_model->selectedRows()).size() == 1
	) _edit_field->setEnabled(true);
	// Удаление записи
	// Пункт активен только если запись (или записи) выбраны в списке
    if(has_selection){											// item_selection_model->hasSelection()
#ifdef USE_WITHOUT_REGISTERED_TREEITEM
	_cut->setEnabled(true);
	_copy->setEnabled(true);
#endif
#ifdef USE_BUTTON_CLOSE
	_close->setEnabled(true);
#endif
	_delete->setEnabled(true);
    }
#ifdef USE_WITHOUT_REGISTERED_TREEITEM
	//// Удаление с копированием записи в буфер обмена
	//// Пункт активен только если запись (или записи) выбраны в списке
	// if(item_selection_model->hasSelection()) {
	// _cut->setEnabled(true);
	// }
	//// Копирование записи в буфер обмена
	//// Пункт активен только если запись (или записи) выбраны в списке
	// if(item_selection_model->hasSelection()) {
	// _copy->setEnabled(true);
	// }
	// Вставка записи из буфера обмена
	// Вставлять записи можно только тогда, когда выбрана
	// только одна строка (добавляется после выделеной строки)
	// или не выбрано ни одной строки (тогда добавляется в конец списка)
	// или записей вообще нет
	// И проверяется, содержит ли буфер обмена данные нужного формата
    if(  (has_selection && 1 == selected_rows)											// (item_selection_model->hasSelection() && (item_selection_model->selectedRows()).size() == 1)
      || ! has_selection										// item_selection_model->hasSelection() == false
      || 0 == _view->model()->rowCount()
	){
	const QMimeData *mime_data = QApplication::clipboard()->mimeData();
	if(mime_data != nullptr)
		if(mime_data->hasFormat("hapnote/records")) _paste->setEnabled(true);
    }
#endif
	// Перемещение записи вверх
	// Пункт возможен только когда выбрана одна строка
	// и указатель стоит не на начале списка
	// и не включена сортировка
    if(  has_selection	// item_selection_model->hasSelection()
//      && 1 == selected_rows										// (item_selection_model->selectedRows()).size() == 1
      && false == sorting_enabled										// view->isSortingEnabled() == false
      && _view->is_selected_set_to_top() == false
	){
	_action_move_top->setEnabled(true);
	_action_move_up->setEnabled(true);
    }
	// Перемещение записи вниз
	// Пункт возможен только когда выбрана одна строка
	// и указатель стоит не в конце списка
	// и не включена сортировка
    if(  has_selection	// item_selection_model->hasSelection()
//      && 1 == selected_rows										// (item_selection_model->selectedRows()).size() == 1
      && false == sorting_enabled										// view->isSortingEnabled() == false
      && _view->is_selected_set_to_bottom() == false
	) _action_move_dn->setEnabled(true);
	// Обновляется состояние области редактирования текста
    if(  has_selection	// item_selection_model->hasSelection()
      && _record_controller->row_count() > 0
	){
	qDebug() << "In table select present";
//	qDebug() << "In table row count is" << _record_controller->row_count();
	// find_object<MetaEditor>(meta_editor_singleton_name)
	globalparameters.meta_editor()->textarea_editable(true);
    }else{
	qDebug() << "In table select non present";
	// find_object<MetaEditor>(meta_editor_singleton_name)
	globalparameters.meta_editor()->textarea_editable(false);
    }
}

//// Получение номера первого выделенного элемента в таблице записи на экране
// int RecordScreen::first_selection_pos(void)
// {
// return _record_controller->first_selectionpos();
// }


//// Получение ID первого выделенного элемента в таблице записи на экране
// QString RecordScreen::first_selection_id(void)
// {
// return _record_controller->first_selectionid();
// }


//// Установка засветки в нужную строку в таблице записи на экране
// void RecordScreen::select_pos(PosProxy pos_proxy_)
// {
// _record_controller->select_pos(pos_proxy_);
// }


//// Установка засветки в нужную запись в таблице записей на экране
// void RecordScreen::select_id(IdType id)
// {
// if(_record_controller->view()->selection_first_id() != id) {
// _record_controller->select_id(id);
// }
// }



// Действия при нажатии кнопки синхронизации
void rs_t::on_syncro_click(void){
	// find_object<MainWindow>("mainwindow")
    globalparameters.main_window()->synchronization();
}

void rs_t::on_walkhistory_previous_click(void){
	// find_object<MainWindow>("mainwindow")
    globalparameters.main_window()->go_walk_history_previous();
}

void rs_t::on_walkhistory_next_click(void){
	// find_object<MainWindow>("mainwindow")
    globalparameters.main_window()->go_walk_history_next();
}

// Возвращение к дереву разделов в мобильном интерфейсе
void rs_t::on_back_click(void){
    globalparameters.window_switcher()->recordtable_to_tree();
}

void rs_t::tree_path(QString path){
    _treepath = path;											// Запоминается путь к ветке в виде строки
    _treepathlabel->setText(tr("<b>Path:</b> ") + _treepath);
	// _treepath_button->setText(tr("<b>Path:</b> ") + _treepath);
}

QString rs_t::tree_path(void){return _treepath;}

rctrl_t *rs_t::record_controller(){return _record_controller;}

// browser::TabWidget *rs_t::tabmanager(){return _tabmanager;}

browser::Browser *rs_t::browser(){return _browser;}

ts_t *rs_t::tree_screen(){return _tree_screen;}

// QAction *rs_t::record_hide(){return _record_hide;}	// move to main_window::_vtab_record->tabBar()->tabBarClicked

