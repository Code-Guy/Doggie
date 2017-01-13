#ifndef GRATINGSOBJ_H
#define GRATINGSOBJ_H

#include "sobj.h"
#include "texture.h"

#include <QImage>
#include <QBitmap>

#include "lvluopluginapi.h"

class OpticalGratingSObj : public SObj
{
	Q_OBJECT

public:
	OpticalGratingSObj();
	OpticalGratingSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	OpticalGratingSObj(OpticalGratingSObj *other);

	virtual ~OpticalGratingSObj();

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void click(QPoint mousePos); // 鼠标点击事件函数

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

	enum BoundaryShape // 边界形状
	{
		Circle = 0, Ellipse, Rectangle, Square
	};
	enum MotionCurve // 运动曲线
	{
		Sin = 0, Cos
	};

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(int _boundaryShape, int _motionCurve, glm::vec3 _color,
		float _luminance, float _sf, float _tf, float _orientation, float _duration); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	void genGratingTexture(); // 生成光栅贴图
	
	BoundaryShape boundaryShape;
	MotionCurve motionCurve;
	glm::vec3 color; // 颜色
	float luminance; // 亮度
	float sf; // Spatial frequency
	float tf; // Temporal frequency
	float orientation; // 朝向
	float duration; // 播放时间

	QImage *gratingImg;
	Texture *gratingTex;

	QBitmap mask[4];

	float t; // 时间
	bool isUpdate; // 由于计算量较大，间隔一帧更新
	bool isPlay; // 是否要播放

	int maxSquareImgSize; // 旋转后的最大正方形大小

	const int GratingImageSize = 800; // 光栅图片的大小

	// 右键属性菜单组件
	QComboBox *boundaryShapeComboBox;
	QComboBox *motionCurveComboBox;
	ColorLabel *colorLabel;
	QDoubleSpinBox *luminanceDoubleSpinBox;
	QDoubleSpinBox *orientationDoubleSpinBox;
	QDoubleSpinBox *sfDoubleSpinBox;
	QDoubleSpinBox *tfDoubleSpinBox;
	QDoubleSpinBox *durationDoubleSpinBox;

private slots:
	void boundaryShapeComboBoxCurrentIndexChanged(int index);
	void motionCurveComboBoxCurrentIndexChanged(int index);
	void colorChanged(glm::vec3 color);
	void luminanceDoubleSpinBoxChanged(double val);
	void orientationDoubleSpinBoxChanged(double val);
	void sfDoubleSpinBoxChanged(double val);
	void tfDoubleSpinBoxChanged(double val);
	void durationDoubleSpinBoxChanged(double val);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // GRATINGSOBJ_H