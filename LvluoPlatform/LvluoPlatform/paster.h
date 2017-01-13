#ifndef PASTER_H
#define PASTER_H

#include "selector.h"

class Paster
{
public:
	Paster(Selector *selector);
	~Paster();

	void copy();
	void cut();
	void paste(QPoint p);

private:
	Selector *selector; // 选择器
	std::vector<ISObj *> copyClipboard; // 复制的剪贴板，清空时不需要释放
	std::vector<ISObj *> cutClipboard; // 剪切的剪贴板，清空时需要释放
};

#endif // PASTER_H