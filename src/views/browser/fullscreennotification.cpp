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




#include "fullscreennotification.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>



namespace browser {
#if QT_VERSION == 0x050600
    W_OBJECT_IMPL(FullScreenNotification)
#endif


    FullScreenNotification::FullScreenNotification(QWidget *parent)
	: QWidget(parent)
	  , width(400)
	  , height(80)
	  , x((parent->geometry().width() - width) / 2)
	  , y(80){
	setVisible(false);
	setWindowFlags(Qt::ToolTip | Qt::WindowDoesNotAcceptFocus);

	QGridLayout *layout = new QGridLayout(this);

	_label = new QLabel(tr("You are now in fullscreen mode. Press ESC to quit!"), this);
	layout->addWidget(_label, 0, 0, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);

	setGeometry(x, y, width, height);

	setStyleSheet("background-color: white;\
        color: black;");

	_animation = new QPropertyAnimation(this, "windowOpacity");
	connect(_animation, &QPropertyAnimation::finished, this, &FullScreenNotification::fadeOutFinished);
    }

    FullScreenNotification::~FullScreenNotification()
    {}

    void FullScreenNotification::show(){
	setWindowOpacity(1.0);
	QTimer::singleShot(300, [&] {
		QWidget *parent = parentWidget();
		x = (parent->geometry().width() - width) / 2;
		QPoint topLeft = parent->mapToGlobal(QPoint(x, y));
		QWidget::move(topLeft.x(), topLeft.y());
		QWidget::show();
		QWidget::raise();
	    });
	QTimer::singleShot(5000, this, &FullScreenNotification::fadeOut);
    }

    void FullScreenNotification::hide(){
	QWidget::hide();
	_animation->stop();
    }

    void FullScreenNotification::fadeOut(){
	_animation->setDuration(800);
	_animation->setStartValue(1.0);
	_animation->setEndValue(0.0);
	_animation->setEasingCurve(QEasingCurve::OutQuad);
	_animation->start();
    }

    void FullScreenNotification::fadeOutFinished(){
	hide();
	setWindowOpacity(1.0);
    }
}


