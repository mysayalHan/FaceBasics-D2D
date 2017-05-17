#pragma once

#include "stdafx.h"
#include <string>
#include <opencv\cv.h>

struct MomentPersonState;
class BVFace
{
private:
	// 人脸框信息
	RectI m_faceBox;
	// 文件存贮位置
	//std::string _filename;
public:
	// 当前帧时间
	INT64 time;
	// 时间差
	double interval;
	//初次出现时间
	INT64 first_time;
	// 当前脸的标号
	int faceNumber;
	// 性别
	int gender;
	// 年龄
	int age;
	// 表情
	int expression;
	// 专注
	bool engaged;
	// 眼镜
	bool wearingGlasses;
	// 左眼闭
	bool leftEyeClosed;
	// 右眼闭
	bool rightEyeClosed;
	// 嘴张开
	bool mouthOpen;
	// 嘴移动
	bool mouthMoved;
	// 看别处
	bool lookAway;
	// 横坐标 in pixel
	int x;
	// 深度 in millimeter
	int z;

	// 当前帧的人脸数据
	cv::Mat faceImage;

	void setMomentState(MomentPersonState *);
	void setFaceBox(RectI);
	RectI getFaceBox();
	void setFaceBox(int LEFT, int TOP, int RIGHT, int BOTTOM);
	std::pair<int, int> getCentralPosition();
	void getCentralPosition(int &central_x, int &central_y);

	BVFace(RectI);
	BVFace();
	~BVFace();
};

