#ifndef FACESOBJ_H
#define FACESOBJ_H

#include "sobj.h"
#include "texture.h"
#include "wave.h"

#include "lvluopluginapi.h"

class FaceSObj : public SObj
{
	Q_OBJECT

public:
	FaceSObj();
	FaceSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	FaceSObj(FaceSObj *other);

	virtual ~FaceSObj();

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void click(QPoint mousePos); // 鼠标点击事件函数
	virtual void rightClick(QPoint mousePos); // 鼠标右键点击事件函数

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(int _waveSpreadLayer, int _waveSpawnSpeed,
		int _pointSpeed, float _pointSize, glm::vec3 _pointColor); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	bool loadFaceModel(QString fileName);

	void spawnWave(float dt);
	void updateWave(float dt);

	int waveSpreadLayer; // wave传播层数
	int waveSpawnSpeed; // wave出生的速度
	int pointSpeed; // 点的速度（每秒传播多少个模型点）
	float pointSize; // 点的大小
	glm::vec3 pointColor; // 点的颜色

	std::vector<glm::vec3> positions; // 随机点坐标

	std::vector<glm::vec3> verts; // 模型点坐标
	std::vector<std::vector<int>> net; // 模型网格
	
	std::vector<Wave *> waves;

	bool isPlay; // 是否要播放

	// 右键属性菜单组件
	QSpinBox *waveSpreadLayerSpinBox;
	QSpinBox *waveSpawnSpeedSpinBox;
	QSpinBox *pointSpeedSpinBox;
	QDoubleSpinBox *pointSizeDoubleSpinBox;
	ColorLabel *pointColorLabel;

private slots:
	void waveSpreadLayerSpinBoxChanged(int val);
	void waveSpawnSpeedSpinBoxChanged(int val);
	void pointSpeedSpinBoxChanged(int val);
	void pointSizeDoubleSpinBoxChanged(double val);
	void pointColorChanged(glm::vec3 color);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // FACESOBJ_H
