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




#include "networkaccessmanager.h"

#include "libraries/qt_single_application5/qtsingleapplication.h"

#include "browser.h"

#include <QtCore/QSettings>

#include <QtGui/QDesktopServices>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>
#include <QtGui/QTextDocument>

#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include "libraries/global_parameters.h"
#include "views/browser/entrance.h"
#include "main.h"

extern gl_para globalparameters;


namespace browser {
#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(NetworkAccessManager)
#endif

    NetworkAccessManager::NetworkAccessManager(QObject *parent)
	: QNetworkAccessManager(parent)
	  , requestFinishedCount(0), requestFinishedFromCacheCount(0), requestFinishedPipelinedCount(0)
	  , requestFinishedSecureCount(0), requestFinishedDownloadBufferCount(0){
	connect(this, &NetworkAccessManager::finished, this, &NetworkAccessManager::requestFinished);
#ifndef QT_NO_OPENSSL
	connect(this, &QNetworkAccessManager::sslErrors, this, &NetworkAccessManager::sslErrors);
#endif
	loadSettings();

	QNetworkDiskCache	*diskCache	= new QNetworkDiskCache(this);
	QString			location	= QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	diskCache->setCacheDirectory(location);
	setCache(diskCache);
    }

    QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData){
	QNetworkRequest request = req;	// copy so we can modify
	// this is a temporary hack until we properly use the pipelining flags from QtWebkit
	// pipeline everything! :)
	request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

	return QNetworkAccessManager::createRequest(op, request, outgoingData);
    }

    void NetworkAccessManager::requestFinished(QNetworkReply *reply){
	requestFinishedCount ++;
	if(reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool() == true) requestFinishedFromCacheCount ++;
	if(reply->attribute(QNetworkRequest::HttpPipeliningWasUsedAttribute).toBool() == true) requestFinishedPipelinedCount ++;
	if(reply->attribute(QNetworkRequest::ConnectionEncryptedAttribute).toBool() == true) requestFinishedSecureCount ++;
	if(reply->attribute(QNetworkRequest::DownloadBufferAttribute).isValid() == true) requestFinishedDownloadBufferCount ++;
	if(requestFinishedCount % 10) return;
#ifdef QT_DEBUG
	double	pctCached		= (double (requestFinishedFromCacheCount) * 100.0 / double (requestFinishedCount));
	double	pctPipelined		= (double (requestFinishedPipelinedCount) * 100.0 / double (requestFinishedCount));
	double	pctSecure		= (double (requestFinishedSecureCount) * 100.0 / double (requestFinishedCount));
	double	pctDownloadBuffer	= (double (requestFinishedDownloadBufferCount) * 100.0 / double (requestFinishedCount));

	qDebug("STATS [%lli requests total] [%3.2f%% from cache] [%3.2f%% pipelined] [%3.2f%% SSL/TLS] [%3.2f%% Zerocopy]", requestFinishedCount, pctCached, pctPipelined, pctSecure, pctDownloadBuffer);
#endif
    }

    void NetworkAccessManager::loadSettings(){
	QSettings settings(globalparameters.root_path() + "/" + globalparameters.target_os() + "/" + globalparameters._browser_conf_filename, QSettings::IniFormat);
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
	setProxy(proxy);
    }

#ifndef QT_NO_OPENSSL
    void NetworkAccessManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &error){
	// check if SSL certificate has been trusted already
	QString replyHost = reply->url().host() + QString(":%1").arg(reply->url().port());
	if(! sslTrustedHostList.contains(replyHost)){
	    Entrance *mainWindow = globalparameters.entrance();	// ->main_window(register_record(QUrl(DockedWindow::_defaulthome)));    //QtSingleApplication::instance()->mainWindow();

	    QStringList errorStrings;
	    for(int i = 0; i < error.count(); ++ i) errorStrings += error.at(i).errorString();
	    QString	errors	= errorStrings.join(QLatin1String("\n"));
	    int		ret	= QMessageBox::warning(mainWindow, QCoreApplication::applicationName()
						      , tr("SSL Errors:\n\n%1\n\n%2\n\n"
							   "Do you want to ignore these errors for this host?").arg(reply->url().toString()).arg(errors)
						      , QMessageBox::Yes | QMessageBox::No
						      , QMessageBox::No);
	    if(ret == QMessageBox::Yes){
		reply->ignoreSslErrors();
		sslTrustedHostList.append(replyHost);
	    }
	}
    }

#endif
}


