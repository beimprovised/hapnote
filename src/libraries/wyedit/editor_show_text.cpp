
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QVBoxLayout>

#include "editor_show_text.h"


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(EditorShowText)
#endif


EditorShowText::EditorShowText(QWidget *parent) : QDialog(parent){
    setupUi();
    setupSignals();
    assembly();
}

EditorShowText::~EditorShowText()
{}

void EditorShowText::setupUi(){
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);

    setWindowFlags(this->windowFlags() & ~ Qt::WindowContextHelpButtonHint);	// Скрывается кнопка с вопросом

    textArea = new QTextEdit(this);
    textArea->setAcceptRichText(true);
    textArea->setSizePolicy(sizePolicy);

    textArea->setReadOnly(true);// Показываемый текст можно только просматривать
}

void EditorShowText::setupSignals()
{}

void EditorShowText::assembly(){
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

	// Добавляется область текста
    mainLayout->addWidget(textArea);
}

void EditorShowText::setHtml(QString text){
    textArea->setHtml(text);
}

void EditorShowText::setDocument(QTextDocument *document){
    textArea->setDocument(document);
}

