// StationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBTool.h"
#include "StationDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CStationDlg 对话框

IMPLEMENT_DYNAMIC(CStationDlg, CDialogEx)

CStationDlg::CStationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStationDlg::IDD, pParent)
	, m_StationName(_T(""))
	, m_StationCard(_T(""))
	, m_StationGroup(_T(""))
{
	m_parent = pParent;
}

CStationDlg::~CStationDlg()
{
}

void CStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Text(pDX, IDC_EDIT_STATION_NAME, m_StationName);
	DDX_Control(pDX, IDC_COMBO_ROAD_ID, m_RoadID);
	DDX_Control(pDX, IDC_COMBO_ROAD_DIRECT, m_RoadDirect);
	DDX_Text(pDX, IDC_EDIT_STATION_ID, m_StationCard);
	DDX_Text(pDX, IDC_EDIT_STATION_GROUP, m_StationGroup);
}


BEGIN_MESSAGE_MAP(CStationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CStationDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CStationDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CStationDlg::OnBnClickedButtonSelect)
	ON_CBN_DROPDOWN(IDC_COMBO_ROAD_ID, &CStationDlg::OnCbnDropdownComboRoadId)
END_MESSAGE_MAP()


// CStationDlg 消息处理程序
BOOL CStationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	m_RoadDirect.InsertString(0, "上行");
	m_RoadDirect.InsertString(1, "下行");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CStationDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;
	CString str;

	tagStation StationData;
	StationData.strStationName = m_StationName.GetBuffer();
	m_RoadID.GetWindowTextA(str);
	StationData.nRoadID = atoi(str.GetBuffer());
	m_RoadDirect.GetWindowTextA(str);
	StationData.nRoadDirect = GetRoadDirectInt(str);
	StationData.nStationCard = atoi(m_StationCard.GetBuffer());
	StationData.nStationGroup = atoi(m_StationGroup.GetBuffer());

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.WriteDB(StationData);
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


void CStationDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	POSITION pos = m_listData.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		tagStation StationInfo;
		int nItem = m_listData.GetNextSelectedItem(pos);
		CString str = m_listData.GetItemText(nItem, 0);
		StationInfo.nID = atoi(str.GetBuffer());

		dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
		bool bRet = dlg->m_DBMgr.DeleteDB(StationInfo);
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


void CStationDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	list<tagStation> listStation;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listStation);
	if (true == bRet)
	{
		InsertData(listStation);
		int nCount = m_listData.GetItemCount();
		if (nCount > 0)
			m_listData.EnsureVisible(nCount-1, FALSE);
	}
	else
	{
		MessageBox("查询失败！");
	}
}

void CStationDlg::InsertData(list<tagStation> listStation)
{
	m_listData.DeleteAllItems();
	for (auto it = listStation.begin(); it != listStation.end(); it++)
	{
		InsertData(*it);
	}
}

void CStationDlg::InsertData(tagStation tStationInfo)
{
	int nIndex = m_listData.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tStationInfo.nID);//第一列

	//在最后一行插入记录值.
	m_listData.InsertItem(&lvItem);

	//插入其它列
	m_listData.SetItemText(nIndex, 1, tStationInfo.strStationName.c_str());
	m_listData.SetItemText(nIndex, 2, itoaa(tStationInfo.nRoadID));
	m_listData.SetItemText(nIndex, 3, GetRoadDirectStr(tStationInfo.nRoadDirect));
	m_listData.SetItemText(nIndex, 4, itoaa(tStationInfo.nStationCard));
	m_listData.SetItemText(nIndex, 5, itoaa(tStationInfo.nStationGroup));
}

void CStationDlg::InitListControl()
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
	m_listData.InsertColumn( 1, "站点名称", LVCFMT_LEFT, 150 );
	m_listData.InsertColumn( 2, "对应道路Id", LVCFMT_LEFT, 80 );
	m_listData.InsertColumn( 3, "对应道路行驶方向", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn( 4, "站点卡号", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 5, "站点分组", LVCFMT_LEFT, 85 );
}

//道路ID 点击事件
void CStationDlg::OnCbnDropdownComboRoadId()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	m_RoadID.ResetContent();
	list<tagRoad> listRoad;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listRoad);
	if (true == bRet)
	{
		for (auto it = listRoad.begin(); it != listRoad.end(); it++)
		{
			int nCount = m_RoadID.GetCount();
			m_RoadID.InsertString(nCount, itoaa(it->nID));
		}
	}
	else
	{
		MessageBox("查询失败！");
	}
}
