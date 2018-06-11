// CrossingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBTool.h"
#include "CrossingDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CCrossingDlg 对话框

IMPLEMENT_DYNAMIC(CCrossingDlg, CDialogEx)

CCrossingDlg::CCrossingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrossingDlg::IDD, pParent)
	, m_strRoadName(_T(""))
	, m_strRoadID(_T(""))
	, m_strStraightID(_T(""))
	, m_DextroversionID(_T(""))
{
	m_parent = pParent;
}

CCrossingDlg::~CCrossingDlg()
{
}

void CCrossingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Text(pDX, IDC_EDIT_ROADNAME, m_strRoadName);
	DDX_Text(pDX, IDC_EDIT_ROADNAME2, m_strRoadID);
	DDX_Control(pDX, IDC_COMBO_ROAD_DIRECT, m_RoadDirect);
	DDX_Text(pDX, IDC_EDIT_STRAIGHT_ID, m_strStraightID);
	DDX_Control(pDX, IDC_COMBO_STRAIGHT_DIRECT, m_StraightDirect);
	DDX_Text(pDX, IDC_EDIT_DEXTROVER_ID, m_DextroversionID);
	DDX_Control(pDX, IDC_COMBO_DEXTROVERSION_DIRECT, m_DextroversionDirect);
}


BEGIN_MESSAGE_MAP(CCrossingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCrossingDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCrossingDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CCrossingDlg::OnBnClickedButtonSelect)
//	ON_CBN_EDITCHANGE(IDC_COMBO_ROAD_DIRECT, &CCrossingDlg::OnCbnEditchangeComboRoadDirect)
	ON_CBN_SELCHANGE(IDC_COMBO_ROAD_DIRECT, &CCrossingDlg::OnCbnSelchangeComboRoadDirect)
	ON_CBN_SELCHANGE(IDC_COMBO_STRAIGHT_DIRECT, &CCrossingDlg::OnCbnSelchangeComboStraightDirect)
	ON_CBN_SELCHANGE(IDC_COMBO_DEXTROVERSION_DIRECT, &CCrossingDlg::OnCbnSelchangeComboDextroversionDirect)
END_MESSAGE_MAP()


// CCrossingDlg 消息处理程序

BOOL CCrossingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RoadDirect.InsertString(0, "东");
	m_RoadDirect.InsertString(1, "南");
	m_RoadDirect.InsertString(2, "西");
	m_RoadDirect.InsertString(3, "北");

	m_StraightDirect.InsertString(0, "东");
	m_StraightDirect.InsertString(1, "南");
	m_StraightDirect.InsertString(2, "西");
	m_StraightDirect.InsertString(3, "北");

	m_DextroversionDirect.InsertString(0, "东");
	m_DextroversionDirect.InsertString(1, "南");
	m_DextroversionDirect.InsertString(2, "西");
	m_DextroversionDirect.InsertString(3, "北");

	InitListControl();
	OnBnClickedButtonSelect();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCrossingDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;
	CString strCBText;

	tagCrossing CrossingData;
	CrossingData.strCrossingName = m_strRoadName.GetBuffer();
	CrossingData.nCrossingID = atoi(m_strRoadID.GetBuffer());
	m_RoadDirect.GetLBText(m_RoadDirect.GetCurSel(), strCBText);
	CrossingData.nDirection = GetDirectInt(strCBText.GetBuffer());
	CrossingData.nStraightLine = atoi(m_strStraightID.GetBuffer());
	m_StraightDirect.GetLBText(m_StraightDirect.GetCurSel(), strCBText);
	CrossingData.nStraightDirect = GetDirectInt(strCBText.GetBuffer());
	CrossingData.nDextroversionLine = atoi(m_DextroversionID.GetBuffer());
	m_DextroversionDirect.GetLBText(m_DextroversionDirect.GetCurSel(), strCBText);
	CrossingData.nDextroversionDirect = GetDirectInt(strCBText.GetBuffer());

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.WriteDB(CrossingData);
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


void CCrossingDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	POSITION pos = m_listData.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		tagCrossing CrossingInfo;
		int nItem = m_listData.GetNextSelectedItem(pos);
		CString str = m_listData.GetItemText(nItem, 0);
		CrossingInfo.nID = atoi(str.GetBuffer());

		dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
		bool bRet = dlg->m_DBMgr.DeleteDB(CrossingInfo);
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


void CCrossingDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	list<tagCrossing> listCrossing;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCrossing);
	if (true == bRet)
	{
		InsertData(listCrossing);
		int nCount = m_listData.GetItemCount();
		if (nCount > 0)
			m_listData.EnsureVisible(nCount-1, FALSE);
	}
	else
	{
		MessageBox("查询失败！");
	}
}

void CCrossingDlg::InsertData(list<tagCrossing> listCrossing)
{
	m_listData.DeleteAllItems();
	for (auto it = listCrossing.begin(); it != listCrossing.end(); it++)
	{
		InsertData(*it);
	}
}

void CCrossingDlg::InsertData(tagCrossing tCrossingInfo)
{
	int nIndex = m_listData.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tCrossingInfo.nID);//第一列

	//在最后一行插入记录值.
	m_listData.InsertItem(&lvItem);

	//插入其它列
	m_listData.SetItemText(nIndex, 1, tCrossingInfo.strCrossingName.c_str());
	m_listData.SetItemText(nIndex, 2, itoaa(tCrossingInfo.nCrossingID));
	m_listData.SetItemText(nIndex, 3, GetDirectStr(tCrossingInfo.nDirection));
	m_listData.SetItemText(nIndex, 4, itoaa(tCrossingInfo.nStraightLine));
	m_listData.SetItemText(nIndex, 5, GetDirectStr(tCrossingInfo.nStraightDirect));
	m_listData.SetItemText(nIndex, 6, itoaa(tCrossingInfo.nDextroversionLine));
	m_listData.SetItemText(nIndex, 7, GetDirectStr(tCrossingInfo.nDextroversionDirect));
}

void CCrossingDlg::InitListControl()
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
	m_listData.InsertColumn( 1, "路口名称", LVCFMT_LEFT, 100 );
	m_listData.InsertColumn( 2, "路口编号", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 3, "路口方向", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 4, "直行编号", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 5, "直行对应方向", LVCFMT_LEFT, 90 );
	m_listData.InsertColumn( 6, "右转编号", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 7, "右转对应方向", LVCFMT_LEFT, 90 );
}


void CCrossingDlg::OnCbnSelchangeComboRoadDirect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_RoadDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_StraightDirect.SetCurSel(GetStraightDirect(nDirect));
	m_DextroversionDirect.SetCurSel(GetDextroversionDirect(nDirect));
}


void CCrossingDlg::OnCbnSelchangeComboStraightDirect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_StraightDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_RoadDirect.SetCurSel(GetStraightDirect(nDirect));
	m_DextroversionDirect.SetCurSel(GetLeftDirect(nDirect));
}


void CCrossingDlg::OnCbnSelchangeComboDextroversionDirect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_DextroversionDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_StraightDirect.SetCurSel(GetDextroversionDirect(nDirect));
	m_RoadDirect.SetCurSel(GetLeftDirect(nDirect));
}
