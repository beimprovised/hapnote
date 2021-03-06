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



#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif




#include <thread>
#include "libraries/qt_single_application5/qtsingleapplication.h"

#include "browser.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "featurepermissionbar.h"
#include "networkaccessmanager.h"
#include "ui_passworddialog.h"
#include "ui_proxy.h"
#include "tabwidget.h"
#include "webview.h"
#include "views/main_window/main_window.h"
#include "views/find_in_base_screen/find_screen.h"
#include "libraries/window_switcher.h"
#include "libraries/walk_history.h"
#include "models/attach_table/attach_table_data.h"
#include "views/tree/tree_view.h"
#include "models/tree/tree_index.hxx"
#include "models/tree/tree_know_model.h"
#include "models/tree/binder.hxx"
#include "models/record_table/linker.hxx"
#include "models/record_table/record_index.hxx"
#include "views/record/editentry.h"

#include <QtGui/QClipboard>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtGui/QMouseEvent>
#include <QWebEnginePage>
#include <QWebChannel>

#if defined(QWEBENGINEPAGE_HITTESTCONTENT)
#include <QWebEngineHitTestResult>
#endif

#ifndef QT_NO_UITOOLS
#include <QtUiTools/QUiLoader>
#endif	// QT_NO_UITOOLS

#include <QtCore/QDebug>
#include <QtCore/QBuffer>

#include "main.h"
#include "models/record_table/record_index.hxx"
#include "models/record_table/record_model.h"
#include "models/record_table/items_flat.h"
#include "models/record_table/record.h"
#include "views/record_table/record_view.h"
#include "libraries/global_parameters.h"
#include "views/browser/entrance.h"
#include "views/record_table/record_screen.h"
#include "controllers/record_table/record_controller.h"
// #include "browserview.moc"
#include "libraries/global_parameters.h"
#include "views/record/meta_editor.h"
#include "models/tree/tree_item.h"
#include "views/tree/tree_screen.h"
// #include "views/browser/browser.h"



#include <iostream>

class gl_para;
extern gl_para globalparameters;
extern AppConfig	appconfig;
extern bool		url_equal(const std::string &url_compare_stored, const std::string &url_compare_get);

namespace browser {
#ifdef USE_POPUP_WINDOW

#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(PopupPage)
#endif

    PopupPage::PopupPage(PopupView *view_, Browser *browser_, Profile *profile_)
	: QWebEnginePage(profile_, view_)
	  , _view(view_)
	  , _browser(browser_)
	  , _keyboard_modifiers(Qt::NoModifier)
	  , _pressed_buttons(Qt::NoButton){
#if defined(QWEBENGINEPAGE_SETNETWORKACCESSMANAGER)
	setNetworkAccessManager(BrowserApplication::networkAccessManager());
#endif
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
	connect(this, &PopupPage::unsupportedContent, this, &PopupPage::handleUnsupportedContent;
#endif
	connect(this, &PopupPage::authenticationRequired, &PopupPage::authenticationRequired);
	connect(this, &PopupPage::proxyAuthenticationRequired, &PopupPage::proxyAuthenticationRequired);
    }

    Browser *PopupPage::browser(){
	return _browser;
//	QObject *w = this->parent();
//	while(w){
//	    if(Browser *mw = qobject_cast<Browser *>(w))return mw;
//	    w = w->parent();
//	}
//	return _entrance->equip_registered().first;	// BrowserApplication::instance()->mainWindow();
    }

    bool PopupPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame){
	Q_UNUSED(type);
	if(isMainFrame){
	    _loading_url = url;
	    emit loadingUrl(_loading_url);
	}
	return true;
    }

    bool PopupPage::certificateError(const QWebEngineCertificateError &error){
	if(error.isOverridable() && ! _certificate_ignored){
	    QMessageBox msgBox;
	    msgBox.setIcon(QMessageBox::Warning);
	    msgBox.setText(error.errorDescription());
	    msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
					 "Accepting an unverified certificate means "
					 "you may not be connected with the host you tried to connect to.\n"
					 "Do you wish to override the security check and continue?"));
	    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	    msgBox.setDefaultButton(QMessageBox::No);

	    return _certificate_ignored = (msgBox.exec() == QMessageBox::Yes);
	}
	// QMessageBox::critical(view(), tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);

	// return false;
	return _certificate_ignored;
    }

	// #include "webview.moc"

	// QWebEnginePage *PopupPage::createWindow(QWebEnginePage::WebWindowType type)
	// {
	// if(type == QWebEnginePage::WebBrowserTab) {
	// return mainWindow()->tabWidget()->newTab()->page();
	// } else if(type == QWebEnginePage::WebBrowserWindow) {
	// BrowserApplication::instance()->newMainWindow();
	// Browser *mainWindow = BrowserApplication::instance()->mainWindow();
	// return mainWindow->currentTab()->page();
	// } else {
	// PopupWindow *popup = new PopupWindow(profile());
	// popup->setAttribute(Qt::WA_DeleteOnClose);
	// popup->show();
	// return popup->page();
	// }
	// }

#if ! defined(QT_NO_UITOOLS)
    QObject *PopupPage::createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues){
	Q_UNUSED(url);
	Q_UNUSED(paramNames);
	Q_UNUSED(paramValues);
	QUiLoader loader;

	return loader.createWidget(classId, view());
    }

#endif	// !defined(QT_NO_UITOOLS)

#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
    void WebPage::handleUnsupportedContent(QNetworkReply *reply){
	QString errorString = reply->errorString();
	if(m_loadingUrl != reply->url()){
		// sub resource of this page
	    qWarning() << "Resource" << reply->url().toEncoded() << "has unknown Content-Type, will be ignored.";
	    reply->deleteLater();

	    return;
	}
	if(reply->error() == QNetworkReply::NoError && ! reply->header(QNetworkRequest::ContentTypeHeader).isValid()) errorString = "Unknown Content-Type";
	QFile file(QLatin1String(":/notfound.html"));
	bool isOpened = file.open(QIODevice::ReadOnly);
	Q_ASSERT(isOpened);
	Q_UNUSED(isOpened)

	QString title = tr("Error loading page: %1").arg(reply->url().toString());
	QString html = QString(QLatin1String(file.readAll()))
	    .arg(title)
	    .arg(errorString)
	    .arg(reply->url().toString());

	QBuffer imageBuffer;
	imageBuffer.open(QBuffer::ReadWrite);
	QIcon	icon	= view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
	QPixmap pixmap	= icon.pixmap(QSize(32, 32));
	if(pixmap.save(&imageBuffer, "PNG")){
	    html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE")
			, QString(QLatin1String(imageBuffer.buffer().toBase64())));
	}
	QList<QWebEngineFrame *> frames;
	frames.append(mainFrame());
	while(! frames.isEmpty()){
	    QWebEngineFrame *frame = frames.takeFirst();
	    if(frame->url() == reply->url()){
		frame->setHtml(html, reply->url());

		return;
	    }
	    QList<QWebEngineFrame *> children = frame->childFrames();

	    foreach(QWebEngineFrame * frame, children)
	    frames.append(frame);
	}
	if(m_loadingUrl == reply->url()) mainFrame()->setHtml(html, reply->url());
    }

#endif

    void PopupPage::authenticationRequired(const QUrl &requestUrl, QAuthenticator *auth){
//	Browser *_browser = _entrance->activiated_registered().first;

	QDialog dialog(_browser);
	dialog.setWindowFlags(Qt::Sheet);

	Ui::PasswordDialog passwordDialog;
	passwordDialog.setupUi(&dialog);

	passwordDialog.iconLabel->setText(QString());
	passwordDialog.iconLabel->setPixmap(_browser->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, _browser).pixmap(32, 32));

	QString introMessage = tr("<qt>Enter username and password for \"%1\" at %2</qt>");
	introMessage = introMessage.arg(auth->realm()).arg(requestUrl.toString().toHtmlEscaped());
	passwordDialog.introLabel->setText(introMessage);
	passwordDialog.introLabel->setWordWrap(true);
	if(dialog.exec() == QDialog::Accepted){
	    QByteArray key = sapp_t::authenticationKey(requestUrl, auth->realm());
	    auth->setUser(passwordDialog.userNameLineEdit->text());
	    auth->setPassword(passwordDialog.passwordLineEdit->text());
	    auth->setOption("key", key);
	    sapp_t::instance()->setLastAuthenticator(auth);
	}else{
		// Set authenticator null if dialog is cancelled
	    *auth = QAuthenticator();
	}
    }

    void PopupPage::proxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost){
	Q_UNUSED(requestUrl);
//	Browser *_browser = _entrance->activiated_registered().first;

	QDialog dialog(_browser);
	dialog.setWindowFlags(Qt::Sheet);

	Ui::ProxyDialog proxyDialog;
	proxyDialog.setupUi(&dialog);

	proxyDialog.iconLabel->setText(QString());
	proxyDialog.iconLabel->setPixmap(_browser->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, _browser).pixmap(32, 32));

	QString introMessage = tr("<qt>Connect to proxy \"%1\" using:</qt>");
	introMessage = introMessage.arg(proxyHost.toHtmlEscaped());
	proxyDialog.introLabel->setText(introMessage);
	proxyDialog.introLabel->setWordWrap(true);
	if(dialog.exec() == QDialog::Accepted){
	    QString	user	= proxyDialog.userNameLineEdit->text();
	    QByteArray	key	= sapp_t::proxyAuthenticationKey(user, proxyHost, auth->realm());
	    auth->setUser(user);
	    auth->setPassword(proxyDialog.passwordLineEdit->text());
	    auth->setOption("key", key);
	    sapp_t::instance()->setLastProxyAuthenticator(auth);
	}else{
		// Set authenticator null if dialog is cancelled
	    *auth = QAuthenticator();
	}
    }

#endif	// USE_POPUP_WINDOW

