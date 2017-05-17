#pragma once

#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include "SomeStruct.h"

class Widget;
class ImageThread
{
protected:
	std::vector<MomentPersonState> states;
	int index;

public:
	ImageThread();
	~ImageThread();

public:
	void run();
	void createPerson();
};

#endif // !IMAGETHREAD_H
