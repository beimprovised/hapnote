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


#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include "qtsingleapplication.h"
#include "qtlocalpeer.h"
#include <QWidget>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QFileInfo>

#include "views/browser/browser.h"
#include "views/browser/tabwidget.h"
#include "views/browser/webview.h"
#include "libraries/global_parameters.h"
#include "views/record_table/record_screen.h"
#include "models/database_config/database_config.h"
#include "models/record_table/linker.hxx"
#include "models/record_table/record_index.hxx"
#include "models/tree/tree_index.hxx"
#include "models/record_table/record_model.h"
#include "models/record_table/items_flat.h"
#include "controllers/record_table/record_controller.h"
#include "views/tree/tree_view.h"
#include "main.h"


#include <utility>

// begin from main.cpp

#include <sys/timeb.h>

#include <QTranslator>
#include <QToolButton>
#include <QSplashScreen>

#if QT_VERSION >= 0x050000
#include <QScroller>
#include <QScrollerProperties>
#include <QScrollBar>
#endif

#include <qtwebenginewidgetsglobal.h>

#include "main.h"



#include <qversiontagging.h>

#ifndef QT_VERSION
// #define QT_VERSION 0x050600
#define QT_VERSION 0x050700
#endif

#if QT_VERSION < 0x050000
#include "libraries/qtSingleApplication/qtsingleapplication.h"
#else
#include "libraries/qt_single_application5/qtsingleapplication.h"
#endif

#include "views/main_window/main_window.h"
#include "models/app_config/app_config.h"
#include "libraries/clipboard_records.h"
#include "libraries/trash_monitoring.h"
#include "libraries/fixed_parameters.h"
#include "libraries/global_parameters.h"
#include "libraries/walk_history.h"
#include "libraries/window_switcher.h"
#include "libraries/crypt/rc5simple.h"
#include "libraries/crypt/password.h"
#include "libraries/global_parameters.h"
#include "views/record_table/record_screen.h"
#include "models/database_config/database_config.h"
#include "models/record_table/record_index.hxx"
#include "models/record_table/record_model.h"
#include "models/record_table/items_flat.h"
#include "controllers/record_table/record_controller.h"


// using namespace std;

//// Фиксированные параметры программы (жестко заданные в текущей версии Hapnote)
// FixedParameters fixedParameters;

//// Глобальные параметры программы (вычислимые на этапе инициализации, иногда меняющиеся в процессе выполнения программы)
// GlobalParameters globalParameters;

//// Конфигурация программы (считанная из файла конфигурации)
// AppConfig hapnoteConfig;

//// Конфигурация данных
// DataBaseConfig dataBaseConfig;

//// Объект слежения за состоянием корзины
// TrashMonitoring trashMonitoring;

//// Объект с историей посещаемых записей
// WalkHistory walkHistory;

//// Указатель на основное окно программы
// QObject *pMainWindow;

// end from main.cpp


/*!
    \class QtSingleApplication qtsingleapplication.h
    \brief The QtSingleApplication class provides an API to detect and
    communicate with running instances of an application.

    This class allows you to create applications where only one
    instance should be running at a time. I.e., if the user tries to
    launch another instance, the already running instance will be
    activated instead. Another usecase is a client-server system,
    where the first started instance will assume the role of server,
    and the later instances will act as clients of that server.

    By default, the full path of the executable file is used to
    determine whether two processes are instances of the same
    application. You can also provide an explicit identifier string
    that will be compared instead.

    The application should create the QtSingleApplication object early
    in the startup phase, and call isRunning() to find out if another
    instance of this application is already running. If isRunning()
    returns false, it means that no other instance is running, and
    this instance has assumed the role as the running instance. In
    this case, the application should continue with the initialization
    of the application user interface before entering the event loop
    with exec(), as normal.

    The messageReceived() signal will be emitted when the running
    application receives messages from another instance of the same
    application. When a message is received it might be helpful to the
    user to raise the application so that it becomes visible. To
    facilitate this, QtSingleApplication provides the
    setActivationWindow() function and the activateWindow() slot.

    If isRunning() returns true, another instance is already
    running. It may be alerted to the fact that another instance has
    started by using the sendMessage() function. Also data such as
    startup parameters (e.g. the name of the file the user wanted this
    new instance to open) can be passed to the running instance with
    this function. Then, the application should terminate (or enter
    client mode).

    If isRunning() returns true, but sendMessage() fails, that is an
    indication that the running instance is frozen.

    Here's an example that shows how to convert an existing
    application to use QtSingleApplication. It is very simple and does
    not make use of all QtSingleApplication's functionality (see the
    examples for that).

    \code
    // Original
    int main(int argc, char **argv)
    {
        QApplication app(argc, argv);

        MyMainWidget mmw;
        mmw.show();
        return app.exec();
    }

    // Single instance
    int main(int argc, char **argv)
    {
        QtSingleApplication app(argc, argv);

        if (app.isRunning())
            return !app.sendMessage(someDataString);

        MyMainWidget mmw;
        app.setActivationWindow(&mmw);
        mmw.show();
        return app.exec();
    }
    \endcode

    Once this QtSingleApplication instance is destroyed (normally when
    the process exits or crashes), when the user next attempts to run the
    application this instance will not, of course, be encountered. The
    next instance to call isRunning() or sendMessage() will assume the
    role as the new running instance.

    For console (non-GUI) applications, QtSingleCoreApplication may be
    used instead of this class, to avoid the dependency on the QtGui
    library.

    \sa QtSingleCoreApplication
 */



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

#include "bookmarks.h"
// #include "views/browser/browser.h"
#include "views/browser/entrance.h"
#include "libraries/global_parameters.h"

#include "cookiejar.h"
#include "downloadmanager.h"
#include "history.h"
#include "networkaccessmanager.h"
#include "tabwidget.h"
#include "webview.h"

#include <QtCore/QBuffer>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>

#include <QtGui/QDesktopServices>
#include <QtGui/QFileOpenEvent>
#include <QtWidgets/QMessageBox>

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslSocket>

#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

#include <QtCore/QDebug>


#ifdef USE_QTM

#include "libraries/qtm/EditingWindow.h"
#include <QtCore>
#include <QMenuBar>

#ifdef Q_OS_MAC
#include "SuperMenu.h"
#endif

#include "libraries/qtm/qtm_version.h.in"

#endif

// const char *application_name = "hapnote";

extern TrashMonitoring		trashmonitoring;
browser::DownloadManager	*sapp_t::_downloadmanager	= nullptr;
browser::HistoryManager		*sapp_t::_historymanager	= nullptr;
QNetworkAccessManager		*sapp_t::_networkaccessmanager	= nullptr;
browser::BookmarksManager	*sapp_t::_bookmarksmanager	= nullptr;

static void set_user_style_sheet(QWebEngineProfile *profile
				, const QString &styleSheet
				, browser::Entrance *_entrance	// , browser::BrowserWindow *mainWindow = 0
    ){
    Q_ASSERT(profile);
    QString			scriptName(QStringLiteral("userStyleSheet"));
    QWebEngineScript		script;
    QList<QWebEngineScript>	styleSheets = profile->scripts()->findScripts(scriptName);
    if(! styleSheets.isEmpty()) script = styleSheets.first();
    Q_FOREACH(const QWebEngineScript &s, styleSheets) profile->scripts()->remove(s);
    if(script.isNull()){
	script.setName(scriptName);
	script.setInjectionPoint(QWebEngineScript::DocumentReady);
	script.setRunsOnSubFrames(true);
	script.setWorldId(QWebEngineScript::ApplicationWorld);
    }
    QString source = QString::fromLatin1("(function() {"							  \
					 "var css = document.getElementById(\"_qt_testBrowser_userStyleSheet\");" \
					 "if (css == undefined) {"						  \
					 "    css = document.createElement(\"style\");"				  \
					 "    css.type = \"text/css\";"						  \
					 "    css.id = \"_qt_testBrowser_userStyleSheet\";"			  \
					 "    document.head.appendChild(css);"					  \
					 "}"									  \
					 "css.innerText = \"%1\";"						  \
					 "})()").arg(styleSheet);
    script.setSourceCode(source);
    profile->scripts()->insert(script);
	// run the script on the already loaded views
	// this has to be deferred as it could mess with the storage initialization on startup
    if(_entrance){
	_entrance->style_source(source);
	//        QMetaObject::invokeMethod(browsemanager, "runScriptOnOpenViews", Qt::QueuedConnection, Q_ARG(QString, source));
    }
}

