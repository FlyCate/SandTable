// RoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBTool.h"
#include "RoadDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CRoadDlg 对话框

IMPLEMENT_DYNAMIC(CRoadDlg, CDialogEx)

CRoadDlg::CRoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRoadDlg::IDD, pParent)
	, m_strRoadName(_T(""))
{
	m_parent = pParent;
}

CRoadDlg::~CRoadDlg()
{
}

void CRoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRoadName);
	DDX_Control(pDX, IDC_COMBO1, m_RoadDirect);
	DDX_Control(pDX, IDC_COMBO2, m_StartCard);
	DDX_Control(pDX, IDC_COMBO3, m_StartCrossingID);
	DDX_Control(pDX, IDC_COMBO4, m_StartCrossingDirect);
	DDX_Control(pDX, IDC_COMBO5, m_EndCard);
	DDX_Control(pDX, IDC_COMBO6, m_EndCrossingID);
	DDX_Control(pDX, IDC_COMBO7, m_EndCrossingDirect);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
}


BEGIN_MESSAGE_MAP(CRoadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRoadDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CRoadDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CRoadDlg::OnBnClickedButtonSelect)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CRoadDlg::OnCbnDropdownCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO3, &CRoadDlg::OnCbnDropdownCombo3)
	ON_CBN_DROPDOWN(IDC_COMBO5, &CRoadDlg::OnCbnDropdownCombo5)
	ON_CBN_DROPDOWN(IDC_COMBO6, &CRoadDlg::OnCbnDropdownCombo6)
END_MESSAGE_MAP()


// CRoadDlg 消息处理程序
BOOL CRoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	m_RoadDirect.InsertString(0, "上行");
	m_RoadDirect.InsertString(1, "下行");

	m_StartCrossingDirect.InsertString(0, "东");
	m_StartCrossingDirect.InsertString(1, "南");
	m_StartCrossingDirect.InsertString(2, "西");
	m_StartCrossingDirect.InsertString(3, "北");

	m_EndCrossingDirect.InsertString(0, "东");
	m_EndCrossingDirect.InsertString(1, "南");
	m_EndCrossingDirect.InsertString(2, "西");
	m_EndCrossingDirect.InsertString(3, "北");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRoadDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;
	CString str;

	tagRoad RoadData;
	RoadData.strRoadName = m_strRoadName.GetBuffer();
	m_RoadDirect.GetWindowTextA(str);
	RoadData.nDirection = GetRoadDirectInt(str.GetBuffer());
	m_StartCard.GetWindowTextA(str);
	RoadData.nStartCard = atoi(str.GetBuffer());
	m_StartCrossingID.GetWindowTextA(str);
	RoadData.nStartCrossingCode = atoi(str.GetBuffer());
	m_StartCrossingDirect.GetWindowTextA(str);
	RoadData.nStartCrossingDirect = GetDirectInt(str.GetBuffer());
	m_EndCard.GetWindowTextA(str);
	RoadData.nEndCard = atoi(str.GetBuffer());
	m_EndCrossingID.GetWindowTextA(str);
	RoadData.nEndCrossingCode = atoi(str.GetBuffer());
	m_EndCrossingDirect.GetWindowTextA(str);
	RoadData.nEndCrossingDirect = GetDirectInt(str.GetBuffer());

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.WriteDB(RoadData);
	if (true == bRet)
	{
		MessageBox("添加成功！");
		OnBnClickedButtonSelect();
	}
	else
	{
		MessageBox("添加失败！");
	}
}


void CRoadDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	POSITION pos = m_listData.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		tagRoad RoadInfo;
		int nItem = m_listData.GetNextSelectedItem(pos);
		CString str = m_listData.GetItemText(nItem, 0);
		RoadInfo.nID = atoi(str.GetBuffer());

		dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
		bool bRet = dlg->m_DBMgr.DeleteDB(RoadInfo);
		if (true == bRet)
		{
			MessageBox("删除成功！");
			m_listData.DeleteItem(nItem);
		}
		else
		{
			MessageBox("删除失败！");
		}
	}
	else
	{
		MessageBox("请选择要删除的行！");
	}
}


void CRoadDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	list<tagRoad> listRoad;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listRoad);
	if (true == bRet)
	{
		InsertData(listRoad);
		int nCount = m_listData.GetItemCount();
		if (nCount > 0)
			m_listData.EnsureVisible(nCount-1, FALSE);
	}
	else
	{
		MessageBox("查询失败！");
	}
}

void CRoadDlg::InsertData(list<tagRoad> listRoad)
{
	m_listData.DeleteAllItems();
	for (auto it = listRoad.begin(); it != listRoad.end(); it++)
	{
		InsertData(*it);
	}
}

