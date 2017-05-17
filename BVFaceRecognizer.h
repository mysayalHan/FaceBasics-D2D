#pragma once

#include "stdafx.h"
#include <fstream>
#include <opencv/cv.h>

class BVFaceRecognizer
{
private:
	cv::Ptr<cv::face::FaceRecognizer> model;
	int img_count;
	std::fstream fs;
public:
	int predict(cv::InputArray src) const;

	// ����ģ�ͻ�ȡԤ����
	int cropFaceFromImage(RectI faceBox, RGBQUAD * pBuffer, int width, int height);

	// ����沿 Mat ����
	cv::Mat getFaceMat(RectI faceBox, RGBQUAD * pBuffer, int width, int height);

	void saveModelAs(const char * filename);
	void trainWithCsv(std::string src_filename, char separator);
	void loadFromFile(std::string persistedFileName);
	BVFaceRecognizer();
	~BVFaceRecognizer();
};