#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(WebPage)
#endif

    WebPage::WebPage(Profile                *profile
		    , boost::intrusive_ptr<TreeItem> item
		    , ts_t                  *tree_screen
		    , Editentry		    *editentry
		    , Entrance              *entrance
		    , Browser               *browser
		    , TabWidget             *tabmanager
		    , rctrl_t               *record_controller
		    , WebView               *parent)
	: QWebEnginePage(profile, parent)
	  , _profile(profile)
	  , _tree_screen(tree_screen)
	  , _editentry(editentry)
	  , _entrance(entrance)
	  , _browser(browser)
	  , _tabmanager(tabmanager)
	  , _record_controller(record_controller)
	  , _view(parent){
	// This signal is emitted with the URL of the main frame when the main frame's title is received. The new URL is specified by url.
	onUrlChanged = [&](const QUrl &url){
		// assert(_page->url() != QUrl());
		// assert(url != QUrl());
	    if(_binder->host()){
		if(  url.toString() != ""
		  && url != QUrl() && ! url.host().isEmpty() && ! url.scheme().isEmpty() && url.isValid()
		  && url != QUrl(Browser::_defaulthome)
		  && url != _loadingurl
		  && url.toString() != _binder->host()->field<url_type>()
		    ){
		    assert(_editentry);
//		if(url.toString() != ""){

		    record_info_update(url, title());
////		    record_view_synchronize(_binder->host());
//		}
		}
	    }
	};
	//
	onTitleChanged = [&](const QString &title){
		// assert(this->url() != QUrl());
		assert(this->title() == title);
		if(_binder->host()){
		    if(  url() != QUrl()
		      && ! url().host().isEmpty()
		      && ! url().scheme().isEmpty()
		      && url() != QUrl(Browser::_defaulthome)
		      && url() == _loadingurl
		      && url().toString() == _binder->host()->field<url_type>()
			){
			assert(_editentry);
			if(  title != ""
			  && title != _binder->host()->field<name_type>()	// && !QUrl(title).isValid()
			    ){
			    record_info_update(url(), title);
				//		    record_view_synchronize(_binder->host());
			}
			// }
		    }
		}
	    };
// , _keyboardmodifiers(Qt::NoModifier)
// , _pressedbuttons(Qt::NoButton)


	_binder = [&]() -> boost::intrusive_ptr<::Binder> {
	    if(item->is_lite()) item->to_fat();
	    item->binder_reset();		// item->binder().reset(); //->break_page();

//		// item_bind(item);  // you can't modify _binder before it's initialize time!!!

		// auto ar = std::make_shared<WebPage::Binder>(item, this);
	    new ::Binder(std::make_shared<WebPage::Binder>(item, this));


	    assert(item->binder()->integrity_external(item, this));	// item->binder() = new TreeItem::coupler(std::make_shared<WebPage::coupler>(item, this));
	    assert(_binder->integrity_external(item, this));


	    return std::forward<boost::intrusive_ptr<::Binder> >(item->binder());
	} ();

	settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
	settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
	settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
	settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

	settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
	settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
	settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
	settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
	settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, true);
	settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
	settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, true);
	settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
	settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
	settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);


#if defined(QWEBENGINEPAGE_SETNETWORKACCESSMANAGER)
	setNetworkAccessManager(QtSingleApplication::networkAccessManager());
#endif
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
	connect(static_cast<QWebEnginePage *const>(this), &QWebEnginePage::unsupportedContent, this, &WebPage::handleUnsupportedContent);
#endif
	connect(static_cast<QWebEnginePage *const>(this), &QWebEnginePage::authenticationRequired, this, &WebPage::authenticationRequired);
	connect(static_cast<QWebEnginePage *const>(this), &QWebEnginePage::proxyAuthenticationRequired, this, &WebPage::proxyAuthenticationRequired);

	connect(static_cast<QWebEnginePage *const>(this), &QWebEnginePage::titleChanged, onTitleChanged);
	connect(static_cast<QWebEnginePage *const>(this), &QWebEnginePage::urlChanged, onUrlChanged);
	connect(this, &WebPage::close_requested, this, &WebPage::binder_reset);

	connect(this, &WebPage::linkHovered, [&](const QString &url) mutable {_hovered_url = url;});

