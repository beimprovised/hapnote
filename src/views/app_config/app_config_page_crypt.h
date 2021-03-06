#ifndef _APPCONFIGPAGE_CRYPT_H_
#define	_APPCONFIGPAGE_CRYPT_H_





#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>

#include "config_page.h"

#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif

class AppConfigPageCrypt : public ConfigPage {
#if QT_VERSION == 0x050600
    W_OBJECT(AppConfigPageCrypt)
#else
    Q_OBJECT
#endif

    public:
	AppConfigPageCrypt(QWidget *parent = 0);
	virtual ~AppConfigPageCrypt();

	int apply_changes(void);

    private slots:

	void	onPassRetrieveButtonClicked(void);
	void	onAutoClosePasswordEnableToggle(bool checked);
	void	onPasswordSaveEnableToggle(bool checked);

    protected:

	QGroupBox	*passRetrieveBox;
	QLabel		*passRetrieveStatus;
	QPushButton	*passRetrieveButton;
	QLabel		*passRetrieveAnnotation;

	QGroupBox	*howPassRequestBox;
	QRadioButton	*howPassRequestRadio1;
	QRadioButton	*howPassRequestRadio2;

	QGroupBox	*storePassRequestBox;
	QCheckBox	*passwordSaveEnable;
	QLabel		*passwordSaveAnnotation;


	QGroupBox	*autoClosePasswordBox;
	QCheckBox	*autoClosePasswordEnable;
	QSpinBox	*autoClosePasswordDelay;

	QGroupBox	*decryptFileToTrashDirectoryBox;
	QCheckBox	*decryptFileToTrashDirectoryEnable;

	QString getRetrieveStatusText(void);
	QString getRetrieveButtonText(void);
	QString getRetrieveAnnotationText(void);

	void	setup_ui(void);
	void	update_ui(void);
	void	setup_signals(void);
	void	assembly(void);
};


#endif	// _APPCONFIGPAGE_CRYPT_H_

