// CardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBTool.h"
#include "CardDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CCardDlg 对话框

IMPLEMENT_DYNAMIC(CCardDlg, CDialogEx)

CCardDlg::CCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCardDlg::IDD, pParent)
	, m_strCardNum(_T(""))
	, m_strXPos(_T(""))
	, m_strYPos(_T(""))
	, m_strTotalHeigh(_T(""))
	, m_strRealHeight(_T(""))
{
	m_parent = pParent;
}

CCardDlg::~CCardDlg()
{
}

void CCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM, m_strCardNum);
	DDX_Text(pDX, IDC_EDIT_XPOS, m_strXPos);
	DDX_Text(pDX, IDC_EDIT_YPOS, m_strYPos);
	DDX_Text(pDX, IDC_EDIT_TOTAL_HEIGH, m_strTotalHeigh);
	DDX_Text(pDX, IDC_EDIT_REAL_HEIGH, m_strRealHeight);
}


BEGIN_MESSAGE_MAP(CCardDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCardDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCardDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CCardDlg::OnBnClickedButtonSelect)
	ON_EN_CHANGE(IDC_EDIT_REAL_HEIGH, &CCardDlg::OnEnChangeEditRealHeigh)
END_MESSAGE_MAP()


// CCardDlg 消息处理程序
BOOL CCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//增
void CCardDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	tagCard CardData;
	CardData.nCardNum = atoi(m_strCardNum.GetBuffer());
	CardData.nXpos = atoi(m_strXPos.GetBuffer());
	CardData.nYpos = atoi(m_strYPos.GetBuffer());

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.WriteDB(CardData);
	if (true == bRet)
	{
		MessageBox("添加成功！");
		OnBnClickedButtonSelect();
		UpdateData(TRUE);
		int nNumber = atoi(m_strCardNum.GetBuffer());
		nNumber++;
		m_strCardNum = itoaa(nNumber);
		UpdateData(FALSE);
	}
	else
	{
		MessageBox("添加失败！");
	}
}

//删
void CCardDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	POSITION pos = m_listData.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		tagCard CardInfo;
		int nItem = m_listData.GetNextSelectedItem(pos);
		CString str = m_listData.GetItemText(nItem, 0);
		CardInfo.nID = atoi(str.GetBuffer());

		dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
		bool bRet = dlg->m_DBMgr.DeleteDB(CardInfo);
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

//查
void CCardDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBToolDlg * dlg = (CDBToolDlg *)m_parent;

	list<tagCard> listCard;

	dlg->m_DBMgr.OpenDB(SANDTABLE_NAME);
	bool bRet = dlg->m_DBMgr.ReadDB(listCard);
	if (true == bRet)
	{
		InsertData(listCard);
		int nCount = m_listData.GetItemCount();
		if (nCount > 0)
			m_listData.EnsureVisible(nCount-1, FALSE);
	}
	else
	{
		MessageBox("查询失败！");
	}
}

void CCardDlg::InsertData(list<tagCard> listCard)
{
	m_listData.DeleteAllItems();
	for (auto it = listCard.begin(); it != listCard.end(); it++)
	{
		InsertData(*it);
	}
}

void CCardDlg::InsertData(tagCard tCardInfo)
{
	int nIndex = m_listData.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tCardInfo.nID);//第一列

	//在最后一行插入记录值.
	m_listData.InsertItem(&lvItem);

	//插入其它列
	m_listData.SetItemText(nIndex, 1, itoaa(tCardInfo.nCardNum));
	m_listData.SetItemText(nIndex, 2, itoaa(tCardInfo.nXpos));
	m_listData.SetItemText(nIndex, 3, itoaa(tCardInfo.nYpos));
}

void CCardDlg::InitListControl()
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
	m_listData.InsertColumn( 1, "Card(卡号)", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn( 2, "Xpos(X坐标)", LVCFMT_LEFT, 200 );
	m_listData.InsertColumn( 3, "Ypos(Y坐标)", LVCFMT_LEFT, 200 );
}


void CCardDlg::OnEnChangeEditRealHeigh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nTotal = atoi(m_strTotalHeigh.GetBuffer());
	int nReal = atoi(m_strRealHeight.GetBuffer());

	m_strYPos = itoaa(nTotal - nReal);
	UpdateData(FALSE);
}