//	update_record_view(item);
    }

    WebPage::~WebPage(){
//	if(_tabmanager->find([&](boost::intrusive_ptr<const ::Binder> b){return b == _binder;}))record_view_remove(host());
//	if(_binder)binder_reset();	// break_page_shared_items();   // move to on_close_requested()
    }

	// this will delete record for ever from database if you use real tree_item as source model of record_view
    void WebPage::record_view_remove(boost::intrusive_ptr<TreeItem> host_){
	if(_tabmanager && _tabmanager->count() > 0){
	    RecordModel *source_model = _record_controller->source_model();	// tab_manager->source_model();
	    assert(host_);
		// assert((item->page_valid() && item->unique_page() == this) || !item->page_valid());
	    if(source_model->item([&](const id_value id){return id == id_value(host_->field<id_type>());})) _record_controller->remove(host_->id());																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																									// && record->unique_page() == this
	}
    }

    void WebPage::record_info_update(const QUrl &url, const QString &title){
	// Q_UNUSED(make_current)
	if(_binder->host()){
		// _record->active_immediately(true);


	    bool is_current = (_view == _tabmanager->currentWebView());

	    int index = _tabmanager->webViewIndex(_view);
	    _tabmanager->setTabToolTip(index, title);

	    bool data_changed = false;
	    if(title != ""){
//	      &&
		if(title != _binder->host()->field<name_type>()){		// && ! QUrl(title).isValid()
//		assert(QUrl(title).isValid());	// always true
//		assert(title.contains("://"));	// not always true
		    if(_binder->host()->field<name_type>().size() == 0 || (_binder->host()->field<name_type>().size() > 0 && ! title.contains("://"))) _binder->host()->field<name_type>(title);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																					// "name",

		    data_changed = true;
		}
		// metaeditor->setName(title);
		if(is_current){
		    auto _mainwindow = globalparameters.main_window();
		    if(! _mainwindow->windowTitle().contains(title)) _mainwindow->setWindowTitle(QString(globalparameters.application_name()) + " : " + title);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						// table->setWorkPos(pos);
		    _editentry->name(title);
		}
	    }
	    auto url_str = url.toString();
	    if(url_str != ""){
		auto _host = _binder->host();
		_host->field<url_type>(url_str);		// "url",
		if(_host->field<home_type>() == Browser::_defaulthome && url_str != Browser::_defaulthome) _host->field<home_type>(url_str);
		data_changed = true;
		if(is_current) _editentry->url(url_str);
	    }
	    if(data_changed){		// && is_current, without is_current will lead current view lost editing focus
		RecordIndex::synchronize(_binder->host());
		auto source_model = [&](){return _tree_screen->view()->source_model();};
		source_model()->emit_datachanged_signal(source_model()->index(_binder->host()));
		_record_controller->on_recordtable_configchange();
	    }
	    if(_editentry->item() != _binder->host() && is_current) metaeditor_sychronize();																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										// metaeditor->bind(_record);

		// auto _tree_screen = globalparameters.tree_screen();
	    auto	it		= _binder->host();
	    auto	tree_view	= _tree_screen->view();
	    if(is_current){		// globalparameters.mainwindow()
		if(tree_view->current_item() != it) tree_view->select_as_current(TreeIndex::create_treeindex_from_item([&] {return tree_view->source_model();}, it));
		if(_record_controller->view()->current_item() != it) _record_controller->select_as_current(_record_controller->index<pos_proxy>(it));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										// IdType(_binder->item()->field("id"))
	    }
	    if(is_current) _view->setFocus();																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																		// make upate validate
	}
	// WebView *view = record->page()->view();

	// if(view->hasFocus()) {
	// view->tabmanager()->setCurrentWidget(view);
	// view->show();
	// }

	// }
    }

    void WebPage::metaeditor_sychronize(){
	boost::intrusive_ptr<TreeItem> current_item = _binder->host();
	// boost::intrusive_ptr<Record> record = this->table_model()->table_data()->record(pos);
	assert(current_item);
	// assert(record == view()->tabmanager()->currentWebView()->page()->current_record()); // may be in backend?

	// Внимание! Наверно, всю эту логику следует перенести в MetaEditor. А здесь только получить данные из таблицы

	// Выясняется указатель на объект редактирования текста записи
	// MetaEditor *_editor_screen = globalparameters.meta_editor();    // find_object<MetaEditor>(meta_editor_singleton_name);
	assert(_editentry);
	// boost::intrusive_ptr<RecordModel::ModelIndex> record_index;
	// try {
	// record_index = new RecordModel::ModelIndex([&] {return _record_controller->source_model();}, _record_controller->source_model()->sibling(current_item), current_item);
	// } catch(std::exception &) {}
	// if(record_index) {
	if(_editentry->item() != current_item) _editentry->bind(current_item);
	// int pos = indexOf(currentWebView());
	// Turns the reference to the table of final data   // Выясняется ссылка на таблицу конечных данных
	// std::shared_ptr<TableData> table = std::make_shared<TableData>();   // _source_model->table_data_internal();

	// for(int i = 0; i < count(); i++) {
	// table->insert_new_record(table->work_pos(), static_cast<WebView *>(widget(i))->page()->current_record());
	// }

	// В таблице конечных данных запоминается какая запись была выбрана
	// чтобы затем при выборе этой же подветки засветка автоматически
	// установилась на последнюю рабочую запись
	// table->work_pos(pos);


	// Устанавливается функция обратного вызова для записи данных
	_editentry->save_callback(_editentry->editor_save_callback);

	// Сохраняется текст и картинки в окне редактирования
	// find_object<MainWindow>("mainwindow")
	globalparameters.main_window()->save_text_area();
	// Для новой выбраной записи выясняется директория и основной файл
	if(current_item->field<id_type>().length() == 0) current_item->field<id_type>(current_item->field<dir_type>().length() > 0 ? current_item->field<dir_type>() : get_unical_id());// "id",	// || current_item->field("url") == Browser::_defaulthome
	if(current_item->field<url_type>() == "") current_item->field<dir_type>(current_item->id());																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																															// "dir",
	if(current_item->field<file_type>() == "") current_item->field<file_type>("text.html");																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							// "file",
	QString current_dir	= current_item->field<dir_type>();	// table->field(pos, "dir");
	QString current_file	= current_item->field<file_type>();		// table->field(pos, "file");
	QString full_dir	= globalparameters.root_path() + "/" + QDir(appconfig.data_dir()).dirName() + "/base/" + current_dir;
	QString full_file_name	= full_dir + "/" + current_file;
	qDebug() << " File " << full_file_name << "\n";
	// If the window contents of the record is already selected record  // Если в окне содержимого записи уже находится выбираемая запись
	if(  _editentry->work_directory() == full_dir
	  && _editentry->file_name() == current_file
	    ){
	    globalparameters.window_switcher()->recordtable_ro_record_editor();

	    return;
	}
	// Перед открытием редактора происходит попытка получения текста записи
	// Этот вызов создаст файл с текстом записи, если он еще не создан (подумать, переделать)
	// Before the opening of the editor it attempts to get the text records
	// This call will create a text file with the record if it is not already created (think remake)
	_editentry->save_textarea();	// table->text(pos);    // ?

	// едактору задаются имя файла и директории
	// И дается команда загрузки файла
	_editentry->work_directory(full_dir);
	_editentry->file_name(current_file);

	// Если идет работа с зашифрованной записью
	// И если имя директории или имя файла пусты, то это означает что
	// запись не была расшифрована, и редактор должен просто показывать пустой текст
	// ничего не сохранять и не считывать
	qDebug() << "RecordTableView::onClickToRecord() : id " << current_item->field<id_type>();	// table->field(pos, "id");
	qDebug() << "RecordTableView::onClickToRecord() : name " << current_item->field<name_type>();	// table->field(pos, "name");
	qDebug() << "RecordTableView::onClickToRecord() : crypt " << current_item->field<crypt_type>();	// table->field(pos, boost::mpl::c_str < crypt_type > ::value);
	if(current_item->field<crypt_type>()	// table->field(pos, boost::mpl::c_str < crypt_type > ::value)
	    == "1")
		if(full_dir.length() == 0 || current_file.length() == 0) _editentry->dir_file_empty_reaction(MetaEditor::DIRFILEEMPTY_REACTION_SUPPRESS_ERROR);
	// В редактор заносится информация, идет ли работа с зашифрованным текстом
	_editentry->misc_field(boost::mpl::c_str < crypt_type >::value, current_item->field<crypt_type>());				// table->field(pos, boost::mpl::c_str < crypt_type > ::value)


	// В редакторе устанавливается функция обратного вызова для чтения данных
	_editentry->load_callback(&Editor::editor_load_callback);

	_editentry->load_textarea();
	// edView->set_textarea(table->get_text(index.row()));

	// Заполняются прочие инфо-поля
	_editentry->pin(current_item->field<pin_type>());		// table->field(pos, "pin")

	_editentry->name(current_item->field<name_type>());		// table->field(pos, "name")

	_editentry->author(current_item->field<author_type>());	// table->field(pos, "author")

	_editentry->home(current_item->field<home_type>());		// table->field(pos, "home")

	_editentry->url(current_item->field<url_type>());		// table->field(pos, "url")

	_editentry->tags(current_item->field<tags_type>());		// table->field(pos, "tags")


	auto id = id_value(current_item->field<id_type>());	// table->field(pos, "id");
	_editentry->misc_field("id", id);

	_editentry->misc_field("title", current_item->field<name_type>());	// table->field(pos, "name")
	// should each record carry it's tree path?
	//// Set the path to the branch in which lies the record (in the form of the names of the branches)   // Устанавливается путь до ветки в которой лежит запись (в виде названий веток)
	// QString path = qobject_cast<TableScreen *>(parent())->getTreePath();
	//// В мобильном интерфейсе редактор должен показывать путь до записи
	// if(appconfig.getInterfaceMode() == "mobile")
	// meta_editor->setTreePath(path);
	// В редакторе восстанавливается позиция курсора и прокрутки если это необходимо
	if(appconfig.remember_cursor_at_ordinary_selection()){
	    _editentry->cursor_position(walkhistory.cursor_position(id));
	    _editentry->scrollbar_position(walkhistory.scrollbar_position(id));
	}
	// Обновление иконки аттачей
	if(current_item->attach_table()->size() == 0)																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																	// table->record(pos)->getAttachTablePointer()->size()
		_editentry->to_attach()->setIcon(_editentry->icon_attach_not_exists());
	// Если нет приаттаченных файлов
	else _editentry->to_attach()->setIcon(_editentry->icon_attach_exists());																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										// Есть приаттаченные файлы

	// }
    }

	// deprecated
    void WebPage::binder_reset(){
//	{	// if(sychronize_to_record_view)// _tabmanager
////	_record_controller->on_recordtable_configchange();
//	}
//	// _record_controller->delete_items_selected();   // source_model()->on_table_config_changed();
	if(_binder){
//	    auto _host_binder = _binder->host() ? _binder->host()->binder() : nullptr;
////	    assert(_host_binder == _binder);
//	    if(_host_binder && _host_binder == _binder){
////		if(_host_binder->page())
//		_host_binder->page(nullptr);
////		if(_host_binder->host())
//		_host_binder->host(nullptr);
////		// _binder->break_page();   // item_break(_binder->item_link());  // break_items();

////		_host_binder.reset(nullptr);
//	    }else{
	    _binder->page(nullptr);

	    _binder->host(nullptr);
//	    }
////            auto _page_binder = _binder->page() ? _binder->page()->binder() : nullptr;
////            if(_page_binder){
////                assert(_page_binder == _binder);
////                if(_page_binder->page())_page_binder->page(nullptr);
////                if(_page_binder->host()){_page_binder->host(boost::intrusive_ptr<TreeItem>(nullptr));}
//////		_binder->page()->binder(nullptr);
//////		_binder->page(nullptr);
////                _page_binder = nullptr;
////            }
//	    _binder.reset(nullptr);
	}
	_record_controller->on_recordtable_configchange();
    }

    Browser *WebPage::browser(){return _browser;}// return _entrance->activiated_registered().first;    //QtSingleApplication::instance()->mainWindow();

    WebView *WebPage::load(boost::intrusive_ptr<TreeItem> item, bool checked){
	// Q_UNUSED(checked)
	assert(item);


	// auto tree_view = _tree_screen->tree_view();

	// boost::intrusive_ptr<TreeIndex> tree_modelindex(nullptr);

	// try {
	// tree_modelindex = new TreeIndex([&] {return tree_view->source_model();}, item->parent(), item->parent()->sibling_order([&](boost::intrusive_ptr<const Linker> il) {return il->host() == item && il == item->linker() && item->parent() == il->host_parent();}));
	// } catch(std::exception &e) {}

	// if(record_index) {

	boost::intrusive_ptr<TreeItem> it;
	if(item->binder()){	// && item->binder() == _binder
	    if(! item->binder()->integrity_external(item, this)){
		// auto it = tree_view->item_register(item, std::bind(&KnowView::view_paste_child, tree_view, tree_modelindex, std::placeholders::_2, std::placeholders::_3));
		it = bind(item);
		// item
		// , std::bind(&TreeScreen::view_paste_as_child, _tree_screen, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
		// );
	    }else it = item;
	}else it = bind(item);
	if(checked) it->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), std::placeholders::_1));
	// }

	return _view;
    }

    WebView *WebPage::activate(){
	QString _url_str	= _binder->host()->field<url_type>();
	QUrl	_url		= QUrl(_url_str);
	if(  _view	// && _loadingurl.isValid()     // && url().isValid()    // && url().toString() != _url_str
//	  && _url_str != Browser::_defaulthome		// url() may be nothing
	  && _url != _loadingurl			// lead loading stop
	    ){
		// triggerAction(QWebEnginePage::Stop);
	    QWebEnginePage::setUrl(_url);
//	    QWebEnginePage::load(_url);
	}
	if(_binder->host()){	// ->_active_request
		// if(_record_binder->bounded_item()->_open_link_in_new_window == 1) { }
	    assert(_browser);
	    if(! _browser->isActiveWindow() || ! _browser->isVisible()){
		if(_view){
		    if(! _view->isTopLevel()) _view->raise();
		    if(! _view->isActiveWindow()) _view->activateWindow();
		}
		if(! _browser->isTopLevel()) _browser->raise();
		if(! _browser->isActiveWindow()) _browser->activateWindow();
	    }
	    auto	_vtab_record	= globalparameters.vtab_record();
	    auto	_record_screen	= _browser->record_screen();
	    if(_vtab_record->currentWidget() != _record_screen) _vtab_record->setCurrentWidget(_record_screen);
	    auto	it		= _binder->host();
	    auto	tree_view	= _tree_screen->view();
	    if(it != tree_view->current_item()) tree_view->select_as_current(TreeIndex::create_treeindex_from_item([&] {return tree_view->source_model();}, it));
//	    if(_url_str != Browser::_defaulthome){	// && _loadingurl.isValid()   // && _loadingurl == _url
	    if(_view){
		if(_record_controller->view()->current_item() != _binder->host() || _view->tabmanager()->currentWebView() != _view){
		    _tabmanager->setCurrentWidget(_view);
		    _view->show();
			// if(checked) // globalparameters.mainwindow()
		    _view->setFocus();		// make upate validate
		    _binder->host()->add_rating();
			// assert(_lineedits);

			// if(_lineedits) {
		    QLineEdit *line_edit = _tabmanager->currentLineEdit();		// qobject_cast<QLineEdit *>(_lineedits->currentWidget());
		    if(line_edit) line_edit->setText(_url_str);
			// }
		    if(_record_controller->view()->current_item() != _binder->host()) _record_controller->select_as_current(_record_controller->index<pos_proxy>(_binder->host()));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																															// if(_record_controller->view()->selection_first<IdType>() != _binder->host()->field<id_type>()){
			// IdType(_binder->item()->field("id"))
		}
//	    }
	    }
	}
	if(_view){
	    _browser->adjustSize();
	    if(_view == _tabmanager->currentWebView()){
		_tabmanager->adjustSize();
		_view->adjustSize();
	    }
	    _view->current_view_global_consistency();
	}
	return _view;
    }

    boost::intrusive_ptr<TreeItem> WebPage::bind(boost::intrusive_ptr<TreeItem> host_){	// , browser::WebPage *page
	boost::intrusive_ptr<TreeItem> result(nullptr);

	// auto binder = [](boost::shared_ptr<WebPage::RecordBinder> ar) {
	// return std::make_shared<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, boost::intrusive_ptr<TreeItem>   // , boost::intrusive_ptr<TreeItem>(TreeItem::*)(WebPage *)
	// >>("", &WebPage::RecordBinder::binder, ar);
	// };
	// auto activator = [](boost::shared_ptr<WebPage::RecordBinder> ar) {
	// return std::make_shared<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *>>("", &WebPage::RecordBinder::activator, ar);
	// };

	// registered record, but have no generator:
	// auto ar =
	// auto _tree_screen = globalparameters.tree_screen();

	// if(!item->is_registered_to_browser() && !item->record_binder()) {
	auto tree_view = _tree_screen->view();
	assert(static_cast<QModelIndex>(tree_view->source_model()->index(host_)).isValid());

	// if(!tree_view->source_model()->index(item).isValid())
	// result = tree_view->item_register(item, std::bind(&KnowView::view_paste_child, tree_view, tree_index, std::placeholders::_2, std::placeholders::_3));
	// else
	result = host_;

	auto create_coupler = [&](boost::intrusive_ptr<TreeItem> result_item){
		// auto ar = std::make_shared<WebPage::Binder>(result_item, this);
		auto result_coupler = new ::Binder(std::make_shared<WebPage::Binder>(result_item, this));
		// std::move(::Binder::item_interface("", &WebPage::Binder::item, ar))          // std::make_shared<TreeItem::coupler::item_interface>("", &WebPage::coupler::item_link, ar)
		// , std::move(::Binder::page_interface("", &WebPage::Binder::page, ar))        // std::make_shared<TreeItem::coupler::page_interface>("", &WebPage::coupler::page_link, ar)
		// , std::move(::Binder::bind_interface("", &WebPage::Binder::bind, ar))             // std::make_shared<TreeItem::coupler::bind_interface>("", &WebPage::coupler::bind, ar)
		// , std::move(::Binder::activate_interface("", &WebPage::Binder::activator, ar))    // std::make_shared<TreeItem::coupler::activate_interface>("", &WebPage::coupler::activator, ar)
		// );


		assert(_binder->integrity_external(result_item, this));	// _binder.reset(new TreeItem::coupler(std::make_shared<WebPage::coupler>(result, this)));
		assert(result_item->binder()->integrity_external(result_item, this));	// result->binder(std::forward<boost::intrusive_ptr<TreeItem::coupler>&>(_binder));
		return result_coupler;
	    };
	if(result->binder() && result->binder() == _binder){
	    if(! result->binder()->integrity_external(result, this)){
		if(_binder) binder_reset();
		create_coupler(result);
	    }
	}else{
	    if(_binder) binder_reset();
	    create_coupler(result);
	}
	// std::make_shared<WebPage::Coupler>(this, item);


	// } else {
	// if(_record_binder != item->record_binder())_record_binder = item->record_binder();
	// }



	////        it->binder(
	////            binder(ar)
	////        );

	////        it->activator(
	////            activator(ar)
	////        );

	// it->record_binder(std::make_shared<CouplerDelegation>(
	// std::make_shared<bounded_item_interface>("", &Coupler::bounded_item, ar)
	// , std::make_shared<bounded_page_interface>("", &Coupler::bounded_page, ar)
	// , std::make_shared<bind_interface>("", &Coupler::binder, ar)
	// , std::make_shared<activate_interface> ("", &Coupler::activator, ar)
	// ));

	assert(result->binder());
	RecordIndex::synchronize(result);
	return result;
    }

    bool WebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame){
	Q_UNUSED(type);
	if(isMainFrame){
	    if(_loadingurl != url){
		_loadingurl = url;
		emit loadingUrl(_loadingurl);
	    }
	}
	return true;
    }

    bool WebPage::certificateError(const QWebEngineCertificateError &error){
	if(error.isOverridable() && ! _certificate_ignored){
	    QMessageBox msgBox;
	    msgBox.setIcon(QMessageBox::Warning);
	    msgBox.setText(error.errorDescription());
	    msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
					 "Accepting an unverified certificate means "
					 "you may not be connected with the host you tried to connect to.\n"
					 "Do you wish to override the security check and continue?"));
	    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	    msgBox.setDefaultButton(QMessageBox::No);

	    return _certificate_ignored = (msgBox.exec() == QMessageBox::Yes);
	}
	// QMessageBox::critical(_view, tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);

	// return false;
	return _certificate_ignored;
    }



	// #include "webview.moc"

    QWebEnginePage *WebPage::createWindow(QWebEnginePage::WebWindowType type){
//	QString url_	= "";
//	QString script	= "location.href";	// "document.title";  //"JavaScript:function(){return this.href;}();";
//	runJavaScript(script, [&](const QVariant &var){url_ = var.toString();assert(url_ != "");});
//	assert(url_ != "");
	QUrl target_url = QUrl(_hovered_url);
	if(! target_url.isValid()) target_url = Browser::_defaulthome;
	// assert(url != "");

	// QWebChannel *channel = new QWebChannel(this);
	// QVariant _content;
	// channel->registerObject(QStringLiteral("content"), &_content);
	// this->setWebChannel(channel);

	// QWebEnginePage
	WebPage *page = nullptr;

	auto tree_view = _tree_screen->view();

	assert(static_cast<QModelIndex>(tree_view->source_model()->index(this->host())).isValid());
	auto parent = this->host() ? this->host()->parent() : nullptr;
	assert(parent);
//	auto parent_parent = parent->parent();

	boost::intrusive_ptr<TreeIndex> this_treeindex	= TreeIndex::create_treeindex_from_item([&] {return tree_view->source_model();}, this->host());		// parent_parent ? TreeIndex::instance([&] {return tree_view->source_model();}, parent) : TreeIndex::instance([&] {return tree_view->source_model();}, this->host());
	auto				find_binder	= [&](boost::intrusive_ptr<const ::Binder> b){return url_equal((b->host()->field<url_type>()).toStdString(), target_url.toString().toStdString()) || url_equal((b->host()->field<home_type>()).toStdString(), target_url.toString().toStdString());};
	auto				find_target	= [&](){
		return globalparameters.main_window()->vtab_record()->find(find_binder);
	    };
	if(type == QWebEnginePage::WebBrowserWindow){
	    WebView *v = find_target();
	    if(v) v->tabmanager()->closeTab(v->tabmanager()->indexOf(v));
	    Browser *_browser = globalparameters.main_window()->vtab_record()->new_browser();			// QtSingleApplication::instance()->newMainWindow();

	    auto it =	// this_index->
		TreeIndex::create_treeitem_from_url(target_url
						   , std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
						   , [&](boost::intrusive_ptr<const TreeItem> it_) -> bool {return url_equal((it_->field<home_type>()).toStdString(), target_url.toString().toStdString()) || url_equal((it_->field<url_type>()).toStdString(), target_url.toString().toStdString());});	// return it_->field<url_type>() == target_url.toString();
		// Browser::_defaulthome


	    page = _browser->bind(RecordIndex::instance([&] {return _browser->record_screen()->record_controller()->source_model();}, it))->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), std::placeholders::_1))->page();


	    assert(page);
	}else if(type == WebBrowserBackgroundTab){
		//// should I think about always open new window in new tab
		//// _openinnewtab = false;  // true
		// QUrl current = url();
		// QUrl requestedurl = requestedUrl(); //equal to current page url

	    WebView *view = find_target();
		// return b->host()->field<url_type>() == target_url.toString();
//	    auto tree_view = _tree_screen->view();
	    if(view){
		page = view->page();
//		auto _item = page->host();
//		if(_item){
//		    auto _index = tree_view->source_model()->index(_item);
//		    if(static_cast<QModelIndex>(_index).isValid())_item->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), std::placeholders::_1));	// tree_view->index_invoke(TreeIndex::instance([&] {return tree_view->source_model();}, _item, _item->parent()));	// view,
//		}
		assert(page->binder() && page->binder()->integrity_external(page->host(), page));
		assert(static_cast<QModelIndex>(tree_view->source_model()->index(page->host())).isValid());
		assert(page);
	    }else{
		// WebPage *page = this->dockedwindow()->tabWidget()->new_view(new_record, true)->page();
		// already create window, why do this? -- refer to demo browser
		assert(static_cast<QModelIndex>(tree_view->source_model()->index(this->_binder->host())).isValid());

		auto it = this_treeindex->bind(target_url
					      , std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)							// std::placeholders::_1
					      , [&](boost::intrusive_ptr<const TreeItem> it_) -> bool {return url_equal(it_->field<home_type>().toStdString(), target_url.toString().toStdString()) || url_equal(it_->field<url_type>().toStdString(), target_url.toString().toStdString());}
			);
//		page = it ? it->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), std::placeholders::_1))->page() : nullptr;
		page = it ? it->page() : nullptr;
		assert(page || _hovered_url == Browser::_defaulthome || _hovered_url == "");
	    }
	    assert(page || _hovered_url == Browser::_defaulthome || _hovered_url == "");
	}else
