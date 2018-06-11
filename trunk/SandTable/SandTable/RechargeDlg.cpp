// RechargeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "RechargeDlg.h"
#include "afxdialogex.h"


// CRechargeDlg �Ի���

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


// CRechargeDlg ��Ϣ�������

//�˳�
void CRechargeDlg::OnBnClickedButtonCancle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//ȷ��
void CRechargeDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

//��
void CRechargeDlg::OnBnClickedButtonLess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//��
void CRechargeDlg::OnBnClickedButtonPlus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//��ʼ��
void CRechargeDlg::Init()
{
	m_lMoney = 10;
	m_strMoney.Format("%d", m_lMoney);
	UpdateData(FALSE);
}

void CRechargeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	Init();
}
