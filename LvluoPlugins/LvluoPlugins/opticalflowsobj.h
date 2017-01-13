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

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void click(QPoint mousePos); // ������¼�����
	virtual void rightClick(QPoint mousePos); // ����Ҽ�����¼�����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

	enum StarShape
	{
		Triangle = 0, Dot = 1
	};

	enum SpaceShape
	{
		Cube = 0, Sphere = 1
	};

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(int _spaceShape, glm::vec3 _spaceSize,
		int _starShape, float _starSize, float _starDensity, glm::vec3 _starColor,
		bool _lifeTimeOn, int _lifeTime,
		int _coherence,
		float _elevation, std::vector<float> _azimuths, float _magnitude,
		float _duration, float _sigma, glm::vec3 _offset, int _division); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void genStars(); // ��������
	void resampleStars(); // ���²�������
	void moveStars(); // �ƶ�����

	float gaussian(float t); // ��˹�ֲ�
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

		glm::vec3 position; // ��ǰλ��
		bool isMove; // �Ƿ��˶�
	};

	SpaceShape spaceShape; // �ռ����״
	glm::vec3 spaceSize; // �ռ�Ĵ�С
	StarShape starShape; // ���ǵ���״
	float starSize; // ���ǵĴ�С
	float starDensity; // ���ǵ��ܶ�
	glm::vec3 starColor; // ���ǵ���ɫ
	bool lifeTimeOn; // �Ƿ�������ʱ��
	int lifeTime; // ��������ʱ�䣬%x��������Ǿ������������λ��
	int coherence; // %x
	float elevation; // ��ֱ��
	std::vector<float> azimuths; // ˮƽ������
	float magnitude; // ģ
	float duration; // �ܵ��˶�ʱ��
	float sigma; // ƽ����
	glm::vec3 offset; // λ��ƫ����
	int division; // ÿ��ˢ����

	std::vector<Star> stars; // ����
	Texture *starTex[2]; // ���ǵ���ͼ
	glm::vec3 dir; // ��˹�˶�����

	bool isPlay; // �Ƿ�Ҫ����

	float t; // ��ʱ��
	float c; // �߼�֡������
	float oc; // ԭʼ�߼�֡������

	int f; // ����֡������
	int a; // azimuth�����ļ�����

	glm::vec3 baseAABB;
	std::vector<glm::vec3> positions;

	// �Ҽ����Բ˵����
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