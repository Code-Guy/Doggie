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
	Selector *selector; // ѡ����
	std::vector<ISObj *> copyClipboard; // ���Ƶļ����壬���ʱ����Ҫ�ͷ�
	std::vector<ISObj *> cutClipboard; // ���еļ����壬���ʱ��Ҫ�ͷ�
};

#endif // PASTER_H