void CRoadDlg::InsertData(tagRoad tRoadInfo)
{
	int nIndex = m_listData.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tRoadInfo.nID);//第一列

	//在最后一行插入记录值.
	m_listData.InsertItem(&lvItem);

	//插入其它列
	m_listData.SetItemText(nIndex, 1, tRoadInfo.strRoadName.c_str());
	m_listData.SetItemText(nIndex, 2, GetRoadDirectStr(tRoadInfo.nDirection));
	m_listData.SetItemText(nIndex, 3, itoaa(tRoadInfo.nStartCard));
	m_listData.SetItemText(nIndex, 4, itoaa(tRoadInfo.nStartCrossingCode));
	m_listData.SetItemText(nIndex, 5, GetDirectStr(tRoadInfo.nStartCrossingDirect));
	m_listData.SetItemText(nIndex, 6, itoaa(tRoadInfo.nEndCard));
	m_listData.SetItemText(nIndex, 7, itoaa(tRoadInfo.nEndCrossingCode));
	m_listData.SetItemText(nIndex, 8, GetDirectStr(tRoadInfo.nEndCrossingDirect));
}

void CRoadDlg::InitListControl()
{
	LONG lStyle; 
	lStyle = GetWindowLong(m_listData.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_REPORT; // 设置style 
	SetWindowLong(m_listData.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_listData.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_listData.SetExtendedStyle(dwStyle); // 设置扩展风格 

	m_listData.InsertColumn( 0, "序号", LVCFMT_LEFT, 40 );// 插入列 
	m_listData.InsertColumn( 1, "道路名称", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 2, "行驶方向", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 3, "起始卡号", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 4, "起始路口编号", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 5, "起始路口方向", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 6, "结束卡号", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 7, "结束路口编号", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 8, "结束路口方向", LVCFMT_LEFT, 85 );
}

//起始点卡号 下拉响应
void CRoadDlg::OnCbnDropdownCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	m_StartCard.ResetContent();
	list<tagCard> listCard;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCard);
	if (true == bRet)
	{
		for (auto it = listCard.begin(); it != listCard.end(); it++)
		{
			int nCount = m_StartCard.GetCount();
			m_StartCard.InsertString(nCount, itoaa(it->nCardNum));
		}
	}
	else
	{
		MessageBox("查询失败！");
	}
}

//起始点路口编号 下拉响应
void CRoadDlg::OnCbnDropdownCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	m_StartCrossingID.ResetContent();
	list<tagCrossing> listCrossing;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCrossing);
	if (true == bRet)
	{
		for (auto it = listCrossing.begin(); it != listCrossing.end(); it++)
		{
			CString strCBText;
			int nCount = m_StartCrossingID.GetCount();
			if (nCount > 0)
			{
				m_StartCrossingID.GetLBText(nCount-1, strCBText);
				if (it->nCrossingID != atoi(strCBText.GetBuffer()))
				{
					m_StartCrossingID.InsertString(nCount, itoaa(it->nCrossingID));
				}
			}
			else
			{
				m_StartCrossingID.InsertString(nCount, itoaa(it->nCrossingID));
			}
		}
	}
	else
	{
		MessageBox("查询失败！");
	}
}

//起始点卡号 下拉响应
void CRoadDlg::OnCbnDropdownCombo5()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	m_EndCard.ResetContent();
	list<tagCard> listCard;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCard);
	if (true == bRet)
	{
		for (auto it = listCard.begin(); it != listCard.end(); it++)
		{
			int nCount = m_EndCard.GetCount();
			m_EndCard.InsertString(nCount, itoaa(it->nCardNum));
		}
	}
	else
	{
		MessageBox("查询失败！");
	}
}

//结束点路口编号 点击响应
void CRoadDlg::OnCbnDropdownCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	m_EndCrossingID.ResetContent();
	list<tagCrossing> listCrossing;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCrossing);
	if (true == bRet)
	{
		for (auto it = listCrossing.begin(); it != listCrossing.end(); it++)
		{
			CString strCBText;
			int nCount = m_EndCrossingID.GetCount();
			if (nCount > 0)
			{
				m_EndCrossingID.GetLBText(nCount-1, strCBText);
				if (it->nCrossingID != atoi(strCBText.GetBuffer()))
				{
					m_EndCrossingID.InsertString(nCount, itoaa(it->nCrossingID));
				}
			}
			else
			{
				m_EndCrossingID.InsertString(nCount, itoaa(it->nCrossingID));
			}
		}
	}
	else
	{
		MessageBox("查询失败！");
	}
}
