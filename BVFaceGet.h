#pragma once

#include "stdafx.h"
#include <opencv\cv.h>

class BVFaceGet
{
public:
	//get face
	cv::Mat getFaceMat(RectI faceBox, RGBQUAD * pBuffer, int width, int height);
};