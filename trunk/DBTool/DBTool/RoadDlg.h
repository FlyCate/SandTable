#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SandTableDB.h"

// CRoadDlg 对话框

class CRoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoadDlg)

public:
	CRoadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ROADINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	void * m_parent;

public:
	CString m_strRoadName;
	CComboBox m_RoadDirect;
	CComboBox m_StartCard;
	CComboBox m_StartCrossingID;
	CComboBox m_StartCrossingDirect;
	CComboBox m_EndCard;
	CComboBox m_EndCrossingID;
	CComboBox m_EndCrossingDirect;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSelect();
	void InsertData(list<tagRoad> listRoad);
	void InsertData(tagRoad tRoadInfo);
	void InitListControl();
	CListCtrl m_listData;
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnCbnDropdownCombo3();
	afx_msg void OnCbnDropdownCombo5();
	afx_msg void OnCbnDropdownCombo6();
};
