#pragma once
#include "afxcmn.h"
#include "SandTableDB.h"

// CCardDlg �Ի���

class CCardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCardDlg)

public:
	CCardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCardDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;

public:
	void InitListControl();

public:
	CListCtrl m_listData;
	CString m_strCardNum;
	CString m_strXPos;
	CString m_strYPos;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSelect();
	void InsertData(list<tagCard> listCard);
	void InsertData(tagCard tCardInfo);
	CString m_strTotalHeigh;
	CString m_strRealHeight;
	afx_msg void OnEnChangeEditRealHeigh();
};
