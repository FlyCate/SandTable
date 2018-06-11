// RechargeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "RechargeDlg.h"
#include "afxdialogex.h"


// CRechargeDlg 对话框

IMPLEMENT_DYNAMIC(CRechargeDlg, CDialogEx)

CRechargeDlg::CRechargeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRechargeDlg::IDD, pParent)
	, m_strMoney(_T(""))
{
	m_parent = pParent;
	m_lMoney = 10;
}

CRechargeDlg::~CRechargeDlg()
{
}

void CRechargeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strMoney);
}


BEGIN_MESSAGE_MAP(CRechargeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CRechargeDlg::OnBnClickedButtonCancle)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CRechargeDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_LESS, &CRechargeDlg::OnBnClickedButtonLess)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CRechargeDlg::OnBnClickedButtonPlus)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRechargeDlg 消息处理程序

//退出
void CRechargeDlg::OnBnClickedButtonCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//确定
void CRechargeDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

//减
void CRechargeDlg::OnBnClickedButtonLess()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_lMoney = atoi(m_strMoney.GetBuffer());

	m_lMoney -= 10;
	if (m_lMoney < 10)
	{
		m_lMoney = 10;
	}

	m_strMoney.Format("%d", m_lMoney);
	UpdateData(FALSE);
}

//加
void CRechargeDlg::OnBnClickedButtonPlus()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_lMoney = atoi(m_strMoney.GetBuffer());

	m_lMoney += 10;
	if (m_lMoney > 200)
	{
		m_lMoney = 200;
	}

	m_strMoney.Format("%d", m_lMoney);
	UpdateData(FALSE);
}

//初始化
void CRechargeDlg::Init()
{
	m_lMoney = 10;
	m_strMoney.Format("%d", m_lMoney);
	UpdateData(FALSE);
}

void CRechargeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	Init();
}