#if QT_VERSION == 0x050600
W_OBJECT_IMPL(QT_QTSINGLEAPPLICATION_EXPORT sapp_t)
#endif


void sapp_t::sys_init(char * *argv){
	// Начальные инициализации основных объектов

	// Запоминается имя файла запущенного бинарника
	// Файл запущенной программы (нулевой аргумент функции main)
	// Store the file name running binaries
	// File running program (a zero argument to main)
    QString main_program_file = QString::fromLatin1(argv[0]);		// Todo: This code must not work correctly with ways to UTF 8   // todo: Этот код наверно некорректно работает с путями в UTF8
    qDebug() << "Set main program file to " << main_program_file;
	// Инициализация глобальных параметров,
	// внутри происходит установка рабочей директории
	// Initialize global parameters,
	// Is being installed inside the working directory
    _globalparameters.init(main_program_file);
    _globalparameters.profile(_profile);

//    _globalparameters.main_program_file(main_program_file);

	// Перехват отладочных сообщений
    set_debug_message_handler();


	//    QtSingleApplication application(argc, argv);

	//    // Do not run another copy    // Не запущен ли другой экземпляр
	//    if(application.isRunning() || !application.isTheOnlyBrowser()) {
	//        QString message = "Another Hapnote exemplar is running.\n";

	//        printf(message.toLocal8Bit());

	//        QMessageBox msgBox;
	//        msgBox.setIcon(QMessageBox::Warning);
	//        msgBox.setText(message);
	//        msgBox.exec();

	//        exit(0);
	//    }

#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
	// Установка увеличенного разрешения для дисплеев с большим DPI (Retina)
	// Set the increased resolution for displays with lots DPI (Retina)
    if(sapp_t::instance()->devicePixelRatio() > 1.0) sapp_t::instance()->setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

#if QT_VERSION < 0x050000
	// Установка кодека текстов
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

	//    // Инициализация глобальных параметров,
	//    // внутри происходит установка рабочей директории
	//    // Initialize global parameters,
	//    // Is being installed inside the working directory
	//    globalparameters.init();

	//    // Initialize the main program of configurable variables    // Инициализация основных конфигурирующих программу переменных
	//    appconfig.init();

	//    // Инициализация переменных, отвечающих за хранилище данных
	//    databaseconfig.init();

	//    QtSingleApplication application(argc, argv, globalparameters, appconfig, databaseconfig);

	//    // Do not run another copy    // Не запущен ли другой экземпляр
	//    if(application.isRunning()) {
	//        QString message = "Another Hapnote exemplar is running.\n";

	//        printf(message.toLocal8Bit());

	//        QMessageBox msgBox;
	//        msgBox.setIcon(QMessageBox::Warning);
	//        msgBox.setText(message);
	//        msgBox.exec();

	//        exit(0);
	//    }

	//    // Установка CSS-оформления
	//    setCssStyle();

	//    // Экран загрузки, показывается только в Андроид версии (так как загрузка идет ~10 сек, и без сплешскрина непонятно что происходит)
	//    QSplashScreen splash(QPixmap(": / resource / pic / hapnote_splash.png"));

	//    if(hapnoteConfig.getShowSplashScreen())
	//        splash.show();


	//    //    // Подключение перевода интерфейса
	//    //    // QString langFileName=globalParameters.getWorkDirectory()+" / resource / translations / hapnote_"+hapnoteconfig.get_interfacelanguage()+".qm";
	//    //    QString langFileName = ": / resource / translations / hapnote_" + hapnoteConfig.get_interfacelanguage() + ".qm";
	//    //    qDebug() << "Use language file " << langFileName;

	//    //    //QTranslator langTranslator;
	//    //    //langTranslator.load(langFileName);
	//    //    application.installTranslator(
	//    //        langFileName    //&langTranslator
	//    //    );


	//    // Создание объекта главного окна
	//    MainWindow win;
	//    win.setWindowTitle(globalparameters.main_program_file());

	//    if(globalParameters.getTargetOs() == "android")
	//        win.show(); // В Андроиде нет десктопа, на нем нельзя сворачивать окно
	//    else {
	//        if(hapnoteConfig.get_runinminimizedwindow() == false)
	//            win.show();
	//        else
	//            win.hide();
	//    }

	//    // win.setObjectName("mainwindow");
	//    // pMainWindow=&win; // Запоминается указатель на основное окно

	//    // После создания окна восстанавливается вид окна в предыдущий запуск
	//    // Эти действия нельзя делать в конструкторе главного окна,
	//    // т.к. окно еще не создано
	//    globalParameters.getWindowSwitcher()->disableSwitch();
	//    win.restoreFindOnBaseVisible();
	//    win.restoreGeometry();
	//    win.restoreTreePosition();
	//    win.restoreRecordTablePosition();
	//    win.restoreEditorCursorPosition();
	//    win.restoreEditorScrollBarPosition();
	//    globalParameters.getWindowSwitcher()->enableSwitch();

	//    if(hapnoteConfig.getInterfaceMode() == "mobile")
	//        globalParameters.getWindowSwitcher()->restoreFocusWidget();

	//    qDebug() << "Restore session succesfull";

	//    // После восстановления последней редактируемой записи
	//    // история перехода очищается, так как в не может попасть
	//    // первая запись в востаналиваемой ветке и сама восстанавливаемая запись
	//    walkHistory.clear();


	//    // Если в конфиге настроено, что нужно синхронизироваться при старте
	//    // И задана команда синхронизации
	//    if(hapnoteConfig.get_synchroonstartup())
	//        if(hapnoteConfig.get_synchrocommand().trimmed().length() > 0)
	//            win.synchronization();


	//    // Если настроено в конфиге, сразу запрашивается пароль доступа
	//    // к зашифрованным данным
	//    // И если есть хоть какие-то зашифрованные данные
	//    if(hapnoteConfig.get_howpassrequest() == "atStartProgram")
	//        if(globalParameters.getCryptKey().length() == 0)
	//            if(dataBaseConfig.get_crypt_mode() > 0) {
	//                // Запрашивается пароль только в том случае, если ветка,
	//                // на которую установливается курсор при старте, незашифрована
	//                // Если ветка зашифрована, пароль и так будет запрошен автоматически
	//                if(win.isTreePositionCrypt() == false) {
	//                    Password password;
	//                    password.retrievePassword();
	//                }
	//            }


	//    // Если в общем конфиге стоит опция хранения пароля
	//    // И хранимый пароль (точнее его хеш) заполнен
	//    if(globalParameters.getCryptKey().length() == 0)
	//        if(dataBaseConfig.get_crypt_mode() > 0)
	//            if(hapnoteConfig.getPasswordSaveFlag())
	//                if(hapnoteConfig.getPasswordMiddleHash().length() > 0) {
	//                    // При запросе пароля ключ шифрования будет восстановлен автоматически
	//                    Password password;
	//                    password.retrievePassword();
	//                }

	//    // Распечатывается дерево сгенерированных объектов
	//    // print_object_tree();

	//    // Проверяется наличие системного трея
	//    /*
	//    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
	//     QMessageBox::critical(0, QObject::tr("Systray"),
	//                           QObject::tr("I couldn't detect any system tray on this system."));
	//     exit(1);
	//    }
	//    */

	//    // При закрытии окна не выходить из программы.
	//    // Окно программы может быть снова открыто из трея
	//    QApplication::setQuitOnLastWindowClosed(false);


	//    // win.show();
	//    application.connect(&application, SIGNAL(lastWindowClosed()), &application, SLOT(quit()));

	//    // app.connect(&app, SIGNAL(app.commitDataRequest(QSessionManager)), SLOT(win.commitData(QSessionManager)));

	//    // Окно сплеш-скрина скрывается
	//    if(hapnoteConfig.getShowSplashScreen())
	//        splash.finish(&win);


	// Initialize the main program of configurable variables    // Инициализация основных конфигурирующих программу переменных
    _appconfig.init();


	// Инициализируется объект слежения за корзиной
    trashmonitoring.init(globalparameters.root_path() + "/" + QDir(_appconfig.trash_dir()).dirName());
    trashmonitoring.update();

	// Инициализация переменных, отвечающих за хранилище данных
    _databaseconfig.init();




    QString version = QString::number(APPLICATION_RELEASE_VERSION) + "." + QString::number(APPLICATION_RELEASE_SUBVERSION) + "." + QString::number(APPLICATION_RELEASE_MICROVERSION);
    QCoreApplication::setOrganizationName(QLatin1String(globalparameters.application_name().toLatin1()));
    QCoreApplication::setApplicationName(QLatin1String(_globalparameters.application_name().toLatin1()));
    QCoreApplication::setApplicationVersion(version);	// QLatin1String("0.1")

    QString serverName = QCoreApplication::applicationName() + QString::fromLatin1(QT_VERSION_STR).remove('.') + QLatin1String("webengine");


    QLocalSocket socket;
    socket.connectToServer(serverName);
    if(socket.waitForConnected(500)){
	QTextStream	stream(&socket);
	QStringList	args = QCoreApplication::arguments();
	if(args.count() > 1) stream << args.last();
	else stream << QString();
	stream.flush();
	socket.waitForBytesWritten();

	return;
    }
	//    _localserver = new QLocalServer(this);
    connect(_localserver, &QLocalServer::newConnection, this, &sapp_t::newLocalSocketConnection);
    if(! _localserver->listen(serverName)){
	if(  _localserver->serverError() == QAbstractSocket::AddressInUseError
	  && QFile::exists(_localserver->serverName())){
	    QFile::remove(_localserver->serverName());
	    _localserver->listen(serverName);
	}
    }
#if defined(Q_OS_OSX)
    QApplication::setQuitOnLastWindowClosed(false);
#else
    QApplication::setQuitOnLastWindowClosed(true);
#endif

#ifndef QT_NO_OPENSSL
    if(! QSslSocket::supportsSsl()){
	QMessageBox::information(0, _globalparameters.main_program_full_file()
				, "This system does not support OpenSSL. SSL websites will not be available.");
    }
#endif

	//    actWin = 0;
	//    peer = new QtLocalPeer(this, appId);
    connect(_peer, &QtLocalPeer::messageReceived, &QtLocalPeer::messageReceived);
}

