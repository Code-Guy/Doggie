#ifndef TEXTSOBJ_H
#define TEXTSOBJ_H

#include "sobj.h"
#include "textatlas.h"

#include "lvluopluginapi.h"

class TextSObj : public SObj
{
	Q_OBJECT

public:
	TextSObj();
	TextSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids);
	TextSObj(TextSObj *other);

	virtual ~TextSObj();

	virtual void destroy(); // ��ʽ���ͷ�

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // ����
	virtual void load(QJsonObject jsonObject = QJsonObject()); // ����

	virtual void getSnapshot(ISObj *snapshot); // ��ȡ����
	virtual void setSnapshot(ISObj *snapshot); // ���ÿ���

	enum Font
	{
		Kai = 0, Hei
	};

private:
	void init(); // ��ʼ��
	void setParamWidget(); // �����Ҽ��˵�
	void inflateParamWidget(glm::vec3 _color, QString _text, int _currentFontIndex, int _fontSize); // ����Ҽ��˵�
	void connectParamWidget(); // �����Ҽ��˵�
	void disconnectParamWidget(); // ȡ�������Ҽ��˵�

	void setText(QString qstr);

	QString text;
	Font fontType; // ����
	glm::vec3 color;
	int fontSize; // �����С

	TextAtlas *textAtlas;
	std::vector<std::vector<FT_ULong>> lines; // �ÿո�ָ��N������
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	// �Ҽ����Բ˵����
	QTextEdit *textEdit;
	ColorLabel *colorLabel;
	QComboBox *fontComboBox;
	QSpinBox *fontSizeSpinBox;

private slots:
	void textChanged();
	void colorChanged(glm::vec3 color);
	void fontChanged(int index);
	void fontSizeChanged(int val);
};

extern "C" LVLUOPLUGIN_API ISObj * __cdecl construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl copyer(ISObj *other);
extern "C" LVLUOPLUGIN_API ISObj * __cdecl nuller();

#endif // TEXTSOBJ_H
