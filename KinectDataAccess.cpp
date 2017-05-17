#include "stdafx.h"
#include "KinectDataAccess.h"
#include <string>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "resource.h"
#include "ImageRenderer.h"
#include <limits>
#include "BVFace.h"
#include "Info.h"
#include "SomeStruct.h"
#include <math.h>
#include "decision_tree.h"
#include "group_decision.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <future>

using namespace cv;
using namespace cv::dnn;
using namespace std;

std::vector<vector<PersonMoment>>group;
std::mutex mu;

// face property text layout offset in X axis
static const float c_FaceTextLayoutOffsetX = -0.1f;

// face property text layout offset in Y axis
static const float c_FaceTextLayoutOffsetY = -0.125f;

std::vector<PersonMoment> KinectDataAccess::PersonMomentInfo;

const int Result_Count = 1;
// define the face frame features required to be computed by this application
static const DWORD c_FaceFrameFeatures =
FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
| FaceFrameFeatures::FaceFrameFeatures_Happy
| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
| FaceFrameFeatures::FaceFrameFeatures_LookingAway
| FaceFrameFeatures::FaceFrameFeatures_Glasses
| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;

vvs tableInfo;
node* root = new node;
string defaultClass;

/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.reshape(1, 1); //reshape the blob to 1x1000 matrix
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}

void getFaceAge(Mat img, int &classname)
{
	cv::dnn::initModule();  //Required if OpenCV is built as static libs
	String modelTxt = "H:\\Business\\Bussiness\\FaceBasics-D2D\\deploy_age2.prototxt";
	String modelBin = "H:\\Business\\Bussiness\\FaceBasics-D2D\\age_net.caffemodel";

	/* Convert from BlobProto to Blob<float> */

	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty())
	{
		/*std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		std::cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << std::endl;
		std::cerr << "http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel" << std::endl;*/
		exit(-1);
	}
	if (img.empty())
	{
		//std::cerr << "Can't read image from the file: " << imageFile << std::endl;
		exit(-1);
	}
	resize(img, img, Size(227, 227));                   //CaffeNet accepts only 227x227 RGB-images
	//cv::subtract(img, mean_img, img);
	Mat inputBlob = blobFromImage(img);   //Convert Mat to batch of images
	net.setBlob(".data", inputBlob);        //set the network input
	net.forward();                          //compute output
	Mat prob = net.getBlob("prob");   //gather output of "prob" layer
	double classProb;
	int classId;
	getMaxClass(prob, &classId, &classProb);//find the best class
	classname = classId;
}

void getFaceGender(Mat img, int classname)
{
	cv::dnn::initModule();  //Required if OpenCV is built as static libs
	String modelTxt = "H:\\Business\\Bussiness\\FaceBasics-D2D\\deploy_gender2.prototxt";
	String modelBin = "H:\\Business\\Bussiness\\FaceBasics-D2D\\gender_net.caffemodel";

	/* Convert from BlobProto to Blob<float> */

	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty())
	{
		/*std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		std::cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << std::endl;
		std::cerr << "http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel" << std::endl;*/
		exit(-1);
	}
	if (img.empty())
	{
		//std::cerr << "Can't read image from the file: " << imageFile << std::endl;
		exit(-1);
	}
	resize(img, img, Size(227, 227));                   //CaffeNet accepts only 227x227 RGB-images
														//cv::subtract(img, mean_img, img);
	Mat inputBlob = blobFromImage(img);   //Convert Mat to batch of images
	net.setBlob(".data", inputBlob);        //set the network input
	net.forward();                          //compute output
	Mat prob = net.getBlob("prob");   //gather output of "prob" layer
	double classProb;
	int classId;
	getMaxClass(prob, &classId, &classProb);//find the best class
	classname = classId;
}

void KinectDataAccess::create_decision_tree()
{
	ifstream inputFile;												// Input file stream
	string singleInstance;											// Single line read from the input file 
	vvs dataTable;

	inputFile.open("train.txt");
	while (getline(inputFile, singleInstance))						// Read from file, parse and store data
	{
		parse(singleInstance, dataTable);
	}
	inputFile.close(); 												// Close input file
	tableInfo = generateTableInfo(dataTable);					// Stores all the attributes and their values in a vector of vector of strings named tableInfo

	root = buildDecisionTree(dataTable, root, tableInfo);			// Recursively build and train decision tree
	defaultClass = returnMostFrequentClass(dataTable);		// Stores the most frequent class in the training data. This is used as the default class label
	dataTable.clear(); 												// clear dataTable of training data to store testing data
	inputFile.clear();
}

