
// DBToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DBTool.h"
#include "DBToolDlg.h"
#include "afxdialogex.h"

const char * SANDTABLE_NAME = "SandTable.db";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char * CONST_DIRECT[] = {"��", "��", "��", "��"};
const char * ROAD_DIRECT[] = {"����", "����"};

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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDBToolDlg �Ի���




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


// CDBToolDlg ��Ϣ�������

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	LONG lStyle; 
	lStyle = GetWindowLong(m_listTables.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_LIST; // ����style 
	SetWindowLong(m_listTables.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_listTables.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_listTables.SetExtendedStyle(dwStyle); // ������չ��� 

	m_listTables.InsertColumn( 0, "���ݱ�", LVCFMT_LEFT, 80 );
	InsertData("����");
	InsertData("��·��Ϣ");
	InsertData("·����Ϣ");
	//InsertData("վ����Ϣ");
	InsertData("��·���Ŷ�Ӧ��");

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDBToolDlg::InsertData(const char * lID)
{
	int nIndex = m_listTables.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)lID;//��һ��

	//�����һ�в����¼ֵ.
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDBToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDBToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���ѡ���� ĳһ�����ݱ�
void CDBToolDlg::OnNMClickListTables(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		RECT rect;
		::GetWindowRect(GetDlgItem(IDC_STATIC_DLG)->GetSafeHwnd(), &rect);
		//ͨ���к� �л���ʾ��Ӧ�ĶԻ���
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
		case 3://��·���Ŷ�Ӧ��
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

	// TODO: �ڴ˴������Ϣ����������
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