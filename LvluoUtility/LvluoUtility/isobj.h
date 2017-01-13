#ifndef ISOBJ_H
#define ISOBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <QStringList>
#include <QRect>
#include <QObject>

#include <QJsonObject>

#include "paramwidget.h"
#include "camera.h"

#include "lvluoapi.h"

enum Dimension // 维度 分2D和3D两种
{
	TwoD, ThreeD
};

enum MoveDirection // 移动方向
{
	Up, Down, Left, Right
};

class LVLUO_API ISObj
{
public:
	virtual void destroy() = 0; // 显式地释放

	virtual void logic(float dt) = 0; // 逻辑
	virtual void render(bool blendOn = false, bool pickOn = false) = 0; // 绘制（默认不开启混合和拾取模式）

	virtual void save(QJsonObject &jsonObject) = 0; // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()) = 0; // 载入

	virtual void click(QPoint mousePos) = 0; // 鼠标点击事件
	virtual void rightClick(QPoint mousePos) = 0; // 鼠标右键点击事件函数

	virtual void getSnapshot(ISObj *snapshot) = 0; // 获取快照
	virtual void setSnapshot(ISObj *snapshot) = 0; // 设置快照

	virtual void updateWinObjRatio() = 0; // 更新窗口坐标到物体坐标的比例

	virtual void select() = 0; // 选择
	virtual void unSelect() = 0; // 取消选择 
	virtual bool isSelected() = 0; // 是否选择

	virtual void setShowAABB(bool flag) = 0; // 是否显示包围框
	virtual void setVisible(bool flag) = 0; // 是否可见

	virtual glm::vec3 getPosition() const = 0; // 获取世界坐标
	virtual glm::vec3 getRotation() const = 0; // 获取旋转
	virtual glm::ivec2 getXY() const = 0; // 获取窗口坐标

	virtual QStringList getIDs() const = 0; // 获取id串
	virtual QString getJoinID() const = 0; // 获取连接的id串
	virtual int getID() const = 0; // 获取id

	virtual glm::mat4 getModelMatrix() = 0; // 获取模型矩阵
	virtual glm::vec3 getAABB() = 0; // 获取包围框
	virtual QRect getRect() = 0; // 获取窗口大小
	virtual void getAABBVertices(glm::vec3 aabbVertices[8]) = 0; // 获取包围框的角点
	virtual glm::vec3 getAABBCenter(glm::vec3 aabbVertices[8]) = 0; // 获取包围框中心

	virtual Dimension getDimension() = 0; // 获取维度

	// 平移
	virtual void moveDZ(int dz) = 0;// 物体在深度（z轴）上的位移，dz为鼠标中键滚轮的格数
	virtual void moveDZ(float dz) = 0; // 物体在深度（z轴）上的位移，dz为世界坐标
	virtual void moveDXY(int dx, int dy) = 0; // 输入鼠标位移的偏移量，把屏幕的位移转换成物体在三维空间中的位移
	virtual void moveXY(int x, int y) = 0; // 输入鼠标位移的量，把屏幕的位移转换成物体在三维空间中的位移

	// 缩放
	virtual void scale(glm::vec3 ratio) = 0; // 缩放
	virtual void setBaseSize(glm::vec3 ratio) = 0; // 设置基础缩放

	// 旋转
	virtual void rotateDz(float angle) = 0; // 物体绕z轴旋转
	virtual void rotateDxy(float angleX, float angleY) = 0; // 物体绕xy轴旋转

	virtual void updateTransform() = 0; // 更新变换
	virtual void setCenter(glm::vec3 center) = 0; // 设置变换中心
	virtual void setMoveDirection(MoveDirection moveDir, bool flag) = 0; // 设置移动方向

	virtual void showParamWidget(QPoint mousePos) = 0; // 显示属性窗口
	virtual void closeParamWidget() = 0; // 关闭属性窗口
	virtual bool isShowParamWidget() = 0; // 属性窗口是否显示
};

#endif // ISOBJ_H
