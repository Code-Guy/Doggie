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

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void click(QPoint mousePos); // ������¼�����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

	enum BoundaryShape // �߽���״
	{
		Circle = 0, Ellipse, Rectangle, Square
	};
	enum MotionCurve // �˶�����
	{
		Sin = 0, Cos
	};

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(int _boundaryShape, int _motionCurve, glm::vec3 _color,
		float _luminance, float _sf, float _tf, float _orientation, float _duration); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void genGratingTexture(); // ���ɹ�դ��ͼ
	
	BoundaryShape boundaryShape;
	MotionCurve motionCurve;
	glm::vec3 color; // ��ɫ
	float luminance; // ����
	float sf; // Spatial frequency
	float tf; // Temporal frequency
	float orientation; // ����
	float duration; // ����ʱ��

	QImage *gratingImg;
	Texture *gratingTex;

	QBitmap mask[4];

	float t; // ʱ��
	bool isUpdate; // ���ڼ������ϴ󣬼��һ֡����
	bool isPlay; // �Ƿ�Ҫ����

	int maxSquareImgSize; // ��ת�����������δ�С

	const int GratingImageSize = 800; // ��դͼƬ�Ĵ�С

	// �Ҽ����Բ˵����
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