/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of a Qt Solutions component.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef QTSINGLEAPPLICATION_H
#define QTSINGLEAPPLICATION_H




#include <QApplication>
#include "libraries/global_parameters.h"
#include "models/app_config/app_config.h"
#include "models/database_config/database_config.h"
#include "views/browser/browser.h"
#include <memory>



#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif


class QtLocalPeer;

#if defined(Q_WS_WIN) || defined(Q_OS_WIN32)
#  if ! defined(QT_QTSINGLEAPPLICATION_EXPORT) && ! defined(QT_QTSINGLEAPPLICATION_IMPORT)
#    define QT_QTSINGLEAPPLICATION_EXPORT
#  elif defined(QT_QTSINGLEAPPLICATION_IMPORT)
#    if defined(QT_QTSINGLEAPPLICATION_EXPORT)
#      undef QT_QTSINGLEAPPLICATION_EXPORT
#    endif
#    define QT_QTSINGLEAPPLICATION_EXPORT __declspec(dllimport)
#  elif defined(QT_QTSINGLEAPPLICATION_EXPORT)
#    undef QT_QTSINGLEAPPLICATION_EXPORT
#    define QT_QTSINGLEAPPLICATION_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTSINGLEAPPLICATION_EXPORT
#endif

#include <QtWidgets/QApplication>

#include <QtCore/QUrl>
#include <QtCore/QPointer>

#include <QtGui/QIcon>
// #include "qtsingleapplication.h"





#include <QtNetwork/QAuthenticator>



#ifdef USE_QTM

// #include <QApplication>
#include <QStringList>
#include <QList>

class SuperMenu;
class QMenuBar;
class EditingWindow;

#endif


QT_BEGIN_NAMESPACE
class QLocalServer;
class QNetworkAccessManager;
class QNetworkProxy;
class QNetworkReply;
class QWebEngineProfile;
QT_END_NAMESPACE

// extern const char *application_name;

namespace browser {
    class BookmarksManager;
    class Browser;
    class CookieJar;
    class DownloadManager;
    class HistoryManager;
    class UrlRequestInterceptor;
    class Profile;
}

class wn_t;