void sapp_t::browser_init(){
    auto conf_location = _globalparameters.root_path() + "/" + _globalparameters.target_os();
    if(! QDir(conf_location).exists()) if(! QDir::root().mkpath(conf_location)) critical_error("void sapp_t::browser_init() can not make path " + conf_location + " for browser.ini");
    auto	brower_conf_file_name = conf_location + "/" + _globalparameters._browser_conf_filename;
    QFileInfo	check_file(brower_conf_file_name);
    if(! (check_file.exists() && check_file.isFile())){
	// Файл перемещается в корзину
	if(! QFile::copy(QString(":/resource/standardconfig/") + _globalparameters.target_os() + "/" + _globalparameters._browser_conf_filename, brower_conf_file_name)) throw std::runtime_error("Can not copy \"" + _globalparameters._browser_conf_filename.toStdString() + "\"");
	// if(! file.open(QIODevice::WriteOnly))throw std::runtime_error("Can not open browser.conf");
	else QFile::setPermissions(brower_conf_file_name, QFile::ReadUser | QFile::WriteUser);
	//        critical_error("Can not remove file\n" + fileNameFrom + "\nto reserve file\n" + fileNameTo);
    }
    QDesktopServices::setUrlHandler(QLatin1String("http"), this, "openUrl");
    QString localSysName = QLocale::system().name();

    installTranslator(QLatin1String("qt_") + localSysName);


	// Подключение перевода интерфейса
	// QString langFileName=globalParameters.getWorkDirectory()+"/resource/translations/hapnote_"+hapnoteconfig.get_interfacelanguage()+".qm";
    QString langFileName = ":/resource/translations/hapnote_" + _appconfig.interface_language() + ".qm";
    qDebug() << "Use language file " << langFileName;

	// QTranslator langTranslator;
	// langTranslator.load(langFileName);
    installTranslator(langFileName);	// &langTranslator


    QSettings settings(_globalparameters.root_path() + "/" + _globalparameters.target_os() + "/" + _globalparameters._browser_conf_filename, QSettings::IniFormat);
    settings.beginGroup(QLatin1String("sessions"));
    _last_session = settings.value(QLatin1String("lastSession")).toByteArray();
    settings.endGroup();

#if defined(Q_OS_OSX)
    connect(this, &QtSingleApplication::lastWindowClosed, this, &QtSingleApplication::lastWindowClosed);
#endif

    QTimer::singleShot(0, this, &sapp_t::postLaunch);
    if(canRestoreSession()) restoreLastSession();
	// }
}

#ifdef USE_QTM
void sapp_t::qtm_init(){
    qDebug() << "setting up application";
    if(arguments().contains("--sandbox") || arguments().contains("--delete-sandbox")){
	setOrganizationName("Catkin Project Sandbox");
	setOrganizationDomain("qtm-sandbox.blogistan.co.uk");
	setApplicationName("QTMsandbox");
	setApplicationVersion(QTM_VERSION);
	_isSandbox = true;
    }else{
	setOrganizationName("Catkin Project");
	setOrganizationDomain("catkin.blogistan.co.uk");
	setApplicationName("QTM");
	setApplicationVersion(QTM_VERSION);
	_isSandbox = false;
    }
    _currentEditingWindow = 0;

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(saveRecentFiles()));
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(handleLastWindowClosed()));
}
#endif

