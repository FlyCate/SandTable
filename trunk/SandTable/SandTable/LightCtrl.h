#pragma once
#include "afxwin.h"
#include "resource.h"

// CLightCtrl 对话框

class CLightCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CLightCtrl)

public:
	CLightCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLightCtrl();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void * m_parent;
public:
	afx_msg void OnPaint();
	int m_RadioValue;
	afx_msg void OnBnDropDownRadioControl();
	afx_msg void OnBnClickedRadioAutoControl();
	afx_msg void OnBnClickedButtonRoadLight();
	afx_msg void OnBnClickedButtonRoadLightClose();
	afx_msg void OnBnClickedButtonLandscapeLight();
	afx_msg void OnBnClickedButtonLandscapeLightClose();
	afx_msg void OnBnClickedButtonBuildingLight();
	afx_msg void OnBnClickedButtonBuildingLightClose();
	afx_msg void OnBnClickedButtonAllLight();
	afx_msg void OnBnClickedButtonAllLightClose();
};
