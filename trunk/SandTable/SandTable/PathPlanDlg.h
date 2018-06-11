#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "RoadPlan.h"

// CPathPlanDlg 对话框

class CPathPlanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPathPlanDlg)

public:
	CPathPlanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPathPlanDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PATH_PLAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;

public:
	CRoadPlan m_roadPlan;
	TAGPOS m_StartPos;
	TAGPOS m_EndPos;
	int m_nStartCardID;
	int m_nEndCardID;
	CListCtrl m_PathList;
	const char * GetExePath();
	afx_msg void OnBnClickedButtonSearch();
	virtual BOOL OnInitDialog();
	CString m_strStart;
	CString m_strEnd;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonEnd();
	void InsertPathList(const char * str);
	afx_msg void OnBnClickedButtonOk();
	void SendCmd();
};