void sapp_t::main_window(){
	// Do not run another copy    // Не запущен ли другой экземпляр
    if(isRunning()){
	QString message = "Another hapnote exemplar is running.\n";

	printf("%s", message.toStdString().c_str());	// message.toLocal8Bit()

	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setText(message);
	msgBox.exec();

	exit(0);
    }
	// Установка CSS-оформления
    _style = set_css_style();

	// Экран загрузки, показывается только в Андроид версии (так как загрузка идет ~10 сек, и без сплешскрина непонятно что происходит)
    QSplashScreen splash(QPixmap(":/resource/pic/hapnote_splash.png"));
    if(_appconfig.show_splash_screen()) splash.show();
	//    // Подключение перевода интерфейса
	//    // QString langFileName=globalParameters.getWorkDirectory()+"/resource/translations/hapnote_"+hapnoteconfig.get_interfacelanguage()+".qm";
	//    QString langFileName = ":/resource/translations/hapnote_" + hapnoteConfig.get_interfacelanguage() + ".qm";
	//    qDebug() << "Use language file " << langFileName;

	//    //QTranslator langTranslator;
	//    //langTranslator.load(langFileName);
	//    application.installTranslator(
	//        langFileName    //&langTranslator
	//    );


	// Создание объекта главного окна
    _window = new wn_t(_globalparameters, _appconfig, _databaseconfig, _profile, _style);	// std::make_shared<MainWindow>(_globalparameters, _appconfig, _databaseconfig);

    _globalparameters.main_window(_window);

    _window->setWindowTitle(_globalparameters.application_name());
    if(_globalparameters.target_os() == "android") _window->show();																																																																																																																																																																																																																																																																																																																																																																		// В Андроиде нет десктопа, на нем нельзя сворачивать окно
    else{
	if(_appconfig.run_in_minimized_window() == false) _window->show();
	else _window->hide();
    }
	// win->setObjectName("mainwindow");
	// pMainWindow=&win; // Запоминается указатель на основное окно

	// После создания окна восстанавливается вид окна в предыдущий запуск
	// Эти действия нельзя делать в конструкторе главного окна,
	// т.к. окно еще не создано
    _globalparameters.window_switcher()->disable();
    _window->restore_find_in_base_visible();
    _window->restore_geometry();
    _window->restore_tree_position();
	//    _window->restore_recordtable_position();
    _window->restore_editor_cursor_position();
    _window->restore_editor_scrollbar_position();
    _globalparameters.window_switcher()->enable();
    if(_appconfig.interface_mode() == "mobile") _globalparameters.window_switcher()->restore_focus_widget();
    qDebug() << "Restore session succesfull";

	// После восстановления последней редактируемой записи
	// история перехода очищается, так как в не может попасть
	// первая запись в востаналиваемой ветке и сама восстанавливаемая запись
    walkhistory.clear();
	// Если в конфиге настроено, что нужно синхронизироваться при старте
	// И задана команда синхронизации
    if(_appconfig.synchro_on_startup())
		if(_appconfig.synchro_command().trimmed().length() > 0) _window->synchronization();
	// Если настроено в конфиге, сразу запрашивается пароль доступа
	// к зашифрованным данным
	// И если есть хоть какие-то зашифрованные данные
    if(_appconfig.howpassrequest() == "atStartProgram")
		if(_globalparameters.crypt_key().length() == 0)
			if(_databaseconfig.get_crypt_mode() > 0){
				// Запрашивается пароль только в том случае, если ветка,
				// на которую установливается курсор при старте, незашифрована
				// Если ветка зашифрована, пароль и так будет запрошен автоматически
			    if(_window->is_tree_position_crypt() == false){
				Password password;
				password.retrievePassword();
			    }
			}
	// Если в общем конфиге стоит опция хранения пароля
	// И хранимый пароль (точнее его хеш) заполнен
    if(_globalparameters.crypt_key().length() == 0){
	if(_databaseconfig.get_crypt_mode() > 0){
	    if(_appconfig.password_save_flag()){
		if(_appconfig.password_middle_hash().length() > 0){
			// При запросе пароля ключ шифрования будет восстановлен автоматически
		    Password password;
		    password.retrievePassword();
		}
	    }
	}
    }
	// Распечатывается дерево сгенерированных объектов
	// print_object_tree();

	// Проверяется наличие системного трея
	/*
	   if(!QSystemTrayIcon::isSystemTrayAvailable()) {
	   QMessageBox::critical(0, QObject::tr("Systray"),
	   QObject::tr("I couldn't detect any system tray on this system."));
	   exit(1);
	   }
	 */

	// При закрытии окна не выходить из программы.
	// Окно программы может быть снова открыто из трея
    QApplication::setQuitOnLastWindowClosed(false);


	// win->show();
    connect(this, &sapp_t::lastWindowClosed, this, &sapp_t::quit);
	// app.connect(&app, SIGNAL(app.commitDataRequest(QSessionManager)), SLOT(win->commitData(QSessionManager)));
	// Окно сплеш-скрина скрывается
    if(_appconfig.show_splash_screen()) splash.finish(_window);
}

/*!
    Creates a QtSingleApplication object. The application identifier
    will be QCoreApplication::applicationFilePath(). \a argc, \a
    argv, and \a GUIenabled are passed on to the QAppliation constructor.

    If you are creating a console application (i.e. setting \a
    GUIenabled to false), you may consider using
    QtSingleCoreApplication instead.
 */

sapp_t::sapp_t(int &argc
	      , char * *argv
	      , gl_para    &globalparameters
	      , AppConfig           &appconfig
	      , DataBaseConfig      &databaseconfig
	      , bool GUIenabled)
    : QApplication(argc, argv)
      , _peer(new QtLocalPeer(this, QString()))
      , _act_window(nullptr)
      , _gui_enabled(GUIenabled)
      , _profile(new browser::Profile(profile_storage_name, this))
      , _localserver(new QLocalServer(this))
      , _private_profile(nullptr)
      , _private_browsing(false)
      , _globalparameters(globalparameters)
      , _appconfig(appconfig)
      , _databaseconfig(databaseconfig){
    sys_init(argv);

	//    _app = new ThreadedApplication(argc, argv, GUIenabled);
	//    boost::thread appThread(boost::ref(*_app));
	//    _app->waitForApplicationRun();
    main_window();

    browser_init();

#ifdef USE_QTM
    qtm_init();
#endif
}

/*!
    Creates a QtSingleApplication object with the application
    identifier \a appId. \a argc and \a argv are passed on to the
    QAppliation constructor.
 */

sapp_t::sapp_t(const QString &appId
	      , int &argc
	      , char * *argv
	      , gl_para    &globalparameters
	      , AppConfig           &appconfig
	      , DataBaseConfig      &databaseconfig)
    : QApplication(argc, argv)
      , _peer(new QtLocalPeer(this, appId))
      , _act_window(nullptr)
      , _gui_enabled(true)
      , _profile(new browser::Profile(profile_storage_name, this))
      , _localserver(new QLocalServer(this))
      , _private_profile(nullptr)
      , _private_browsing(false)
      , _globalparameters(globalparameters)
      , _appconfig(appconfig)
      , _databaseconfig(databaseconfig){
    sys_init(argv);
    main_window();
    browser_init();
#ifdef USE_QTM
    qtm_init();
#endif
}

/*!
    Creates a QtSingleApplication object. The application identifier
    will be QCoreApplication::applicationFilePath(). \a argc, \a
    argv, and \a type are passed on to the QAppliation constructor.
 */

// Comment by Xi
/*
   QtSingleApplication::QtSingleApplication(int &argc, char **argv, Type type)
    : QApplication(argc, argv, type)
   {
    sysInit();
   }
 */


#if defined(Q_WS_X11)
/*!
   Special constructor for X11, ref. the documentation of
   QApplication's corresponding constructor. The application identifier
   will be QCoreApplication::applicationFilePath(). \a dpy, \a visual,
   and \a cmap are passed on to the QApplication constructor.
 */
QtSingleApplication::QtSingleApplication(Display *dpy, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, visual, cmap){
    sysInit();
}

/*!
   Special constructor for X11, ref. the documentation of
   QApplication's corresponding constructor. The application identifier
   will be QCoreApplication::applicationFilePath(). \a dpy, \a argc, \a
   argv, \a visual, and \a cmap are passed on to the QApplication
   constructor.
 */
QtSingleApplication::QtSingleApplication(Display *dpy, int &argc, char * *argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap){
    sysInit();
}

/*!
   Special constructor for X11, ref. the documentation of
   QApplication's corresponding constructor. The application identifier
   will be \a appId. \a dpy, \a argc, \a
   argv, \a visual, and \a cmap are passed on to the QApplication
   constructor.
 */
QtSingleApplication::QtSingleApplication(Display *dpy, const QString &appId, int argc, char * *argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap){
    sysInit(appId);
}

#endif


/*!
    Returns true if another instance of this application is running;
    otherwise false.

    This function does not find instances of this application that are
    being run by a different user (on Windows: that are running in
    another session).

    \sa sendMessage()
 */

bool sapp_t::isRunning(){
    return _peer->isClient();
}

/*!
    Tries to send the text \a message to the currently running
    instance. The QtSingleApplication object in the running instance
    will emit the messageReceived() signal when it receives the
    message.

    This function returns true if the message has been sent to, and
    processed by, the current instance. If there is no instance
    currently running, or if the running instance fails to process the
    message within \a timeout milliseconds, this function return false.

    \sa isRunning(), messageReceived()
 */
bool sapp_t::sendMessage(const QString &message, int timeout){
    return _peer->sendMessage(message, timeout);
}

/*!
    Returns the application identifier. Two processes with the same
    identifier will be regarded as instances of the same application.
 */
QString sapp_t::id() const {
    return _peer->applicationId();
}

/*!
   Sets the activation window of this application to \a aw. The
   activation window is the widget that will be activated by
   activateWindow(). This is typically the application's main window.

   If \a activateOnMessage is true (the default), the window will be
   activated automatically every time a message is received, just prior
   to the messageReceived() signal being emitted.

   \sa activateWindow(), messageReceived()
 */

void sapp_t::setActivationWindow(QWidget *aw, bool activateOnMessage){
    _act_window = aw;
    if(activateOnMessage) connect(_peer, &QtLocalPeer::messageReceived, this, &sapp_t::activateWindow);
    else disconnect(_peer, &QtLocalPeer::messageReceived, this, &sapp_t::activateWindow);
}

