#ifndef __RECORD_H__
#define __RECORD_H__

#include <QString>
#include <QMap>
#include <QByteArray>
#include <QDomElement>

#include "utility/delegate.h"
#include "models/attachTable/Attach.h"
#include "models/attachTable/AttachTableData.h"
#include "views/browser/dockedwindow.h"


// Класс одной записи в таблице записей
// Class entries in the table of records

// В полях записей с crypt=0 хранятся незашифрованные данные
// В полях записей с crypt=1 хранятся зашифрованные данные
// Это необходимо, чтобы дерево знаний генерировалось в/из XML быстро и без шифрации

// In the fields of records crypt = 0 are stored unencrypted data
// In the fields of records crypt = 1 are stored encrypted data
// It is necessary that the tree of knowledge was generated in / from XML quickly and without encryption

// class Attach;
// class AttachTableData;

namespace browser {
    class DockedWindow;
    class WebView;
    class WebPage;
}


class Record {

    // К закрытым функциям может иметь доступ объекты приаттаченного файла
    friend class Attach;
    friend class AttachTableData;

public:
    Record();
    Record(const Record &obj);

    virtual ~Record();


    browser::WebPage *binded_only_page();   // const; // {return _page;}

    void setupDataFromDom(QDomElement iDomElement);
    QDomElement exportDataToDom(QDomDocument *doc) const;

    QString getText() const;
    QString getTextDirect();
    void setText(QString iText);

    QString getField(QString name) const;
    void setField(QString name, QString value);
    bool isNaturalFieldExists(QString name) const;


    // Setting and reading data without transformation. Used to generate / XML readers    // Установка и чтение данных без преобразований. Используется при генерации/чтении XML
    QString getNaturalFieldSource(QString name) const;
    void setNaturalFieldSource(QString name, QString value);

    QMap<QString, QString> getNaturalFieldList() const;

    QMap<QString, QByteArray> getPictureFiles() const;
    void setPictureFiles(QMap<QString, QByteArray> iPictureFiles);

    AttachTableData getAttachTable() const;
    AttachTableData *getAttachTablePointer();
    void setAttachTable(AttachTableData iAttachTable);

    bool isEmpty() const;
    bool isLite() const;
    void switchToLite();
    void switchToFat();

    // Шифрация
    void switchToEncryptAndSaveLite(void);
    void switchToEncryptAndSaveFat(void);

    // Расшифровка
    void switchToDecryptAndSaveLite(void);
    void switchToDecryptAndSaveFat(void);

    void pushFatAttributes();
    bool is_holder();
    void active_request(int pos, int openLinkIn);
    //    Record *active_immediately(bool ai) {_active_immediately = ai; return this;}
    //    bool active_immediately() {return _active_immediately;}

    void generator(std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, Record *const>> g) {_generator = g;}
    std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, Record *const>> generator() const {return _generator;}

    void activator(std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, void>> a) {_activator = a;}
    std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, void>> activator() const {return _activator;}

    browser::WebView *generate();
protected:

    browser::WebPage *_page;
    // ---------------------------------------------------------------------
    // Свойства класса (не забыть перечислить все в конструкторе копривания)
    // Class properties (do not forget to list all the constructor koprivaniya)
    // ---------------------------------------------------------------------

    bool liteFlag;

    // Установка содержимого свойств происходит в вышестоящем коде
    // Set the properties of the contents occurs in the upstream code

    // Light properties // Легкие свойства
    QMap<QString, QString> fieldList;   // // A list of the properties of records (attributes) ImyaAtributa - Meaning // Перечень свойств записи (атрибутов) ИмяАтрибута - Значение

    // Полновесные свойства
    QByteArray text; // Содержимое файла с текстом записи
    QMap<QString, QByteArray> pictureFiles; // Содержимое картинок, используемых в тексте записи (используется при переносе через буфер обмена, при DragAndDrop)

    // Таблица прикрепляемых файлов
    AttachTableData attachTableData;


    // -----------------
    // Защищенные методы
    // -----------------

    void saveTextDirect(QString iText);
    void saveText();

    QString getIdAndNameAsString() const; // Внутренний метод для облегчения печати отладочной информации

    QString getFullDirName() const;
    QString getShortDirName() const;

    QString getFullTextFileName() const;
    QString getFullFileName(QString fileName) const;

    void switchToEncryptFields(void);
    void switchToDecryptFields(void);

    void checkAndFillFileDir(QString &nameDirFull, QString &nameFileFull);
    void checkAndCreateTextFile();

    QString getNaturalField(QString name) const;
    QString getCalculableField(QString name) const;
private:
    Record *bind_page(browser::WebPage *bind_page);  // {_page = page; _page->record(this);}
    void page_to_nullptr();   // {_page->record(nullptr); _page = nullptr; }
    friend browser::WebPage;

    bool    _active_request = false;
    int     _position = -1;
    int     _openlinkinnewwindow = 0;
    //    bool    _active_immediately = false;
    std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, browser::WebView *, Record *const>> _generator;
    std::shared_ptr<sd::_interface<sd::meta_info<boost::shared_ptr<void>>, void>> _activator;
};

#endif // __RECORD_H__