class QT_QTSINGLEAPPLICATION_EXPORT sapp_t	//: public BrowserApplication { //
    : public QApplication {
#if QT_VERSION == 0x050600
    W_OBJECT(QT_QTSINGLEAPPLICATION_EXPORT sapp_t)
#else
    Q_OBJECT
#endif

    public:
	//    typedef BrowserApplication BaseType;
	sapp_t(int &argc
	      , char * *argv
	      , gl_para &globalparameters
	      , AppConfig &appconfig
	      , DataBaseConfig &databaseconfig
	      , bool _gui_enabled = true);
	sapp_t(const QString &id
	      , int &argc
	      , char * *argv
	      , gl_para &globalparameters
	      , AppConfig &appconfig
	      , DataBaseConfig &databaseconfig);
	~sapp_t();
	// Comment by Xi
	// QtSingleApplication(int &argc, char **argv, Type type);

	//    static QtSingleApplication *instance();


#if defined(Q_WS_X11)
	QtSingleApplication(Display * dpy, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
	QtSingleApplication(Display * dpy, int &argc, char * *argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
	QtSingleApplication(Display * dpy, const QString &appId, int argc, char * *argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
#endif

	bool isRunning();
	QString id() const;

	void setActivationWindow(QWidget *aw, bool activateOnMessage = true);
	QWidget *activationWindow() const;

	// Obsolete:
	void initialize(bool dummy = true){isRunning();Q_UNUSED(dummy)}

    public Q_SLOTS:
	bool sendMessage(const QString &message, int timeout = 5000);
	void activateWindow();


Q_SIGNALS:
	void messageReceived(const QString &message)
#if QT_VERSION == 0x050600
	W_SIGNAL(messageReceived, (const QString &), message)	// ;
#else
	;
#endif


    private:
	void sys_init(char * *argv);
	void main_window();
	void browser_init();
#ifdef USE_QTM
	void qtm_init();
#endif
	QtLocalPeer	*_peer;
	QWidget		*_act_window;
	bool _gui_enabled;
	wn_t		*_window;
	// private slots:
	//    void newLocalSocketConnection();

	// copy paste

    public:

	//    BrowserApplication(int &argc, char **argv);

	//    ~BrowserApplication();

	static sapp_t	*instance();	// move to derived class
	void loadSettings();

	//    bool isTheOnlyBrowser() const;
	//    BrowserWindow *mainWindow();
	//    QList<BrowserWindow *> mainWindows();
	QIcon icon(const QUrl &url) const;
	QIcon defaultIcon() const;

	void saveSession();
	bool canRestoreSession() const;
	bool privateBrowsing() const {return _private_browsing;}

	void setLastAuthenticator(QAuthenticator *);
	void setLastProxyAuthenticator(QAuthenticator *);


	// TODO: Remove these functions (QTBUG-47967)
	static QByteArray authenticationKey(const QUrl &, const QString &);
	static QByteArray proxyAuthenticationKey(const QNetworkProxy &, const QString &);
	static QByteArray proxyAuthenticationKey(const QString &, const QString &, const QString &);


	static browser::HistoryManager		*historyManager();
	static browser::CookieJar		*cookieJar();
	static browser::DownloadManager		*request_download_manager();
	static QNetworkAccessManager		*networkAccessManager();
	static browser::BookmarksManager	*bookmarksManager();

	void installTranslator(const QString &name);

#if defined(Q_OS_OSX)
	bool event(QEvent *event);
#endif

    public slots:
	//    BrowserWindow *newMainWindow();
	void restoreLastSession();
#if defined(Q_OS_OSX)
	void lastWindowClosed();
	void quitBrowser();
#endif
	void setPrivateBrowsing(bool);

	void authenticationRequired(QNetworkReply *, QAuthenticator *);
	void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *);

    signals:
	void privateBrowsingChanged(bool b)
#if QT_VERSION == 0x050600
	W_SIGNAL(privateBrowsingChanged, (bool), b)	// ;
#else
	;
#endif

    private slots:
	void postLaunch();
	//    void openUrl(const QUrl &url);
	void newLocalSocketConnection();

    private:
	//    void clean();
	//    void installTranslator(const QString &name);

	static browser::HistoryManager		*_historymanager;
	static browser::DownloadManager		*_downloadmanager;
	static QNetworkAccessManager		*_networkaccessmanager;
	static browser::BookmarksManager	*_bookmarksmanager;

	//    QList<QPointer<BrowserWindow> > _mainWindows;
	browser::Profile	*_profile;
	QLocalServer		*_localserver;
	QByteArray _last_session;
	// QWebEngineProfile
	browser::Profile	*_private_profile;
	bool _private_browsing;
	mutable QIcon _default_icon;

	gl_para	&_globalparameters;
	AppConfig		&_appconfig;
	DataBaseConfig		&_databaseconfig;

	QAuthenticator _last_authenticator;
	QAuthenticator _last_proxy_authenticator;
	QString	_style;

#ifdef USE_QTM


	// class Application : public QApplication {
//	Q_OBJECT

    public:
//	Application(int &, char * *);
	void setupRecentFiles();
	QStringList recentFileTitles();
	QStringList recentFilenames();
	typedef struct _RF {
	    QString	title;
	    QString	filename;
	}recentFile;
	QList<recentFile> recentFiles();
	recentFile getRecentFile(int);
	QStringList titles();
	QStringList filenames();
	EditingWindow *currentEditingWindow(){return _currentEditingWindow;}
	void deleteSandbox();
	bool isSandbox(){return _isSandbox;}

    public slots:
	void setRecentFiles(const QStringList &, const QStringList &);
	void addRecentFile(const QString &, const QString &);
	void saveAll();
	void setMainWindow(EditingWindow *sm);

    signals:
	void recentFilesUpdated(QStringList, QStringList);
	void recentFilesUpdated(const QList<sapp_t::recentFile> &);
	void mainWindowChanged(EditingWindow *);

    private:
	QList<sapp_t::recentFile> _recentFiles;
	#ifdef Q_OS_MAC
	SuperMenu *superMenu;
	#endif
	EditingWindow *_currentEditingWindow;
	bool _isSandbox;

    private slots:
	void saveRecentFiles();
	void handleWindowChange(QWidget *, QWidget *);
	void handleLastWindowClosed();
	// };
#endif
};





#endif	// QTSINGLEAPPLICATION_H