/*!
    Returns the applications activation window if one has been set by
    calling setActivationWindow(), otherwise returns 0.

    \sa setActivationWindow()
 */
QWidget *sapp_t::activationWindow() const {
    return _act_window;
}

/*!
   De-minimizes, raises, and activates this application's activation window.
   This function does nothing if no activation window has been set.

   This is a convenience function to show the user that this
   application instance has been activated when he has tried to start
   another instance.

   This function should typically be called in response to the
   messageReceived() signal. By default, that will happen
   automatically, if an activation window has been set.

   \sa setActivationWindow(), messageReceived(), initialize()
 */
void sapp_t::activateWindow(){
    if(_act_window){
	_act_window->setWindowState(_act_window->windowState() & ~ Qt::WindowMinimized);
	_act_window->raise();
	_act_window->activateWindow();
    }
}

/*!
    \fn void QtSingleApplication::messageReceived(const QString& message)

    This signal is emitted when the current instance receives a \a
    message from another instance of this application.

    \sa sendMessage(), setActivationWindow(), activateWindow()
 */


/*!
    \fn void QtSingleApplication::initialize(bool dummy = true)

    \obsolete
 */


sapp_t *sapp_t::instance(){
    return static_cast<sapp_t *>(QCoreApplication::instance());
}

void sapp_t::newLocalSocketConnection(){
    QLocalSocket *socket = _peer->server()->nextPendingConnection();
    if(! socket) return;
    socket->waitForReadyRead(1000);
    QTextStream stream(socket);
    QString	_url;
    stream >> _url;
    if(_url.isEmpty()) _url = browser::Browser::_defaulthome;																																																																																																																																																																																																																																																																																																																																	//    browser::DockedWindow *w = nullptr;

	// if(!url.isEmpty()) {

    QSettings settings(_globalparameters.root_path() + "/" + _globalparameters.target_os() + "/" + _globalparameters._browser_conf_filename, QSettings::IniFormat);
    settings.beginGroup(QLatin1String("general"));
    int openLinksIn = settings.value(QLatin1String("openLinksIn"), 0).toInt();
    settings.endGroup();

    auto	entrance	= _globalparameters.entrance();
    auto	tree_screen	= _globalparameters.tree_screen();
    auto	tree_view	= tree_screen->view();
//    boost::intrusive_ptr<TreeIndex> _tree_modelindex(nullptr);
    auto	current_item	= tree_view->current_item();
    auto	parent		= current_item->parent();
    if(! parent) throw std::runtime_error(formatter() << "! parent");																																																																																																																																																																																																																																																																																																																																																																									// std::exception();
//    try {
//        _tree_modelindex = new TreeIndex([&] {return tree_view->source_model(); }, parent, parent->sibling_order([&] (boost::intrusive_ptr<const Linker> il) {
//            return il == current_item->linker() && il->host() == current_item && parent == il->host_parent();
//        }));
//    } catch(std::exception &e) {} //    Record *record = request_record(url);
//    //    std::pair<browser::Browser *, browser::WebView *> dp;
    if(entrance && tree_screen){	// && _tree_modelindex
	if(openLinksIn == 1){
	    auto browser = _globalparameters.main_window()->vtab_record()->new_browser();
//            boost::intrusive_ptr<TreeIndex> tree_index;
//            try {tree_index = new TreeIndex([&] {return tree_view->source_model(); }, tree_view->current_item()->parent(), tree_view->current_item()->linker()->sibling_order()); } catch(std::exception &e) {throw e; }

	    boost::intrusive_ptr<TreeItem> it;

//            if(tree_index)
	    it = TreeIndex::create_treeitem_from_url(_url, std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), [&](boost::intrusive_ptr<const TreeItem> it) -> bool {return url_equal(it->field<home_type>().toStdString(), _url.toStdString()) || url_equal(it->field<url_type>().toStdString(), _url.toStdString());});	// instance([&] {return tree_view->source_model();}, tree_view->current_item())->

//            boost::intrusive_ptr<RecordIndex> record_index(nullptr);

//            try {
	    boost::intrusive_ptr<RecordIndex> record_index = RecordIndex::instance([&] {return browser->record_screen()->record_controller()->source_model();}, it);	// , browser->record_screen()->record_controller()->source_model()->sibling(it)
//            } catch(std::exception &) {}
//            if(record_index){
	    browser->bind(record_index)->activate(std::bind(&HidableTabWidget::find, _globalparameters.main_window()->vtab_record(), std::placeholders::_1));	// tabmanager()->newTab(tree_view->session_root_item()->item_direct(0), it);
//            }
//            else{
//                tree_view->index_invoke(tree_view->source_model()->index(it));
//            }
	}else{
//            boost::intrusive_ptr<TreeIndex> tree_index;
//            try {tree_index = new TreeIndex([&] {return tree_view->source_model(); }, tree_view->current_item()->parent(), tree_view->current_item()->linker()->sibling_order()); } catch(std::exception &e) {throw e; }

//            if(tree_index)
	    TreeIndex::activate([&] {return tree_view->source_model();}
			       , tree_view->current_item()
			       , _url
			       , std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
			       , [&](boost::intrusive_ptr<const TreeItem> it) -> bool {return url_equal(it->field<home_type>().toStdString(), _url.toStdString()) || url_equal(it->field<url_type>().toStdString(), _url.toStdString());}
		)->activate(std::bind(&HidableTabWidget::find, _globalparameters.main_window()->vtab_record(), std::placeholders::_1));
	}
	// browser_view->mainWindow()->load_record(record);
    }
	//    record->active_immediately(true);

	// }

    delete socket;
	//    dp.first->raise();
	//    dp.first->activateWindow();
}

sapp_t::~sapp_t(){
//    if(_profile){
//	_profile->deleteLater();_profile = nullptr;
//    }
	// delete
    _downloadmanager->deleteLater();

	//    for(int i = 0; i < _mainWindows.size(); ++i) {
	//        BrowserWindow *window = _mainWindows.at(i);
	//        delete window;
	//    }

	// delete
    _networkaccessmanager->deleteLater();
	// delete
    _bookmarksmanager->deleteLater();
    if(_window){
	_window->deleteLater();	// delete _window;
	_window = nullptr;
    }
    if(_profile){_profile->deleteLater();_profile = nullptr;}
}

// #if defined(Q_OS_OSX)
// void QtSingleApplication::lastWindowClosed()
// {
//    clean();
//    BrowserWindow *mw = globalParameters.getBrowserView()->getBrowserWindow();  //new BrowserWindow;
//    mw->slotHome();
//    _mainwindows.prepend(mw);
// }
// #endif

// QtSingleApplication *QtSingleApplication::instance()
// {
//    return (static_cast<QtSingleApplication *>(QCoreApplication::instance()));
// }

#if defined(Q_OS_OSX)
#include <QtWidgets/QMessageBox>
void QtSingleApplication::quitBrowser(){
    clean();
    int tabCount = 0;
    for(int i = 0; i < _mainwindows.count(); ++ i) tabCount += _mainwindows.at(i)->tabWidget()->count();
    if(tabCount > 1){
	int ret = QMessageBox::warning(mainWindow(), QString()
				      ,	tr("There are %1 windows and %2 tabs open\n"
					   "Do you want to quit anyway?").arg(_mainwindows.count()).arg(tabCount)
				      ,	QMessageBox::Yes | QMessageBox::No
				      ,	QMessageBox::No);
	if(ret == QMessageBox::No) return;
    }
    exit(0);
}

#endif

/*!
    Any actions that can be delayed until the window is visible
 */
