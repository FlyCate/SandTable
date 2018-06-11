// RoadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DBTool.h"
#include "RoadDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CRoadDlg �Ի���

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


// CRoadDlg ��Ϣ�������
BOOL CRoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	m_RoadDirect.InsertString(0, "����");
	m_RoadDirect.InsertString(1, "����");

	m_StartCrossingDirect.InsertString(0, "��");
	m_StartCrossingDirect.InsertString(1, "��");
	m_StartCrossingDirect.InsertString(2, "��");
	m_StartCrossingDirect.InsertString(3, "��");

	m_EndCrossingDirect.InsertString(0, "��");
	m_EndCrossingDirect.InsertString(1, "��");
	m_EndCrossingDirect.InsertString(2, "��");
	m_EndCrossingDirect.InsertString(3, "��");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRoadDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ӳɹ���");
		OnBnClickedButtonSelect();
	}
	else
	{
		MessageBox("���ʧ�ܣ�");
	}
}


void CRoadDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox("ɾ���ɹ���");
			m_listData.DeleteItem(nItem);
		}
		else
		{
			MessageBox("ɾ��ʧ�ܣ�");
		}
	}
	else
	{
		MessageBox("��ѡ��Ҫɾ�����У�");
	}
}


void CRoadDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
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
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tRoadInfo.nID);//��һ��

	//�����һ�в����¼ֵ.
	m_listData.InsertItem(&lvItem);

	//����������
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
	lStyle = GetWindowLong(m_listData.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_REPORT; // ����style 
	SetWindowLong(m_listData.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_listData.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_listData.SetExtendedStyle(dwStyle); // ������չ��� 

	m_listData.InsertColumn( 0, "���", LVCFMT_LEFT, 40 );// ������ 
	m_listData.InsertColumn( 1, "��·����", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 2, "��ʻ����", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 3, "��ʼ����", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 4, "��ʼ·�ڱ��", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 5, "��ʼ·�ڷ���", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 6, "��������", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 7, "����·�ڱ��", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 8, "����·�ڷ���", LVCFMT_LEFT, 85 );
}

//��ʼ�㿨�� ������Ӧ
void CRoadDlg::OnCbnDropdownCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
	}
}

//��ʼ��·�ڱ�� ������Ӧ
void CRoadDlg::OnCbnDropdownCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
	}
}

//��ʼ�㿨�� ������Ӧ
void CRoadDlg::OnCbnDropdownCombo5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
	}
}

//������·�ڱ�� �����Ӧ
void CRoadDlg::OnCbnDropdownCombo6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
	}
}
