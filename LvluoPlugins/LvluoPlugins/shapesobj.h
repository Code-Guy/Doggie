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
	void inflateParamWidget(glm::vec3 _color); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	Texture *texture;
	glm::vec3 color;

	// 右键属性菜单组件
	ColorLabel *colorLabel;

private slots:
	void colorChanged(glm::vec3 color);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // SHAPESOBJ_H
