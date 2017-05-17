#include "ImageThread.h"
#include "KinectDataAccess.h"

ImageThread::ImageThread()
{
	createPerson();
}

void ImageThread::run()
{
	HINSTANCE hinstance = (HINSTANCE)::GetModuleHandle(NULL);
	int nCmdShow = 10;
	KinectDataAccess kda;
	kda.Run(hinstance, nCmdShow);
}

ImageThread::~ImageThread()
{

}

void ImageThread::createPerson()
{
	//0
	{
		PersonMomentState pms;
		pms.id = 0;
		pms.age = 20;
		pms.sex = SEX_MALE;
		pms.depth = 2;
		pms.x = 1.5;
		pms.wearingGlasse = true;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//1
	{
		PersonMomentState pms;
		pms.id = 1;
		pms.age = 20;
		pms.sex = SEX_FEMALE;
		pms.depth = 2;
		pms.x = 3.5;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//2
	{
		PersonMomentState pms;
		pms.id = 2;
		pms.age = 20;
		pms.sex = SEX_MALE;
		pms.depth = 2;
		pms.x = 2;
		pms.wearingGlasse = true;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//3
	{
		PersonMomentState pms;
		pms.id = 3;
		pms.age = 20;
		pms.sex = SEX_FEMALE;
		pms.depth = 2;
		pms.x = 2.5;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//4
	{
		PersonMomentState pms;
		pms.id = 4;
		pms.age = 20;
		pms.sex = SEX_FEMALE;
		pms.depth = 2.5;
		pms.x = 2;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//5
	{
		PersonMomentState pms;
		pms.id = 5;
		pms.age = 40;
		pms.sex = SEX_FEMALE;
		pms.depth = 2.5;
		pms.x = 2.5;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//6
	{
		PersonMomentState pms;
		pms.id = 6;
		pms.age = 20;
		pms.sex = SEX_MALE;
		pms.depth = 2.5;
		pms.x = 1.8;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//7
	{
		PersonMomentState pms;
		pms.id = 7;
		pms.age = 36;
		pms.sex = SEX_FEMALE;
		pms.depth = 2.5;
		pms.x = 3.1;
		pms.wearingGlasse = false;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//8
	{
		PersonMomentState pms;
		pms.id = 8;
		pms.age = 36;
		pms.sex = SEX_MALE;
		pms.depth = 3;
		pms.x = 2;
		pms.wearingGlasse = true;
		pms.engaged = true;
		pms.expression = 0;
		states.push_back(pms);
	}
	//9
	{
		PersonMomentState pms;
		pms.id = 9;
		pms.age = 66;
		pms.sex = SEX_FEMALE;
		pms.depth = 2.5;
		pms.x = 2;
		pms.wearingGlasse = false;
		states.push_back(pms);
		pms.expression = 0;
	}
}