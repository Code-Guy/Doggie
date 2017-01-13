#include "textsobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

#include <algorithm>

// 默认初始化数据
const QString DefaultText = QString::fromLocal8Bit("请输入文字");
const glm::vec3 DefaultColor = glm::vec3(0.0f, 0.0f, 0.0f);
const TextSObj::Font DefaultFontType = TextSObj::Font::Kai;
const int DefaultFontSize = 64;

const float Scaling = 0.004f; // 缩放系数
const float Clamping = 0.001f; // 切边系数

TextSObj::TextSObj()
{

}

TextSObj::TextSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

TextSObj::TextSObj(TextSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->color, other->textEdit->document()->toPlainText(), other->fontType, other->fontSize);
	connectParamWidget();
	isRecord = true;
}

TextSObj::~TextSObj()
{
	delete textAtlas;
	delete textEdit;
	delete colorLabel;
	delete fontComboBox;
	delete fontSizeSpinBox;
}

void TextSObj::destroy()
{
	delete this;
}

void TextSObj::logic(float dt)
{
	SObj::logic(dt);

	float x = 0;
	float y = 0;
	float maxLineWidth = 0; // 最大的行宽
	float lineHeight = fontSize * Scaling; // 最大行高

	positions.clear();
	texCoords.clear();
	for (std::vector<FT_ULong> &line : lines)
	{
		x = 0;
		float lastLineWidth = 0; // 上一行的宽度
		for (FT_ULong ul : line)
		{
			TextAtlas::GlyphLayout gl = textAtlas->getGlyphLayout(ul);

			float x2 = x + gl.bl * Scaling;
			float y2 = -y - gl.bt * Scaling;
			float w = gl.bw * Scaling;
			float h = gl.bh * Scaling;

			/* Advance the cursor to the start of the next character */
			float advanceX = gl.ax * Scaling;
			float advanceY = gl.ay * Scaling;
			x += advanceX;
			y += advanceY;

			lastLineWidth += advanceX;

			/* Skip glyphs that have no pixels */
			if (!w || !h)
				continue;

			positions.push_back(glm::vec3(x2, -y2, 0.0f));
			positions.push_back(glm::vec3(x2, -y2 - h, 0.0f));
			positions.push_back(glm::vec3(x2 + w, -y2, 0.0f));
			positions.push_back(glm::vec3(x2 + w, -y2, 0.0f));
			positions.push_back(glm::vec3(x2, -y2 - h, 0.0f));
			positions.push_back(glm::vec3(x2 + w, -y2 - h, 0.0f));

			float glcx = Clamping * gl.bw / textAtlas->getWidth();
			float glcy = Clamping * gl.bh / textAtlas->getHeight();
			texCoords.push_back(glm::vec2(gl.tx + glcx, gl.ty + glcy));
			texCoords.push_back(glm::vec2(gl.tx + glcx, gl.ty + gl.bh / textAtlas->getHeight() - glcy));
			texCoords.push_back(glm::vec2(gl.tx + gl.bw / textAtlas->getWidth() - glcy, gl.ty + glcy));
			texCoords.push_back(glm::vec2(gl.tx + gl.bw / textAtlas->getWidth() - glcy, gl.ty + glcy));
			texCoords.push_back(glm::vec2(gl.tx + glcx, gl.ty + gl.bh / textAtlas->getHeight() - glcy));
			texCoords.push_back(glm::vec2(gl.tx + gl.bw / textAtlas->getWidth() - glcy, gl.ty + gl.bh / textAtlas->getHeight() - glcy));
		}

		if (lastLineWidth > maxLineWidth)
		{
			maxLineWidth = lastLineWidth;
		}
		y -= lineHeight;
	}

	// 将文本居中
	for (glm::vec3 &position : positions)
	{
		position.x -= maxLineWidth / 2;
		position.y -= y / 2;
		position.y -= lineHeight;
	}

	// 更新包围框
	aabb = glm::vec3(maxLineWidth / 2, -y / 2, 0.001f);
}

void TextSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureMaskShader()->enable();
		ResourceManager::getInstance()->getTextureMaskShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureMaskShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureMaskShader()->setColorID(colorID);
		ResourceManager::getInstance()->getTextureMaskShader()->setColor(color);
		ResourceManager::getInstance()->getTextureMaskShader()->setMVP(
			camera->getViewProjectionMatrix() * m);

		textAtlas->bind();

		PrimitiveManager::getInstance()->renderQuads(positions, texCoords);
	}
}

void TextSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Text", text);
	jsonObject.insert("Color", Tool::vec3ToStr(color));
	jsonObject.insert("Font Type", fontType);
	jsonObject.insert("Font Size", fontSize);
}

void TextSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultColor, DefaultText, DefaultFontType, DefaultFontSize);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(Tool::vec3FromStr(jsonObject.value("Color").toString()),
			jsonObject.value("Text").toString(), 
			jsonObject.value("Font Type").toInt(),
			jsonObject.value("Font Size").toInt());
	}
	connectParamWidget();
	isRecord = true;
}

void TextSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	TextSObj *castedSnapshot = dynamic_cast<TextSObj *>(snapshot);
	castedSnapshot->text = text;
	castedSnapshot->fontType = fontType;
	castedSnapshot->color = color;
	castedSnapshot->fontSize = fontSize;
}

void TextSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	TextSObj *castedSnapshot = dynamic_cast<TextSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->color, castedSnapshot->text, castedSnapshot->fontType, castedSnapshot->fontSize);
	connectParamWidget();
	isRecord = true;
}

void TextSObj::init()
{
	textAtlas = new TextAtlas;
}

void TextSObj::setParamWidget()
{
	paramWidget->addText("Text", textEdit);
	paramWidget->addColor("Color", colorLabel);
	paramWidget->addEnum("Font Type", QStringList{ QString::fromLocal8Bit("楷体"), QString::fromLocal8Bit("黑体") }, fontComboBox);
	paramWidget->addInt("Font Size", 16, 512, 1, fontSizeSpinBox);
}

void TextSObj::inflateParamWidget(glm::vec3 _color, QString _text, int _currentFontIndex, int _fontSize)
{
	colorLabel->setColor(_color);
	textEdit->setText(_text);
	fontComboBox->setCurrentIndex(_currentFontIndex);
	fontSizeSpinBox->setValue(_fontSize);

	colorChanged(_color);
	fontSizeChanged(_fontSize);
	fontChanged(_currentFontIndex);
	textChanged();
}

void TextSObj::connectParamWidget()
{
	connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
	connect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	connect(fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontChanged(int)));
	connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChanged(int)));
}

void TextSObj::disconnectParamWidget()
{
	disconnect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
	disconnect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	disconnect(fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontChanged(int)));
	disconnect(fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChanged(int)));
}

void TextSObj::setText(QString qstr)
{
	if (!text.isEmpty())
	{
		// 获得文本中不重复的字符
		std::u32string u32str = qstr.toStdU32String();
		std::vector<FT_ULong> uniqueUls;
		for (auto u32c : u32str)
		{
			uniqueUls.push_back(u32c);
		}
		std::sort(uniqueUls.begin(), uniqueUls.end());
		uniqueUls.erase(unique(uniqueUls.begin(), uniqueUls.end()), uniqueUls.end());
		textAtlas->update(uniqueUls, ResourceManager::getInstance()->getFace(fontType), fontSize);

		// 生成新的文本行
		lines.clear();
		QStringList sqstrs = qstr.split('\n');
		for (QString sqstr : sqstrs)
		{
			u32str = sqstr.toStdU32String();
			std::vector<FT_ULong> line;
			for (auto u32c : u32str)
			{
				line.push_back(u32c);
			}
			lines.push_back(line);
		}
	}
}

void TextSObj::textChanged()
{
	recordLastSnapshot();
	text = textEdit->document()->toPlainText();
	setText(text);
	recordCurSnapshot();
}

void TextSObj::colorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	this->color = color;
	recordCurSnapshot();
}

void TextSObj::fontChanged(int index)
{
	recordLastSnapshot();
	fontType = (Font)index;
	if (isRecord)
	{
		setText(text);
	}
	recordCurSnapshot();
}

void TextSObj::fontSizeChanged(int val)
{
	recordLastSnapshot();
	fontSize = val;
	if (isRecord)
	{
		setText(text);
	}
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new TextSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new TextSObj(dynamic_cast<TextSObj *>(other));
}

ISObj *nuller()
{
	return new TextSObj();
}