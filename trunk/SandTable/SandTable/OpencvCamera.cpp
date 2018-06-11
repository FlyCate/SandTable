#include "StdAfx.h"
#include "OpencvCamera.h"
#include <process.h>

#pragma comment( lib, "opencv_highgui2410d.lib")
#pragma comment( lib, "opencv_core2410d.lib")

void CaptureThread(void * arg)
{
	COpencvCamera * ovCapture = (COpencvCamera *) arg;
	ovCapture->m_bCaptureThreadExit = false;

	Mat frame;
	
	while (ovCapture->m_bCaptureRun == true)
	{
		ovCapture->m_Capture >> frame;
		if (frame.empty())
		{
			Sleep(10);
		}

		if (ovCapture->m_CaptureCallBack != NULL)
		{
			ovCapture->m_CaptureCallBack(frame);
		}

		Sleep(40);
	}
	ovCapture->m_bCaptureThreadExit = true;
}

const char * itoaa(const int nNum)
{
	static char szTemp[64] = { 0 };

	memset(szTemp, 0, 64);
	sprintf(szTemp, "%d", nNum);

	return szTemp;
}

COpencvCamera::COpencvCamera(void)
{
	m_lCameraID = 100;
	m_CaptureCallBack = NULL;
	m_bIsCameraOpen = false;
	m_bCaptureRun = false;
	m_bCaptureThreadExit = true;
	m_mFrame.release();
	m_Capture.release();
}

COpencvCamera::~COpencvCamera(void)
{
	m_bIsCameraOpen = false;
	m_mFrame.release();
	m_Capture.release();
}

long COpencvCamera::GetCameraList(map<long, string> & mapCameraList)
{
	mapCameraList.clear();

	for (int i = 0; i < 10; i++)
	{
		VideoCapture capture(i);

		if (capture.isOpened())
		{
			string strCameraName = "摄像头";
			strCameraName += itoaa(i);
			mapCameraList[i] = strCameraName;
		}
		capture.release();
	}

	return mapCameraList.size();
}

long COpencvCamera::OpenCamera()
{
	if (m_bIsCameraOpen == true)
	{
		TRACE("摄像头已经打开!");
		return OPENCV_CAMERA_SUCCESS;
	}

	m_Capture.open(m_lCameraID);

	if (m_Capture.isOpened() == false)
	{
		TRACE("摄像头打开失败!");
		return OPENCV_CAMERA_ERROR;
	}

	m_bIsCameraOpen = true;
	return OPENCV_CAMERA_SUCCESS;
}

long COpencvCamera::CloseCamera()
{
	if (m_bIsCameraOpen == false)
	{
		TRACE("摄像头已经关闭!");
		return OPENCV_CAMERA_SUCCESS;
	}

	m_Capture.release();
	if (m_Capture.isOpened() == true)
	{
		TRACE("摄像头关闭失败!");
		return OPENCV_CAMERA_ERROR;
	}

	m_bIsCameraOpen = false;
	return OPENCV_CAMERA_SUCCESS;
}

long COpencvCamera::SnapshotCamera(const char * pFilePath)
{
	if (m_bIsCameraOpen == false)
	{
		return OPENCV_CAMERA_ERROR;
	}

	m_mFrame.release();
	m_Capture >> m_mFrame; 
	if (m_mFrame.empty())
	{
		TRACE("抓拍失败！");
		return OPENCV_CAMERA_ERROR;
	}

	imwrite(pFilePath, m_mFrame);

	return OPENCV_CAMERA_SUCCESS;
}

const Mat & COpencvCamera::GetSnapshotFrame()
{
	return m_mFrame;
}

long COpencvCamera::BeginCaptureThread(CaptureCallBack capture)
{
	if (capture == NULL)
	{
		return OPENCV_CAMERA_ERROR;
	}

	m_CaptureCallBack = capture;
	m_bCaptureRun = true;

	uintptr_t hStdOut = _beginthread(CaptureThread, 0, this);
	if (hStdOut == -1)
	{
		TRACE("启动捕获线程失败\n");
		return OPENCV_CAMERA_ERROR;
	}

	TRACE("线程启动成功!\n");
	return OPENCV_CAMERA_SUCCESS;
}

long COpencvCamera::EndCaptureThread()
{
	m_bCaptureRun = false;
	while(m_bCaptureThreadExit == false);
	TRACE("线程退出成功!\n");
	return OPENCV_CAMERA_SUCCESS;
}

long COpencvCamera::GetCameraID()
{
	return m_lCameraID;
}

long COpencvCamera::SetCameraID(const long & lCameraID)
{
	if (lCameraID < 0)
	{
		m_lCameraID = 100;
		return OPENCV_CAMERA_SUCCESS;
	}
	m_lCameraID = lCameraID;
	return OPENCV_CAMERA_SUCCESS;
}
