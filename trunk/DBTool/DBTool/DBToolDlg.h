
// DBToolDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "CardDlg.h"
#include "CrossingDlg.h"
#include "RoadDlg.h"
#include "SandTableDB.h"
#include "StationDlg.h"
#include "RoadToCard.h"

extern const char * SANDTABLE_NAME;

const char * GetDirectStr(const int nDirect);
const int GetDirectInt(const char * strDirect);
const int GetStraightDirect(int nDirect);
const int GetDextroversionDirect(int nDirect);
const int GetLeftDirect(int nDirect);
const char * GetRoadDirectStr(const int nDirect);
const int GetRoadDirectInt(const char * strDirecit);
const char * itoaa(int nNum);

// CDBToolDlg 对话框
class CDBToolDlg : public CDialogEx
{
// 构造
public:
	CDBToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DBTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listTables;
	CCardDlg * m_CardDlg;
	CCrossingDlg * m_CrossingDlg;
	CRoadDlg * m_RoadDlg;
	CStationDlg * m_StationDlg;
	CRoadToCard * m_RoadToCardDlg;

	CSandTableDB m_DBMgr;

private:
	void InsertData(const char * lID);
public:
	afx_msg void OnNMClickListTables(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMove(int x, int y);
};
