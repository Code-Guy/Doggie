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
	virtual void logic(float dt); // �߼�
	virtual void render(); // ����

	virtual QPaintEngine* paintEngine() const { return NULL; } // ���������˸
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
	void updateMousePos(QPoint mousePos); // �������λ��
	void recentProject(QString fileName); // ��������ļ�
	void setSnapshot(const QImage &snapshot);

private slots:
	void updateCursorShape(Operator::CustomCursorShape cursorShape);
	void querySnapshot();

private:
	void initWidgetProps(); // ��ʼ��widget����
	void initWindowContext(); // ��ʼ��window����context
	void initGLEW(); // ��ʼ��Glew
	void initGLStates(); // ��ʼ��opengl״̬
	void initPlugins(); // ��ʼ��SObj���

	void parseIDs(std::vector<QStringList> &sObjIDs, std::map<QString, std::vector<QStringList>> &idssMap);
	std::vector<SObjIcon *> genSObjIcons(const std::vector<QStringList> &layerSObjIDs, int layer = 0); // �ݹ�������Ҽ������˵�
	QPoint adjustSObjSelectionWidgetPos(QPoint originPos);

	void save(QString projectFileName);
	void renderImpl();

	void timeOut();

	glm::ivec2 sceneSize; // ������С
	QTimer updateTimer; // ���¼�ʱ��
	QTime lastTime;

	std::vector<ISObj *> sObjs; // Stimulus Objects

	// ���SObj��·�����ļ���
	std::vector<QString> pluginPaths;
	std::vector<QString> pluginNames;

	std::vector<HINSTANCE> pluginHandles;

	SObjSelectionWidget *sObjSelectionWidget; // �Ҽ�ISObjѡ�񴰿�
	Operator *op;
	Camera *camera;
	Grid *grid;
	MultiSampleFBO *multiSampleFBO;

	QString curProjectFileName; // �򿪵Ĺ����ļ���

	QImage snapshot; // ����
};

struct NearFirstComparer
{
	bool operator() (const ISObj *lhs, const ISObj *rhs)
	{
		return lhs->getPosition().z < rhs->getPosition().z;
	}
};

#endif // SCENE_H