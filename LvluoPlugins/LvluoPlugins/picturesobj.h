#ifndef PICTURESOBJ_H
#define PICTURESOBJ_H

#include "sobj.h"
#include "texture.h"

#include "lvluopluginapi.h"

class PictureSObj : public SObj
{
	Q_OBJECT

public:
	PictureSObj();
	PictureSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	PictureSObj(PictureSObj *other);

	virtual ~PictureSObj();

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(QString _fileName); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void setTexture(QString fileName);

	Texture *texture;
	Texture *defaultTex;

	QString fileName; // ͼƬ�ļ���

	// �Ҽ����Բ˵����
	FileWidget *fileWidget;

private slots:
	void fileNameChanged(QString fileName);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // PICTURESOBJ_H
