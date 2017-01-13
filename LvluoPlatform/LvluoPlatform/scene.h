#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>
#include <Windows.h>

#include "sobjselectionwidget.h"
#include "camera.h"
#include "operator.h"
#include "grid.h"
#include "multisamplefbo.h"

class Scene : public QWidget
{
	Q_OBJECT

public:
	Scene(int fps = 60, QWidget *parent = 0);
	~Scene();

	glm::ivec4 getViewport();
	glm::ivec2 getPosition();
	QImage &getSnapshot();

	void dragSObj(QStringList ids, QPoint mousePos);

	bool create();
	void open(QString projectFileName = QString());
	bool save();
	bool saveAs();
	bool saveModified();
	void clear();

	void copy();
	void cut();
	void paste();
	void remove();
	void selectAll();

	void undo();
	void redo();

	void group();
	void ungroup();

protected:
	virtual void logic(float dt); // 逻辑
	virtual void render(); // 绘制

	virtual QPaintEngine* paintEngine() const { return NULL; } // 避免界面闪烁
	virtual void paintEvent(QPaintEvent* evt);
	virtual void resizeEvent(QResizeEvent *evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseReleaseEvent(QMouseEvent *evt);
	virtual void mouseMoveEvent(QMouseEvent *evt);
	virtual void mouseDoubleClickEvent(QMouseEvent *evt);
	virtual void keyPressEvent(QKeyEvent *evt);
	virtual void keyReleaseEvent(QKeyEvent *evt);
	virtual void wheelEvent(QWheelEvent *evt);
	virtual void focusInEvent(QFocusEvent *evt);

signals:
	void updateMousePos(QPoint mousePos); // 更新鼠标位置
	void recentProject(QString fileName); // 最近工程文件
	void setSnapshot(const QImage &snapshot);

private slots:
	void updateCursorShape(Operator::CustomCursorShape cursorShape);
	void querySnapshot();

private:
	void initWidgetProps(); // 初始化widget属性
	void initWindowContext(); // 初始化window界面context
	void initGLEW(); // 初始化Glew
	void initGLStates(); // 初始化opengl状态
	void initPlugins(); // 初始化SObj插件

	void parseIDs(std::vector<QStringList> &sObjIDs, std::map<QString, std::vector<QStringList>> &idssMap);
	std::vector<SObjIcon *> genSObjIcons(const std::vector<QStringList> &layerSObjIDs, int layer = 0); // 递归地生成右键级联菜单
	QPoint adjustSObjSelectionWidgetPos(QPoint originPos);

	void save(QString projectFileName);
	void renderImpl();

	void timeOut();

	glm::ivec2 sceneSize; // 场景大小
	QTimer updateTimer; // 更新计时器
	QTime lastTime;

	std::vector<ISObj *> sObjs; // Stimulus Objects

	// 插件SObj的路径和文件名
	std::vector<QString> pluginPaths;
	std::vector<QString> pluginNames;

	std::vector<HINSTANCE> pluginHandles;

	SObjSelectionWidget *sObjSelectionWidget; // 右键ISObj选择窗口
	Operator *op;
	Camera *camera;
	Grid *grid;
	MultiSampleFBO *multiSampleFBO;

	QString curProjectFileName; // 打开的工程文件名

	QImage snapshot; // 快照
};

struct NearFirstComparer
{
	bool operator() (const ISObj *lhs, const ISObj *rhs)
	{
		return lhs->getPosition().z < rhs->getPosition().z;
	}
};

#endif // SCENE_H