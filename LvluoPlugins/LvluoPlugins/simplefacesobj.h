#ifndef SIMPLEFACESOBJ_H
#define SIMPLEFACESOBJ_H

#include "sobj.h"
#include "lvluopluginapi.h"

class SimpleFaceSObj : public SObj
{
	Q_OBJECT

public:
	SimpleFaceSObj();
	SimpleFaceSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	SimpleFaceSObj(SimpleFaceSObj *other);

	virtual ~SimpleFaceSObj();

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void click(QPoint mousePos); // 鼠标点击事件函数
	virtual void rightClick(QPoint mousePos); // 鼠标右键点击事件函数

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

private:
	struct Point // 背景点
	{
		Point(glm::vec3 position) : position(position)
		{
			t = 0.0f;
			visible = true;
		}

		void setVisible(bool flag)
		{
			visible = flag;
		}

		glm::vec3 position;
		float t;
		bool visible;
	};

	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(int _pointSize, glm::vec3 _pointColor,
		glm::vec3 _bgPointMoveDir, int _bgPointSpawnSpeed, float _bgPointMoveSpeed, bool _bgPointThroughFace,
		int _facePointSpawnSpeed, float _facePointLifeTime); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	void emitBgPoints(float dt);
	void updateBgPoints(float dt);

	void emitFacePoints(float dt);
	void updateFacePoints(float dt);

	glm::vec3 uniformEllipseSurfaceSample(glm::vec3 ellipse); // 均匀椭球面分布
	float randomRange(float lower, float upper);
	
	bool checkBgPointVisibility(Point &point);

	std::vector<Point> bgPoints; // 背景点
	std::vector<Point> facePoints; // 人脸点
	std::vector<glm::vec3> bgPointPositions; // 背景点位置
	std::vector<glm::vec3> facePointPositions; // 人脸点位置

	float bgPointLifeTime; // 背景点寿命
	glm::vec3 faceEllipse; // 人脸椭球

	bool isPlay; // 是否要播放

	// 属性
	// point
	int pointSize; // 点的大小
	glm::vec3 pointColor; // 点的颜色
	// background
	glm::vec3 bgPointMoveDir; // 背景运动方向
	int bgPointSpawnSpeed; // 背景点生成速度
	float bgPointMoveSpeed; // 背景点运动速度
	bool bgPointThroughFace; // 是否穿过人脸

	// face
	int facePointSpawnSpeed; // 人脸点生成速度
	float facePointLifeTime; // 人脸点寿命

	// 右键属性菜单组件
	QSpinBox *pointSizeSpinBox;
	ColorLabel *pointColorLabel;
	DoubleSpinBoxVec3 *bgPointMoveDirDoubleSpinBoxVec3;
	QSpinBox *bgPointSpawnSpeedSpinBox;
	QDoubleSpinBox *bgPointMoveSpeedDoubleSpinBox;
	QCheckBox *bgPointThroughFaceCheckBox;
	QSpinBox *facePointSpawnSpeedSpinBox;
	QDoubleSpinBox *facePointLifeTimeDoubleSpinBox;

private slots:
	void pointSizeSpinBoxValueChanged(int val);
	void pointColorLabelColorChanged(glm::vec3 color);
	void bgPointMoveDirDoubleSpinBoxVec3ValueChanged(glm::vec3 val);
	void bgPointSpawnSpeedSpinBoxValueChanged(int val);
	void bgPointMoveSpeedDoubleSpinBoxValueChanged(double val);
	void bgPointThroughFaceCheckBoxStateChanged(int val);
	void throughFaceCheckBoxChanged(int val);
	void facePointSpawnSpeedSpinBoxValueChanged(int val);
	void facePointLifeTimeDoubleSpinBoxValueChanged(double val);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // SIMPLEFACESOBJ_H
