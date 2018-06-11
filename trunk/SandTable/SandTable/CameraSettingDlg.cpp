// CameraSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "CameraSettingDlg.h"
#include "afxdialogex.h"
#include "SandTableDlg.h"
#include "CvvImage.h"
#include "SandTableDB.h"

// CCameraSettingDlg 对话框

IMPLEMENT_DYNAMIC(CCameraSettingDlg, CDialogEx)

CCameraSettingDlg::CCameraSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSettingDlg::IDD, pParent)
{
	m_parent = pParent;
}

CCameraSettingDlg::~CCameraSettingDlg()
{
}

void CCameraSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RED, m_RedList);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_SpeedList);
}


BEGIN_MESSAGE_MAP(CCameraSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RED, &CCameraSettingDlg::OnBnClickedButtonRed)
	ON_BN_CLICKED(IDC_BUTTON_SPEED, &CCameraSettingDlg::OnBnClickedButtonSpeed)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, &CCameraSettingDlg::OnBnClickedButtonFresh)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCameraSettingDlg::OnBnClickedButtonSave)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCameraSettingDlg 消息处理程序

BOOL CCameraSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	OnBnClickedButtonFresh();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//闯红灯预览
void CCameraSettingDlg::OnBnClickedButtonRed()
{
	// TODO: 在此添加控件通知处理程序代码
	m_camera.SetCameraID(GetRedCameraID());
	long lRet = m_camera.OpenCamera();
	if (lRet != 0)
	{
		MessageBox("打开摄像头失败，预览失败！");
		return;
	}

	while (m_camera.SnapshotCamera("预览.jpg") != OPENCV_CAMERA_SUCCESS)
	{
		static int i = 0;
		if (i++ > 10)
		{
			MessageBox("抓拍失败，摄像头故障！");
			m_camera.CloseCamera();
			return;
		}
		Sleep(50);
	}
	/////////////////////////////////////////  
	//Mat to CvvImage  
	IplImage iplSrcImg = m_camera.GetSnapshotFrame();    //注：浅拷贝只复制数据头  
	CvvImage cvvSrcImg;  
	cvvSrcImg.CopyOf( &iplSrcImg ); 
	/////////////////////////////////////////

	//show image
	CDC* pDC = GetDlgItem(IDC_STATIC_DRAW)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_STATIC_DRAW)->GetClientRect(&rect);
	cvvSrcImg.DrawToHDC(hDC, &rect); 
	cvvSrcImg.Destroy();

	ReleaseDC( pDC );

	m_camera.CloseCamera();
}

//超速预览
void CCameraSettingDlg::OnBnClickedButtonSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	m_camera.SetCameraID(GetSpeedCameraID());
	long lRet = m_camera.OpenCamera();
	if (lRet != 0)
	{
		MessageBox("打开摄像头失败，预览失败！");
		return;
	}

	while (m_camera.SnapshotCamera("预览.jpg") != OPENCV_CAMERA_SUCCESS)
	{
		static int i = 0;
		if (i++ > 10)
		{
			MessageBox("抓拍失败，摄像头故障！");
			m_camera.CloseCamera();
			return;
		}
		Sleep(50);
	}
	/////////////////////////////////////////  
	//Mat to CvvImage  
	IplImage iplSrcImg = m_camera.GetSnapshotFrame();    //注：浅拷贝只复制数据头  
	CvvImage cvvSrcImg;  
	cvvSrcImg.CopyOf( &iplSrcImg ); 
	/////////////////////////////////////////

	//show image
	CDC* pDC = GetDlgItem(IDC_STATIC_DRAW)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_STATIC_DRAW)->GetClientRect(&rect);
	cvvSrcImg.DrawToHDC(hDC, &rect); 
	cvvSrcImg.Destroy();

	ReleaseDC(pDC);

	m_camera.CloseCamera();
}

//刷新
void CCameraSettingDlg::OnBnClickedButtonFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	m_camera.GetCameraList(dlg->m_mapCameraList);

	m_RedList.ResetContent();
	m_SpeedList.ResetContent();

	for (auto & it = dlg->m_mapCameraList.begin(); it != dlg->m_mapCameraList.end(); it++)
	{
		m_RedList.InsertString(it->first, it->second.c_str());
		m_SpeedList.InsertString(it->first, it->second.c_str());
		m_RedList.SetCurSel(dlg->m_RedCamera.GetCameraID());
		m_SpeedList.SetCurSel(dlg->m_SpeedCamera.GetCameraID());
	}

	m_RedList.InsertString(m_RedList.GetCount(), "无");
	m_SpeedList.InsertString(m_SpeedList.GetCount(), "无");
}

//应用
void CCameraSettingDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RedList.GetCurSel() == m_SpeedList.GetCurSel())
	{
		MessageBox("闯红灯与超速选择了同一个摄像头！请重新选择！");
		return;
	}

	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	dlg->m_RedCamera.SetCameraID(GetRedCameraID());
	dlg->m_SpeedCamera.SetCameraID(GetSpeedCameraID());

	CSandTableDB tDB;
	bool bRet = tDB.OpenDB(SANDTABLE_CAMERA);
	if (bRet == true)
	{
		tagCamera tCamera;
		bRet = tDB.DeleteDB(tCamera);

		tCamera.strCameraType = "闯红灯";
		tCamera.nCameraID = GetRedCameraID();
		bRet = tDB.WriteDB(tCamera);

		tCamera.strCameraType = "超速";
		tCamera.nCameraID = GetSpeedCameraID();
		bRet = tDB.WriteDB(tCamera);

		tDB.CloseDB();
	}
}

int CCameraSettingDlg::GetRedCameraID()
{
	CString str;
	m_RedList.GetWindowTextA(str);
	if (str.Compare("无") == 0)
	{
		return -1;
	}

	return m_RedList.GetCurSel();
}

int CCameraSettingDlg::GetSpeedCameraID()
{
	CString str;
	m_SpeedList.GetWindowTextA(str);
	if (str.Compare("无") == 0)
	{
		return -1;
	}

	return m_SpeedList.GetCurSel();
}


void CCameraSettingDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
