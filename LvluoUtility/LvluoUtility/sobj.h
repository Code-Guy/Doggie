#ifndef SOBJ_H
#define SOBJ_H

#include "isobj.h"

class LVLUO_API SObj : public QObject, public ISObj
{
	Q_OBJECT

public:
	SObj(); // �չ��캯��
	SObj(const Camera *camera, glm::ivec2 *sceneSize,
		QStringList ids, Dimension dimen); // ʵ�������캯��
	SObj(SObj *other); // �������캯��

	virtual ~SObj(); // ����������

	virtual void destroy() = 0; // ��ʽ���ͷ�

	virtual void logic(float dt); // �߼�
	virtual void render(bool blendOn = false, bool pickOn = false); // ���ƣ�Ĭ�ϲ�������Ϻ�ʰȡģʽ��

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void click(QPoint mousePos); // ������¼�
	virtual void rightClick(QPoint mousePos); // ����Ҽ�����¼�����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

	virtual void updateWinObjRatio(); // ���´������굽��������ı���

	virtual void select(); // ѡ��
	virtual void unSelect(); // ȡ��ѡ�� 
	virtual bool isSelected(); // �Ƿ�ѡ��

	virtual void setShowAABB(bool flag); // �Ƿ���ʾ��Χ��
	virtual void setVisible(bool flag); // �Ƿ�ɼ�

	virtual glm::vec3 getPosition() const; // ��ȡ��������
	virtual glm::vec3 getRotation() const; // ��ȡ��ת
	virtual glm::ivec2 getXY() const; // ��ȡ��������

	virtual QStringList getIDs() const; // ��ȡid��
	virtual QString getJoinID() const; // ��ȡ���ӵ�id��
	virtual int getID() const; // ��ȡid

	virtual glm::mat4 getModelMatrix(); // ��ȡģ�;���
	virtual glm::vec3 getAABB(); // ��ȡ��Χ��
	virtual QRect getRect(); // ��ȡ���ڴ�С
	virtual void getAABBVertices(glm::vec3 aabbVertices[8]); // ��ȡ��Χ��Ľǵ�
	virtual glm::vec3 getAABBCenter(glm::vec3 aabbVertices[8]); // ��ȡ��Χ������

	virtual Dimension getDimension(); // ��ȡά��

	// ƽ��
	virtual void moveDZ(int dz);// ��������ȣ�z�ᣩ�ϵ�λ�ƣ�dzΪ����м����ֵĸ���
	virtual void moveDZ(float dz); // ��������ȣ�z�ᣩ�ϵ�λ�ƣ�dzΪ��������
	virtual void moveDXY(int dx, int dy); // �������λ�Ƶ�ƫ����������Ļ��λ��ת������������ά�ռ��е�λ��
	virtual void moveXY(int x, int y); // �������λ�Ƶ���������Ļ��λ��ת������������ά�ռ��е�λ��

	// ����
	virtual void scale(glm::vec3 ratio); // ����
	virtual void setBaseSize(glm::vec3 ratio); // ���û�������

	// ��ת
	virtual void rotateDz(float angle); // ������z����ת
	virtual void rotateDxy(float angleX, float angleY); // ������xy����ת

	virtual void updateTransform(); // ���±任
	virtual void setCenter(glm::vec3 center); // ���ñ任����
	virtual void setMoveDirection(MoveDirection moveDir, bool flag); // �����ƶ�����

	virtual void showParamWidget(QPoint mousePos); // ��ʾ���Դ���
	virtual void closeParamWidget(); // �ر����Դ���
	virtual bool isShowParamWidget(); // ���Դ����Ƿ���ʾ

protected:
	void recordLastSnapshot(); // �������ǰ�Ŀ���
	void recordCurSnapshot(); // ���浱ǰ����

	glm::vec3 position; // λ��
	glm::vec3 rotation; // roll(��x��) yaw(��y��) pitch(��z��)
	glm::vec3 size; // �ȱ�����С

	glm::vec3 center; // ��ת��������
	bool centerTranformOn; // ���������ĵ�任ģʽ

	glm::vec3 aabb; // AABB��Χ���С
	glm::vec3 baseSize; // ���Ų��������ڵĵȱ�����С
	glm::vec3 baseCenterSize;

	glm::mat4 m; // ģ�;���

	Dimension dimen; // ά��
	QStringList ids; // �㼶id

	int id; // sobj��ʶ��
	glm::vec3 colorID; // ��ɫ��ʶ��

	ParamWidget *paramWidget; // SObj���Դ���

	const float LayerDepthOffset = 0.0001f; // SObj�ڲ�ͼ�����Ȳ�

	bool isRecord; // �Ƿ��Ѿ�׼��������
	bool isVisible; // �Ƿ���Լ�

	const Camera *camera;

private:
	void init(); // ��ʼ��
	glm::vec3 getColorID();
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _size); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	bool selected; // �Ƿ�ѡ��
	bool showAABB; // �Ƿ���ʾê��

	glm::vec3 centerRotation; // �Ʊ任������ת�Ƕ�
	glm::vec3 centerSize; // �Ʊ任��������ϵ��

	glm::mat4 rawM; // ԭʼģ�;���

	glm::vec2 winObjRatio; // ��Ļ���뵽�������ı���ֵ

	// ������ƶ�����
	bool isUp, isDown, isLeft, isRight;

	// �Ҽ����Բ˵����
	DoubleSpinBoxVec3 *positionDoubleSpinBoxVec3;
	DoubleSpinBoxVec3 *rotationDoubleSpinBoxVec3;
	DoubleSpinBoxVec3 *sizeDoubleSpinBoxVec3;

	SObj *lastSnapshot;
	SObj *curSnapshot;

	static int sObjID; // sobj�ı�־��
	glm::ivec2 *sceneSize; // ������С

private slots:
	void positionDoubleSpinBoxVec3Changed(glm::vec3 val);
	void rotationDoubleSpinBoxVec3Changed(glm::vec3 val);
	void sizeDoubleSpinBoxVec3Changed(glm::vec3 val);
};

#endif // SOBJ_H
