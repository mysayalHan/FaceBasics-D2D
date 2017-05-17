#include "BVFace.h"
#include "SomeStruct.h"

BVFace::BVFace()
{
	//_filename = BVFace::acquireFileName();
}
BVFace::BVFace(RectI _facebox) 
{
	//_filename = BVFace::acquireFileName();
	m_faceBox = _facebox;
}

BVFace::~BVFace()
{
}

RectI BVFace::getFaceBox() 
{
	return m_faceBox;
}

void BVFace::setMomentState(MomentPersonState * mps)
{
	mps->age = age;
	mps->sex = gender;
	mps->engaged = engaged;
	mps->expression = expression;
	mps->id = faceNumber;
	mps->leftEyeClosed = leftEyeClosed;
	mps->rightEyeClosed = rightEyeClosed;
	mps->lookAway = lookAway;
	mps->wearingGlasse = wearingGlasses;
	mps->depth = z;
	mps->intervalTime = interval;
	mps->mouthMoved = mouthMoved;
	mps->mouthOpen = mouthOpen;
	mps->x = x;
	faceImage.copyTo(mps->mat);
}

void BVFace::setFaceBox(RectI facebox) 
{
	m_faceBox = facebox;
}

void BVFace::setFaceBox(int LEFT, int TOP, int RIGHT, int BOTTOM)
{
	m_faceBox.Left = LEFT;
	m_faceBox.Top = LEFT;
	m_faceBox.Right = RIGHT;
	m_faceBox.Bottom = BOTTOM;
}

std::pair<int, int> BVFace::getCentralPosition() 
{
	return std::pair<int, int>((m_faceBox.Left + m_faceBox.Right) / 2, (m_faceBox.Bottom + m_faceBox.Top) / 2);
}

void BVFace::getCentralPosition(int &central_x, int &central_y) 
{
	central_x = (m_faceBox.Left + m_faceBox.Right) / 2;
	central_y = (m_faceBox.Bottom + m_faceBox.Top) / 2;
}