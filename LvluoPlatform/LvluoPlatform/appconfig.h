#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <glm/glm.hpp>
#include "paramwidget.h"

class AppConfig : public QObject
{
	Q_OBJECT

public:
	AppConfig(QObject *parent = Q_NULLPTR);
	~AppConfig();
	
	void show();

	std::vector<QString> &getRecentProjectsFileNames();
	void removeRecentProjectsFileName(QString fileName);

	glm::ivec2 getPanelSize();
	glm::vec3 getPanelColor();
	glm::ivec2 getPanelPosition();
	bool getGridOn();
	int getGridSize();
	glm::vec3 getGridColor();
	int getMSAA();

	void setPanelSize(glm::ivec2 size);
	void setPanelPosition(glm::ivec2 position);

	static AppConfig *getInstance();

signals:
	void panelSizeChanged(glm::ivec2 size);
	void panelPositionChanged(glm::ivec2 position);
	void gridSizeChanged(int size);

private:
	void open();
	void save();

	std::vector<QString> recentProjectsFileNames;
	glm::ivec2 panelSize;
	glm::vec3 panelColor;
	glm::ivec2 panelPosition;
	bool gridOn;
	int gridSize;
	glm::vec3 gridColor;
	int msaa;

	ParamWidget *paramWidget;

	SpinBoxIVec2 *panelSizeSpinBoxIVec2;
	ColorLabel *panelColorLabel;
	SpinBoxIVec2 *panelPositionSpinBoxIVec2;
	QCheckBox *gridOnCheckBox;
	QSpinBox *gridSizeSpinBox;
	ColorLabel *gridColorLabel;
	QComboBox *msaaComboBox;

	static AppConfig *instance;

private slots:
	void panelSizeSpinBoxIVec2ValueChanged(glm::ivec2 val);
	void panelColorLabelColorChanged(glm::vec3 val);
	void panelPositionSpinBoxIVec2ValueChanged(glm::ivec2 val);
	void gridOnCheckBoxChanged(int val);
	void gridSizeSpinBoxValueChanged(int val);
	void gridColorLabelColorChanged(glm::vec3 val);
	void msaaComboBoxCurrentTextChanged(QString val);
};

#endif // APPCONFIG_H