/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>S_OK on success else the failure code</returns>
HRESULT KinectDataAccess::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize Kinect and get color, body and face readers
		IColorFrameSource* pColorFrameSource = nullptr;
		IBodyFrameSource* pBodyFrameSource = nullptr;
		IDepthFrameSource* pDepthFrameSource = nullptr;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		if (SUCCEEDED(hr))
		{
			// create a face frame source + reader to track each body in the fov
			for (int i = 0; i < BODY_COUNT; i++)
			{
				if (SUCCEEDED(hr))
				{
					// create the face frame source by specifying the required face frame features
					hr = CreateFaceFrameSource(m_pKinectSensor, 0, c_FaceFrameFeatures, &m_pFaceFrameSources[i]);
				}
				if (SUCCEEDED(hr))
				{
					// open the corresponding reader
					hr = m_pFaceFrameSources[i]->OpenReader(&m_pFaceFrameReaders[i]);
				}
			}
		}

		SafeRelease(pColorFrameSource);
		SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		return E_FAIL;
	}

	return hr;
}

inline bool trueOrFalse(int num) 
{
	if (num == 0 || num == 3) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void KinectDataAccess::PrintMessage(int FaceNumber, const DetectionResult* pFaceProperties, const int * otherResults) {

	INT64 nTime;

	/*PersonTime pt;
	pt._time = _Time;
	pt._num = FaceNumber;
	pt._sex = otherResults[0];
	pt._happy = trueOrFalse(pFaceProperties[0]);
	pt._engaged = trueOrFalse(pFaceProperties[1]);
	pt._wearingGlasse = trueOrFalse(pFaceProperties[2]);
	pt._leftEyeClosed = trueOrFalse(pFaceProperties[3]);
	pt._rightEyeClosed = trueOrFalse(pFaceProperties[4]);
	pt._mouthOpen = trueOrFalse(pFaceProperties[5]);
	pt._mouthMoved = trueOrFalse(pFaceProperties[6]);
	pt._lookAway = trueOrFalse(pFaceProperties[7]);
	pt._age = otherResults[1];
	pt.inSet = false;
	if (pt._age > 2) pt._age -= 3;

	for (int iProperty = 0; iProperty < Result_Count; iProperty++) {
	switch (otherResults[iProperty]) {
	case 0:
	of << "0";
	break;
	case 1:
	of << "1";
	break;
	case 2:
	of << "2";
	break;
	case 3:
	of << " 0";
	break;
	case 4:
	of << " 1";
	break;
	case 5:
	of << " 2";
	break;
	default:
	break;
	}
	}*/

}

//face information
void KinectDataAccess::write_BVFace(PersonMoment * face, const DetectionResult * pFaceProperties)
{

	face->engaged = trueOrFalse(pFaceProperties[1]);
	face->wearingGlasses = trueOrFalse(pFaceProperties[2]);

}

/// <summary>
/// Maps coordinates from ColorSpace to CameraSpace
/// Expects that the Points in ColorSpace are mirrored at x (as Kinects returns it by default).
/// </summary>
HRESULT KinectDataAccess::MapColorToCameraCoordinates(const std::vector<ColorSpacePoint>& colorsps, std::vector<CameraSpacePoint>& camerasps)
{
	//Access frame
	ICoordinateMapper* pMapper;
	HRESULT hr = m_pKinectSensor->get_CoordinateMapper(&pMapper);
	if (SUCCEEDED(hr))
	{
		if (!m_pDepthBuffer) {
			return S_FALSE;
		}
		hr = pMapper->MapColorFrameToCameraSpace(cDepthWidth * cDepthHeight, m_pDepthBuffer, cColorWidth * cColorHeight, m_cameraSpacePoints);
		if (SUCCEEDED(hr))
		{
			for (ColorSpacePoint colorsp : colorsps)
			{
				int colorX = static_cast<int>(colorsp.X + 0.5f);
				int colorY = static_cast<int>(colorsp.Y + 0.5f);
				long colorIndex = (long)(colorY * cColorWidth + colorX);
				CameraSpacePoint csp = m_cameraSpacePoints[colorIndex];
				camerasps.push_back(CameraSpacePoint{ -csp.X, -csp.Y, csp.Z });
			}
		}
	}
	return hr;
}

inline bool available(RectI r) 
{
	return r.Bottom >= 0 && r.Bottom <= 1080
		&& r.Top >= 0 && r.Top <= 1080
		&& r.Left >= 0 && r.Left <= 1920
		&& r.Right >= 0 && r.Right <= 1920
		&& (r.Right - r.Left) * (r.Bottom - r.Top) > 0;
}

void KinectDataAccess::ProcessFaces(RGBQUAD * pBuffer)
{
	HRESULT hr;
	IBody* ppBodies[BODY_COUNT] = { 0 };
	bool bHaveBodyData = SUCCEEDED(UpdateBodyData(ppBodies));
	//对于当前帧，初始化每个人是否出现
	for (vector<PersonMoment>::iterator citer = PersonMomentInfo.begin();
		citer != PersonMomentInfo.end(); ++citer)
	{
		citer->appear = false;
	}


	//_cwprintf(L"%d",BODY_COUNT);
	// iterate through each face reader
	for (int iFace = 0; iFace < BODY_COUNT; ++iFace)
	{
		// retrieve the latest face frame from this reader
		IFaceFrame* pFaceFrame = nullptr;
		hr = m_pFaceFrameReaders[iFace]->AcquireLatestFrame(&pFaceFrame);

		BOOLEAN bFaceTracked = false;
		if (SUCCEEDED(hr) && nullptr != pFaceFrame)
		{
			// check if a valid face is tracked in this face frame
			hr = pFaceFrame->get_IsTrackingIdValid(&bFaceTracked);

		}

		if (SUCCEEDED(hr))
		{
			if (bFaceTracked)
			{
				IFaceFrameResult* pFaceFrameResult = nullptr;
				RectI faceBox = { 0 };
				PointF facePoints[FacePointType::FacePointType_Count];
				Vector4 faceRotation;
				DetectionResult faceProperties[FaceProperty::FaceProperty_Count];
				D2D1_POINT_2F faceTextLayout;
				int results[Result_Count];

				hr = pFaceFrame->get_FaceFrameResult(&pFaceFrameResult);
				// need to verify if pFaceFrameResult contains data before trying to access it
				if (SUCCEEDED(hr) && pFaceFrameResult != nullptr)
				{
					hr = pFaceFrameResult->get_FaceBoundingBoxInColorSpace(&faceBox);
					faceBox.Bottom += 80;
					faceBox.Top -= 80;
					faceBox.Left -= 80;
					faceBox.Right += 80;

					if (SUCCEEDED(hr))
					{
						hr = pFaceFrameResult->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, facePoints);
					}

					if (SUCCEEDED(hr))
					{
						hr = pFaceFrameResult->get_FaceRotationQuaternion(&faceRotation);
					}

					if (SUCCEEDED(hr))
					{
						hr = pFaceFrameResult->GetFaceProperties(FaceProperty::FaceProperty_Count, faceProperties);
					}

					if (SUCCEEDED(hr))
					{
						hr = GetFaceTextPositionInColorSpace(ppBodies[iFace], &faceTextLayout);
					}

					if (SUCCEEDED(hr))
					{
						// draw face frame results
						m_pDrawDataStreams->DrawFaceFrameResults(iFace, &faceBox, facePoints, &faceRotation, faceProperties, &faceTextLayout);
					}
					if (available(faceBox))//存在人脸框
					{

						int nWidth = 0;
						int nHeight = 0;
						int ndepth = 0;
						INT64 nTime = 0;
						IDepthFrame* pDepthFrame = nullptr;
						UINT nBufferSize = 0;
						IFrameDescription* pFrameDescription = NULL;
						USHORT nDepthMinReliableDistance = 0;
						//acquire depth data
						HRESULT hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
						USHORT nDepthMaxDistance = 0;

						if (SUCCEEDED(hr))//获得深度信息
						{
							IFrameDescription* pFrameDescription = NULL;
							int nWidth = 0;
							int nHeight = 0;
							USHORT nDepthMinReliableDistance = 0;
							USHORT nDepthMaxDistance = 0;
							UINT nBufferSize = 0;
							UINT16 *pBuffer = NULL;

							hr = pDepthFrame->get_RelativeTime(&nTime);

							if (SUCCEEDED(hr))
							{
								hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
							}

							if (SUCCEEDED(hr))
							{
								hr = pFrameDescription->get_Width(&nWidth);
							}

							if (SUCCEEDED(hr))
							{
								hr = pFrameDescription->get_Height(&nHeight);
							}

							if (SUCCEEDED(hr))
							{
								hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
							}

							if (SUCCEEDED(hr))
							{
								// In order to see the full range of depth (including the less reliable far field depth)
								// we are setting nDepthMaxDistance to the extreme potential depth threshold
								nDepthMaxDistance = USHRT_MAX;

								// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
								//hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
							}

							if (SUCCEEDED(hr))
							{
								//hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
								hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &tmp_DepthBuffer);
								memcpy(m_pDepthBuffer, tmp_DepthBuffer, cDepthWidth* cDepthHeight * sizeof(UINT16));
							}

							SafeRelease(pFrameDescription);

						}

						SafeRelease(pDepthFrame);

						int x_in_image = m_DepthSpacePoints[(faceBox.Left + faceBox.Right) / 2].X;
						int y_in_image = m_DepthSpacePoints[(faceBox.Top + faceBox.Bottom) / 2 * cColorWidth + (faceBox.Left + faceBox.Right) / 2].Y;
						if (x_in_image != 0x80000000 &&
							y_in_image != 0x80000000  )
						{
							std::vector<ColorSpacePoint> csp;
							csp.push_back(ColorSpacePoint{ (float)(faceBox.Left + faceBox.Right) / 2, (float)(faceBox.Bottom + faceBox.Top) / 2 });
							std::vector<CameraSpacePoint> cmsp;

							MapColorToCameraCoordinates(csp, cmsp);
							CameraSpacePoint p0 = cmsp[0];
							ndepth = (int)(p0.Z * 1000);
						}

						bool flag = false;//是否找到之前存在的人脸

						INT32 x_center = (faceBox.Left + faceBox.Right);
						INT32 y_center = (faceBox.Top + faceBox.Bottom);

						for (vector<PersonMoment>::iterator citer = PersonMomentInfo.begin();
							citer != PersonMomentInfo.end(); ++citer)
						{
							INT32 y_face_center = (citer->faceBox.Bottom + citer->faceBox.Top) / 2;
							INT32 x_face_center = (citer->faceBox.Left + citer->faceBox.Right) / 2;
							//100的半径
							if (pow((x_center - x_face_center), 2) + pow((y_center - y_face_center), 2) > 10000)//找到之前的人脸
							{
								if (citer->z - ndepth > 1000)//距离变近
								{
									citer->dis_change = true;
									citer->z = ndepth;
									citer->appear = true;
								}
								flag = true;
								_cwprintf(L"%d\n", ndepth);
								//citer->faceImg = faceRecognizer->getFaceMat(faceBox, pBuffer, cColorWidth, cColorHeight);
								//imshow("1", citer->faceImg);							
							}
						}

						//存在空缺，可以插入新的人
						if (PersonMomentInfo.size() < 6 && flag == false && faceBox.Left > 0 && faceBox.Right < 1920 && faceBox.Top  > 0 && faceBox.Bottom < 1080)
						{
							PersonMoment res;
							res.z = ndepth;
							res.group = 0;
							res.dis_change = false;
							res.faceBox = faceBox;
							res.disapper_time = 0;
							res.start_time = nTime;
							res.faceImg = faceRecognizer->getFaceMat(faceBox, pBuffer, cColorWidth, cColorHeight);//新获得的人脸图像

							//插入代码检测
							thread t1(getFaceAge, res.faceImg, ref(res.age));
							thread t2(getFaceGender, res.faceImg, ref(res.gender));

							t1.detach();
							t2.detach();

							//res.age = getFaceAge(res.faceImg);
							//future<int>f2 = async(launch::async, getFaceGender, res.faceImg);
							//res.gender = f2.get();

							write_BVFace(&res, faceProperties);
							PersonMomentInfo.insert(PersonMomentInfo.end(),res);
						}
					}
				}
				SafeRelease(pFaceFrameResult);
			}
			else
			{
				// face tracking is not valid - attempt to fix the issue
				// a valid body is required to perform this step
				if (bHaveBodyData)
				{
					// check if the corresponding body is tracked 
					// if this is true then update the face frame source to track this body
					IBody* pBody = ppBodies[iFace];
					if (pBody != nullptr)
					{
						BOOLEAN bTracked = false;
						hr = pBody->get_IsTracked(&bTracked);

						UINT64 bodyTId;
						if (SUCCEEDED(hr) && bTracked)
						{
							// get the tracking ID of this body
							hr = pBody->get_TrackingId(&bodyTId);
							if (SUCCEEDED(hr))
							{
								// update the face frame source with the tracking ID
								m_pFaceFrameSources[iFace]->put_TrackingId(bodyTId);
							}
						}
					}
				}
			}
		}
		SafeRelease(pFaceFrame);
	}

	for (vector<PersonMoment>::iterator citer = PersonMomentInfo.begin();
		citer != PersonMomentInfo.end();)//删除长期未出现的人脸
	{

		int facecenter_x = (citer->faceBox.Left + citer->faceBox.Right) / 2;
		int flag[6] = { 0 };

		if (citer->appear = false)
		{
			citer->disapper_time += 1;
		}
		if (citer->disapper_time > 10)
		{
			mu.lock();
			citer = PersonMomentInfo.erase(citer);
			mu.unlock();
		}
		else
		{
			++citer;
		}

		//只关心组，并不关心当前组的具体编号
		//根据位置，深度，时间等信息判断关系
		//只关心新加入的

		if (flag[distance(PersonMomentInfo.begin(),citer)] == 0 && citer->group == 0)//未判断组别的关系
		{
			int find = 0;
			vector<PersonMoment>little_group;
			for (vector<PersonMoment>::iterator citer1 = citer;
				citer1 != PersonMomentInfo.end();)
			{
				int facecenter_x1 = (citer1->faceBox.Left + citer1->faceBox.Right) / 2;

				if (pow(facecenter_x - facecenter_x1, 2) < 2500 && pow(citer->z - citer1->z, 2) < 400 && find == 0)//相同group
				{
					little_group.insert(little_group.end(), *citer);
					flag[distance(PersonMomentInfo.begin(),citer)] = 1;
					little_group.insert(little_group.end(), *citer1);
					flag[distance(PersonMomentInfo.begin(), citer1)] = 1;
					find = 1;
				}
				if (pow(facecenter_x - facecenter_x1, 2) < 2500 && pow(citer->z - citer1->z, 2) < 400 && find == 1)
				{
					little_group.insert(little_group.end(), *citer1);
					flag[distance(PersonMomentInfo.begin(), citer1)] = 1;
				}
				citer++;
			}

			if (find == 0)
			{
				citer->group = 1;
			}
			if (!little_group.empty())
			{
				group.insert(group.end(), little_group);
			}
		}

		/*for (vector<PersonMoment>::iterator citer1 = PersonMomentInfo.begin();
			citer1 != citer;)
		{
			int facecenter_x1 = (citer1->faceBox.Left + citer1->faceBox.Right) / 2;

			if (pow(facecenter_x - facecenter_x1, 2) < 2500 && pow(citer->z - citer1->z, 2) < 400)//相同group
			{
				citer->group_id = citer1->group_id;
				break;
			}
		
			citer->group_id = 
		}*/
	}

	for (vector<vector<PersonMoment>>::iterator citer = group.begin();
		citer != group.end();citer++)
	{
		group_decision moment_group;
		moment_group.num = (*citer).size();
		int max = 0;
		int min = 10;
		int male = 0;
		int female = 0;
		for (vector<PersonMoment>::iterator citer1 = (*citer).begin();
			citer1 != (*citer).end();citer1++)
		{
			if (citer1->age > max)
			{
				max = citer1->age;
			}
			if (citer1->age < min)
			{
				min = citer1->age;
			}

			if (citer1->gender = 1)
			{
				male = 1;
			}
			else
			{
				female = 1;
			}
		}

		if (max - min <= 1)
		{
			moment_group.age_diff = 0;
		}
		else if (max - min == 2)
		{
			moment_group.age_diff = 1;
		}
		else
		{
			moment_group.age_diff = 2;
		}

		if (male == 1 && female == 0)
		{
			moment_group.gender_diff = 1;
		}
		else if (male == 0 && female == 1)
		{
			moment_group.gender_diff = 2;
		}
		else
		{
			moment_group.gender_diff = 3;
		}

		int type = moment_group.tree_predict(tableInfo, defaultClass, root);
		for (vector<PersonMoment>::iterator citer2 = (*citer).begin();
			citer2 != (*citer).end(); citer2++)
		{
			for (vector<PersonMoment>::iterator citer3 = PersonMomentInfo.begin();
				citer3 != PersonMomentInfo.end(); citer3++)
			{
				if (citer2->id == citer3->id && citer3->group == 0)
				{
					citer3->group = type;
					break;
				}
			}
		}
	}

	if (bHaveBodyData)
	{
		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

}

