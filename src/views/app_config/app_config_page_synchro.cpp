
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QWidget>
#include <QBoxLayout>
#include <QLabel>

#include "app_config_page_synchro.h"
#include "models/app_config/app_config.h"
#include "libraries/disk_helper.h"


extern AppConfig appconfig;


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(AppConfigPageSynchro)
#endif


AppConfigPageSynchro::AppConfigPageSynchro(QWidget *parent) : ConfigPage(parent){
    qDebug() << "Create synchro config page";

    synchroCommand = new QLineEdit(this);
    synchroCommand->setText(appconfig.synchro_command());
    synchroCommand->setCursorPosition(0);


    synchroOnStartup = new QCheckBox(this);
    synchroOnStartup->setText(tr("Synchronize at Hapnote startup"));
    synchroOnStartup->setChecked(appconfig.synchro_on_startup());

    synchroOnExit = new QCheckBox(this);
    synchroOnExit->setText(tr("Synchronize when exit from Hapnote"));
    synchroOnExit->setChecked(appconfig.synchro_on_exit());


	// Собирается основной слой
    QVBoxLayout *central_layout = new QVBoxLayout();

	// Область ввода команды синхронизации
    QLabel *commandText = new QLabel(this);
    commandText->setText(tr("Synchronization command"));

    QLabel *commandAboutText = new QLabel(this);
    commandAboutText->setText(tr("Use <b>%a</b> macro for get database directory path"));
    commandAboutText->setWordWrap(true);

    central_layout->addWidget(commandText);
    central_layout->addWidget(synchroCommand);
    central_layout->addWidget(commandAboutText);



	// Группировщик виджетов для настройки автоматического старта синхронизации
    synchroOnBox = new QGroupBox(this);
    synchroOnBox->setTitle(tr("Automatic start synchronization"));

	// Виджеты вставляются в группировщик
    QVBoxLayout *synchroOnLayout = new QVBoxLayout;
    synchroOnLayout->addWidget(synchroOnStartup);
    synchroOnLayout->addWidget(synchroOnExit);
    synchroOnBox->setLayout(synchroOnLayout);

    central_layout->addWidget(synchroOnBox);

    central_layout->addStretch();

	// Основной слой устанавливается
    setLayout(central_layout);
}

// Метод должен возвращать уровень сложности сделанных изменений
// 0 - изменения не требуют перезапуска программы
// 1 - изменения требуют перезапуска программы
int AppConfigPageSynchro::apply_changes(void){
    qDebug() << "Apply changes synchro";
// Сохраняется строка с командой синхронизации
    if(appconfig.synchro_command() != synchroCommand->text())appconfig.synchro_command(synchroCommand->text());
// Сохраняется настройка запуска синхронизации при старте
    if(appconfig.synchro_on_startup() != synchroOnStartup->isChecked())appconfig.synchro_on_startup(synchroOnStartup->isChecked());
// Сохраняется настройка запуска синхронизации при выходе
    if(appconfig.synchro_on_exit() != synchroOnExit->isChecked())appconfig.synchro_on_exit(synchroOnExit->isChecked());
    return 0;
}

