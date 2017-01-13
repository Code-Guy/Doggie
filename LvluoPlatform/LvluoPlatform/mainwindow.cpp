#include "mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QPrintDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("Lv Luo"));

	appConfig = AppConfig::getInstance();
	sceneProperty = SceneProperty::getInstance();
	recordVideoWidget = new RecordVideoWidget;
	scene = new Scene;

	aboutWidget = new QWidget;
	aboutWidget->setWindowTitle("About");
	QPalette palette;
	QPixmap bgPix = QPixmap("Resources/Icons/about_panel.png");
	palette.setBrush(QPalette::Background, QBrush(bgPix));
	aboutWidget->setPalette(palette);
	aboutWidget->resize(bgPix.size());

	initRecentProjectsActions();

	ui.action_New->setIcon(QIcon("Resources/Icons/new.png"));
	ui.action_Open->setIcon(QIcon("Resources/Icons/open.png"));
	ui.action_Close->setIcon(QIcon("Resources/Icons/close.png"));
	ui.action_Save->setIcon(QIcon("Resources/Icons/save.png"));
	ui.action_Print->setIcon(QIcon("Resources/Icons/print.png"));
	ui.action_Export->setIcon(QIcon("Resources/Icons/export.png"));
	ui.action_Save_As->setIcon(QIcon("Resources/Icons/saveAs.png"));
	ui.action_Properties->setIcon(QIcon("Resources/Icons/property.png"));
	ui.action_Recent_Projects->setIcon(QIcon("Resources/Icons/recent_project.png"));
	ui.action_Exit->setIcon(QIcon("Resources/Icons/exit.png"));
	ui.action_Undo->setIcon(QIcon("Resources/Icons/undo.png"));
	ui.action_Redo->setIcon(QIcon("Resources/Icons/redo.png"));
	ui.action_Select_All->setIcon(QIcon("Resources/Icons/select_all.png"));
	ui.action_Clear_All->setIcon(QIcon("Resources/Icons/clear_all.png"));
	ui.action_Cut->setIcon(QIcon("Resources/Icons/cut.png"));
	ui.action_Copy->setIcon(QIcon("Resources/Icons/copy.png"));
	ui.action_Paste->setIcon(QIcon("Resources/Icons/paste.png"));
	ui.action_Delete->setIcon(QIcon("Resources/Icons/delete.png"));
	ui.action_Group->setIcon(QIcon("Resources/Icons/group.png"));
	ui.action_Ungroup->setIcon(QIcon("Resources/Icons/ungroup.png"));
	ui.action_Record_Video->setIcon(QIcon("Resources/Icons/record_video.png"));
	ui.action_Options->setIcon(QIcon("Resources/Icons/options.png"));
	ui.action_Fullscreen->setIcon(QIcon("Resources/Icons/fullscreen.png"));
	ui.action_About->setIcon(QIcon("Resources/Icons/about.png"));

	connect(scene, SIGNAL(updateMousePos(QPoint)), this, SLOT(updateMousePos(QPoint)));
	connect(scene, SIGNAL(recentProject(QString)), this, SLOT(recentProject(QString)));
	connect(recordVideoWidget, SIGNAL(getSnapshot()), scene, SLOT(querySnapshot()));
	connect(scene, SIGNAL(setSnapshot(const QImage &)), recordVideoWidget, SLOT(setSnapshot(const QImage &)));

	connect(ui.action_New, SIGNAL(triggered()), this, SLOT(newActionTriggered()));
	connect(ui.action_Open, SIGNAL(triggered()), this, SLOT(openActionTriggered()));
	connect(ui.action_Close, SIGNAL(triggered()), this, SLOT(closeActionTriggered()));
	connect(ui.action_Save, SIGNAL(triggered()), this, SLOT(saveActionTriggered()));
	connect(ui.action_Print, SIGNAL(triggered()), this, SLOT(printActionTriggered()));
	connect(ui.action_Export, SIGNAL(triggered()), this, SLOT(exportActionTriggered()));
	connect(ui.action_Save_As, SIGNAL(triggered()), this, SLOT(saveAsActionTriggered()));
	connect(ui.action_Properties, SIGNAL(triggered()), this, SLOT(propertiesActionTriggered()));
	connect(ui.action_Exit, SIGNAL(triggered()), this, SLOT(exitActionTriggered()));
	connect(ui.action_Undo, SIGNAL(triggered()), this, SLOT(undoActionTriggered()));
	connect(ui.action_Redo, SIGNAL(triggered()), this, SLOT(redoActionTriggered()));
	connect(ui.action_Select_All, SIGNAL(triggered()), this, SLOT(selectAllActionTriggered()));
	connect(ui.action_Clear_All, SIGNAL(triggered()), this, SLOT(clearAllActionTriggered()));
	connect(ui.action_Cut, SIGNAL(triggered()), this, SLOT(cutActionTriggered()));
	connect(ui.action_Copy, SIGNAL(triggered()), this, SLOT(copyActionTriggered()));
	connect(ui.action_Paste, SIGNAL(triggered()), this, SLOT(pasteActionTriggered()));
	connect(ui.action_Delete, SIGNAL(triggered()), this, SLOT(deleteActionTriggered()));
	connect(ui.action_Group, SIGNAL(triggered()), this, SLOT(groupActionTriggered()));
	connect(ui.action_Ungroup, SIGNAL(triggered()), this, SLOT(ungroupActionTriggered()));
	connect(ui.action_Record_Video, SIGNAL(triggered()), this, SLOT(recordVideoActionTriggered()));
	connect(ui.action_Options, SIGNAL(triggered()), this, SLOT(optionsActionTriggered()));
	connect(ui.action_Fullscreen, SIGNAL(triggered()), this, SLOT(fullscreenActionTriggered()));
	connect(ui.action_About, SIGNAL(triggered()), this, SLOT(aboutActionTriggered()));

	connect(appConfig, SIGNAL(panelSizeChanged(glm::ivec2)), this, SLOT(sizeChanged(glm::ivec2)));
	connect(appConfig, SIGNAL(panelPositionChanged(glm::ivec2)), this, SLOT(positionChanged(glm::ivec2)));

	sizeChanged(appConfig->getPanelSize());
	positionChanged(appConfig->getPanelPosition());

	addActions(ui.menuBar->actions());
	setCentralWidget(scene);
}