/// <summary>
/// Computes the face result text position by adding an offset to the corresponding 
/// body's head joint in camera space and then by projecting it to screen space
/// </summary>
/// <param name="pBody">pointer to the body data</param>
/// <param name="pFaceTextLayout">pointer to the text layout position in screen space</param>
/// <returns>indicates success or failure</returns>
HRESULT KinectDataAccess::GetFaceTextPositionInColorSpace(IBody* pBody, D2D1_POINT_2F* pFaceTextLayout)
{
	HRESULT hr = E_FAIL;

	if (pBody != nullptr)
	{
		BOOLEAN bTracked = false;
		hr = pBody->get_IsTracked(&bTracked);

		if (SUCCEEDED(hr) && bTracked)
		{
			Joint joints[JointType_Count];
			hr = pBody->GetJoints(_countof(joints), joints);
			if (SUCCEEDED(hr))
			{
				CameraSpacePoint headJoint = joints[JointType_Head].Position;
				CameraSpacePoint textPoint =
				{
					headJoint.X + c_FaceTextLayoutOffsetX,
					headJoint.Y + c_FaceTextLayoutOffsetY,
					headJoint.Z
				};

				ColorSpacePoint colorPoint = { 0 };
				hr = m_pCoordinateMapper->MapCameraPointToColorSpace(textPoint, &colorPoint);

				if (SUCCEEDED(hr))
				{
					pFaceTextLayout->x = colorPoint.X;
					pFaceTextLayout->y = colorPoint.Y;
				}
			}
		}
	}

	return hr;
}

