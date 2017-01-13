#include "sobjselectionwidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMouseEvent>

SObjSelectionWidget::SObjSelectionWidget(const QString &id, std::vector<SObjIcon *> &sObjIcons,
	QFrame *parent) : id(id), sObjIcons(sObjIcons), currentSObjIcon(nullptr), QFrame(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setFrameStyle(QFrame::WinPanel | QFrame::Raised);

	titleLabel.setText(id);
	titleLabel.setStyleSheet("font-size : 10pt; font-weight: bold;");

	selectionNameLabel.setStyleSheet("QLabel { background-color: #3299FF; color: white; }");
	selectionNameLabel.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	selectionNameLabel.setAlignment(Qt::AlignCenter);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QGroupBox *groupBox = new QGroupBox;
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setSpacing(5);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->setColumnStretch(0, 1);
	gridLayout->setColumnStretch(1, 1);
	gridLayout->setColumnStretch(2, 1);

	for (int i=0; i<sObjIcons.size(); i++) // 默认一行三个图标
	{
		int r = i / 3;
		int c = i % 3;
		gridLayout->addWidget(sObjIcons[i], r, c);
		connect(sObjIcons[i], SIGNAL(hover(QString, SObjIcon *)), this, SLOT(hover(QString, SObjIcon *)));
	}

	QHBoxLayout *hLayout1 = new QHBoxLayout;
	hLayout1->addSpacing(8);
	hLayout1->addWidget(&titleLabel);

	QHBoxLayout *hLayout2 = new QHBoxLayout;
	hLayout2->setContentsMargins(8, 0, 8, 0);
	hLayout2->addWidget(&selectionNameLabel);

	QHBoxLayout *hLayout3 = new QHBoxLayout;
	hLayout3->setContentsMargins(8, 0, 8, 0);
	groupBox->setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 3px; padding: 0px; margin: 0px; }");
	groupBox->setLayout(gridLayout);
	hLayout3->addWidget(groupBox);

	QFrame *lineFrame = new QFrame;
	lineFrame->setFrameShape(QFrame::HLine);
	lineFrame->setFrameShadow(QFrame::Sunken);
	lineFrame->setLineWidth(1);

	mainLayout->setSpacing(3);
	mainLayout->setContentsMargins(0, 8, 0, 8);
	mainLayout->addLayout(hLayout1);
	mainLayout->addWidget(lineFrame);
	mainLayout->addLayout(hLayout2);
	mainLayout->addLayout(hLayout3);

	setLayout(mainLayout);
}

SObjSelectionWidget::~SObjSelectionWidget() 
{
	
}

void SObjSelectionWidget::enterEvent(QEvent *evt)
{
	activateWindow();
}

void SObjSelectionWidget::closeEvent(QCloseEvent *evt)
{
	for (SObjIcon *sObjIcon : sObjIcons)
	{
		sObjIcon->shutdown();
	}
}

void SObjSelectionWidget::hover(QString id, SObjIcon *emitter)
{
	selectionNameLabel.setText(id);
	if (currentSObjIcon != emitter)
	{
		if (currentSObjIcon != nullptr)
		{
			currentSObjIcon->shutdown();
		}
		currentSObjIcon = emitter;
	}
}