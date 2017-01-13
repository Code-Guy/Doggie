#include "sobjicon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>

#include "sobjselectionwidget.h"
#include "scene.h"
#include "tool.h"

SObjIcon::SObjIcon(Scene *scene, QStringList ids, SObjSelectionWidget *childWidget, int fps, QWidget *parent) :
	scene(scene), ids(ids), childWidget(childWidget), QWidget(parent)
{
	std::vector<QString> imageFileNames = Tool::traverseFile("Resources/SObjs/" + Tool::getLayerDir(ids) + "/Icons/");
	for (QString imageFileName : imageFileNames)
	{
		images.push_back(getSquareImage(QPixmap(imageFileName)));
	}
	currentImageIndex = 0;
	imageLabel.setPixmap(images[currentImageIndex]);
	imageLabel.setMargin(3);
	if (childWidget)
	{
		imageLabel.setFrameStyle(QFrame::WinPanel | QFrame::Raised);
	}
	else
	{
		imageLabel.setStyleSheet("QLabel { border:2px solid #F0F0F0; }");
	}

	nameLabel.setText(ids.last());
	nameLabel.adjustSize();

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->setSpacing(5);
	vLayout->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout *hLayout1 = new QHBoxLayout;
	QHBoxLayout *hLayout2 = new QHBoxLayout;
	hLayout1->setAlignment(Qt::AlignHCenter);
	hLayout2->setAlignment(Qt::AlignHCenter);
	hLayout1->addWidget(&imageLabel);
	hLayout2->addWidget(&nameLabel);
	vLayout->addLayout(hLayout1);
	vLayout->addLayout(hLayout2);

	setLayout(vLayout);
	
	if (images.size() > 1)
	{
		connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
		updateTimer.start(1000 / fps);
	}	
}

SObjIcon::~SObjIcon() 
{
	
}

void SObjIcon::shutdown()
{
	if (childWidget)
	{
		childWidget->close();
	}
}

void SObjIcon::enterEvent(QEvent *evt)
{
	if (childWidget)
	{
		imageLabel.setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
		childWidget->move(adjustChildWidgetPos(mapToGlobal(QPoint(0, 0)) + QPoint(30, 30)));
		childWidget->show();
	}
	else
	{
		imageLabel.setStyleSheet("QLabel { border:2px solid blue; }");
	}

	emit hover(nameLabel.text(), this);
}

void SObjIcon::leaveEvent(QEvent *evt)
{
	if (childWidget)
	{
		imageLabel.setFrameStyle(QFrame::WinPanel | QFrame::Raised);
	}
	else
	{
		imageLabel.setStyleSheet("QLabel { border:2px solid #F0F0F0; }");
	}
}

void SObjIcon::mousePressEvent(QMouseEvent *evt)
{
	if (!childWidget && evt->button() == Qt::LeftButton)
	{
		scene->dragSObj(ids, scene->mapFromGlobal(mapToGlobal(evt->pos())));
	}
}

void SObjIcon::mouseReleaseEvent(QMouseEvent *evt)
{
	postMouseEvent(evt);
}

void SObjIcon::updateImage()
{
	currentImageIndex = (currentImageIndex + 1) % images.size();
	imageLabel.setPixmap(images[currentImageIndex]);
}

QPixmap SObjIcon::getSquareImage(const QPixmap &image)
{
	int imageSize = image.width() > image.height() ? image.width() : image.height();
	QPixmap bgImage(imageSize, imageSize);
	bgImage.fill(Qt::transparent);

	QPainter painter(&bgImage);
	if (image.width() > image.height())
	{
		painter.drawPixmap(QPoint(0, (imageSize - image.height()) / 2), image);
	}
	else
	{
		painter.drawPixmap(QPoint((imageSize - image.width()) / 2, 0), image);
	}

	return bgImage;
}

void SObjIcon::postMouseEvent(QMouseEvent *evt)
{
	QMouseEvent *event = new QMouseEvent(evt->type(),
		scene->mapFromGlobal(mapToGlobal(evt->pos())),
		evt->button(), evt->buttons(), evt->modifiers());
	QApplication::postEvent(scene, event);
}

QPoint SObjIcon::adjustChildWidgetPos(QPoint pos)
{
	QPoint adjustPos = pos;
	glm::ivec2 position = scene->getPosition();
	glm::ivec4 viewport = scene->getViewport();
	childWidget->show();
	childWidget->layout()->invalidate();
	childWidget->hide();
	QSize size = childWidget->size();

	if (pos.x() + size.width() > position.x + viewport.z)
	{
		adjustPos.setX(adjustPos.x() - size.width());
	}
	if (pos.y() + size.height() > position.y + viewport.w)
	{
		adjustPos.setY(adjustPos.y() - size.height());
	}
	return adjustPos;
}