/// <summary>
/// Updates body data
/// </summary>
/// <param name="ppBodies">pointer to the body data storage</param>
/// <returns>indicates success or failure</returns>
HRESULT KinectDataAccess::UpdateBodyData(IBody** ppBodies)
{
	HRESULT hr = E_FAIL;

	if (m_pBodyFrameReader != nullptr)
	{
		IBodyFrame* pBodyFrame = nullptr;
		hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
		}
		SafeRelease(pBodyFrame);
	}

	return hr;
}

/// <summary>
/// Main processing function
/// </summary>
void KinectDataAccess::Update()
{
	if (!m_pColorFrameReader || !m_pBodyFrameReader)
	{
		return;
	}

	IColorFrame* pColorFrame = nullptr;
	HRESULT hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = nullptr;
		int nWidth = 0;
		int nHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nBufferSize = 0;
		RGBQUAD *pBuffer = nullptr;


		hr = pColorFrame->get_RelativeTime(&nTime);

		if (SUCCEEDED(hr))
		{
			_PREV_TIME = _Time;
			_Time = nTime;
			hr = pColorFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
			}
			else if (m_pColorRGBX)
			{
				pBuffer = m_pColorRGBX;
				nBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		if (SUCCEEDED(hr))
		{
			DrawStreams(nTime, pBuffer, nWidth, nHeight);
		}

		SafeRelease(pFrameDescription);
	}

	SafeRelease(pColorFrame);
}

