#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SandTableDB.h"

// CCrossingDlg 对话框

class CCrossingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCrossingDlg)

public:
	CCrossingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCrossingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CROSSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void InsertData(list<tagCrossing> listCrossing);
	void InsertData(tagCrossing tCrossingInfo);
	void InitListControl();

private:
	void * m_parent;

public:
	CListCtrl m_listData;
	CString m_strRoadName;
	CString m_strRoadID;
	CComboBox m_RoadDirect;
	CString m_strStraightID;
	CComboBox m_StraightDirect;
	CString m_DextroversionID;
	CComboBox m_DextroversionDirect;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSelect();
	
//	afx_msg void OnCbnEditchangeComboRoadDirect();
	afx_msg void OnCbnSelchangeComboRoadDirect();
	afx_msg void OnCbnSelchangeComboStraightDirect();
	afx_msg void OnCbnSelchangeComboDextroversionDirect();
};
