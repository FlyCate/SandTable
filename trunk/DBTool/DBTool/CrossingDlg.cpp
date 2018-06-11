// CrossingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DBTool.h"
#include "CrossingDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CCrossingDlg �Ի���

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


// CCrossingDlg ��Ϣ�������

BOOL CCrossingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RoadDirect.InsertString(0, "��");
	m_RoadDirect.InsertString(1, "��");
	m_RoadDirect.InsertString(2, "��");
	m_RoadDirect.InsertString(3, "��");

	m_StraightDirect.InsertString(0, "��");
	m_StraightDirect.InsertString(1, "��");
	m_StraightDirect.InsertString(2, "��");
	m_StraightDirect.InsertString(3, "��");

	m_DextroversionDirect.InsertString(0, "��");
	m_DextroversionDirect.InsertString(1, "��");
	m_DextroversionDirect.InsertString(2, "��");
	m_DextroversionDirect.InsertString(3, "��");

	InitListControl();
	OnBnClickedButtonSelect();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCrossingDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ӳɹ���");
		OnBnClickedButtonSelect();
	}
	else
	{
		MessageBox("���ʧ�ܣ�");
	}
}


void CCrossingDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


void CCrossingDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
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
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tCrossingInfo.nID);//��һ��

	//�����һ�в����¼ֵ.
	m_listData.InsertItem(&lvItem);

	//����������
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
	m_listData.InsertColumn( 1, "·������", LVCFMT_LEFT, 100 );
	m_listData.InsertColumn( 2, "·�ڱ��", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 3, "·�ڷ���", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 4, "ֱ�б��", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 5, "ֱ�ж�Ӧ����", LVCFMT_LEFT, 90 );
	m_listData.InsertColumn( 6, "��ת���", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn( 7, "��ת��Ӧ����", LVCFMT_LEFT, 90 );
}


void CCrossingDlg::OnCbnSelchangeComboRoadDirect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_RoadDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_StraightDirect.SetCurSel(GetStraightDirect(nDirect));
	m_DextroversionDirect.SetCurSel(GetDextroversionDirect(nDirect));
}


void CCrossingDlg::OnCbnSelchangeComboStraightDirect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_StraightDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_RoadDirect.SetCurSel(GetStraightDirect(nDirect));
	m_DextroversionDirect.SetCurSel(GetLeftDirect(nDirect));
}


void CCrossingDlg::OnCbnSelchangeComboDextroversionDirect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_DextroversionDirect.GetWindowTextA(str);
	int nDirect = GetDirectInt(str.GetBuffer());

	m_StraightDirect.SetCurSel(GetDextroversionDirect(nDirect));
	m_RoadDirect.SetCurSel(GetLeftDirect(nDirect));
}