#ifdef USE_POPUP_WINDOW
	if(type == WebBrowserTab)
#endif
	{
		//// should I think about always open new window in new tab
		//// _openinnewtab = false;  // true
		// QUrl current = url();
		// QUrl requestedurl = requestedUrl(); //equal to current page url

	    WebView *view = find_target();
		// return b->host()->field<url_type>() == target_url.toString();
		//	    auto tree_view = _tree_screen->view();
	    if(view){
		page = view->page();
		auto _item = page->host();
		if(_item){
		    auto _index = tree_view->source_model()->index(_item);
		    if(static_cast<QModelIndex>(_index).isValid()) _item->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), std::placeholders::_1));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													// tree_view->index_invoke(TreeIndex::instance([&] {return tree_view->source_model();}, _item, _item->parent()));	// view,
		}
		assert(page->binder() && page->binder()->integrity_external(page->host(), page));
		assert(static_cast<QModelIndex>(tree_view->source_model()->index(page->host())).isValid());
		assert(page);
	    }else{
		// WebPage *page = this->dockedwindow()->tabWidget()->new_view(new_record, true)->page();
		// already create window, why do this? -- refer to demo browser
		assert(static_cast<QModelIndex>(tree_view->source_model()->index(this->_binder->host())).isValid());

		auto it = this_treeindex->bind(target_url
					      , std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)							// std::placeholders::_1
					      , [&](boost::intrusive_ptr<const TreeItem> it_) -> bool {return url_equal(it_->field<home_type>().toStdString(), target_url.toString().toStdString()) || url_equal(it_->field<url_type>().toStdString(), target_url.toString().toStdString());}
			);
		page = it ? it->activate(std::bind(&HidableTabWidget::find, globalparameters.main_window()->vtab_record(), find_binder)	// std::placeholders::_1
			)->page() : nullptr;
		assert(page || _hovered_url == Browser::_defaulthome || _hovered_url == "");
	    }
	    assert(page || _hovered_url == Browser::_defaulthome || _hovered_url == "");
	}
