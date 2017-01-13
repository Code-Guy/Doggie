#ifndef DEMOSOBJ_H
#define DEMOSOBJ_H

#include "sobj.h" // 父类SObj
#include "lvluopluginapi.h" // 定义SObj插件导出

class DemoSObj : public SObj
{
	Q_OBJECT

public:
	DemoSObj(); // 标准构造函数
	DemoSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids); // 拷贝构造函数
	DemoSObj(DemoSObj *other); // 空构造函数

	virtual ~DemoSObj(); // 析构函数

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt); // 逻辑
	virtual void render(bool blendOn = false, bool pickOn = false); // 绘制

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

	// 属性
	glm::vec3 color; // 颜色

	// 右键属性菜单组件
	ColorLabel *colorLabel;

private slots:
	void colorChanged(glm::vec3 color);
};

// SObj插件类的三个实例化接口
extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject); // 标准构造函数
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other); // 拷贝构造函数
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller(); // 空构造函数

#endif // DEMOSOBJ_H