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

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void click(QPoint mousePos); // ������¼�����
	virtual void rightClick(QPoint mousePos); // ����Ҽ�����¼�����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(int _waveSpreadLayer, int _waveSpawnSpeed,
		int _pointSpeed, float _pointSize, glm::vec3 _pointColor); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	bool loadFaceModel(QString fileName);

	void spawnWave(float dt);
	void updateWave(float dt);

	int waveSpreadLayer; // wave��������
	int waveSpawnSpeed; // wave�������ٶ�
	int pointSpeed; // ����ٶȣ�ÿ�봫�����ٸ�ģ�͵㣩
	float pointSize; // ��Ĵ�С
	glm::vec3 pointColor; // �����ɫ

	std::vector<glm::vec3> positions; // ���������

	std::vector<glm::vec3> verts; // ģ�͵�����
	std::vector<std::vector<int>> net; // ģ������
	
	std::vector<Wave *> waves;

	bool isPlay; // �Ƿ�Ҫ����

	// �Ҽ����Բ˵����
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