void KinectDataAccess::DEBUG_test_mapColorToDepth(RGBQUAD *pBuffer) {


	// Loop over each row and column of the color image
	// Zero out any pixels that don't correspond to a body index
	for (int colorIndex = 0; colorIndex < cColorHeight * cColorWidth; ++colorIndex)
	{
		float colorMappedToDepthX = m_DepthSpacePoints[colorIndex].X;
		float colorMappedToDepthY = m_DepthSpacePoints[colorIndex].Y;

		// The sentinel value is -inf, -inf, meaning that no depth pixel corresponds to this color pixel.
		if (colorMappedToDepthX != -std::numeric_limits<float>::infinity() &&
			colorMappedToDepthY != -std::numeric_limits<float>::infinity())
		{
			// Make sure the depth pixel maps to a valid point in color space
			int depthX = (int)(colorMappedToDepthX + 0.5f);
			int depthY = (int)(colorMappedToDepthY + 0.5f);

			if ((depthX >= 0) && (depthX < cDepthWidth) && (depthY >= 0) && (depthY < cDepthHeight))
			{
				int depthIndex = (depthY * cDepthWidth) + depthX;
				UINT8 intensity = m_pDepthBuffer[depthIndex] % 256;
				pBuffer[colorIndex].rgbBlue = pBuffer[colorIndex].rgbGreen = pBuffer[colorIndex].rgbRed = intensity;
				pBuffer[colorIndex].rgbReserved = 0;

			}
		}
	}

}

