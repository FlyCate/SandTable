#pragma once


// CQuestion 对话框

class CQuestion : public CDialogEx
{
	DECLARE_DYNAMIC(CQuestion)

public:
	CQuestion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuestion();

// 对话框数据
	enum { IDD = IDD_DIALOG_OK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCount;
	CString m_strQuestion;
	void SetQuestion(const char * pQuestion);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
