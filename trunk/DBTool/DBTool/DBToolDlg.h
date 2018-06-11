
// DBToolDlg.h : ͷ�ļ�
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

// CDBToolDlg �Ի���
class CDBToolDlg : public CDialogEx
{
// ����
public:
	CDBToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DBTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
