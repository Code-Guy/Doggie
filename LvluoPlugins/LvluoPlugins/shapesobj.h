#ifndef SHAPESOBJ_H
#define SHAPESOBJ_H

#include "sobj.h"
#include "texture.h"

#include "lvluopluginapi.h"

class ShapeSObj : public SObj
{
	Q_OBJECT

public:
	ShapeSObj();
	ShapeSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	ShapeSObj(ShapeSObj *other);

	virtual ~ShapeSObj();

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
	void inflateParamWidget(glm::vec3 _color); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	Texture *texture;
	glm::vec3 color;

	// �Ҽ����Բ˵����
	ColorLabel *colorLabel;

private slots:
	void colorChanged(glm::vec3 color);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // SHAPESOBJ_H