#ifdef USE_POPUP_WINDOW
	else{	// type == WebDialog
	    PopupWindow *popup = new PopupWindow(_browser
			// view()->tabmanager()
						, _profile
			// , QUrl(Browser::_defaulthome)
			// , _record_controller
			// , _page_controller
			// , view()->tabmanager()->browser()
		    );

	    popup->setAttribute(Qt::WA_DeleteOnClose);
	    popup->show();
		// return

		// page =
	    return popup->page();
	}
#endif		// USE_POPUP_WINDOW
	// }
	// if(page) {
	//// not realy needed for each time
	// connect(static_cast<QWebEnginePage *const>(page), &QWebEnginePage::setUrl, [&](const QUrl & url) {
	// boost::intrusive_ptr<TreeItem> _current_item = page->_binder->item();
	// if(_current_item->field("url") != url.toString()) {
	// _current_item->field("url", url.toString());
	// page->load(_current_item); // record->generate();
	// page->activate();     // record->active();
	// } else
	// page->activate();
	// });
	// connect(static_cast<QWebEnginePage *const>(page), &QWebEnginePage::load, [&](const QUrl & url) {
	// boost::intrusive_ptr<TreeItem> _current_item = page->_binder->item();
	// if(_current_item->field("url") != url.toString()) {
	// _current_item->field("url", url.toString());
	// page->load(_current_item); // record->generate();
	// page->activate();     // record->active();
	// } else
	// page->activate();
	// });
	// }
	assert(page || _hovered_url == Browser::_defaulthome || _hovered_url == "" || type == WebDialog);

	return page;
    }

#if ! defined(QT_NO_UITOOLS)
    QObject *WebPage::createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues){
	Q_UNUSED(url);
	Q_UNUSED(paramNames);
	Q_UNUSED(paramValues);
	QUiLoader loader;

	return loader.createWidget(classId, _view);
    }

#endif	// !defined(QT_NO_UITOOLS)

#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
    void WebPage::handleUnsupportedContent(QNetworkReply *reply){
	QString errorString = reply->errorString();
	if(_loadingurl != reply->url()){
		// sub resource of this page
	    qWarning() << "Resource" << reply->url().toEncoded() << "has unknown Content-Type, will be ignored.";
	    reply->deleteLater();

	    return;
	}
	if(reply->error() == QNetworkReply::NoError && ! reply->header(QNetworkRequest::ContentTypeHeader).isValid()) errorString = "Unknown Content-Type";
	QFile	file(QLatin1String(":/notfound.html"));
	bool	isOpened = file.open(QIODevice::ReadOnly);
	Q_ASSERT(isOpened);
	Q_UNUSED(isOpened)

	QString title = tr("Error loading page: %1").arg(reply->url().toString());
	QString html = QString(QLatin1String(file.readAll()))
	    .arg(title)
	    .arg(errorString)
	    .arg(reply->url().toString());

	QBuffer imageBuffer;
	imageBuffer.open(QBuffer::ReadWrite);
	QIcon	icon	= view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
	QPixmap pixmap	= icon.pixmap(QSize(32, 32));
	if(pixmap.save(&imageBuffer, "PNG")){
	    html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE")
			, QString(QLatin1String(imageBuffer.buffer().toBase64())));
	}
	QList<QWebEngineFrame *> frames;
	frames.append(mainFrame());
	while(! frames.isEmpty()){
	    QWebEngineFrame *frame = frames.takeFirst();
	    if(frame->url() == reply->url()){
		frame->setHtml(html, reply->url());

		return;
	    }
	    QList<QWebEngineFrame *> children = frame->childFrames();

	    foreach(QWebEngineFrame * frame, children)
	    frames.append(frame);
	}
	if(_loadingurl == reply->url()) mainFrame()->setHtml(html, reply->url());
    }

#endif

    void WebPage::authenticationRequired(const QUrl &requestUrl, QAuthenticator *auth){
	// if(_entrance->browsers().count() == 0) {
	// _entrance->new_browser(QUrl(browser::Browser::_defaulthome));
	// }

	Browser *browser = globalparameters.main_window()->vtab_record()->activated_browser();	// QtSingleApplication::instance()->mainWindow();

	QDialog dialog(browser);
	dialog.setWindowFlags(Qt::Sheet);

	Ui::PasswordDialog passwordDialog;
	passwordDialog.setupUi(&dialog);

	passwordDialog.iconLabel->setText(QString());
	passwordDialog.iconLabel->setPixmap(browser->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, browser).pixmap(32, 32));

	QString introMessage = tr("<qt>Enter username and password for \"%1\" at %2</qt>");
	introMessage = introMessage.arg(auth->realm()).arg(requestUrl.toString().toHtmlEscaped());
	passwordDialog.introLabel->setText(introMessage);
	passwordDialog.introLabel->setWordWrap(true);
	if(dialog.exec() == QDialog::Accepted){
	    QByteArray key = sapp_t::authenticationKey(requestUrl, auth->realm());
	    auth->setUser(passwordDialog.userNameLineEdit->text());
	    auth->setPassword(passwordDialog.passwordLineEdit->text());
	    auth->setOption("key", key);
	    sapp_t::instance()->setLastAuthenticator(auth);
	}else{
		// Set authenticator null if dialog is cancelled
	    *auth = QAuthenticator();
	}
    }

    void WebPage::proxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost){
	Q_UNUSED(requestUrl);

	// if(_entrance->browsers().count() == 0) {
	// _entrance->new_browser(QUrl(browser::Browser::_defaulthome));
	// }

	Browser *browser = globalparameters.main_window()->vtab_record()->activated_browser();	// QtSingleApplication::instance()->mainWindow();

	QDialog dialog(browser);
	dialog.setWindowFlags(Qt::Sheet);

	Ui::ProxyDialog proxyDialog;
	proxyDialog.setupUi(&dialog);

	proxyDialog.iconLabel->setText(QString());
	proxyDialog.iconLabel->setPixmap(browser->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, browser).pixmap(32, 32));

	QString introMessage = tr("<qt>Connect to proxy \"%1\" using:</qt>");
	introMessage = introMessage.arg(proxyHost.toHtmlEscaped());
	proxyDialog.introLabel->setText(introMessage);
	proxyDialog.introLabel->setWordWrap(true);
	if(dialog.exec() == QDialog::Accepted){
	    QString	user	= proxyDialog.userNameLineEdit->text();
	    QByteArray	key	= sapp_t::proxyAuthenticationKey(user, proxyHost, auth->realm());
	    auth->setUser(user);
	    auth->setPassword(proxyDialog.passwordLineEdit->text());
	    auth->setOption("key", key);
	    sapp_t::instance()->setLastProxyAuthenticator(auth);
	}else{
		// Set authenticator null if dialog is cancelled
	    *auth = QAuthenticator();
	}
    }





#ifdef USE_POPUP_WINDOW

#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(PopupView)
#endif

    PopupView::PopupView(QWidget *parent)
	: QWebEngineView(parent)
	  , _progress(0)
	  , _page(0)
	  , _icon_reply(0){
	connect(this, &PopupView::loadProgress, this, &PopupView::setProgress);
	connect(this, &QWebEngineView::loadFinished, this, &PopupView::loadFinished);
	connect(this, &QWebEngineView::renderProcessTerminated
	       , [=](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode){
		const char *status = "";
		switch(termStatus){
		    case QWebEnginePage::NormalTerminationStatus
			: status = "(normal exit)";
			break;

		    case QWebEnginePage::AbnormalTerminationStatus:
			status = "(abnormal exit)";
			break;

		    case QWebEnginePage::CrashedTerminationStatus:
			status = "(crashed)";
			break;

		    case QWebEnginePage::KilledTerminationStatus:
			status = "(killed)";
			break;
		}
		qInfo() << "Render process exited with code" << statusCode << status;
		QTimer::singleShot(0, [this] {reload();});
	    });
    }

    void PopupView::setPage(PopupPage *page_){
	if(_page) _page->deleteLater();
	_page = page_;
	QWebEngineView::setPage(page_);
#if defined(QWEBENGINEPAGE_STATUSBARMESSAGE)
	connect(page(), &PopupPage::statusBarMessage, &PopupView::setStatusBarText);
#endif
	connect(_page, &PopupPage::loadingUrl, this, &PopupView::urlChanged);
	connect(page(), &PopupPage::iconUrlChanged, this, &PopupView::onIconUrlChanged);
	connect(page(), &PopupPage::featurePermissionRequested, this, &PopupView::onFeaturePermissionRequested);
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
	page()->setForwardUnsupportedContent(true);
#endif
    }

    void PopupView::contextMenuEvent(QContextMenuEvent *event){
	QMenu										*menu	= page()->createStandardContextMenu();
	const QList<QAction *>								actions = menu->actions();
	QList<QAction *>::const_iterator						it	= qFind(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::OpenLinkInThisWindow));
	if(it != actions.cend()){
	    (*it)->setText(tr("Open Link in This Window"));
	    ++ it;
	    menu->insertAction(*it, page()->action(QWebEnginePage::OpenLinkInNewWindow));
	    menu->insertAction(*it, page()->action(QWebEnginePage::OpenLinkInNewTab));
	}
	menu->popup(event->globalPos());
    }

    void PopupView::wheelEvent(QWheelEvent *event){
#if defined(QWEBENGINEPAGE_SETTEXTSIZEMULTIPLIER)
	if(QApplication::keyboardModifiers() & Qt::ControlModifier){
	    int numDegrees	= event->delta() / 8;
	    int numSteps	= numDegrees / 15;
	    setTextSizeMultiplier(textSizeMultiplier() + numSteps * 0.1);
	    event->accept();

	    return;
	}
#endif
	QWebEngineView::wheelEvent(event);
    }

    void PopupView::openLinkInNewTab(){
	pageAction(QWebEnginePage::OpenLinkInNewTab)->trigger();
    }

    void PopupView::onFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature){
	FeaturePermissionBar *permissionBar = new FeaturePermissionBar(this);
	connect(permissionBar, &FeaturePermissionBar::featurePermissionProvided, page(), &QWebEnginePage::setFeaturePermission);

	// Discard the bar on new loads (if we navigate away or reload).
	connect(page(), &QWebEnginePage::loadStarted, permissionBar, &QObject::deleteLater);

	permissionBar->requestPermission(securityOrigin, feature);
    }

    void PopupView::setProgress(int progress){
	_progress = progress;
    }

    void PopupView::loadFinished(bool success){
	if(success && 100 != _progress){
	    qWarning()	<< "Received finished signal while progress is still:" << progress()
			<< "Url:" << url();
	}
	_progress = 0;
    }

    void PopupView::loadUrl(const QUrl &url){
	_initial_url = url;
	load(url);
    }

    QString PopupView::lastStatusBarText() const {
	return _statusbar_text;
    }

    QUrl PopupView::url() const {
	QUrl url = QWebEngineView::url();
	if(! url.isEmpty()) return url;
	return _initial_url;
    }

    QIcon PopupView::icon() const {
	if(! _icon.isNull()) return _icon;
	return sapp_t::instance()->defaultIcon();
    }

    void PopupView::onIconUrlChanged(const QUrl &url){
	QNetworkRequest iconRequest(url);
	_icon_reply = sapp_t::networkAccessManager()->get(iconRequest);
	_icon_reply->setParent(this);
	connect(_icon_reply, &QNetworkReply::finished, this, &PopupView::iconLoaded);
    }

    void PopupView::iconLoaded(){
	_icon = QIcon();
	if(_icon_reply){
	    QByteArray	data = _icon_reply->readAll();
	    QPixmap	pixmap;
	    pixmap.loadFromData(data);
	    _icon.addPixmap(pixmap);
	    _icon_reply->deleteLater();
	    _icon_reply = 0;
	}
	emit iconChanged();
    }

    void PopupView::mousePressEvent(QMouseEvent *event){
	_page->_pressed_buttons		= event->buttons();
	_page->_keyboard_modifiers	= event->modifiers();
	QWebEngineView::mousePressEvent(event);
    }

    void PopupView::mouseReleaseEvent(QMouseEvent *event){
	QWebEngineView::mouseReleaseEvent(event);
	if(! event->isAccepted() && (_page->_pressed_buttons & Qt::MidButton)){
	    QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
	    if(! url.isEmpty() && url.isValid() && ! url.scheme().isEmpty()) setUrl(url);
	}
    }

    void PopupView::setStatusBarText(const QString &string){
	_statusbar_text = string;
    }

