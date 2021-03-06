#ifndef ATTACHTABLESCREEN_H
#define ATTACHTABLESCREEN_H



#include <QWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QAction>


#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif

class AttachTableView;
class AttachTableController;


// Виджет

class AttachTableScreen : public QWidget {
#if QT_VERSION == 0x050600
    W_OBJECT(AttachTableScreen)
#else
    Q_OBJECT
#endif

    public:
	AttachTableScreen(QWidget *parent = 0);
	virtual ~AttachTableScreen();

	void clear(void);

	QAction *actionAddAttach;
	QAction *actionAddLink;
	QAction *actionEditFileName;
	QAction *actionDeleteAttach;
	QAction *actionOpenAttach;
	QAction *actionSaveAsAttach;
	QAction *actionShowAttachInfo;

    signals:

    public slots:

    protected:

	void	setupActions(void);
	void	setupUI(void);
	void	setupSignals(void);
	void	assembly(void);


	// Указатель на контроллер таблицы приаттаченных файлов
	AttachTableController *attachTableController;

	// Указатель на таблицу приаттаченных файлов (представление)
	AttachTableView *attachTableView;

	// Экранные элементы
	QToolBar	*toolsLine;
	QVBoxLayout	*screenLayout;

	QAction *actionSwitchToEditor;
};

#endif	// ATTACHTABLESCREEN_H
