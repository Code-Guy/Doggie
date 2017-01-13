#ifndef FLOWSOBJ_H
#define FLOWSOBJ_H

#include "sobj.h"
#include "texture.h"

#include "lvluopluginapi.h"

class OpticalFlowSObj : public SObj
{
	Q_OBJECT

public:
	OpticalFlowSObj();
	OpticalFlowSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	OpticalFlowSObj(OpticalFlowSObj *other);

	virtual ~OpticalFlowSObj();

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void click(QPoint mousePos); // 鼠标点击事件函数
	virtual void rightClick(QPoint mousePos); // 鼠标右键点击事件函数

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

	enum StarShape
	{
		Triangle = 0, Dot = 1
	};

	enum SpaceShape
	{
		Cube = 0, Sphere = 1
	};

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(int _spaceShape, glm::vec3 _spaceSize,
		int _starShape, float _starSize, float _starDensity, glm::vec3 _starColor,
		bool _lifeTimeOn, int _lifeTime,
		int _coherence,
		float _elevation, std::vector<float> _azimuths, float _magnitude,
		float _duration, float _sigma, glm::vec3 _offset, int _division); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	void genStars(); // 生成星星
	void resampleStars(); // 重新采样星星
	void moveStars(); // 移动星星

	float gaussian(float t); // 高斯分布
	QString getAzimuthsStr();
	std::vector<float> fromAzimuthsStr(QString str);

	struct Star
	{
		Star(glm::vec3 position) : 
			position(position), isMove(true)
		{

		}

		bool operator<(const Star &other)
		{
			return position.z < other.position.z;
		}

		glm::vec3 position; // 当前位置
		bool isMove; // 是否运动
	};

	SpaceShape spaceShape; // 空间的形状
	glm::vec3 spaceSize; // 空间的大小
	StarShape starShape; // 星星的形状
	float starSize; // 星星的大小
	float starDensity; // 星星的密度
	glm::vec3 starColor; // 星星的颜色
	bool lifeTimeOn; // 是否开启重置时间
	int lifeTime; // 到达重置时间，%x以外的星星就重新生成随机位置
	int coherence; // %x
	float elevation; // 垂直角
	std::vector<float> azimuths; // 水平角序列
	float magnitude; // 模
	float duration; // 总的运动时间
	float sigma; // 平方差
	glm::vec3 offset; // 位置偏移量
	int division; // 每秒刷新率

	std::vector<Star> stars; // 星星
	Texture *starTex[2]; // 星星的贴图
	glm::vec3 dir; // 高斯运动方向

	bool isPlay; // 是否要播放

	float t; // 计时器
	float c; // 逻辑帧计数器
	float oc; // 原始逻辑帧计数器

	int f; // 绘制帧计数器
	int a; // azimuth动画的计数器

	glm::vec3 baseAABB;
	std::vector<glm::vec3> positions;

	// 右键属性菜单组件
	QComboBox *spaceShapeComboBox;
	DoubleSpinBoxVec3 *spaceSizeDoubleSpinBoxVec3;
	QComboBox *starShapeComboBox;
	QDoubleSpinBox *starSizeDoubleSpinBox;
	QSpinBox *starDensitySpinBox;
	ColorLabel *colorLabel;
	QCheckBox *lifeTimeOnCheckBox;
	QSpinBox *lifeTimeSpinBox;
	QSpinBox *coherenceSpinBox;
	QDoubleSpinBox *elevationDoubleSpinBox;
	DoubleSpinBoxList *azimuthDoubleSpinBoxList;
	QDoubleSpinBox *magnitudeDoubleSpinBox;
	QDoubleSpinBox *durationDoubleSpinBox;
	QDoubleSpinBox *sigmaDoubleSpinBox;
	DoubleSpinBoxVec3 *offsetDoubleSpinBoxVec3;
	QSpinBox *divisionSpinBox;

private slots:
	void spaceShapeComboBoxComboBoxCurrentIndexChanged(int index);
	void spaceSizeDoubleSpinBoxVec3Changed(glm::vec3 val);
	void starShapeComboBoxComboBoxCurrentIndexChanged(int index);
	void starSizeDoubleSpinBoxChanged(double val);
	void starDensitySpinBoxChanged(int val);
	void colorChanged(glm::vec3 color);
	void lifeTimeOnCheckBoxStateChanged(int state);
	void lifeTimeSpinBoxChanged(int val);
	void coherenceSpinBoxChanged(int val);
	void elevationDoubleSpinBoxChanged(double val);
	void azimuthDoubleSpinBoxListValueChanged(std::vector<float> vals);
	void magnitudeDoubleSpinBoxChanged(double val);
	void durationDoubleSpinBoxChanged(double val);
	void sigmaDoubleSpinBoxChanged(double val);
	void offsetDoubleSpinBoxVec3Changed(glm::vec3 val);
	void divisionSpinBoxChanged(int val);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // FLOWSOBJ_H