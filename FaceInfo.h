#pragma once

#ifndef FACEINFO_H
#define FACEINFO_H

class Face_Info
{
	friend class KinectDataAccess;

public:
	int FacialExpression();
	int FacialRender_and_age();
};
#endif // !FACEINFO_H
