
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QString>
#include <QWidget>
#include <QBoxLayout>
#include <QDir>
#include <QLineEdit>
#include <QAbstractSpinBox>
#include <QtDebug>
#include <QLabel>

#include "../../main.h"

#include "editor_config_misc.h"
#include "editor_config.h"
#include "libraries/global_parameters.h"

extern gl_para globalparameters;


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(EditorConfigMisc)
#endif


EditorConfigMisc::EditorConfigMisc(QWidget *parent) : ConfigPage(parent){
    qDebug() << "Create editor config misc widget";

	// Устанавливается указатель на объект работы с конфигфайлом
    conf = globalparameters.editor_config();	// find_object<EditorConfig>("editorconfig");


	// Шаг изменения отступа
    QLabel *indentStepLabel = new QLabel(this);
    indentStepLabel->setText(tr("Indent step"));

    QLabel *indentStepFlexion = new QLabel(this);
    indentStepFlexion->setText(tr("pixels"));

    indentStep = new QSpinBox(this);
    indentStep->setMinimum(1);
    indentStep->setMaximum(250);
    indentStep->setValue(conf->get_indent_step());


	// Сборка всех блоков в окно
    QGridLayout *configLayout = new QGridLayout();
    configLayout->addWidget(indentStepLabel,   0, 0);
    configLayout->addWidget(indentStep,         0, 1);
    configLayout->addWidget(indentStepFlexion, 0, 2);


    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->addLayout(configLayout);
    centralLayout->addStretch();

    setLayout(centralLayout);
}
int EditorConfigMisc:: apply_changes(void){
    qDebug() << "Apply changes editor misc";

	// Уровень сложности вносимых изменений
	// 0 - изменения можно делать на лету
	// 1 - для принятия изменений нужна перезагрузка
    int difficultChanges = 0;
	// Если был изменен размер основного шрифта
    if(conf->get_indent_step() != indentStep->value()){
	conf->set_indent_step(indentStep->value());
    }
    return difficultChanges;
}
