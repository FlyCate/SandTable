// Question.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "Question.h"
#include "afxdialogex.h"


// CQuestion 对话框

IMPLEMENT_DYNAMIC(CQuestion, CDialogEx)

CQuestion::CQuestion(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuestion::IDD, pParent)
	, m_strQuestion(_T(""))
{

}

CQuestion::~CQuestion()
{
}

void CQuestion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CTRL, m_strQuestion);
}


BEGIN_MESSAGE_MAP(CQuestion, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CQuestion::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CQuestion 消息处理程序


void CQuestion::SetQuestion(const char * pQuestion)
{
	m_strQuestion = pQuestion;
	SetTimer(0, 1000, NULL);
	m_nCount = 0;
	UpdateData(FALSE);
}

void CQuestion::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(FALSE);
}


void CQuestion::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 0)
	{
		m_nCount++;
		if (m_nCount == 3)
		{
			KillTimer(0);
			ShowWindow(FALSE);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CQuestion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CRect mainrt,newrct;
	GetWindowRect(mainrt);
	newrct.left = nScreenWidth/2 - mainrt.Width()/2;
	newrct.right += newrct.left + mainrt.Width();
	newrct.top =  nScreenHeight/2 - mainrt.Height()/2;
	newrct.bottom =  newrct.top +  mainrt.Height();
	this->MoveWindow(newrct);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
