// MapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "MapDlg.h"
#include "afxdialogex.h"
#include "PathPlanDlg.h"

// CMapDlg �Ի���

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapDlg::IDD, pParent)
{
	m_parent = pParent;
}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CMapDlg ��Ϣ�������
BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CPathPlanDlg * dlg = (CPathPlanDlg *) m_parent;

	RECT rect;
	//���õ�ͼͼƬ
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_DRAW); 
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//��ʾͼƬ
	CString strPicPath = dlg->GetExePath();
	strPicPath += "res/MapLow.bmp";
	::GetWindowRect(GetDlgItem(IDC_STATIC_DRAW)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //��Դ�Ż򱾵��ļ���
		IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
		rect.right - rect.left,                  //��� 0ΪĬ�ϴ�С
		rect.bottom - rect.top,                  //�߶� ����Ϊ��λ
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nDrawWidth = rect.right - rect.left;
	m_nDrawHeight = rect.bottom - rect.top;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

TAGPOS CMapDlg::GetPos()
{
	m_pos.y = m_nDrawHeight - m_pos.y;
	m_pos.x = m_pos.x * MAP_LOW_WIDTH / m_nDrawWidth;
	m_pos.y = m_pos.y * MAP_LOW_HEIGHT / m_nDrawHeight;

	return m_pos;
}


void CMapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_pos.x = point.x;
	m_pos.y = point.y;

	OnOK();

	CDialogEx::OnLButtonUp(nFlags, point);
}
