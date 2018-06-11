#pragma once
#include "RoadPlan.h"
#include "afxwin.h"

// CChooseRoad 对话框

class CChooseRoad : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseRoad)

public:
	CChooseRoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChooseRoad();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_ComboBox;
	list<tagRoad> m_lstRoad;

public:
	CString m_strRoadName;
	int m_nRoadID;

public:
	void SetRoadList(list<tagRoad> lstRoad);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
