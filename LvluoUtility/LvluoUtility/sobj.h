#ifndef SOBJ_H
#define SOBJ_H

#include "isobj.h"

class LVLUO_API SObj : public QObject, public ISObj
{
	Q_OBJECT

public:
	SObj(); // 空构造函数
	SObj(const Camera *camera, glm::ivec2 *sceneSize,
		QStringList ids, Dimension dimen); // 实例化构造函数
	SObj(SObj *other); // 拷贝构造函数

	virtual ~SObj(); // 虚析构函数

	virtual void destroy() = 0; // 显式地释放

	virtual void logic(float dt); // 逻辑
	virtual void render(bool blendOn = false, bool pickOn = false); // 绘制（默认不开启混合和拾取模式）

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void click(QPoint mousePos); // 鼠标点击事件
	virtual void rightClick(QPoint mousePos); // 鼠标右键点击事件函数

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

	virtual void updateWinObjRatio(); // 更新窗口坐标到物体坐标的比例

	virtual void select(); // 选择
	virtual void unSelect(); // 取消选择 
	virtual bool isSelected(); // 是否选择

	virtual void setShowAABB(bool flag); // 是否显示包围框
	virtual void setVisible(bool flag); // 是否可见

	virtual glm::vec3 getPosition() const; // 获取世界坐标
	virtual glm::vec3 getRotation() const; // 获取旋转
	virtual glm::ivec2 getXY() const; // 获取窗口坐标

	virtual QStringList getIDs() const; // 获取id串
	virtual QString getJoinID() const; // 获取连接的id串
	virtual int getID() const; // 获取id

	virtual glm::mat4 getModelMatrix(); // 获取模型矩阵
	virtual glm::vec3 getAABB(); // 获取包围框
	virtual QRect getRect(); // 获取窗口大小
	virtual void getAABBVertices(glm::vec3 aabbVertices[8]); // 获取包围框的角点
	virtual glm::vec3 getAABBCenter(glm::vec3 aabbVertices[8]); // 获取包围框中心

	virtual Dimension getDimension(); // 获取维度

	// 平移
	virtual void moveDZ(int dz);// 物体在深度（z轴）上的位移，dz为鼠标中键滚轮的格数
	virtual void moveDZ(float dz); // 物体在深度（z轴）上的位移，dz为世界坐标
	virtual void moveDXY(int dx, int dy); // 输入鼠标位移的偏移量，把屏幕的位移转换成物体在三维空间中的位移
	virtual void moveXY(int x, int y); // 输入鼠标位移的量，把屏幕的位移转换成物体在三维空间中的位移

	// 缩放
	virtual void scale(glm::vec3 ratio); // 缩放
	virtual void setBaseSize(glm::vec3 ratio); // 设置基础缩放

	// 旋转
	virtual void rotateDz(float angle); // 物体绕z轴旋转
	virtual void rotateDxy(float angleX, float angleY); // 物体绕xy轴旋转

	virtual void updateTransform(); // 更新变换
	virtual void setCenter(glm::vec3 center); // 设置变换中心
	virtual void setMoveDirection(MoveDirection moveDir, bool flag); // 设置移动方向

	virtual void showParamWidget(QPoint mousePos); // 显示属性窗口
	virtual void closeParamWidget(); // 关闭属性窗口
	virtual bool isShowParamWidget(); // 属性窗口是否显示

protected:
	void recordLastSnapshot(); // 保存操作前的快照
	void recordCurSnapshot(); // 保存当前快照

	glm::vec3 position; // 位置
	glm::vec3 rotation; // roll(绕x轴) yaw(绕y轴) pitch(绕z轴)
	glm::vec3 size; // 等比例大小

	glm::vec3 center; // 旋转缩放中心
	bool centerTranformOn; // 开启绕中心点变换模式

	glm::vec3 aabb; // AABB包围框大小
	glm::vec3 baseSize; // 缩放操作所基于的等比例大小
	glm::vec3 baseCenterSize;

	glm::mat4 m; // 模型矩阵

	Dimension dimen; // 维度
	QStringList ids; // 层级id

	int id; // sobj标识符
	glm::vec3 colorID; // 颜色标识符

	ParamWidget *paramWidget; // SObj属性窗口

	const float LayerDepthOffset = 0.0001f; // SObj内部图层的深度差

	bool isRecord; // 是否已经准备好数据
	bool isVisible; // 是否可以见

	const Camera *camera;

private:
	void init(); // 初始化
	glm::vec3 getColorID();
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _size); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	bool selected; // 是否被选中
	bool showAABB; // 是否显示锚点

	glm::vec3 centerRotation; // 绕变换中心旋转角度
	glm::vec3 centerSize; // 绕变换中心缩放系数

	glm::mat4 rawM; // 原始模型矩阵

	glm::vec2 winObjRatio; // 屏幕距离到世界距离的比例值

	// 物体的移动方向
	bool isUp, isDown, isLeft, isRight;

	// 右键属性菜单组件
	DoubleSpinBoxVec3 *positionDoubleSpinBoxVec3;
	DoubleSpinBoxVec3 *rotationDoubleSpinBoxVec3;
	DoubleSpinBoxVec3 *sizeDoubleSpinBoxVec3;

	SObj *lastSnapshot;
	SObj *curSnapshot;

	static int sObjID; // sobj的标志符
	glm::ivec2 *sceneSize; // 场景大小

private slots:
	void positionDoubleSpinBoxVec3Changed(glm::vec3 val);
	void rotationDoubleSpinBoxVec3Changed(glm::vec3 val);
	void sizeDoubleSpinBoxVec3Changed(glm::vec3 val);
};

#endif // SOBJ_H
