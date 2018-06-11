// MapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "MapDlg.h"
#include "afxdialogex.h"
#include "PathPlanDlg.h"

// CMapDlg 对话框

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


// CMapDlg 消息处理程序
BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CPathPlanDlg * dlg = (CPathPlanDlg *) m_parent;

	RECT rect;
	//设置地图图片
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_DRAW); 
	//设置静态控件窗口风格为位图居中显示
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//显示图片
	CString strPicPath = dlg->GetExePath();
	strPicPath += "res/MapLow.bmp";
	::GetWindowRect(GetDlgItem(IDC_STATIC_DRAW)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
		rect.right - rect.left,                  //宽度 0为默认大小
		rect.bottom - rect.top,                  //高度 像素为单位
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nDrawWidth = rect.right - rect.left;
	m_nDrawHeight = rect.bottom - rect.top;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_pos.x = point.x;
	m_pos.y = point.y;

	OnOK();

	CDialogEx::OnLButtonUp(nFlags, point);
}
