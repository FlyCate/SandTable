#pragma once

#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>    
using namespace cv; 

#include <map>
using namespace std;

#define OPENCV_CAMERA_SUCCESS 0
#define OPENCV_CAMERA_ERROR   -1

typedef int (*CaptureCallBack)(Mat & frame);

class COpencvCamera
{
public:
	COpencvCamera(void);
	~COpencvCamera(void);

public:
	long GetCameraList(map<long, string> & mapCameraList);
	long OpenCamera();
	long CloseCamera();
	long SnapshotCamera(const char * pFilePath);
	const Mat & GetSnapshotFrame();
	long BeginCaptureThread(CaptureCallBack capture);
	long EndCaptureThread();
	friend void CaptureThread(void * arg);
	long GetCameraID();
	long SetCameraID(const long & lCameraID);

private:
	long m_lCameraID;
	bool m_bIsCameraOpen;
	Mat  m_mFrame;
	VideoCapture m_Capture;
	CaptureCallBack m_CaptureCallBack;
	bool m_bCaptureRun;
	bool m_bCaptureThreadExit;
};

