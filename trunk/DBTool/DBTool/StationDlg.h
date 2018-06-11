#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SandTableDB.h"

// CStationDlg �Ի���

class CStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStationDlg)

public:
	CStationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;

public:
	virtual BOOL OnInitDialog();

public:
	CListCtrl m_listData;
	CString m_StationName;
	CComboBox m_RoadID;
	CComboBox m_RoadDirect;
	CString m_StationCard;
	CString m_StationGroup;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSelect();
	void InsertData(list<tagStation> listStation);
	void InsertData(tagStation tStationInfo);
	void InitListControl();
	afx_msg void OnCbnDropdownComboRoadId();
};