void sapp_t::postLaunch(){
    QString directory = _globalparameters.root_path();	// QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if(directory.isEmpty()) directory = QDir::homePath() + QLatin1String("/.") + QCoreApplication::applicationName();
#if defined(QWEBENGINESETTINGS_PATHS)
    QWebEngineSettings::setIconDatabasePath(directory);
    QWebEngineSettings::setOfflineStoragePath(directory);
#endif

    setWindowIcon(QIcon(QLatin1String(":browser.svg")));

    loadSettings();
	// newMainWindow() needs to be called in main() for this to happen
    if(_globalparameters.main_window()->vtab_record()->record_screens().size() > 0){	// _mainWindows.count()
	QStringList args = QCoreApplication::arguments();
//        browser::Browser *browser = _globalparameters.entrance()->activated_browser();
	//        assert(browser);
	//        if(!browser) {
	//            std::pair<browser::Browser *, browser::WebView *> dp = _globalparameters.entrance()->new_browser(QUrl(browser::Browser::_defaulthome));
	//            browser = dp.first;
	//        }
	if(args.count() > 1){
//            browser->loadPage(args.last());			// mainWindow()->loadPage(args.last());
	    tv_t	*tree_view	= _globalparameters.tree_screen()->view();
	    auto	it		= tree_view->session_root_auto();
	    TreeIndex::activate([&] {return tree_view->source_model();}
			       , it
			       , args.last()
			       , std::bind(&tv_t::move, tree_view, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), [&](boost::intrusive_ptr<const TreeItem> it_) -> bool {return url_equal(it_->field<home_type>().toStdString(), args.last().toStdString()) || url_equal(it_->field<url_type>().toStdString(), args.last().toStdString());}
		);
	}
	//        else
	//            browser->slotHome(); // mainWindow()->slotHome();
    }
    sapp_t::historyManager();
}

void sapp_t::loadSettings(){
    QSettings settings(_globalparameters.root_path() + "/" + _globalparameters.target_os() + "/" + _globalparameters._browser_conf_filename, QSettings::IniFormat);

    settings.beginGroup(QLatin1String("websettings"));

    QWebEngineSettings *defaultSettings = QWebEngineSettings::globalSettings();
	//    QWebEngineProfile *defaultProfile = QWebEngineProfile::defaultProfile();

    QString	standardFontFamily	= defaultSettings->fontFamily(QWebEngineSettings::StandardFont);
    int		standardFontSize	= defaultSettings->fontSize(QWebEngineSettings::DefaultFontSize);
    QFont	standardFont		= QFont(standardFontFamily, standardFontSize);
    standardFont = qvariant_cast<QFont>(settings.value(QLatin1String("standardFont"), standardFont));
    defaultSettings->setFontFamily(QWebEngineSettings::StandardFont, standardFont.family());
    defaultSettings->setFontSize(QWebEngineSettings::DefaultFontSize, standardFont.pointSize());

    QString	fixedFontFamily = defaultSettings->fontFamily(QWebEngineSettings::FixedFont);
    int		fixedFontSize	= defaultSettings->fontSize(QWebEngineSettings::DefaultFixedFontSize);
    QFont	fixedFont	= QFont(fixedFontFamily, fixedFontSize);
    fixedFont = qvariant_cast<QFont>(settings.value(QLatin1String("fixedFont"), fixedFont));
    defaultSettings->setFontFamily(QWebEngineSettings::FixedFont, fixedFont.family());
    defaultSettings->setFontSize(QWebEngineSettings::DefaultFixedFontSize, fixedFont.pointSize());

    defaultSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, settings.value(QLatin1String("enableJavascript"), true).toBool());
    defaultSettings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, settings.value(QLatin1String("enableScrollAnimator"), true).toBool());

	// #if defined(QTWEBENGINE_PLUGINS)
    defaultSettings->setAttribute(QWebEngineSettings::PluginsEnabled, settings.value(QLatin1String("enablePlugins"), true).toBool());
	// #endif
    defaultSettings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

    QString css = settings.value(QLatin1String("userStyleSheet")).toString();
    if(css == "" && _style != "") css = _style;
    else css = instance()->styleSheet();
    set_user_style_sheet(_profile, css, _globalparameters.entrance());	// ->main_window(register_record(QUrl(browser::DockedWindow::_defaulthome)))


    _profile->setHttpUserAgent(settings.value(QLatin1String("httpUserAgent")).toString());
    settings.endGroup();


	// this group lead chrome_odthread deadlock? or initialize failure
    settings.beginGroup(QLatin1String("cookies"));

    _profile->setHttpCacheType(QWebEngineProfile::HttpCacheType::DiskHttpCache);
    _profile->setPersistentCookiesPolicy(QWebEngineProfile::PersistentCookiesPolicy::ForcePersistentCookies);	// AllowPersistentCookies

    QWebEngineProfile::PersistentCookiesPolicy persistentCookiesPolicy = QWebEngineProfile::PersistentCookiesPolicy(
	settings.value(QLatin1String("persistentCookiesPolicy")).toInt()		// QWebEngineProfile::ForcePersistentCookies  //vQWebEngineProfile::AllowPersistentCookies   //
	);
    _profile->setPersistentCookiesPolicy(persistentCookiesPolicy);
    QString	persistent_data_path = settings.value(QLatin1String("persistentDataPath")).toString();
    QDir	cookie_path(persistent_data_path);
    if(! cookie_path.exists()){
	//        QDir dir;
	cookie_path.mkpath(persistent_data_path);
    }
    _profile->setPersistentStoragePath(persistent_data_path);

    settings.endGroup();


    settings.beginGroup(QLatin1String("proxy"));
    QNetworkProxy proxy;
    if(settings.value(QLatin1String("enabled"), false).toBool()){
	if(settings.value(QLatin1String("type"), 0).toInt() == 0) proxy = QNetworkProxy::Socks5Proxy;
	else proxy = QNetworkProxy::HttpProxy;
	proxy.setHostName(settings.value(QLatin1String("hostName")).toString());
	proxy.setPort(settings.value(QLatin1String("port"), 1080).toInt());
	proxy.setUser(settings.value(QLatin1String("userName")).toString());
	proxy.setPassword(settings.value(QLatin1String("password")).toString());
    }
    QNetworkProxy::setApplicationProxy(proxy);
    settings.endGroup();
}

// QList<BrowserWindow *> QtSingleApplication::mainWindows()
// {
//    clean();
//    QList<BrowserWindow *> list;

//    for(int i = 0; i < _mainWindows.count(); ++i)
//        list.append(_mainWindows.at(i));

//    return list;
// }

// void QtSingleApplication::clean()
// {
//    // cleanup any deleted main windows first
//    for(int i = _mainWindows.count() - 1; i >= 0; --i)
//        if(_mainWindows.at(i).isNull())
//            _mainWindows.removeAt(i);
// }

void sapp_t::saveSession(){
    if(_private_browsing) return;
	//    globalparameters.entrance()->clean();

    std::shared_ptr<QSettings> settings = std::make_shared<QSettings>(_globalparameters.root_path() + "/" + _globalparameters.target_os() + "/" + _globalparameters._browser_conf_filename, QSettings::IniFormat);
    settings->beginGroup(QLatin1String("sessions"));

    QByteArray	data;
    QBuffer	buffer(&data);
    QDataStream stream(&buffer);
    buffer.open(QIODevice::ReadWrite);

    auto	_browsers	= [&] {set<browser::Browser *> bs;for(auto rs : _globalparameters.main_window()->vtab_record()->record_screens()) bs.insert(rs->browser());return bs;} ();
    uint	count_win	= static_cast<uint>(_browsers.size());;
    stream << count_win;// static_cast<uint>(_browsers.size());
    for(auto &browser : _browsers) stream << browser->save_state();
    settings->setValue(QLatin1String("lastSession"), data);
    settings->endGroup();
}

bool sapp_t::canRestoreSession() const {
    return ! _last_session.isEmpty();
}

