
// DBToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DBTool.h"
#include "DBToolDlg.h"
#include "afxdialogex.h"

const char * SANDTABLE_NAME = "SandTable.db";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char * CONST_DIRECT[] = {"东", "南", "西", "北"};
const char * ROAD_DIRECT[] = {"上行", "下行"};

const char * GetDirectStr(const int nDirect)
{
	return CONST_DIRECT[nDirect];
}

const int GetDirectInt(const char * strDirect)
{
	for (int i = 0; i < 4; i++)
	{
		if (0 == strcmp(CONST_DIRECT[i], strDirect))
		{
			return i;
		}
	}

	return -1;
}

const int GetStraightDirect(int nDirect)
{
	nDirect += 2;
	if (nDirect > 3)
	{
		nDirect -= 4;
	}

	return nDirect;
}

const int GetDextroversionDirect(int nDirect)
{
	nDirect += 3;
	if (nDirect > 3)
	{
		nDirect -= 4;
	}

	return nDirect;
}

const int GetLeftDirect(int nDirect)
{
	nDirect += 1;
	if (nDirect > 3)
	{
		nDirect -= 4;
	}

	return nDirect;
}

const char * GetRoadDirectStr(const int nDirect)
{
	return ROAD_DIRECT[nDirect];
}

const int GetRoadDirectInt(const char * strDirecit)
{
	for (int i = 0; i < 2; i++)
	{
		if (0 == strcmp(ROAD_DIRECT[i], strDirecit))
		{
			return i;
		}
	}

	return -1;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDBToolDlg 对话框




CDBToolDlg::CDBToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_CardDlg = NULL;
	m_CrossingDlg = NULL;
	m_RoadDlg = NULL;
	m_StationDlg = NULL;
	m_RoadToCardDlg = NULL;
}

void CDBToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TABLES, m_listTables);
}

BEGIN_MESSAGE_MAP(CDBToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST_TABLES, &CDBToolDlg::OnNMClickListTables)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CDBToolDlg 消息处理程序

const char * itoaa(int nNum)
{
	static char szTemp[64] = {0};
	memset(szTemp, 0, 64);
	sprintf(szTemp, "%d", nNum);
	return szTemp;
}

BOOL CDBToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	LONG lStyle; 
	lStyle = GetWindowLong(m_listTables.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_LIST; // 设置style 
	SetWindowLong(m_listTables.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_listTables.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_listTables.SetExtendedStyle(dwStyle); // 设置扩展风格 

	m_listTables.InsertColumn( 0, "数据表", LVCFMT_LEFT, 80 );
	InsertData("卡号");
	InsertData("道路信息");
	InsertData("路口信息");
	//InsertData("站点信息");
	InsertData("道路卡号对应表");

	if (m_CardDlg == NULL)
	{
		m_CardDlg = new CCardDlg(this);
		m_CardDlg->Create(IDD_DIALOG_CARD, this);
		m_CardDlg->ShowWindow(FALSE);
	}

	if (m_CrossingDlg == NULL)
	{
		m_CrossingDlg = new CCrossingDlg(this);
		m_CrossingDlg->Create(IDD_DIALOG_CROSSING, this);
		m_CrossingDlg->ShowWindow(FALSE);
	}

	if (m_RoadDlg == NULL)
	{
		m_RoadDlg = new CRoadDlg(this);
		m_RoadDlg->Create(IDD_DIALOG_ROADINFO, this);
		m_RoadDlg->ShowWindow(FALSE);
	}

// 	if (m_StationDlg == NULL)
// 	{
// 		m_StationDlg = new CStationDlg(this);
// 		m_StationDlg->Create(IDD_DIALOG_STATION, this);
// 		m_StationDlg->ShowWindow(FALSE);
// 	}

	if (m_RoadToCardDlg == NULL)
	{
		m_RoadToCardDlg = new CRoadToCard(this);
		m_RoadToCardDlg->Create(IDD_DIALOG_GROUP, this);
		m_RoadToCardDlg->ShowWindow(FALSE);
	}

	MoveWindow(100, 100, 765, 494, FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDBToolDlg::InsertData(const char * lID)
{
	int nIndex = m_listTables.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)lID;//第一列

	//在最后一行插入记录值.
	m_listTables.InsertItem(&lvItem);
}

void CDBToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == WS_THICKFRAME)
	{
		return;
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDBToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDBToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击选择了 某一个数据表
void CDBToolDlg::OnNMClickListTables(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		RECT rect;
		::GetWindowRect(GetDlgItem(IDC_STATIC_DLG)->GetSafeHwnd(), &rect);
		//通过行号 切换显示对应的对话框
		switch(pNMListView->iItem)
		{
		case 0://card
			{
				m_CardDlg->MoveWindow(&rect);
				m_CardDlg->ShowWindow(TRUE);
				m_CrossingDlg->ShowWindow(FALSE);
				m_RoadDlg->ShowWindow(FALSE);
				//m_StationDlg->ShowWindow(FALSE);
			}
			break;
		case 1://road
			{
				m_RoadDlg->MoveWindow(&rect);
				m_RoadDlg->ShowWindow(TRUE);
				m_CrossingDlg->ShowWindow(FALSE);
				m_CardDlg->ShowWindow(FALSE);
				//m_StationDlg->ShowWindow(FALSE);
				m_RoadToCardDlg->ShowWindow(FALSE);
			}
			break;
		case 2://crossing
			{
				m_CrossingDlg->MoveWindow(&rect);
				m_CrossingDlg->ShowWindow(TRUE);
				m_CardDlg->ShowWindow(FALSE);
				m_RoadDlg->ShowWindow(FALSE);
				//m_StationDlg->ShowWindow(FALSE);
				m_RoadToCardDlg->ShowWindow(FALSE);
			}
			break;
// 		case 3://station
// 			{
// 				//m_StationDlg->MoveWindow(&rect);
// 				//m_StationDlg->ShowWindow(TRUE);
// 				m_CrossingDlg->ShowWindow(FALSE);
// 				m_CardDlg->ShowWindow(FALSE);
// 				m_RoadDlg->ShowWindow(FALSE);
// 				m_RoadToCardDlg->ShowWindow(FALSE);
// 			}
// 			break;
		case 3://道路卡号对应表
			{
				m_RoadToCardDlg->MoveWindow(&rect);
				m_RoadToCardDlg->ShowWindow(TRUE);
				//m_StationDlg->ShowWindow(FALSE);
				m_CrossingDlg->ShowWindow(FALSE);
				m_CardDlg->ShowWindow(FALSE);
				m_RoadDlg->ShowWindow(FALSE);
			}
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}


void CDBToolDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	RECT rect;
	::GetWindowRect(GetDlgItem(IDC_STATIC_DLG)->GetSafeHwnd(), &rect);
	if (m_CardDlg != NULL)
	{
		m_CardDlg->MoveWindow(&rect);
	}

	if (m_CrossingDlg != NULL)
	{
		m_CrossingDlg->MoveWindow(&rect);
	}

	if (m_RoadDlg != NULL)
	{
		m_RoadDlg->MoveWindow(&rect);
	}

	if (m_StationDlg != NULL)
	{
		m_StationDlg->MoveWindow(&rect);
	}

	if (m_RoadToCardDlg != NULL)
	{
		m_RoadToCardDlg->MoveWindow(&rect);
	}
}