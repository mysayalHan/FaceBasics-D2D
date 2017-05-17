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
	//对于所有帧的集合，由图像处理模块加入，信息统计模块处理
	static std::vector <BVFace> PersonCamCatchInfo;
	//生成训练样本
};
#endif // !INFO_H
