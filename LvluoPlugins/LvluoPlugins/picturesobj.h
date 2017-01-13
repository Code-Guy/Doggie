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
	void inflateParamWidget(QString _fileName); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	void setTexture(QString fileName);

	Texture *texture;
	Texture *defaultTex;

	QString fileName; // 图片文件名

	// 右键属性菜单组件
	FileWidget *fileWidget;

private slots:
	void fileNameChanged(QString fileName);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // PICTURESOBJ_H