void sapp_t::restoreLastSession(){
    QList<QByteArray>	historywindows;
    QBuffer		buffer(&_last_session);
    QDataStream		stream(&buffer);
    buffer.open(QIODevice::ReadOnly);
    uint windowCount;
    stream >> windowCount;
    for(uint i = 0; i < windowCount; ++ i){
	QByteArray windowState;
	stream >> windowState;
	historywindows.append(windowState);
    }
	//    QList<QPointer<browser::Browser > > opened_windows = globalparameters.entrance()->browsers();
	//    if(opened_windows.count() == 0) {
	//        globalparameters.entrance()->new_browser(
	//            QUrl(browser::Browser::_defaulthome)
	//        );
	//    }
    for(int i = 0; i < historywindows.count(); ++ i){
//	//        browser::BrowserWindow *newWindow = 0;
//	//        QList<QPointer<browser::DockedWindow > > opened_windows = globalparameters.entrance()->window_list();
//	browser::Browser *browser = globalparameters.main_window()->vtab_record()->activated_browser();
////	assert(browser->currentTab()->page()->url() == QUrl() || browser->currentTab()->page()->url() == QUrl(browser::Browser::_defaulthome));
	if(  0 == i
	  && _globalparameters.main_window()->vtab_record()->record_screens().size() == 1
//	  && browser->tabWidget()->count() == 1
//	  && browser->currentTab()->page()->url() == QUrl(browser::Browser::_defaulthome)	// ?
	    ){
//		// newWindow = globalParameters.browsermanager()->main_window();
//	    globalparameters.entrance()->restore_state(historywindows.at(i));
	    _globalparameters.main_window()->vtab_record()->activated_browser()->restore_state(historywindows.at(i));
	}else{
		// newWindow =
	    _globalparameters.main_window()->vtab_record()->new_browser()->restore_state(historywindows.at(i));
	}
	// newWindow->restoreState(windows.at(i));
    }
}

// bool QtSingleApplication::isTheOnlyBrowser() const
// {
//    return (_localserver != 0);
// }

void sapp_t::installTranslator(const QString &name){
    QTranslator *translator = new QTranslator(this);

    translator->load(name, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication::installTranslator(translator);
}

#if defined(Q_OS_OSX)
bool QtSingleApplication::event(QEvent *event){
    switch(event->type()){
	case QEvent::ApplicationActivate: {
	    clean();
	    if(! _mainwindows.isEmpty()){
		BrowserWindow *mw = mainWindow();
		if(mw && ! mw->isMinimized()) mainWindow()->show();
		return true;
	    }
	}

	case QEvent::FileOpen:
	    if(! _mainwindows.isEmpty()){
		mainWindow()->loadPage(static_cast<QFileOpenEvent *>(event)->file());

		return true;
	    }
	default:
	    break;
    }
    return QApplication::event(event);
}

#endif

// void QtSingleApplication::openUrl(const QUrl &url)
// {
//    mainWindow()->loadPage(url.toString());
// }

// BrowserWindow *QtSingleApplication::newMainWindow()
// {
//    //BrowserView *browser_view = globalParameters.getBrowserView();
//    //BrowserWindow *browser = nullptr;

//    //if(browser_view) {
//    //        browser = browser_view->getBrowserWindow();     //
//    BrowserWindow *browser = new BrowserWindow();
//    //        if(!browser) {
//    //            browser = new BrowserWindow(browser_view);  //incomplete
//    //        }
//    _mainWindows.prepend(browser);
//    //browser->show();
//    //}

//    //this->exec();
//    return browser;
// }

// BrowserWindow *QtSingleApplication::mainWindow()
// {
//    clean();

//    if(_mainWindows.isEmpty()) {
//        //        BrowserView *browser_view = globalParameters.getBrowserView();
//        //        BrowserWindow *browser = nullptr;

//        //        if(browser_view) {
//        //            browser = browser_view->getBrowserWindow();

//        //            if(nullptr == browser) {
//        //                browser = new BrowserWindow(browser_view);
//        //            }
//        //        }

//        //        _mainwindows.prepend(browser);
//        newMainWindow();
//    }

//    return _mainWindows[0];
// }

// void QtSingleApplication::newLocalSocketConnection()
// {
//    QLocalSocket *socket = _localserver->nextPendingConnection();

//    if(!socket)
//        return;

//    socket->waitForReadyRead(1000);
//    QTextStream stream(socket);
//    QString url;
//    stream >> url;

//    if(!url.isEmpty()) {
//        QSettings settings;
//        settings.beginGroup(QLatin1String("general"));
//        int openLinksIn = settings.value(QLatin1String("openLinksIn"), 0).toInt();
//        settings.endGroup();

//        if(openLinksIn == 1) {
//            //globalParameters.getBrowserView()->getBrowserWindow();  //newMainWindow();
//        } else
//            mainWindow()->tabWidget()->newTab();

//        openUrl(url);
//    }

//    delete socket;
//    mainWindow()->raise();
//    mainWindow()->activateWindow();
// }

browser::CookieJar *sapp_t::cookieJar(){
#if defined(QWEBENGINEPAGE_SETNETWORKACCESSMANAGER)

    return (browser::CookieJar *) networkAccessManager()->cookieJar();
#else

    return 0;
#endif
}

browser::DownloadManager *sapp_t::request_download_manager(){
	//    if(!_downloadmanager) {
	//        _downloadmanager = new browser::DownloadManager();
	//    }

	//    return _downloadmanager;

    return globalparameters.request_download_manager();
}

QNetworkAccessManager *sapp_t::networkAccessManager(){
	// #if defined(QWEBENGINEPAGE_SETNETWORKACCESSMANAGER)
	//    if(!s_networkAccessManager) {
	//        s_networkAccessManager = new browser::NetworkAccessManager();
	//        s_networkAccessManager->setCookieJar(new browser::CookieJar);
	//    }
	//    return s_networkAccessManager;
	// #else
	//    if(!_networkaccessmanager) {
	//        _networkaccessmanager = new QNetworkAccessManager();
	//    }
	//    return _networkaccessmanager;
	// #endif
    if(! _networkaccessmanager){
	_networkaccessmanager = new QNetworkAccessManager();
	connect(_networkaccessmanager, &QNetworkAccessManager::authenticationRequired
	       , sapp_t::instance(), &sapp_t::authenticationRequired);
	connect(_networkaccessmanager, &QNetworkAccessManager::proxyAuthenticationRequired
	       , sapp_t::instance(), &sapp_t::proxyAuthenticationRequired);
    }
    return _networkaccessmanager;
}

browser::HistoryManager *sapp_t::historyManager(){
    if(! _historymanager) _historymanager = new browser::HistoryManager();
    return _historymanager;
}

browser::BookmarksManager *sapp_t::bookmarksManager(){
    if(! _bookmarksmanager) _bookmarksmanager = new browser::BookmarksManager();
    return _bookmarksmanager;
}

QIcon sapp_t::icon(const QUrl &url) const {
#if defined(QTWEBENGINE_ICONDATABASE)
    QIcon icon = QWebEngineSettings::iconForUrl(url);
    if(! icon.isNull()) return icon.pixmap(16, 16);
#else
    Q_UNUSED(url);
#endif

    return defaultIcon();
}

QIcon sapp_t::defaultIcon() const {
    if(_default_icon.isNull()) _default_icon = QIcon(QLatin1String(":defaulticon.png"));
    return _default_icon;
}

void sapp_t::setPrivateBrowsing(bool privateBrowsing){
    if(_private_browsing == privateBrowsing) return;
    _private_browsing = privateBrowsing;
    auto browsers = [&] {set<browser::Browser *> bs;for(auto rs : _globalparameters.main_window()->vtab_record()->record_screens()) bs.insert(rs->browser());return bs;} ();
    if(privateBrowsing){
	if(! _private_profile) _private_profile = new browser::Profile(profile_storage_name, this);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																// new QWebEngineProfile(this);
	for(auto &browser : browsers) browser->tabWidget()->setProfile(_private_profile);
    }else{
	for(auto &browser : browsers){
	    browser->tabWidget()->setProfile(_profile	// QWebEngineProfile::defaultProfile()
		);
	    browser->lastsearch() = QString::null;
	    browser->tabWidget()->clear();
	}
    }
    emit privateBrowsingChanged(privateBrowsing);
}

// TODO: Remove these functions (QTBUG-47967)
QByteArray sapp_t::authenticationKey(const QUrl &url, const QString &realm){
    QUrl copy = url;

    copy.setFragment(realm);

    return "auth:" + copy.toEncoded(QUrl::RemovePassword | QUrl::RemovePath | QUrl::RemoveQuery);
}

QByteArray sapp_t::proxyAuthenticationKey(const QNetworkProxy &proxy, const QString &realm){
    QString host = QString("%1:%2").arg(proxy.hostName()).arg(proxy.port());

    return sapp_t::proxyAuthenticationKey(proxy.user(), host, realm);
}

QByteArray sapp_t::proxyAuthenticationKey(const QString &user, const QString &host, const QString &realm){
    QUrl key;

    key.setScheme(QLatin1String("proxy-http"));
    key.setUserName(user);
    key.setHost(host);
    key.setFragment(realm);

    return "auth:" + key.toEncoded();
}

void sapp_t::setLastAuthenticator(QAuthenticator *authenticator){
    _last_authenticator = QAuthenticator(*authenticator);
}

void sapp_t::setLastProxyAuthenticator(QAuthenticator *authenticator){
    _last_proxy_authenticator = QAuthenticator(*authenticator);
}

void sapp_t::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator){
    if(_last_authenticator.isNull()) return;
    Q_ASSERT(_last_authenticator.option("key").isValid());
    QByteArray	lastKey = _last_authenticator.option("key").toByteArray();
    QByteArray	key	= sapp_t::authenticationKey(reply->url(), authenticator->realm());
    if(lastKey == key) *authenticator = _last_authenticator;
}

