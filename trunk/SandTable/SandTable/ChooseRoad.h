#pragma once
#include "RoadPlan.h"
#include "afxwin.h"

// CChooseRoad �Ի���

class CChooseRoad : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseRoad)

public:
	CChooseRoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChooseRoad();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
