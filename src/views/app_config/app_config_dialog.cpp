
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QStackedWidget>
#include <QDialog>
#include <QDebug>

#include "main.h"
#include "config_dialog.h"
#include "app_config_dialog.h"
#include "app_config_page_main.h"
#include "app_config_page_crypt.h"
#include "app_config_page_misc.h"
#include "app_config_page_synchro.h"
#include "app_config_page_table.h"
#include "models/app_config/app_config.h"
#include "libraries/disk_helper.h"


extern AppConfig appconfig;


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(AppConfigDialog)
#endif

AppConfigDialog::AppConfigDialog(rctrl_t *_record_controller, QString first_page_name) : QWidget(), _record_controller(_record_controller){
    if(appconfig.interface_mode() == "mobile"){	// if(true)
	qDebug() << "Screen size X Y: " << screen_size_x() << screen_size_y();
	this->setMinimumSize(screen_size_x(), screen_size_y());
	this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    }
    config_dialog = new ConfigDialog(this);
    config_dialog->set_window_title(tr("Hapnote settings"));

    pageMain		= config_dialog->add_widget(new AppConfigPageMain(this), tr("Main"));
    pageCrypt		= config_dialog->add_widget(new AppConfigPageCrypt(this), tr("Crypt"));
    pageSynchro		= config_dialog->add_widget(new AppConfigPageSynchro(this), tr("Synchro"));
    pageRecordTable	= config_dialog->add_widget(new AppConfigPageTable(_record_controller, this), tr("Note area"));
    pageMisc		= config_dialog->add_widget(new AppConfigPageMisc(this), tr("Misc"));

    config_dialog->updateListWidth();
    if(first_page_name.size() > 0) change_page(first_page_name);
    config_dialog->exec();
}

// Переход на нужную страницу настроек (переход совершается кодом извне)
void AppConfigDialog::change_page(QString name){
    QListWidgetItem *item = nullptr;
    if(name == "pageMain") item = pageMain;
    if(name == "pageCrypt") item = pageCrypt;
    if(name == "pageSynchro") item = pageSynchro;
    if(name == "pageRecordTable") item = pageRecordTable;
    if(name == "pageMisc") item = pageMisc;
    if(item != nullptr) config_dialog->externalChangePage(item);
    else qDebug() << "AppConfigDialog::changePage cant find item for name: " << name;
}

