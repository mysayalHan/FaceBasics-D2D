#pragma once

#include "BVFaceGet.h"
#include <opencv\cv.h>
#include "ImageRenderer.h"
#include "resource.h"
#include "BVFace.h"
#include "SomeStruct.h"

class KinectDataAccess
{

public:
	void create_decision_tree();
	void run();
	/// <summary>
	/// Constructor
	/// </summary>
	KinectDataAccess();
	//����ÿһ֡���������ļ���,���ھ���ģ��
	static std::vector<PersonMoment> PersonMomentInfo;

	/// <summary>
	/// Destructor
	/// </summary>
	~KinectDataAccess();

	/// <summary>
	/// Creates the main window and begins processing
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="nCmdShow"></param>
	int                    Run(HINSTANCE hInstance, int nCmdShow);

private:
	/// <summary>
	/// Main processing function
	/// </summary>
	void                   Update();

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>S_OK on success else the failure code</returns>
	HRESULT                InitializeDefaultSensor();

	/// <summary>
	/// Initializes the default settings
	/// </summary>
	void                   InitSettings();


	/// <summary>
	/// Processes new face frames
	/// </summary>
	void                   ProcessFaces(RGBQUAD * pBuffer);

	void				   ProcessDepth(INT64 nTime, const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);
	HRESULT                GetFaceTextPositionInColorSpace(IBody* pBody, D2D1_POINT_2F* pFaceTextLayout);
	void				   DEBUG_test_mapColorToDepth(RGBQUAD * pBuffer);
	/// <summary>
	/// Print the message
	/// </summary>
	void				   PrintMessage(int FaceNumber, const DetectionResult* pFaceProperties, const int * otherResults);
	void				   write_BVFace(PersonMoment *, const DetectionResult* pFaceProperties);

	BVFaceGet *	   faceRecognizer;

	/// <summary>
	/// Updates body data
	/// </summary>
	/// <param name="ppBodies">pointer to the body data storage</param>
	/// <returns>indicates success or failure</returns>
	HRESULT                UpdateBodyData(IBody** ppBodies);
	HRESULT				   MapColorToCameraCoordinates(const std::vector<ColorSpacePoint>& colorsps, std::vector<CameraSpacePoint>& camerasps);

	CameraSpacePoint* m_cameraSpacePoints;
	INT64                  m_nStartTime;
	INT64                  m_nLastCounter;
	double                 m_fFreq;
	ULONGLONG              m_nNextStatusTime;
	DWORD                  m_nFramesSinceUpdate;


	//-----------------------------------------------------------
	UINT16* m_pDepthBuffer;							/*�洢�����Ϣ�ľ��󣨶�άѹ��һά��*/

	IDepthFrameReader*  m_pDepthFrameReader;		/*���ͼreader*/

	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

	static const int        cColorWidth = 1920;
	static const int        cColorHeight = 1080;

	DepthSpacePoint* m_DepthSpacePoints;

	UINT16 * tmp_DepthBuffer;

	CameraSpacePoint* m_WorldCoordinates;			/*����ͼ��ÿһ�����Ӧ����������*/


													// Current Kinect
	IKinectSensor*         m_pKinectSensor;


	// Coordinate mapper
	ICoordinateMapper*     m_pCoordinateMapper;

	// Color reader
	IColorFrameReader*     m_pColorFrameReader;

	// Body reader
	IBodyFrameReader*      m_pBodyFrameReader;

	// Face sources
	IFaceFrameSource*	   m_pFaceFrameSources[BODY_COUNT];

	// Face readers
	IFaceFrameReader*	   m_pFaceFrameReaders[BODY_COUNT];

	//ImageRenderer*         m_pDrawDataStreams;

	HWND                   m_hWnd;

	INT64 _Time;
	INT64 _PREV_TIME;
	INT64 _DEL_TIME;

	/// <summary>
	/// Renders the color and face streams
	/// </summary>			
	/// <param name="nTime">timestamp of frame</param>
	/// <param name="pBuffer">pointer to frame data</param>
	/// <param name="nWidth">width (in pixels) of input image data</param>
	/// <param name="nHeight">height (in pixels) of input image data</param>
	void                   DrawStreams(INT64 nTime, RGBQUAD* pBuffer, int nWidth, int nHeight);

	static LRESULT  CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT  CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// Direct2D
	ImageRenderer*         m_pDrawDataStreams;
	ID2D1Factory*          m_pD2DFactory;
	ImageRenderer*          m_pDrawDepth;
	RGBQUAD*                m_pDepthRGBX;
	RGBQUAD*                m_pColorRGBX;

};