#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(PopupWindow)
#endif

    PopupWindow::PopupWindow(Browser *browser_, Profile *profile_)
	: QWidget(browser_)
	  , _browser(browser_)
	  , _addressbar(new QLineEdit(this))
	  , _view(new PopupView(this)){
	setWindowFlags(Qt::Dialog);	// Qt::Window |
	_view->setPage(new PopupPage(_view, browser_, profile_));
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	setLayout(layout);
	layout->addWidget(_addressbar);
	layout->addWidget(_view);
	_view->setFocus();

	connect(_view, &PopupView::titleChanged, this, &QWidget::setWindowTitle);
	connect(_view, &PopupView::urlChanged, this, &PopupWindow::setUrl);
	connect(page(), &PopupPage::geometryChangeRequested, this, &PopupWindow::adjustGeometry);
	connect(page(), &PopupPage::windowCloseRequested, this, &QWidget::close);
    }

    void PopupWindow::adjustGeometry(const QRect &newGeometry){
	const int	x1	= frameGeometry().left() - geometry().left();
	const int	y1	= frameGeometry().top() - geometry().top();
	const int	x2	= frameGeometry().right() - geometry().right();
	const int	y2	= frameGeometry().bottom() - geometry().bottom();

	setGeometry(newGeometry.adjusted(x1, y1 - _addressbar->height(), x2, y2));
    }

#endif	// USE_POPUP_WINDOW





    void WebView::onLoadFinished(bool success){
	if(success && 100 != _progress){
	    qWarning()	<< "Received finished signal while progress is still:" << progress()
			<< "Url:" << _page->url();
	}
	if(success){
	    _page->onUrlChanged(_page->url());
	    _page->onTitleChanged(_page->title());

//	    _page->record_info_update(_page->url(), _page->title());
		// }

	    auto					target_		= _page->binder()->host();
	    auto					tree_view_	= _page->_tree_screen->view();
	    QList<boost::intrusive_ptr<TreeItem> >	items_;
	    assert(target_);
	    if(target_) items_ = tree_view_->source_model()->children([&](boost::intrusive_ptr<const TreeItem> it_){return url_equal(it_->field<url_type>().toStdString(), target_->field<url_type>().toStdString()) || it_->id() == target_->id() || it_ == target_;});
//                    || (it_->field<home_type>() != "" && ti->field<home_type>() != "" && url_equal(it_->field<home_type>().toStdString(), ti->field<home_type>().toStdString()))
	    if(target_ && items_.size() > 1){
		QList<boost::intrusive_ptr<TreeItem> > others_same;
		for(auto it_ : items_){
		    if(it_ != target_)
				if(! others_same.contains(it_)) others_same << it_;
		}
		for(auto _it : others_same){
		    if(_it->binder()){
			if(_it->binder()->page()){
			    if(_it->binder()->page()->view() == _tabmanager->currentWebView()) _record_controller->select_as_current(_record_controller->index<pos_proxy>(target_));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											// if(_record_controller->source_model()->current_item() != _it) _record_controller->select_as_current(_record_controller->index<pos_proxy>(_it));
			    auto it_ = TreeLevel::instance(TreeIndex::create_treeindex_from_item([&] {return tree_view_->source_model();}, target_), _it)->merge();																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																	// TreeIndex::instance([&] {return v->source_model();}, ti, ti->parent()), _it);

				//                    std::thread(&KnowView::view_merge, v, TreeIndex::instance([&] {return v->source_model();}, it->parent(), it), j_).join();
			}
		    }
		}
	    }
//	    recovery_global_consistency();
//	    target_ = _page->binder()->host();
	    auto current = _tabmanager->currentWebView()->page()->binder()->host();
	    if(_record_controller->source_model()->current_item() != current){
		auto pp	= _record_controller->index<pos_proxy>(current);
//		auto	index_proxy_	= _record_controller->index<index_proxy>(pp);
		_record_controller->select_as_current(pp);
//		auto v = _record_controller->view();
//		if(v){
//		    v->setCurrentIndex(index_proxy_);
//		    v->setFocus();
//		    v->edit(index_proxy_);
//		}
		current_view_global_consistency();
	    }
	    _load_finished = true;
//	    setFocus();
	}
	_progress = 0;
    }

    bool WebView::load_finished() const {return _load_finished;}

    WebView::~WebView(){
	// if(_page->record()) {
	// _page->record()->page(nullptr);
	////            globalparameters.getRecordTableScreen()->previousInFocusChain();    //_record;
	// _page->record(nullptr);
	// }
	if(_page){
		// delete _page;
		// _page = nullptr; //
		// emit _page->close_requested();
	    _page->deleteLater();
	}
    }

    WebPage *WebView::page() const {return _page;}




#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(WebView)
#endif

    WebView::WebView(boost::intrusive_ptr<TreeItem> host_
		    , Profile		*profile			// , bool openinnewtab
		    , ts_t		*tree_screen
		    , Editentry		*editentry
		    , Entrance		*entrance
		    , Browser		*browser
		    , TabWidget		*tabmanager
		    , rctrl_t		*table_controller)
	: QWebEngineView(static_cast<QWidget *>(tabmanager))	// ->parent()
	  , _browser(browser)
	  , _tabmanager(tabmanager)					// , _record(record)
	  , _record_controller(table_controller)
	  , _page(new WebPage(profile
			     , host_			// , openinnewtab
			     , tree_screen
			     , editentry
			     , entrance
			     , browser
			     , tabmanager
			     , table_controller
			     , this)
	      )
		// , _initialurl(record ? record->getNaturalFieldSource("url") : QUrl())
	  , _progress(0)
	  , _iconreply(0){
	settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
	settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
	settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
	settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

	settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
	settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
	settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
	settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
	settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, true);
	settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
	settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, true);
	settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
	settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
	settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

	// if(record) {record->view(this);}

	connect(this, &WebView::loadProgress, this, &WebView::setProgress);

	connect(this, &WebView::loadFinished, this, &WebView::onLoadFinished);
	connect(this, &QWebEngineView::renderProcessTerminated
	       , [=](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode){
		const char *status = "";
		switch(termStatus){
		    case QWebEnginePage::NormalTerminationStatus:
			status = "(normal exit)";
			break;

		    case QWebEnginePage::AbnormalTerminationStatus:
			status = "(abnormal exit)";
			break;

		    case QWebEnginePage::CrashedTerminationStatus:
			status = "(crashed)";
			break;

		    case QWebEnginePage::KilledTerminationStatus:
			status = "(killed)";
			break;
		}
		qInfo() << "Render process exited with code" << statusCode << status;

		QTimer::singleShot(0, [this] {reload();});
	    });

	// connect(this->webPage(), &WebPage::loadFinished, this, &WebView::onLoadFinished);
	// connect(this->webPage(), &WebPage::titleChanged, this, &WebView::onTitleChanged);
	// connect(this->webPage(), &WebPage::urlChanged, this, &WebView::onUrlChanged);
	////    &WebPage::titleChanged(const QString &title);
	////    &WebPage::urlChanged(const QUrl &url);

	QWebEngineView::setPage(_page);

#if defined(QWEBENGINEPAGE_STATUSBARMESSAGE)
	connect(page(), &WebPage::statusBarMessage, &WebPage::setStatusBarText);