MainWindow::~MainWindow()
{
	delete scene;
	//delete appConfig;
	delete sceneProperty;
	delete recordVideoWidget;
	delete aboutWidget;
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
	if (scene->saveModified())
	{
		evt->accept();
		qApp->exit();
	}
	else
	{
		evt->ignore();
	}
}

void MainWindow::updateMousePos(QPoint mousePos)
{
	ui.statusBar->showMessage(QString("(%1, %2)").arg(mousePos.x()).arg(mousePos.y()));
}

void MainWindow::newActionTriggered()
{
	scene->create();
}

void MainWindow::openActionTriggered()
{
	scene->open();
}

void MainWindow::closeActionTriggered()
{
	scene->create();
}

void MainWindow::saveActionTriggered()
{
	scene->save();
}

void MainWindow::printActionTriggered()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer);
	if (printDialog.exec() == QDialog::Accepted)
	{
		QPainter painter;
		QImage &snapshot = scene->getSnapshot();

		float xscale = (float)printer.pageRect().width() / snapshot.width();
		float yscale = (float)printer.pageRect().height() / snapshot.height();
		float scale = qMin(xscale, yscale);

		painter.begin(&printer);

		painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2, 
			printer.paperRect().y() + printer.pageRect().height() / 2);
		painter.scale(scale, scale);
		painter.translate(-snapshot.width() / 2,
			-snapshot.height() / 2);

		painter.drawImage(QPoint(0, 0), snapshot);
		painter.end();
	}
}

