#include "appconfig.h"
#include "tool.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

AppConfig *AppConfig::instance = nullptr;

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
	open();

	paramWidget = new ParamWidget("Options");

	paramWidget->addIVec2("Panel Size", 100, 1920, 1, panelSizeSpinBoxIVec2);
	paramWidget->addColor("Panel Color", panelColorLabel);
	paramWidget->addIVec2("Panel Position", 0, 1920, 1, panelPositionSpinBoxIVec2);
	paramWidget->addBool("Grid On", gridOnCheckBox);
	paramWidget->addInt("Grid Size", 1, 100, 1, gridSizeSpinBox);
	paramWidget->addColor("Grid Color", gridColorLabel);
	paramWidget->addEnum("MSAA", QStringList{ "1", "2", "4", "8", "16" }, msaaComboBox);

	connect(panelSizeSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelSizeSpinBoxIVec2ValueChanged(glm::ivec2)));
	connect(panelColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(panelColorLabelColorChanged(glm::vec3)));
	connect(panelPositionSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelPositionSpinBoxIVec2ValueChanged(glm::ivec2)));
	connect(gridOnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(gridOnCheckBoxChanged(int)));
	connect(gridSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(gridSizeSpinBoxValueChanged(int)));
	connect(gridColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(gridColorLabelColorChanged(glm::vec3)));
	connect(msaaComboBox, SIGNAL(currentTextChanged(const QString)), this, SLOT(msaaComboBoxCurrentTextChanged(QString)));

	panelSizeSpinBoxIVec2->setIVec2(panelSize);
	panelColorLabel->setColor(panelColor);
	panelPositionSpinBoxIVec2->setIVec2(panelPosition);
	gridOnCheckBox->setChecked(gridOn);
	gridSizeSpinBox->setValue(gridSize);
	gridColorLabel->setColor(gridColor);
	msaaComboBox->setCurrentText(QString::number(msaa));
}

AppConfig::~AppConfig()
{
	save();

	delete panelSizeSpinBoxIVec2;
	delete panelColorLabel;
	delete panelPositionSpinBoxIVec2;
	delete gridOnCheckBox;
	delete gridSizeSpinBox;
	delete gridColorLabel;
	delete msaaComboBox;
	delete paramWidget;
}

void AppConfig::show()
{
	paramWidget->show();
	paramWidget->activateWindow();
}

std::vector<QString> &AppConfig::getRecentProjectsFileNames()
{
	return recentProjectsFileNames;
}

void AppConfig::removeRecentProjectsFileName(QString fileName)
{
	auto iter = std::find(recentProjectsFileNames.begin(), recentProjectsFileNames.end(), fileName);
	if (iter != recentProjectsFileNames.end())
	{
		recentProjectsFileNames.erase(iter);
	}
}

glm::ivec2 AppConfig::getPanelSize()
{
	return panelSize;
}

glm::vec3 AppConfig::getPanelColor()
{
	return panelColor;
}

glm::ivec2 AppConfig::getPanelPosition()
{
	return panelPosition;
}

bool AppConfig::getGridOn()
{
	return gridOn;
}

int AppConfig::getGridSize()
{
	return gridSize;
}

glm::vec3 AppConfig::getGridColor()
{
	return gridColor;
}

int AppConfig::getMSAA()
{
	return msaa;
}

void AppConfig::setPanelSize(glm::ivec2 size)
{
	disconnect(panelSizeSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelSizeSpinBoxIVec2ValueChanged(glm::ivec2)));
	panelSize = size;
	panelSizeSpinBoxIVec2->setIVec2(size);
	connect(panelSizeSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelSizeSpinBoxIVec2ValueChanged(glm::ivec2)));
}

void AppConfig::setPanelPosition(glm::ivec2 position)
{
	disconnect(panelPositionSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelPositionSpinBoxIVec2ValueChanged(glm::ivec2)));
	panelPosition = position;
	panelPositionSpinBoxIVec2->setIVec2(position);
	connect(panelPositionSpinBoxIVec2, SIGNAL(ivec2ValueChanged(glm::ivec2)), this, SLOT(panelPositionSpinBoxIVec2ValueChanged(glm::ivec2)));
}

AppConfig *AppConfig::getInstance()
{
	if (instance == nullptr)
	{
		instance = new AppConfig;
	}
	return instance;
}

void AppConfig::open()
{
	QFile file("Resources/Config/app.config");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		printf("open project file failed!\n");
		return;
	}
	QByteArray byteArray = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);

	if (jsonError.error != QJsonParseError::NoError)
	{
		printf("parse application configuration file failed!\n");
		return;
	}

	QJsonObject appConfigObject = document.object();
	QJsonObject recentProjectsFileNamesObject = appConfigObject.value("Recent Projects").toObject();
	QStringList keys = recentProjectsFileNamesObject.keys();
	keys.sort(Qt::CaseSensitive);
	for (QString key : keys)
	{
		recentProjectsFileNames.push_back(recentProjectsFileNamesObject.value(key).toString());
	}
	panelSize = Tool::ivec2FromStr(appConfigObject.value("Panel Size").toString());
	panelColor = Tool::vec3FromStr(appConfigObject.value("Panel Color").toString());
	panelPosition = Tool::ivec2FromStr(appConfigObject.value("Panel Postion").toString());
	gridOn = appConfigObject.value("Grid On").toBool();
	gridSize = appConfigObject.value("Grid Size").toInt();
	gridColor = Tool::vec3FromStr(appConfigObject.value("Grid Color").toString());
	msaa = appConfigObject.value("MSAA").toInt();
}

void AppConfig::save()
{
	QJsonObject appConfigObject;
	QJsonObject recentProjectsFileNamesObject;
	int i = 0;
	for (QString recentProjectsFileName : recentProjectsFileNames)
	{
		recentProjectsFileNamesObject.insert(QString::number(i++), recentProjectsFileName);
	}
	appConfigObject.insert("Recent Projects", recentProjectsFileNamesObject);
	appConfigObject.insert("Panel Size", Tool::ivec2ToStr(panelSize));
	appConfigObject.insert("Panel Color", Tool::vec3ToStr(panelColor));
	appConfigObject.insert("Panel Postion", Tool::ivec2ToStr(panelPosition));
	appConfigObject.insert("Grid On", gridOn);
	appConfigObject.insert("Grid Size", gridSize);
	appConfigObject.insert("Grid Color", Tool::vec3ToStr(gridColor));
	appConfigObject.insert("MSAA", msaa);

	QJsonDocument document;
	document.setObject(appConfigObject);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);

	QFile file("Resources/Config/app.config");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		printf("save application configuration file failed!\n");
		return;
	}

	file.write(byteArray);
	file.close();
}

void AppConfig::panelSizeSpinBoxIVec2ValueChanged(glm::ivec2 val)
{
	panelSize = val;
	emit panelSizeChanged(panelSize);
}

void AppConfig::panelColorLabelColorChanged(glm::vec3 val)
{
	panelColor = val;
}

void AppConfig::panelPositionSpinBoxIVec2ValueChanged(glm::ivec2 val)
{
	panelPosition = val;
	emit panelPositionChanged(panelPosition);
}

void AppConfig::gridOnCheckBoxChanged(int val)
{
	gridOn = val;
}

void AppConfig::gridSizeSpinBoxValueChanged(int val)
{
	gridSize = val;
	emit gridSizeChanged(gridSize);
}

void AppConfig::gridColorLabelColorChanged(glm::vec3 val)
{
	gridColor = val;
}

void AppConfig::msaaComboBoxCurrentTextChanged(QString val)
{
	msaa = val.toInt();
}