#endif
	connect(this->_page, &WebPage::loadingUrl, this, [&](const QUrl &url){
		auto _binder = this->_page->binder();
		if(_binder->host()){
		    if(  url.toString() != ""
		      && url != QUrl() && ! url.host().isEmpty() && ! url.scheme().isEmpty()
		      && url != QUrl(Browser::_defaulthome)
			// && url != _loadingurl
		      && url.toString() != _binder->host()->field<url_type>()
		    )
				if(url.isValid() && url != this->_page->_loadingurl) this->urlChanged(url);
		}
	    });
	connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::iconUrlChanged, this, &WebView::onIconUrlChanged);
	connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::featurePermissionRequested, this, &WebView::onFeaturePermissionRequested);
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
	page()->setForwardUnsupportedContent(true);
#endif


	connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::loadFinished, this, &WebView::onLoadFinished);
	connect(this, &WebView::close_requested, &WebView::on_close_requested);
	// &WebPage::titleChanged(const QString &title);
	// &WebPage::urlChanged(const QUrl &url);
	// _page->load(_record->getField("url"));    // auto  loaded

	// _page = new WebPage(profile
	// , requested_item // , openinnewtab
	// , _browser
	// , _tabmanager
	// , table_controller
	// , this);
	setObjectName("web view");
	setFocus();
	// _record_controller->addnew_item_fat(requested_item);

// set_kinetic_scrollarea(qobject_cast<QAbstractItemView *>(this));    //  does not work for base class is not QAbstractItemView
    }

    void WebView::page(WebPage *_page){
	if(_page) _page->deleteLater();
	this->_page = _page;
	QWebEngineView::setPage(_page);
	if(_page){
#if defined(QWEBENGINEPAGE_STATUSBARMESSAGE)
	    connect(page(), &WebPage::statusBarMessage, &WebPage::setStatusBarText);
#endif
	    connect(this->_page, &WebPage::loadingUrl, this	// &WebView::urlChanged
		   , [&](const QUrl &url){
		    auto _binder = this->_page->binder();
		    if(_binder->host()){
			if(  url.toString() != ""
			  && url != QUrl() && ! url.host().isEmpty() && ! url.scheme().isEmpty()
			  && url != QUrl(Browser::_defaulthome)
			  && url.toString() != _binder->host()->field<url_type>()
			)
				if(url.isValid() && url != this->_page->_loadingurl) this->urlChanged(url);
		    }
		}
		);
	    connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::iconUrlChanged, this, &WebView::onIconUrlChanged);
	    connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::featurePermissionRequested, this, &WebView::onFeaturePermissionRequested);
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
	    page()->setForwardUnsupportedContent(true);
