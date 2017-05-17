#pragma once

#include "stdafx.h"
#include <string>
#include <opencv\cv.h>

struct MomentPersonState;
class BVFace
{
private:
	// ��������Ϣ
	RectI m_faceBox;
	// �ļ�����λ��
	//std::string _filename;
public:
	// ��ǰ֡ʱ��
	INT64 time;
	// ʱ���
	double interval;
	//���γ���ʱ��
	INT64 first_time;
	// ��ǰ���ı��
	int faceNumber;
	// �Ա�
	int gender;
	// ����
	int age;
	// ����
	int expression;
	// רע
	bool engaged;
	// �۾�
	bool wearingGlasses;
	// ���۱�
	bool leftEyeClosed;
	// ���۱�
	bool rightEyeClosed;
	// ���ſ�
	bool mouthOpen;
	// ���ƶ�
	bool mouthMoved;
	// ����
	bool lookAway;
	// ������ in pixel
	int x;
	// ��� in millimeter
	int z;

	// ��ǰ֡����������
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

