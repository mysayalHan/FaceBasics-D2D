#pragma once
#ifndef INFO_H
#define INFO_H

#include <Windows.h>
#include "SomeStruct.h"
#include <iostream>
#include <vector>
#include "BVFace.h"

class info 
{
public:
	friend class KinectDataAccess;
private:
	//��������֡�ļ��ϣ���ͼ����ģ����룬��Ϣͳ��ģ�鴦��
	static std::vector <BVFace> PersonCamCatchInfo;
	//����ѵ������
};
#endif // !INFO_H