#endif


	    connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::loadFinished, this, &WebView::onLoadFinished);
	    connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::titleChanged, this->_page	// &WebPage::onTitleChanged
		   , [&](const QString &title){
		    assert(this->title() == title);
		    auto host_ = this->_page->binder()->host();
		    if(host_){
			auto url_ = this->_page->url();
			if(  url_ != QUrl()
			  && ! url_.host().isEmpty()
			  && ! url_.scheme().isEmpty()
			  && url_.isValid()
			  && url_ != QUrl(Browser::_defaulthome)
			  && url_ == this->_page->_loadingurl
			  && url_.toString() == host_->field<url_type>()
			){
			    assert(this->_page->_editentry);
			    if(  title != ""
			      && title != host_->field<name_type>()	// && !QUrl(title).isValid()
			    ){
				this->_page->record_info_update(url_, title);
				//		    record_view_synchronize(_binder->host());
			    }
				// }
			}
		    }
		}
		);
	    connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::urlChanged, this->_page	// &WebPage::onUrlChanged
		   , [&](const QUrl &url){
		    auto _binder = this->_page->binder();
		    if(_binder->host()){
			if(  url.toString() != ""
			  && url != QUrl() && ! url.host().isEmpty() && ! url.scheme().isEmpty() && url.isValid()
			  && url != QUrl(Browser::_defaulthome)
			  && url != this->_page->_loadingurl
			  && url.toString() != _binder->host()->field<url_type>()
			)
//				if(url.isValid() && url != this->_page->_loadingurl)
				this->_page->record_info_update(url, this->_page->title());																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																									// this->_page->onUrlChanged(url);
		    }
		}
		);
	}
    }

    void WebView::activateWindow(){
	QObject::disconnect(_home_connection);

	_home_connection
	    = QObject::connect(
	    _tabmanager->browser()->_historyhome
			      , &QAction::triggered
			      , this, [&](bool checked = true) -> void {
		Q_UNUSED(checked)
		if(_page->binder()){
		    boost::intrusive_ptr<TreeItem> _item = _page->binder()->host();
		    assert(_item);
		    QString home = _item->field<home_type>();
		    QUrl homeurl = QUrl(home);
		    if(homeurl.isValid() && homeurl != _page->url()){
			_item->field<url_type>(home);		// "url",
			_page->load(_item, true);
		    }
		}
	    }
	    );
	QWebEngineView::activateWindow();
    }

    void WebView::contextMenuEvent(QContextMenuEvent *event){
#if defined(QWEBENGINEPAGE_HITTESTCONTENT)
	QWebEngineHitTestResult r = page()->hitTestContent(event->pos());
	if(! r.linkUrl().isEmpty()){
	    QMenu menu(this);
	    menu.addAction(pageAction(QWebEnginePage::OpenLinkInNewWindow));
	    menu.addAction(tr("Open in New Tab"), this, &WebView::openLinkInNewTab);
	    menu.addSeparator();
	    menu.addAction(pageAction(QWebEnginePage::DownloadLinkToDisk));
		// Add link to bookmarks...
	    menu.addSeparator();
	    menu.addAction(pageAction(QWebEnginePage::CopyLinkToClipboard));
	    if(page()->settings()->testAttribute(QWebEngineSettings::DeveloperExtrasEnabled)) menu.addAction(pageAction(QWebEnginePage::InspectElement));
	    menu.exec(mapToGlobal(event->pos()));

	    return;
	}
#endif

	QMenu										*menu	= _page->createStandardContextMenu();
	const QList<QAction *>								actions = menu->actions();
	QList<QAction *>::const_iterator						it	= qFind(actions.cbegin(), actions.cend(), _page->action(QWebEnginePage::OpenLinkInThisWindow));
	if(it != actions.cend()){
	    (*it)->setText(tr("Open Link in This Window"));
	    ++ it;
	    menu->insertAction(*it, _page->action(QWebEnginePage::OpenLinkInNewWindow));
	    menu->insertAction(*it, _page->action(QWebEnginePage::OpenLinkInNewTab));
	}
	menu->popup(event->globalPos());

	// QWebEngineView::contextMenuEvent(event);
    }

    void WebView::wheelEvent(QWheelEvent *event){
#if defined(QWEBENGINEPAGE_SETTEXTSIZEMULTIPLIER)
	if(QApplication::keyboardModifiers() & Qt::ControlModifier){
	    int numDegrees	= event->delta() / 8;
	    int numSteps	= numDegrees / 15;
	    setTextSizeMultiplier(textSizeMultiplier() + numSteps * 0.1);
	    event->accept();

	    return;
	}
#endif
	QWebEngineView::wheelEvent(event);
    }

    void WebView::focusOutEvent(QFocusEvent *event){
	if(event)
		if(this != _tabmanager->currentWebView()) QWebEngineView::focusOutEvent(event);
    }

    void WebView::openLinkInNewTab(){
	pageAction(QWebEnginePage::OpenLinkInNewTab)->trigger();
    }

    void WebView::openLinkInNewTab(const QUrl &_url){
	Q_UNUSED(_url);

#if defined(QWEBENGINEPAGE_WEBACTION_OPENLINKINNEWWINDOW)
	_page->_openinnewtab = true;
	pageAction(QWebEnginePage::OpenLinkInNewWindow)->trigger();
#endif

	////QString url(QString(""));
	////QUrl _url = globalParameters.getBrowserView()->mainWindow()->currentTab()->url();

	////    if(!_record_ontroller) {
	////        //RecordTableController *
	////        _record_ontroller = globalParameters.getRecordTableScreen()->getRecordTableController();
	////    }

	////    RecordTableData *recordTableData = _record_ontroller->getRecordTableModel()->getRecordTableData();

	////if(_url.isValid() && _record == nullptr) {
	// _page->record(bind_record(_url));

	////        QModelIndex proxyindex = _record_ontroller->convertIdToProxyIndex(_page->record()->getField("id"));
	////        int position = _record_ontroller->convertProxyIndexToPos(proxyindex);
	////        _record_ontroller->getView()->setSelectionToPos(position);
	setFocus();
	// globalparameters.mainwindow()
	if(_record_controller->view()->selection_first<id_value>() != _page->host()->field<id_type>()) _record_controller->select_as_current(_record_controller->index<pos_proxy>(_page->host()));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											// IdType(_page->item()->field("id"))

	// }
    }

    void WebView::onFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature){
	FeaturePermissionBar *permissionBar = new FeaturePermissionBar(this);
	connect(permissionBar, &FeaturePermissionBar::featurePermissionProvided, _page, &QWebEnginePage::setFeaturePermission);

	// Discard the bar on new loads (if we navigate away or reload).
	connect(static_cast<QWebEnginePage *const>(this->_page), &QWebEnginePage::loadStarted, permissionBar, &QObject::deleteLater);

	permissionBar->requestPermission(securityOrigin, feature);
    }

	// void PageView::loadUrl(const QUrl &url)
	// {
	////        _initialurl = url;
	// _page->load(url);   // load(url);
	// }


    QString WebView::lastStatusBarText() const {return _statusbartext;}

    int WebView::progress() const {return _progress;}

    void WebView::setProgress(int progress){_progress = progress;}

    TabWidget *const &WebView::tabmanager() const {return _tabmanager;}

    rctrl_t *WebView::record_controller(){return _record_controller;}

    void WebView::record_controller(rctrl_t *_record_controller){this->_record_controller = _record_controller;}

	// QUrl PageView::url() const
	// {
	// QUrl url = QWebEngineView::url();

	// if(!url.isEmpty())
	// return url;

	// return _initialurl;
	// }


    QIcon WebView::icon() const {
	if(! _icon.isNull()) return _icon;
	return sapp_t::instance()->defaultIcon();
    }

    void WebView::onIconUrlChanged(const QUrl &url){
	QNetworkRequest iconRequest(url);
	_iconreply = sapp_t::networkAccessManager()->get(iconRequest);
	_iconreply->setParent(this);
	connect(_iconreply, &QNetworkReply::finished, this, &WebView::iconLoaded);
    }

    void WebView::on_close_requested(){
	close();

	emit _page->close_requested();

	// int tab_widget_count = tabmanager()->count();
	// int tab_bar_count = tabmanager()->tabbar()->count();
	// int source_model_size = _record_controller->source_model()->size();
	// assert(tab_bar_count == tab_widget_count);

	// if(source_model_size > tab_widget_count) {
	// bool found = false;

	// for(int i = 0; i < source_model_size; i++) {
	// auto item_maybe_to_removed = _record_controller->source_model()->child(i);

	// if(item_maybe_to_removed->unique_page() == page() || !item_maybe_to_removed->page_valid()) {
	////                    if(_tabmanager->indexOf(item_maybe_to_removed->unique_page()->view()) != -1)
	////                        _tabmanager->closeTab(_tabmanager->indexOf(item_maybe_to_removed->unique_page()->view()));  //_record_controller->source_model()->remove_child(item_maybe_to_removed->id());

	// page()->break_page_linked_item(item_maybe_to_removed);  // emit _page->close_requested();
	// found = true;
	// break;
	// }
	// }

	// assert(found == true);
	// }

	// assert(tabmanager()->count() == tabmanager()->tabbar()->count());
	// assert(_record_controller->source_model()->size() == tabmanager()->tabbar()->count());
	// assert(_record_controller->source_model()->size() == tabmanager()->count());
    }

    void WebView::iconLoaded(){
	_icon = QIcon();
	if(_iconreply){
	    QByteArray	data = _iconreply->readAll();
	    QPixmap	pixmap;
	    pixmap.loadFromData(data);
	    _icon.addPixmap(pixmap);
	    _iconreply->deleteLater();
	    _iconreply = 0;
	}
	emit iconChanged();
    }

    void WebView::mousePressEvent(QMouseEvent *event){
	_page->_pressedbuttons		= event->buttons();
	_page->_keyboardmodifiers	= event->modifiers();
	QWebEngineView::mousePressEvent(event);
    }

    void WebView::mouseReleaseEvent(QMouseEvent *event){
	QWebEngineView::mouseReleaseEvent(event);
	if(! event->isAccepted() && (_page->_pressedbuttons & Qt::MidButton)){
	    QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
	    if(! url.isEmpty() && url.isValid() && ! url.scheme().isEmpty()) setUrl(url);
	}
    }

    void WebView::setStatusBarText(const QString &string){
	_statusbartext = string;
    }

    WebView *WebView::load(boost::intrusive_ptr<TreeItem> it, bool checked){
	WebView *v = nullptr;
	if(it) v = _page->load(it, checked);
	return v;
    }

    void WebView::current_view_global_consistency(){
	rs_t			*_record_screen = _browser->record_screen();
	wn_t			*_mainwindow	= globalparameters.main_window();
//	HidableTabWidget	*_vtab_tree	= _mainwindow->vtab_tree();
	HidableTabWidget			*_vtab_record	= _mainwindow->vtab_record();
	ts_t					*_tree_screen	= globalparameters.tree_screen();
	auto					_tree_view	= _tree_screen->view();
// auto _tabmanager = v->tabmanager();
	auto index = _tabmanager->webViewIndex(this);
// auto _record_controller = _tabmanager->record_controller();
	if(index != _tabmanager->currentIndex()) _tabmanager->setCurrentIndex(index);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													// c->index<PosSource>(c->source_model()->index(_binder->item()))
	if(! _browser->isVisible()){
	    _browser->raise();
	    _browser->activateWindow();
	}
// _record_controller->synchronize_record_view(_page->item());
	if(_vtab_record->currentWidget() != _record_screen) _vtab_record->setCurrentWidget(_record_screen);
//	for(int i = 0; i < _vtab_tree->count(); i ++){
//	    auto tree_viewer = _vtab_tree->widget(i);
//	    if(tree_viewer->objectName() == tree_screen_viewer_name){
//		auto tree_viewer_ = dynamic_cast<tsv_t *>(tree_viewer);
//		if(tree_viewer_){
//		    if(tree_viewer_->widget_right() == _record_screen){
//			if(_vtab_tree->currentIndex() != i){
//			    if(! tree_viewer_->tree_screen())tree_viewer_->tree_screen(_tree_screen);
//			    _vtab_tree->setCurrentIndex(i);
//			}
//		    }
//		}
//	    }
//	}
	auto _target_item_in_browser = _page->binder()->host();
	if(_target_item_in_browser != _tree_view->current_item()) _tree_view->select_as_current(TreeIndex::create_treeindex_from_item([&] {return _tree_view->source_model();}, _target_item_in_browser));
	if((_record_controller->view()->current_item() != _target_item_in_browser) || (_tabmanager->currentWebView() != this)) _record_controller->select_as_current(_record_controller->index<pos_proxy>(_target_item_in_browser));
	if(! _mainwindow->windowTitle().contains(_page->title())) _mainwindow->setWindowTitle(QString(globalparameters.application_name()) + " : " + _page->title());																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						// table->setWorkPos(pos);
	_page->_editentry->name(_page->title());
    }

	// void WebView::switch_show()
	// {
	// _tabmanager->setCurrentIndex(_tabmanager->webViewIndex(this));
	// _page->load(_page->current_record());
	// show();
	// }

    WebPage::Binder::Binder(boost::intrusive_ptr<TreeItem> item_, WebPage *page_)	// , bool make_current = true
	:	// std::enable_shared_from_this<Coupler>()
	  _host(item_)
	  , _page(page_){			// , _make_current(make_current)
	// _bounded_page->record_binder(new TreeItem::coupler_delegation(shared_from_this()));
	// _bounded_item->record_binder(_bounded_page->record_binder());
    }

    WebView *WebPage::Binder::bind(){	// boost::intrusive_ptr<TreeItem> item
	assert(_page);
	////                boost::intrusive_ptr<TreeItem> result = _the->record_controller()->source_model()->find(item);
	WebView *view = nullptr;
//	if(_page){
//	    view = _page->view();
//	    if(! _item){
//		_item = _page->binder() ? _page->binder()->host() : nullptr;
//		//
//	    }
//	}
//	////                if(result) {
//	////                    view = item->bind();    //
//	////                    // item->unique_page()->view();
//	////                } else {
//	// WebView *view = _bounded_page->bind(item);
//	////        (item.get()->*_bind)(_bounded_page);
//	////        //                }
//	// _bounded_item = item;
	if(_host){
	    if(! _page){
		auto	_browser	= globalparameters.main_window()->vtab_record()->activated_browser();
		auto	record_index	= RecordIndex::instance([&] {return _browser->record_screen()->record_controller()->source_model();}, _host);

		_page	= _browser->bind(record_index)->page();
		view	= _page->view();
	    }
		// if((_bounded_item && _bounded_item.get() != item.get())) { // || (_item && !_item->page_valid())    // bug, they may all are nullptr, conflict with upon
		// _bounded_page->item_break(_bounded_item);
		// }
		// _bounded_item = item;
	    if(_host->binder() != _page->binder()){
		assert(_host->binder());
		_page->binder_reset();
		// _page->binder(// std::forward<boost::intrusive_ptr<TreeItem::Coupler>&>(
		// _item->binder()
		//// )
		// )
		// ;
	    }
	    _page->bind(_host);
	    if(_page->url().toString() != _host->field<url_type>()) _page->setUrl(QUrl(_host->field<url_type>()));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																			// _bounded_page = view->page();
		// MetaEditor *_editor_screen = globalparameters.meta_editor();    // find_object<MetaEditor>(meta_editor_singleton_name);
	    assert(_page->_editentry);
	    if(_page->_editentry->item() != _host) _page->metaeditor_sychronize();
	    assert(_page->binder()->integrity_external(_host, _page));
	}else{
	    if(_page){
		view = _page->view();
		if(! _page->binder()) new ::Binder(std::make_shared<WebPage::Binder>(_host, _page));
		_host = _page->binder()->host();
	    }
	}
	return view;	// _the->load(record, _make_current);
    }

    WebPage::Binder::~Binder(){
	// if(_bounded_item && _bounded_page) { // || (_item && !_item->page_valid())    // bug, they may all are nullptr, conflict with upon
	// if(_bounded_page->record_binder() && _bounded_item->record_binder())
	// _bounded_page->item_break(_bounded_item);
	// }

	// _bounded_page->record_binder().reset();
	// _bounded_item->record_binder().reset();
    }

    WebView *WebPage::Binder::activator(
	// boost::intrusive_ptr<TreeItem> item
	){
	// assert(_item_link->binder());
	// assert(_item_link->binder() == _page_link->binder());
	// assert(_item_link->page_link() == _page_link);
	// assert(_item_link == _page_link->item_link());
	////        WebView *view = nullptr;

	// assert(_item_link);
	// assert(_item_link->page_valid());
	assert(_host->binder()->integrity_external(_host, _page));
	// return _bounded_item->unique_page()->activate();

	////        if(_page->view() != _page->_tabmanager->currentWebView()) {
	// view = _bounded_page->activate();
	////        } else {
	////            view = _page->view();
	////            view->show();
	////        }

	return _page->activate();	// view;
    }

    boost::intrusive_ptr<TreeItem> WebPage::host() const {return _binder ? _binder->host() : nullptr;}

//    WebPage *WebPage::page() const {return _binder ? _binder->page() : nullptr;}

    boost::intrusive_ptr<::Binder> WebPage::binder(){return _binder;}

    const boost::intrusive_ptr<::Binder> && WebPage::binder() const {return std::forward<const boost::intrusive_ptr<::Binder> >(_binder);}

    boost::intrusive_ptr<::Binder> WebPage::binder(boost::intrusive_ptr<::Binder> &&binder_){
	boost::intrusive_ptr<::Binder> result(nullptr);
	if(binder_){
	    binder_->page(this);
	    if(binder_ != _binder) this->_binder = std::forward<boost::intrusive_ptr<::Binder> >(binder_);
	    result = this->_binder;
	}else{
	    _binder->host(nullptr);
	    _binder->page(nullptr);
	    _binder = nullptr;
	}
	return result;
    }

    WebView *WebPage::view(){return _view;}		// assert(_view);

    rctrl_t *WebPage::record_controller(){return _record_controller;}

    TabWidget *WebPage::tabmanager(){return _tabmanager;}
}





