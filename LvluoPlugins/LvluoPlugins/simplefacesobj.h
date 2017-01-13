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

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void click(QPoint mousePos); // ������¼�����
	virtual void rightClick(QPoint mousePos); // ����Ҽ�����¼�����

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

private:
	struct Point // ������
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

	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(int _pointSize, glm::vec3 _pointColor,
		glm::vec3 _bgPointMoveDir, int _bgPointSpawnSpeed, float _bgPointMoveSpeed, bool _bgPointThroughFace,
		int _facePointSpawnSpeed, float _facePointLifeTime); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void emitBgPoints(float dt);
	void updateBgPoints(float dt);

	void emitFacePoints(float dt);
	void updateFacePoints(float dt);

	glm::vec3 uniformEllipseSurfaceSample(glm::vec3 ellipse); // ����������ֲ�
	float randomRange(float lower, float upper);
	
	bool checkBgPointVisibility(Point &point);

	std::vector<Point> bgPoints; // ������
	std::vector<Point> facePoints; // ������
	std::vector<glm::vec3> bgPointPositions; // ������λ��
	std::vector<glm::vec3> facePointPositions; // ������λ��

	float bgPointLifeTime; // ����������
	glm::vec3 faceEllipse; // ��������

	bool isPlay; // �Ƿ�Ҫ����

	// ����
	// point
	int pointSize; // ��Ĵ�С
	glm::vec3 pointColor; // �����ɫ
	// background
	glm::vec3 bgPointMoveDir; // �����˶�����
	int bgPointSpawnSpeed; // �����������ٶ�
	float bgPointMoveSpeed; // �������˶��ٶ�
	bool bgPointThroughFace; // �Ƿ񴩹�����

	// face
	int facePointSpawnSpeed; // �����������ٶ�
	float facePointLifeTime; // ����������

	// �Ҽ����Բ˵����
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
