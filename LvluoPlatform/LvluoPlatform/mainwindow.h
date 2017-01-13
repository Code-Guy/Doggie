#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "scene.h"
#include "appconfig.h"
#include "sceneproperty.h"
#include "recordvideowidget.h"
#include "recentprojectaction.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	virtual void closeEvent(QCloseEvent *evt);

private slots:
	void updateMousePos(QPoint mousePos); // 更新鼠标位置

	void newActionTriggered();
	void openActionTriggered();
	void closeActionTriggered();
	void saveActionTriggered();
	void printActionTriggered();
	void exportActionTriggered();
	void saveAsActionTriggered();
	void propertiesActionTriggered();
	void exitActionTriggered();
	void undoActionTriggered();
	void redoActionTriggered();
	void selectAllActionTriggered();
	void clearAllActionTriggered();
	void cutActionTriggered();
	void copyActionTriggered();
	void pasteActionTriggered();
	void deleteActionTriggered();
	void groupActionTriggered();
	void ungroupActionTriggered();
	void recordVideoActionTriggered();
	void optionsActionTriggered();
	void fullscreenActionTriggered();
	void aboutActionTriggered();

	void sizeChanged(glm::ivec2 size);
	void positionChanged(glm::ivec2 position);

	void recentProject(QString fileName);
	void recentProjectActionTriggered(RecentProjectAction *emitter, QString fileName);

private:
	void initRecentProjectsActions();
	void addRecentProjectsAction(QString fileName);

	Ui::MainWindowClass ui;

	Scene *scene;
	AppConfig *appConfig;
	SceneProperty *sceneProperty;
	QWidget *aboutWidget;
	RecordVideoWidget *recordVideoWidget;
};

#endif // MAINWINDOW_H
