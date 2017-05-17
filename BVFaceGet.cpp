#include "BVFaceGet.h"
#include<opencv2\opencv.hpp>

using namespace cv;

cv::Mat BVFaceGet::getFaceMat(RectI faceBox, RGBQUAD * pBuffer, int width, int height)
{
		//1920,1080
		Mat res(Size(width, height), CV_8UC4, pBuffer);
		Rect rect(faceBox.Left, faceBox.Top, faceBox.Right - faceBox.Left, faceBox.Bottom - faceBox.Top);
		Mat image_roi = res(rect);
		return image_roi;
}