void MainWindow::exportActionTriggered()
{
	QImage &snapshot = scene->getSnapshot();
	QString exportFileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Export"),
		"./untitled.jpg", tr("JPEG (*.jpg);;PNG (*.png)"));
	if (!exportFileName.isNull())
	{
		snapshot.save(exportFileName);
	}
}

void MainWindow::saveAsActionTriggered()
{
	scene->saveAs();
}

void MainWindow::propertiesActionTriggered()
{
	sceneProperty->show();
}

void MainWindow::exitActionTriggered()
{
	close();
}

void MainWindow::undoActionTriggered()
{
	scene->undo();
}

void MainWindow::redoActionTriggered()
{
	scene->redo();
}

void MainWindow::selectAllActionTriggered()
{
	scene->selectAll();
}

void MainWindow::clearAllActionTriggered()
{
	scene->clear();
}

void MainWindow::cutActionTriggered()
{
	scene->cut();
}

void MainWindow::copyActionTriggered()
{
	scene->copy();
}

void MainWindow::pasteActionTriggered()
{
	scene->paste();
}

void MainWindow::deleteActionTriggered()
{
	scene->remove();
}

void MainWindow::groupActionTriggered()
{
	scene->group();
}

void MainWindow::ungroupActionTriggered()
{
	scene->ungroup();
}

void MainWindow::recordVideoActionTriggered()
{
	recordVideoWidget->show();
}

void MainWindow::optionsActionTriggered()
{
	appConfig->setPanelSize(glm::ivec2(size().width(), size().height()));
	appConfig->setPanelPosition(glm::ivec2(pos().x(), pos().y()));
	appConfig->show();
}

void MainWindow::fullscreenActionTriggered()
{
	if (!isFullScreen())
	{
		showFullScreen();
		ui.menuBar->hide();
		ui.statusBar->hide();
		ui.mainToolBar->hide();
	}
	else
	{
		showNormal();
		ui.menuBar->show();
		ui.statusBar->show();
		ui.mainToolBar->show();
	}
}

void MainWindow::aboutActionTriggered()
{
	aboutWidget->show();
}

void MainWindow::sizeChanged(glm::ivec2 size)
{
	resize(size.x, size.y);
}

void MainWindow::positionChanged(glm::ivec2 position)
{
	move(position.x, position.y);
}

void MainWindow::recentProject(QString fileName)
{
	std::vector<QString> &recentProjectsFileNames = appConfig->getRecentProjectsFileNames();
	if (std::find(recentProjectsFileNames.begin(), recentProjectsFileNames.end(), fileName) == recentProjectsFileNames.end())
	{
		recentProjectsFileNames.push_back(fileName);
		addRecentProjectsAction(fileName);
	}
}

void MainWindow::recentProjectActionTriggered(RecentProjectAction *emitter, QString fileName)
{
	if (QFile::exists(fileName))
	{
		scene->open(fileName);
	}
	else
	{
		ui.action_Recent_Projects->menu()->removeAction(emitter);
		appConfig->removeRecentProjectsFileName(fileName);
		QMessageBox::critical(Q_NULLPTR, tr("Error"), tr("project file doesn't exist!"));
	}
}

void MainWindow::initRecentProjectsActions()
{
	QMenu *menu = new QMenu;
	ui.action_Recent_Projects->setMenu(menu);
	for (QString recentProjectFileName : appConfig->getRecentProjectsFileNames())
	{
		addRecentProjectsAction(recentProjectFileName);
	}
}

void MainWindow::addRecentProjectsAction(QString fileName)
{
	RecentProjectAction *recentProjectAction = new RecentProjectAction(fileName);
	ui.action_Recent_Projects->menu()->addAction(recentProjectAction);
	connect(recentProjectAction, SIGNAL(triggered(RecentProjectAction *, QString)), 
		this, SLOT(recentProjectActionTriggered(RecentProjectAction *, QString)));
}