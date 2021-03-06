
#if QT_VERSION == 0x050600
#include <wobjectimpl.h>
#endif


#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "editor_split_cell_form.h"


#if QT_VERSION == 0x050600
W_OBJECT_IMPL(EditorSplitCellForm)
#endif



EditorSplitCellForm::EditorSplitCellForm(int fixHorisontalSplit, int fixVerticalSplit){
    this->setWindowTitle(tr("Split cell"));


	// Устанавливаются значения разбиения ячейки
	// Если значение равно 0, пользователь может сам выбрать число разбиения
	// Если значение больше 0, ячейку можно разбить только на указанное значение
    fixHSplit	= fixHorisontalSplit;
    fixVSplit	= fixVerticalSplit;


	// Подготавливаются все объекты формы
	// И те, котрые будут отображаться, и те которые отображаться не будут

    checkboxFixHSplit	= new QCheckBox();
    checkboxFixVSplit	= new QCheckBox();

    checkboxLabelFixHSplit	= new QLabel("<b>" + QString::number(fixHSplit) + "</b> ");
    checkboxLabelFixVSplit	= new QLabel("<b>" + QString::number(fixVSplit) + "</b> ");


    labelPictogramH = new QLabel();
    labelPictogramH->setPixmap(QPixmap(":/resource/pic/edit_table_split_h.svg"));

    labelPictogramV = new QLabel();
    labelPictogramV->setPixmap(QPixmap(":/resource/pic/edit_table_split_v.svg"));


    labelH	= new QLabel(tr("Split horizontally by: "));
    labelV	= new QLabel(tr("Split vertically by: "));

    numSplitH	= new QSpinBox();
    numSplitV	= new QSpinBox();
    numSplitH->setRange(1, 100);
    numSplitH->setValue(1);
    numSplitV->setRange(1, 100);
    numSplitV->setValue(1);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
    buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditorSplitCellForm::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditorSplitCellForm::reject);


	// Сборка формы в зависимости от того, как можно разделять
	// ячейку по горизонтали и вертикали

    QGridLayout *gridLayout = new QGridLayout();

    gridLayout->addWidget(labelPictogramH, 0, 0);
    gridLayout->addWidget(labelH, 0, 1);
    if(fixHSplit == 0)gridLayout->addWidget(numSplitH, 0, 2);
    else{
	QHBoxLayout *fixValueH = new QHBoxLayout();
	fixValueH->addWidget(checkboxLabelFixHSplit);
	fixValueH->addStretch();
	fixValueH->addWidget(checkboxFixHSplit);

	gridLayout->addLayout(fixValueH, 0, 2);
    }
    gridLayout->addWidget(labelPictogramV, 1, 0);
    gridLayout->addWidget(labelV, 1, 1);
    if(fixVSplit == 0)gridLayout->addWidget(numSplitV, 1, 2);
    else{
	QHBoxLayout *fixValueV = new QHBoxLayout();
	fixValueV->addWidget(checkboxLabelFixVSplit);
	fixValueV->addStretch();
	fixValueV->addWidget(checkboxFixVSplit);

	gridLayout->addLayout(fixValueV, 1, 2);
    }
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(buttonBox);
}

EditorSplitCellForm::~EditorSplitCellForm(){
	// Так как неизвестно, какие объекты попадут на слои,
	// (и при этом установят parent на данный слой)
	// надо все объекты удалять вручную, чтобы небыло утечек памяти
    if(checkboxFixHSplit != nullptr)delete checkboxFixHSplit;
    if(checkboxFixVSplit != nullptr)delete checkboxFixVSplit;
    if(checkboxLabelFixHSplit != nullptr)delete checkboxLabelFixHSplit;
    if(checkboxLabelFixVSplit != nullptr)delete checkboxLabelFixVSplit;
    if(labelPictogramH != nullptr)delete labelPictogramH;
    if(labelPictogramV != nullptr)delete labelPictogramV;
    if(labelH != nullptr)delete labelH;
    if(labelV != nullptr)delete labelV;
    if(labelH != nullptr)delete numSplitH;
    if(labelV != nullptr)delete numSplitV;
}

int EditorSplitCellForm::get_num_split_h(void){
    if(fixHSplit == 0)return numSplitH->value();
    else{
	if(checkboxFixHSplit->isChecked())return fixHSplit;
	else return 1;
    }
}

int EditorSplitCellForm::get_num_split_v(void){
    if(fixVSplit == 0)return numSplitV->value();
    else{
	if(checkboxFixVSplit->isChecked())return fixVSplit;
	else return 1;
    }
}

