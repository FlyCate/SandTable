#pragma once
#include "RoadPlan.h"

// CMapDlg 对话框

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMapDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;
	TAGPOS m_pos;
	int m_nDrawWidth;
	int m_nDrawHeight;

public:
	TAGPOS GetPos();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
