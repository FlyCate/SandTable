#pragma once
#include "afxwin.h"
#include "OpencvCamera.h"

// CCameraSettingDlg 对话框

class CCameraSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraSettingDlg)

public:
	CCameraSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
