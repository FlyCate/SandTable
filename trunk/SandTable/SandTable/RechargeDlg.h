#pragma once


// CRechargeDlg �Ի���

class CRechargeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRechargeDlg)

public:
	CRechargeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRechargeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	
private:
	void * m_parent;
public:
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonLess();
	afx_msg void OnBnClickedButtonPlus();
	void Init();
	CString m_strMoney;
	unsigned long m_lMoney;
	afx_msg void OnPaint();
};
