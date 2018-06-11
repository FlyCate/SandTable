#pragma once
#include "afxwin.h"
#include "OpencvCamera.h"

// CCameraSettingDlg �Ի���

class CCameraSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraSettingDlg)

public:
	CCameraSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;
	COpencvCamera m_camera;

public:
	CComboBox m_RedList;
	CComboBox m_SpeedList;
	afx_msg void OnBnClickedButtonRed();
	afx_msg void OnBnClickedButtonSpeed();
	afx_msg void OnBnClickedButtonFresh();
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog();
	int GetRedCameraID();
	int GetSpeedCameraID();
	afx_msg void OnClose();
};
