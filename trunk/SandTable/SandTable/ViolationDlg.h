#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CViolationDlg �Ի���

class CViolationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViolationDlg)

public:
	CViolationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViolationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_VIOLATION_INQUIRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void * m_parent;
	int m_nSelectItem;

public:
	CComboBox m_ViolationType;
	CListCtrl m_listViolation;
	void InsertData(const char * lID, const char * strType, const char * picName);
	void DrawPic(const char * szPicName);
	afx_msg void OnNMClickListVolite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFresh();
	afx_msg void OnBnClickedButtonDel();
};
