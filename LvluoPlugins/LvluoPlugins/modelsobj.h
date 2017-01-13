#ifndef MODELSOBJ_H
#define MODELSOBJ_H

#include "sobj.h"
#include "model.h"
#include "light.h"

#include "lvluopluginapi.h"

class ModelSObj : public SObj
{
	Q_OBJECT

public:
	ModelSObj();
	ModelSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	ModelSObj(ModelSObj *other);

	virtual ~ModelSObj();

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
	void inflateParamWidget(glm::vec3 _dlDirection, glm::vec3 _dlAmbient, glm::vec3 _dlDiffuse, glm::vec3 _dlSpecular); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	Model *model;

	DirectionalLight dl;
	PointLight pls[PointLightNum];
	SpotLight sl;

	// �Ҽ����Բ˵����
	DoubleSpinBoxVec3 *dlDirectionDoubleSpinBoxVec3;
	ColorLabel *dlAmbientColorLabel;
	ColorLabel *dlDiffuseColorLabel;
	ColorLabel *dlSpecularColorLabel;

private slots:
	void dlDirectionDoubleSpinBoxVec3Changed(glm::vec3 val);
	void dlAmbientColorChanged(glm::vec3 color);
	void dlDiffuseColorChanged(glm::vec3 color);
	void dlSpecularColorChanged(glm::vec3 color);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // MODELSOBJ_H
