#ifndef _CONFIGPAGE_MISC_H_
#define	_CONFIGPAGE_MISC_H_




#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>

#include "config_page.h"


#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif

class AppConfigPageMisc : public ConfigPage {
#if QT_VERSION == 0x050600
    W_OBJECT(AppConfigPageMisc)
#else
    Q_OBJECT
#endif

    public:
	AppConfigPageMisc(QWidget *parent = 0);
	int apply_changes(void);

    private slots:

    protected:

	QCheckBox	*cutBranchConfirm;
	QCheckBox	*printDebugMessages;
	QCheckBox	*runInMinimizedWindow;

	// Объединяющая рамка
	QGroupBox *historyBox;

	QCheckBox	*rememberAtHistoryNavigationCheckBox;
	QCheckBox	*rememberAtOrdinarySelectionCheckBox;
};


#endif	// _CONFIGPAGE_MISC_H_