/// <summary>
/// Handle new depth data
/// <param name="nTime">timestamp of frame</param>
/// <param name="pBuffer">pointer to frame data</param>
/// <param name="nWidth">width (in pixels) of input image data</param>
/// <param name="nHeight">height (in pixels) of input image data</param>
/// <param name="nMinDepth">minimum reliable depth</param>
/// <param name="nMaxDepth">maximum reliable depth</param>
/// </summary>
void KinectDataAccess::ProcessDepth(INT64 nTime, const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	if (m_hWnd)
	{
	}

	// Make sure we've received valid data
	if (m_pDepthRGBX && pBuffer && (nWidth == cDepthWidth) && (nHeight == cDepthHeight))
	{
		RGBQUAD* pRGBX = m_pDepthRGBX;

		// end pixel is start + width*height - 1
		const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);

		while (pBuffer < pBufferEnd)
		{
			USHORT depth = *pBuffer;

			// To convert to a byte, we're discarding the most-significant
			// rather than least-significant bits.
			// We're preserving detail, although the intensity will "wrap."
			// Values outside the reliable depth range are mapped to 0 (black).

			// Note: Using conditionals in this loop could degrade performance.
			// Consider using a lookup table instead when writing production code.
			BYTE intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth % 256) : 0);

			pRGBX->rgbRed = intensity;
			pRGBX->rgbGreen = intensity;
			pRGBX->rgbBlue = intensity;

			++pRGBX;
			++pBuffer;
		}

		// Draw the data with Direct2D
		m_pDrawDepth->BeginDrawing();
		m_pDrawDepth->DrawBackground(reinterpret_cast<BYTE*>(m_pDepthRGBX), cDepthWidth * cDepthHeight * sizeof(RGBQUAD));
		m_pDrawDepth->EndDrawing();
	}
}

