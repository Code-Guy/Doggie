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

	virtual void destroy(); // 显式地释放

	virtual void logic(float dt);
	virtual void render(bool blendOn = false, bool pickOn = false);

	virtual void save(QJsonObject &jsonObject); // 保存
	virtual void load(QJsonObject jsonObject = QJsonObject()); // 载入

	virtual void getSnapshot(ISObj *snapshot); // 获取快照
	virtual void setSnapshot(ISObj *snapshot); // 设置快照

	enum Font
	{
		Kai = 0, Hei
	};

private:
	void init(); // 初始化
	void setParamWidget(); // 设置右键菜单
	void inflateParamWidget(glm::vec3 _color, QString _text, int _currentFontIndex, int _fontSize); // 填充右键菜单
	void connectParamWidget(); // 链接右键菜单
	void disconnectParamWidget(); // 取消链接右键菜单

	void setText(QString qstr);

	QString text;
	Font fontType; // 字体
	glm::vec3 color;
	int fontSize; // 字体大小

	TextAtlas *textAtlas;
	std::vector<std::vector<FT_ULong>> lines; // 用空格分割的N行文字
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	// 右键属性菜单组件
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