void sapp_t::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator){
    if(_last_proxy_authenticator.isNull()) return;
    QNetworkProxy::ProxyType proxyType = proxy.type();
    if(proxyType != QNetworkProxy::HttpProxy || proxyType != QNetworkProxy::HttpCachingProxy) return;
    Q_ASSERT(_last_proxy_authenticator.option("host").isValid());
    QByteArray	lastKey = _last_proxy_authenticator.option("key").toByteArray();
    QByteArray	key	= sapp_t::proxyAuthenticationKey(proxy, authenticator->realm());
    if(lastKey == key) *authenticator = _last_authenticator;
}

#ifdef USE_QTM



// Application::Application(int &argc, char * *argv)
//    : QApplication(argc, argv){
//    qDebug() << "setting up application";
//    if(arguments().contains("--sandbox") || arguments().contains("--delete-sandbox")){
//	setOrganizationName("Catkin Project Sandbox");
//	setOrganizationDomain("qtm-sandbox.blogistan.co.uk");
//	setApplicationName("QTMsandbox");
//	setApplicationVersion(QTM_VERSION);
//	_isSandbox = true;
//    }else{
//	setOrganizationName("Catkin Project");
//	setOrganizationDomain("catkin.blogistan.co.uk");
//	setApplicationName("QTM");
//	setApplicationVersion(QTM_VERSION);
//	_isSandbox = false;
//    }
//    _currentEditingWindow = 0;

//    connect(this, SIGNAL(aboutToQuit()), this, SLOT(saveRecentFiles()));
//    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(handleLastWindowClosed()));
// }

void sapp_t::setupRecentFiles(){
#ifdef Q_OS_MAC
    QString orgString = organizationDomain();
#else
    QString orgString = organizationName();
#endif
    QSettings settings(orgString, applicationName());
	// qDebug() << "Settings path:" << settings.fileName();
    int		i;
    recentFile	currentRF;
    QString	crf;

    settings.beginGroup("recentFiles");
    for(i = 0; i < 20; i ++){
	crf = settings.value(QString("recentFile%1").arg(i), "").toString();
	// qDebug() << "Recent file:" << crf;
	/*    if( crf.isEmpty() )
	      qDebug() << QString( "recentFile%1" ).arg( i ) << "is empty"; */
	currentRF.filename	= crf.section("filename:", 1, 1).section(" ##title:", 0, 0);
	currentRF.title		= crf.section(" ##title:", 1, 1);
	if(currentRF.filename.isEmpty()) break;
	_recentFiles.append(currentRF);
	// qDebug() << "Added one recent file";
    }
}

QStringList sapp_t::titles(){
    int		i;
    QStringList returnValue;
    for(i = 0; i < _recentFiles.count(); ++ i){
	if(_recentFiles.value(i).title.isEmpty()) returnValue << QString();
	else returnValue << _recentFiles.value(i).title;
    }
    return returnValue;
}

QStringList sapp_t::filenames(){
    int		i;
    QStringList returnValue;
    for(i = 0; i < _recentFiles.count(); ++ i){
	if(_recentFiles.value(i).filename.isEmpty()) returnValue << QString();
	else returnValue << _recentFiles.value(i).filename;
    }
    return returnValue;
}

void sapp_t::deleteSandbox(){
	// First remove configuration settings
    QSettings	settings;
    QStringList groupNames;
    groupNames	<< "accounts" << "application" << "fonts" << "geometry"
		<< "highlighting" << "recentFiles" << "sysTrayIcon";
    Q_FOREACH(QString groupName, groupNames){
	settings.beginGroup(groupName);
	settings.remove("");
	settings.endGroup();
    }
	// Now remove entry files
#ifdef Q_OS_WIN
    QString sandbox = QString("%1\\QTMSandbox").arg(QDir::homePath());
#else
    QString sandbox = QString("%1/qtm-sandbox").arg(QDir::homePath());
#endif

    QDir sandboxDir(sandbox);
    sandboxDir.remove("qtmaccounts2.xml");
    sandboxDir.remove("qtmautolinkdict.xml");
    QStringList cqtFilter, cqtFiles;
    cqtFilter << "*.cqt";

    cqtFiles = sandboxDir.entryList(cqtFilter);
    Q_FOREACH(QString cqtFile, cqtFiles) sandboxDir.remove(cqtFile);
}

QList<sapp_t::recentFile> sapp_t::recentFiles(){
    return _recentFiles;
}

sapp_t::recentFile sapp_t::getRecentFile(int index){
    recentFile return_value;
    if(index >= _recentFiles.count()){
	return_value.title	= QString();
	return_value.filename	= QString();
    }else return_value = _recentFiles.at(index);
    return return_value;
}


void sapp_t::setRecentFiles(const QStringList &titles, const QStringList &filenames){
    int			i;
    QList<recentFile>	rfs;
    recentFile		thisFile;
    for(i = 0; i < titles.count() && i < 20; ++ i){
	if(titles.at(i).isEmpty()) thisFile.title = QString();
	else thisFile.title = titles.at(i);
	if(filenames.at(i).isEmpty()) thisFile.filename = QString();
	else thisFile.filename = filenames.at(i);
	rfs << thisFile;
    }
    _recentFiles = rfs;
    emit recentFilesUpdated(titles, filenames);
}

void sapp_t::addRecentFile(const QString &title, const QString &filename){
    recentFile	thisFile;
    int		i;

    thisFile.title	= title;
    thisFile.filename	= filename;
    for(i = 0; i < 20; ++ i){
	if(i == _recentFiles.count()) break;
	if(_recentFiles.value(i).filename == filename) _recentFiles.removeAt(i);
    }
    _recentFiles.prepend(thisFile);

    emit recentFilesUpdated(_recentFiles);
}

void sapp_t::saveAll(){
    EditingWindow	*e;
    QWidgetList		tlw = QApplication::topLevelWidgets();
    Q_FOREACH(QWidget *w, tlw){
	e = qobject_cast<EditingWindow *>(w);
	if(e) e->save();
    }
}

void sapp_t::setMainWindow(EditingWindow *ew){
#ifdef Q_OS_MAC
    emit mainWindowChanged(ew);
#endif
    _currentEditingWindow = ew;
}

void sapp_t::saveRecentFiles(){
    int		i;
    QSettings	settings;

    settings.beginGroup("recentFiles");
    for(i = 0; i < 20; ++ i){
	settings.setValue(QString("recentFile%1").arg(i)
			 , QString("filename:%1 ##title:%2")
	    .arg(_recentFiles.value(i).filename)
	    .arg(_recentFiles.value(i).title));
    }
}

void sapp_t::handleWindowChange(QWidget *oldW, QWidget *newW){
    (void) oldW;
    (void) newW;
#ifdef Q_OS_MAC
    QWidget		*oldWindow	= oldW->window();
    QWidget		*newWindow	= newW->window();
    EditingWindow	*newEW;
    if(oldWindow != newWindow){
	newEW = qobject_cast<EditingWindow *>(newWindow);
	if(newEW) superMenu->setMainWindow(newEW);
	else superMenu->setMainWindow(0);
    }
#endif
}

void sapp_t::handleLastWindowClosed(){
    _currentEditingWindow = 0;
}


#endif