KinectDataAccess::KinectDataAccess():
	m_hWnd(NULL),
	m_nStartTime(0),
	m_nLastCounter(0),
	m_nFramesSinceUpdate(0),
	m_fFreq(0),
	m_nNextStatusTime(0),
	m_pKinectSensor(nullptr),
	m_pCoordinateMapper(nullptr),
	m_pColorFrameReader(nullptr),
	m_pBodyFrameReader(nullptr),
	m_pDepthFrameReader(nullptr),
	m_pDepthBuffer(nullptr),
	m_pColorRGBX(nullptr)
{
	m_WorldCoordinates = new CameraSpacePoint[cColorWidth * cColorHeight];
	LARGE_INTEGER qpf = { 0 };
	if (QueryPerformanceFrequency(&qpf))
	{
		m_fFreq = double(qpf.QuadPart);
	}

	for (int i = 0; i < BODY_COUNT; i++)
	{
		m_pFaceFrameSources[i] = nullptr;
		m_pFaceFrameReaders[i] = nullptr;
	}
	//
	//load model
	//
	tmp_DepthBuffer = new UINT16[cDepthWidth * cDepthHeight];
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];
	m_pDepthBuffer = new UINT16[cDepthHeight * cDepthWidth];
	m_DepthSpacePoints = new DepthSpacePoint[cColorWidth * cColorHeight];
	m_cameraSpacePoints = new CameraSpacePoint[cColorWidth * cColorHeight];
}

