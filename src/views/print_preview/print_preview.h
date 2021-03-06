#ifndef _PRINTPREVIEW_H_
#define _PRINTPREVIEW_H_


#include <QMainWindow>
#include <QPrinter>
#include <QVBoxLayout>
#include <QDialog>


#if QT_VERSION == 0x050600
#include <wobjectdefs.h>
#include <QObject>
#endif



class PreviewView;
class QTextDocument;
class FlatToolButton;

class PrintPreview : public QDialog {
#if QT_VERSION == 0x050600
    W_OBJECT(PrintPreview)
#else
    Q_OBJECT
#endif

    public:
	PrintPreview(const QTextDocument *document, QWidget *parent);
	virtual ~PrintPreview();

    private slots:
	void	print();
	void	page_setup();

    private:
	void setup();

	QTextDocument	*doc;
	PreviewView	*view;
	QPrinter	printer;

	QVBoxLayout *centralLayout;
};

#endif	// _PRINTPREVIEW_H_
