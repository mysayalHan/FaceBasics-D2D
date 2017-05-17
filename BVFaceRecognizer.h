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

	// 根据模型获取预测结果
	int cropFaceFromImage(RectI faceBox, RGBQUAD * pBuffer, int width, int height);

	// 获得面部 Mat 数据
	cv::Mat getFaceMat(RectI faceBox, RGBQUAD * pBuffer, int width, int height);

	void saveModelAs(const char * filename);
	void trainWithCsv(std::string src_filename, char separator);
	void loadFromFile(std::string persistedFileName);
	BVFaceRecognizer();
	~BVFaceRecognizer();
};
