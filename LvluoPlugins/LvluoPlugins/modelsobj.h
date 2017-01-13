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

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(glm::vec3 _dlDirection, glm::vec3 _dlAmbient, glm::vec3 _dlDiffuse, glm::vec3 _dlSpecular); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	Model *model;

	DirectionalLight dl;
	PointLight pls[PointLightNum];
	SpotLight sl;

	// 右键属性菜单组件
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
