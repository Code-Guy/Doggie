#ifndef DEMOSOBJ_H
#define DEMOSOBJ_H

#include "sobj.h" // ����SObj
#include "lvluopluginapi.h" // ����SObj�������

class DemoSObj : public SObj
{
	Q_OBJECT

public:
	DemoSObj(); // ��׼���캯��
	DemoSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids); // �������캯��
	DemoSObj(DemoSObj *other); // �չ��캯��

	virtual ~DemoSObj(); // ��������

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt); // �߼�
	virtual void render(bool blendOn = false, bool pickOn = false); // ����

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(glm::vec3 _color); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	// ����
	glm::vec3 color; // ��ɫ

	// �Ҽ����Բ˵����
	ColorLabel *colorLabel;

private slots:
	void colorChanged(glm::vec3 color);
};

// SObj����������ʵ�����ӿ�
extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject); // ��׼���캯��
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other); // �������캯��
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller(); // �չ��캯��

#endif // DEMOSOBJ_H