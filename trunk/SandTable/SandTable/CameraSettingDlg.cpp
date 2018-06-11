// CameraSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "CameraSettingDlg.h"
#include "afxdialogex.h"
#include "SandTableDlg.h"
#include "CvvImage.h"
#include "SandTableDB.h"

// CCameraSettingDlg �Ի���

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


// CCameraSettingDlg ��Ϣ�������

BOOL CCameraSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	OnBnClickedButtonFresh();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//�����Ԥ��
void CCameraSettingDlg::OnBnClickedButtonRed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_camera.SetCameraID(GetRedCameraID());
	long lRet = m_camera.OpenCamera();
	if (lRet != 0)
	{
		MessageBox("������ͷʧ�ܣ�Ԥ��ʧ�ܣ�");
		return;
	}

	while (m_camera.SnapshotCamera("Ԥ��.jpg") != OPENCV_CAMERA_SUCCESS)
	{
		static int i = 0;
		if (i++ > 10)
		{
			MessageBox("ץ��ʧ�ܣ�����ͷ���ϣ�");
			m_camera.CloseCamera();
			return;
		}
		Sleep(50);
	}
	/////////////////////////////////////////  
	//Mat to CvvImage  
	IplImage iplSrcImg = m_camera.GetSnapshotFrame();    //ע��ǳ����ֻ��������ͷ  
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

//����Ԥ��
void CCameraSettingDlg::OnBnClickedButtonSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_camera.SetCameraID(GetSpeedCameraID());
	long lRet = m_camera.OpenCamera();
	if (lRet != 0)
	{
		MessageBox("������ͷʧ�ܣ�Ԥ��ʧ�ܣ�");
		return;
	}

	while (m_camera.SnapshotCamera("Ԥ��.jpg") != OPENCV_CAMERA_SUCCESS)
	{
		static int i = 0;
		if (i++ > 10)
		{
			MessageBox("ץ��ʧ�ܣ�����ͷ���ϣ�");
			m_camera.CloseCamera();
			return;
		}
		Sleep(50);
	}
	/////////////////////////////////////////  
	//Mat to CvvImage  
	IplImage iplSrcImg = m_camera.GetSnapshotFrame();    //ע��ǳ����ֻ��������ͷ  
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

//ˢ��
void CCameraSettingDlg::OnBnClickedButtonFresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	m_RedList.InsertString(m_RedList.GetCount(), "��");
	m_SpeedList.InsertString(m_SpeedList.GetCount(), "��");
}

//Ӧ��
void CCameraSettingDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RedList.GetCurSel() == m_SpeedList.GetCurSel())
	{
		MessageBox("������볬��ѡ����ͬһ������ͷ��������ѡ��");
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

		tCamera.strCameraType = "�����";
		tCamera.nCameraID = GetRedCameraID();
		bRet = tDB.WriteDB(tCamera);

		tCamera.strCameraType = "����";
		tCamera.nCameraID = GetSpeedCameraID();
		bRet = tDB.WriteDB(tCamera);

		tDB.CloseDB();
	}
}

int CCameraSettingDlg::GetRedCameraID()
{
	CString str;
	m_RedList.GetWindowTextA(str);
	if (str.Compare("��") == 0)
	{
		return -1;
	}

	return m_RedList.GetCurSel();
}

int CCameraSettingDlg::GetSpeedCameraID()
{
	CString str;
	m_SpeedList.GetWindowTextA(str);
	if (str.Compare("��") == 0)
	{
		return -1;
	}

	return m_SpeedList.GetCurSel();
}


void CCameraSettingDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}
