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

#include "xbel.h"

#include <QtCore/QFile>


namespace browser {



    BookmarkNode::BookmarkNode(BookmarkNode::Type type, BookmarkNode *parent) :
        expanded(false)
        , _parent(parent)
        , _type(type)
    {
        if(parent)
            parent->add(this);
    }

    BookmarkNode::~BookmarkNode()
    {
        if(_parent)
            _parent->remove(this);

        qDeleteAll(_children);
        _parent = 0;
        _type = BookmarkNode::Root;
    }

    bool BookmarkNode::operator==(const BookmarkNode &other)
    {
        if(url != other.url
           || title != other.title
           || desc != other.desc
           || expanded != other.expanded
           || _type != other._type
           || _children.count() != other._children.count())
            return false;

        for(int i = 0; i < _children.count(); ++i)
            if(!((*(_children[i])) == (*(other._children[i]))))
                return false;

        return true;
    }

    BookmarkNode::Type BookmarkNode::type() const
    {
        return _type;
    }

    void BookmarkNode::setType(Type type)
    {
        _type = type;
    }

    QList<BookmarkNode *> BookmarkNode::children() const
    {
        return _children;
    }

    BookmarkNode *BookmarkNode::parent() const
    {
        return _parent;
    }

    void BookmarkNode::add(BookmarkNode *child, int offset)
    {
        Q_ASSERT(child->_type != Root);

        if(child->_parent)
            child->_parent->remove(child);

        child->_parent = this;

        if(-1 == offset)
            offset = _children.size();

        _children.insert(offset, child);
    }

    void BookmarkNode::remove(BookmarkNode *child)
    {
        child->_parent = 0;
        _children.removeAll(child);
    }


    XbelReader::XbelReader()
    {
    }

    BookmarkNode *XbelReader::read(const QString &fileName)
    {
        QFile file(fileName);

        if(!file.exists()) {
            return new BookmarkNode(BookmarkNode::Root);
        }

        file.open(QFile::ReadOnly);
        return read(&file);
    }

    BookmarkNode *XbelReader::read(QIODevice *device)
    {
        BookmarkNode *root = new BookmarkNode(BookmarkNode::Root);
        setDevice(device);

        if(readNextStartElement()) {
            QString version = attributes().value(QLatin1String("version")).toString();

            if(name() == QLatin1String("xbel")
               && (version.isEmpty() || version == QLatin1String("1.0"))) {
                readXBEL(root);
            } else {
                raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
            }
        }

        return root;
    }

    void XbelReader::readXBEL(BookmarkNode *parent)
    {
        Q_ASSERT(isStartElement() && name() == QLatin1String("xbel"));

        while(readNextStartElement()) {
            if(name() == QLatin1String("folder"))
                readFolder(parent);
            else if(name() == QLatin1String("bookmark"))
                readBookmarkNode(parent);
            else if(name() == QLatin1String("separator"))
                readSeparator(parent);
            else
                skipCurrentElement();
        }
    }

    void XbelReader::readFolder(BookmarkNode *parent)
    {
        Q_ASSERT(isStartElement() && name() == QLatin1String("folder"));

        BookmarkNode *folder = new BookmarkNode(BookmarkNode::Folder, parent);
        folder->expanded = (attributes().value(QLatin1String("folded")) == QLatin1String("no"));

        while(readNextStartElement()) {
            if(name() == QLatin1String("title"))
                readTitle(folder);
            else if(name() == QLatin1String("desc"))
                readDescription(folder);
            else if(name() == QLatin1String("folder"))
                readFolder(folder);
            else if(name() == QLatin1String("bookmark"))
                readBookmarkNode(folder);
            else if(name() == QLatin1String("separator"))
                readSeparator(folder);
            else
                skipCurrentElement();
        }
    }

    void XbelReader::readTitle(BookmarkNode *parent)
    {
        Q_ASSERT(isStartElement() && name() == QLatin1String("title"));
        parent->title = readElementText();
    }

    void XbelReader::readDescription(BookmarkNode *parent)
    {
        Q_ASSERT(isStartElement() && name() == QLatin1String("desc"));
        parent->desc = readElementText();
    }

    void XbelReader::readSeparator(BookmarkNode *parent)
    {
        new BookmarkNode(BookmarkNode::Separator, parent);
        // empty elements have a start and end element
        readNext();
    }

    void XbelReader::readBookmarkNode(BookmarkNode *parent)
    {
        Q_ASSERT(isStartElement() && name() == QLatin1String("bookmark"));
        BookmarkNode *bookmark = new BookmarkNode(BookmarkNode::Bookmark, parent);
        bookmark->url = attributes().value(QLatin1String("href")).toString();

        while(readNextStartElement()) {
            if(name() == QLatin1String("title"))
                readTitle(bookmark);
            else if(name() == QLatin1String("desc"))
                readDescription(bookmark);
            else
                skipCurrentElement();
        }

        if(bookmark->title.isEmpty())
            bookmark->title = QObject::tr("Unknown title");
    }


    XbelWriter::XbelWriter()
    {
        setAutoFormatting(true);
    }

    bool XbelWriter::write(const QString &fileName, const BookmarkNode *root)
    {
        QFile file(fileName);

        if(!root || !file.open(QFile::WriteOnly))
            return false;

        return write(&file, root);
    }

    bool XbelWriter::write(QIODevice *device, const BookmarkNode *root)
    {
        setDevice(device);

        writeStartDocument();
        writeDTD(QLatin1String("<!DOCTYPE xbel>"));
        writeStartElement(QLatin1String("xbel"));
        writeAttribute(QLatin1String("version"), QLatin1String("1.0"));

        if(root->type() == BookmarkNode::Root) {
            for(int i = 0; i < root->children().count(); ++i)
                writeItem(root->children().at(i));
        } else {
            writeItem(root);
        }

        writeEndDocument();
        return true;
    }

    void XbelWriter::writeItem(const BookmarkNode *parent)
    {
        switch(parent->type()) {
            case BookmarkNode::Folder:
                writeStartElement(QLatin1String("folder"));
                writeAttribute(QLatin1String("folded"), parent->expanded ? QLatin1String("no") : QLatin1String("yes"));
                writeTextElement(QLatin1String("title"), parent->title);

                for(int i = 0; i < parent->children().count(); ++i)
                    writeItem(parent->children().at(i));

                writeEndElement();
                break;

            case BookmarkNode::Bookmark:
                writeStartElement(QLatin1String("bookmark"));

                if(!parent->url.isEmpty())
                    writeAttribute(QLatin1String("href"), parent->url);

                writeTextElement(QLatin1String("title"), parent->title);

                if(!parent->desc.isEmpty())
                    writeAttribute(QLatin1String("desc"), parent->desc);

                writeEndElement();
                break;

            case BookmarkNode::Separator:
                writeEmptyElement(QLatin1String("separator"));
                break;

            default:
                break;
        }
    }


}


