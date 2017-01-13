#include "paramwidget.h"
#include <QHBoxLayout>
#include <QKeyEvent>

ParamWidget::ParamWidget(QString id, QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::Tool);
	setFocusPolicy(Qt::StrongFocus);
	setWindowTitle(id);
	setMinimumWidth(150);

	mainLayout = new QVBoxLayout;
	mainLayout->setSpacing(8);
	mainLayout->setContentsMargins(0, 8, 0, 8);

	setLayout(mainLayout);
}

ParamWidget::~ParamWidget()
{

}

void ParamWidget::addBool(QString id,
	QCheckBox *&checkBox)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	checkBox = new QCheckBox;

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(checkBox);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addFloat(QString id,
	float minVal, float maxVal, float singleStepVal,
	QDoubleSpinBox *&doubleSpinBox)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	doubleSpinBox = new QDoubleSpinBox;
	doubleSpinBox->setMinimum(minVal);
	doubleSpinBox->setMaximum(maxVal);
	doubleSpinBox->setSingleStep(singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(doubleSpinBox);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addFloats(QString id,
	float defaultVal,
	float minVal, float maxVal, float singleStepVal,
	DoubleSpinBoxList *&doubleSpinBoxList)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QVBoxLayout *vLayout = new QVBoxLayout;
	QLabel *idLabel = new QLabel(id + ":");
	vLayout->addSpacing(8);
	vLayout->addWidget(idLabel);
	vLayout->addStretch();

	doubleSpinBoxList = new DoubleSpinBoxList(defaultVal, minVal, maxVal, singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addLayout(vLayout);
	hLayout->addWidget(doubleSpinBoxList);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addInt(QString id,
	int minVal, int maxVal, int singleStepVal,
	QSpinBox *&spinBox)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	spinBox = new QSpinBox;
	spinBox->setMinimum(minVal);
	spinBox->setMaximum(maxVal);
	spinBox->setSingleStep(singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(spinBox);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addVec2(QString id,
	float minVal, float maxVal, float singleStepVal,
	DoubleSpinBoxVec2 *&doubleSpinBoxVec2)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");
	doubleSpinBoxVec2 = new DoubleSpinBoxVec2(minVal, maxVal, singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(doubleSpinBoxVec2);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addIVec2(QString id,
	int minVal, int maxVal, int singleStepVal,
	SpinBoxIVec2 *&spinBoxIVec2)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");
	spinBoxIVec2 = new SpinBoxIVec2(minVal, maxVal, singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(spinBoxIVec2);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addVec3(QString id,
	float minVal, float maxVal, float singleStepVal,
	DoubleSpinBoxVec3 *&doubleSpinBoxVec3)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");
	doubleSpinBoxVec3 = new DoubleSpinBoxVec3(minVal, maxVal, singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(doubleSpinBoxVec3);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addVec4(QString id,
	float minVal, float maxVal, float singleStepVal,
	DoubleSpinBoxVec4 *&doubleSpinBoxVec4)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");
	doubleSpinBoxVec4 = new DoubleSpinBoxVec4(minVal, maxVal, singleStepVal);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(doubleSpinBoxVec4);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addString(QString id,
	QLineEdit *&lineEdit)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	lineEdit = new QLineEdit;

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(lineEdit);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addText(QString id,
	QTextEdit *&textEdit)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	textEdit = new QTextEdit;

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(textEdit);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addColor(QString id,
	ColorLabel *&colorLabel)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	colorLabel = new ColorLabel;

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(colorLabel);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addEnum(QString id,
	QStringList enums,
	QComboBox *&comboxBox)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	comboxBox = new QComboBox;
	comboxBox->addItems(enums);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(comboxBox);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addFile(QString id,
	QString filter,
	FileWidget *&fileWidget)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setAlignment(Qt::AlignLeft);

	QLabel *idLabel = new QLabel(id + ":");

	fileWidget = new FileWidget(filter);

	hLayout->addSpacing(8);
	hLayout->addWidget(idLabel);
	hLayout->addWidget(fileWidget);
	hLayout->addSpacing(8);

	mainLayout->addLayout(hLayout);
}

void ParamWidget::addSeperateLine()
{
	QFrame *seperateLineFrame = new QFrame;
	seperateLineFrame->setFrameShape(QFrame::HLine);
	seperateLineFrame->setFrameShadow(QFrame::Sunken);
	seperateLineFrame->setLineWidth(1);

	mainLayout->addWidget(seperateLineFrame);
}

void ParamWidget::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Escape)
	{
		close();
	}
}