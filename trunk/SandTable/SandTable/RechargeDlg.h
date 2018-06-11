#pragma once


// CRechargeDlg 对话框

class CRechargeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRechargeDlg)

public:
	CRechargeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRechargeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
