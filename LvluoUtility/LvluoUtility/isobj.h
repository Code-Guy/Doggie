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

enum Dimension // ά�� ��2D��3D����
{
	TwoD, ThreeD
};

enum MoveDirection // �ƶ�����
{
	Up, Down, Left, Right
};

class LVLUO_API ISObj
{
public:
	virtual void destroy() = 0; // ��ʽ���ͷ�

	virtual void logic(float dt) = 0; // �߼�
	virtual void render(bool blendOn = false, bool pickOn = false) = 0; // ���ƣ�Ĭ�ϲ�������Ϻ�ʰȡģʽ��

	virtual void save(QJsonObject &jsonObject) = 0; // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()) = 0; // ����

	virtual void click(QPoint mousePos) = 0; // ������¼�
	virtual void rightClick(QPoint mousePos) = 0; // ����Ҽ�����¼�����

	virtual void getSnapshot(ISObj *snapshot) = 0; // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot) = 0; // ���ÿ���

	virtual void updateWinObjRatio() = 0; // ���´������굽��������ı���

	virtual void select() = 0; // ѡ��
	virtual void unSelect() = 0; // ȡ��ѡ�� 
	virtual bool isSelected() = 0; // �Ƿ�ѡ��

	virtual void setShowAABB(bool flag) = 0; // �Ƿ���ʾ��Χ��
	virtual void setVisible(bool flag) = 0; // �Ƿ�ɼ�

	virtual glm::vec3 getPosition() const = 0; // ��ȡ��������
	virtual glm::vec3 getRotation() const = 0; // ��ȡ��ת
	virtual glm::ivec2 getXY() const = 0; // ��ȡ��������

	virtual QStringList getIDs() const = 0; // ��ȡid��
	virtual QString getJoinID() const = 0; // ��ȡ���ӵ�id��
	virtual int getID() const = 0; // ��ȡid

	virtual glm::mat4 getModelMatrix() = 0; // ��ȡģ�;���
	virtual glm::vec3 getAABB() = 0; // ��ȡ��Χ��
	virtual QRect getRect() = 0; // ��ȡ���ڴ�С
	virtual void getAABBVertices(glm::vec3 aabbVertices[8]) = 0; // ��ȡ��Χ��Ľǵ�
	virtual glm::vec3 getAABBCenter(glm::vec3 aabbVertices[8]) = 0; // ��ȡ��Χ������

	virtual Dimension getDimension() = 0; // ��ȡά��

	// ƽ��
	virtual void moveDZ(int dz) = 0;// ��������ȣ�z�ᣩ�ϵ�λ�ƣ�dzΪ����м����ֵĸ���
	virtual void moveDZ(float dz) = 0; // ��������ȣ�z�ᣩ�ϵ�λ�ƣ�dzΪ��������
	virtual void moveDXY(int dx, int dy) = 0; // �������λ�Ƶ�ƫ����������Ļ��λ��ת������������ά�ռ��е�λ��
	virtual void moveXY(int x, int y) = 0; // �������λ�Ƶ���������Ļ��λ��ת������������ά�ռ��е�λ��

	// ����
	virtual void scale(glm::vec3 ratio) = 0; // ����
	virtual void setBaseSize(glm::vec3 ratio) = 0; // ���û�������

	// ��ת
	virtual void rotateDz(float angle) = 0; // ������z����ת
	virtual void rotateDxy(float angleX, float angleY) = 0; // ������xy����ת

	virtual void updateTransform() = 0; // ���±任
	virtual void setCenter(glm::vec3 center) = 0; // ���ñ任����
	virtual void setMoveDirection(MoveDirection moveDir, bool flag) = 0; // �����ƶ�����

	virtual void showParamWidget(QPoint mousePos) = 0; // ��ʾ���Դ���
	virtual void closeParamWidget() = 0; // �ر����Դ���
	virtual bool isShowParamWidget() = 0; // ���Դ����Ƿ���ʾ
};

#endif // ISOBJ_H