KinectDataAccess::~KinectDataAccess()
{
	// done with face sources and readers
	for (int i = 0; i < BODY_COUNT; i++)
	{
		SafeRelease(m_pFaceFrameSources[i]);
		SafeRelease(m_pFaceFrameReaders[i]);
	}

	if (m_pColorRGBX)
	{
		delete[] m_pColorRGBX;
		m_pColorRGBX = NULL;
	}

	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);

	// done with color frame reader
	SafeRelease(m_pColorFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int KinectDataAccess::Run(HINSTANCE hInstance, int nCmdShow)
{
	//create decision_tree 
	create_decision_tree();


	HRESULT hr = InitializeDefaultSensor();
	if (SUCCEEDED(hr))
	{
		MSG       msg = { 0 };
		WNDCLASS  wc;

		// Dialog custom window class
		ZeroMemory(&wc, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbWndExtra = DLGWINDOWEXTRA;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
		wc.lpfnWndProc = DefDlgProcW;
		wc.lpszClassName = L"FaceBasicsAppDlgWndClass";

		if (!RegisterClassW(&wc))
		{
			return 0;
		}

		// Create main application window
		HWND hWndApp = CreateDialogParamW(
			NULL,
			MAKEINTRESOURCE(IDD_APP),
			NULL,
			(DLGPROC)KinectDataAccess::MessageRouter,
			reinterpret_cast<LPARAM>(this));

		DWORD errorcode = GetLastError();
		// Show window
		ShowWindow(hWndApp, nCmdShow);

		// Main message loop
		while (WM_QUIT != msg.message)
		{
			Update();

			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// If a dialog message will be taken care of by the dialog proc
				if (hWndApp && IsDialogMessageW(hWndApp, &msg))
				{
					continue;
				}

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

		return static_cast<int>(msg.wParam);
	}
	else
	{
		return 0;
	}
}

//void KinectDataAccess::run() {
//
//	HRESULT hr;
//	hr = InitializeDefaultSensor();
//	if (SUCCEEDED(hr))
//	{
//		while (true) 
//		{
//			Update();
//		}
//	}
//
//	std::cout << "initialize failure.\n";
//	getchar();
//
//}

void KinectDataAccess::InitSettings() {

}

/// <summary>
/// Renders the color and face streams
/// </summary>
/// <param name="nTime">timestamp of frame</param>
/// <param name="pBuffer">pointer to frame data</param>
/// <param name="nWidth">width (in pixels) of input image data</param>
/// <param name="nHeight">height (in pixels) of input image data</param>
void KinectDataAccess::DrawStreams(INT64 nTime, RGBQUAD* pBuffer, int nWidth, int nHeight)
{
	//_cwprintf(L"%d\n", nWidth);
	if (m_hWnd)
	{
		HRESULT hr;
		hr = m_pDrawDataStreams->BeginDrawing();

		if (SUCCEEDED(hr))
		{
			// Make sure we've received valid color data
			if (pBuffer && (nWidth == cColorWidth) && (nHeight == cColorHeight))
			{
				// Draw the data with Direct2D
				hr = m_pDrawDataStreams->DrawBackground(reinterpret_cast<BYTE*>(pBuffer), cColorWidth * cColorHeight * sizeof(RGBQUAD));
			}
			else
			{
				// Recieved invalid data, stop drawing
				hr = E_INVALIDARG;
			}

			if (SUCCEEDED(hr))
			{
				// begin processing the face frames
				ProcessFaces(pBuffer);
			}

			m_pDrawDataStreams->EndDrawing();
		}

		if (!m_nStartTime)
		{
			m_nStartTime = nTime;
		}

		double fps = 0.0;

		LARGE_INTEGER qpcNow = { 0 };
		if (m_fFreq)
		{
			if (QueryPerformanceCounter(&qpcNow))
			{
				if (m_nLastCounter)
				{
					m_nFramesSinceUpdate++;
					fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
				}
			}
		}

		/*WCHAR szStatusMessage[64];
		StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

		if (SetStatusMessage(szStatusMessage, 1000, false))
		{
		m_nLastCounter = qpcNow.QuadPart;
		m_nFramesSinceUpdate = 0;
		}*/
	}
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK KinectDataAccess::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KinectDataAccess* pThis = nullptr;

	if (WM_INITDIALOG == uMsg)
	{
		pThis = reinterpret_cast<KinectDataAccess*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<KinectDataAccess*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK KinectDataAccess::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Bind application window handle
		m_hWnd = hWnd;

		// Init Direct2D
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

		// Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
		// We'll use this to draw the data we receive from the Kinect to the screen
		m_pDrawDataStreams = new ImageRenderer();
		m_pDrawDepth = new ImageRenderer();
		HRESULT hr = m_pDrawDataStreams->Initialize(GetDlgItem(m_hWnd, 1000), m_pD2DFactory, cColorWidth, cColorHeight, cColorWidth * sizeof(RGBQUAD));
		if (SUCCEEDED(hr))
			hr = m_pDrawDepth->Initialize(GetDlgItem(m_hWnd, 1000), m_pD2DFactory, cDepthWidth, cDepthHeight, cDepthWidth * sizeof(RGBQUAD));
		// Get and initialize the default Kinect sensor
		InitializeDefaultSensor();
	}
	break;
	//case 
	// If the titlebar X is clicked, destroy app
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// Quit the main message pump
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}
