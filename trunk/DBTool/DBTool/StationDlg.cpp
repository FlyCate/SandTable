// StationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DBTool.h"
#include "StationDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CStationDlg �Ի���

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


// CStationDlg ��Ϣ�������
BOOL CStationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	m_RoadDirect.InsertString(0, "����");
	m_RoadDirect.InsertString(1, "����");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CStationDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ӳɹ���");
		OnBnClickedButtonSelect();
	}
	else
	{
		MessageBox("���ʧ�ܣ�");
	}
}


void CStationDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


void CStationDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
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
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tStationInfo.nID);//��һ��

	//�����һ�в����¼ֵ.
	m_listData.InsertItem(&lvItem);

	//����������
	m_listData.SetItemText(nIndex, 1, tStationInfo.strStationName.c_str());
	m_listData.SetItemText(nIndex, 2, itoaa(tStationInfo.nRoadID));
	m_listData.SetItemText(nIndex, 3, GetRoadDirectStr(tStationInfo.nRoadDirect));
	m_listData.SetItemText(nIndex, 4, itoaa(tStationInfo.nStationCard));
	m_listData.SetItemText(nIndex, 5, itoaa(tStationInfo.nStationGroup));
}

void CStationDlg::InitListControl()
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
	m_listData.InsertColumn( 1, "վ������", LVCFMT_LEFT, 150 );
	m_listData.InsertColumn( 2, "��Ӧ��·Id", LVCFMT_LEFT, 80 );
	m_listData.InsertColumn( 3, "��Ӧ��·��ʻ����", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn( 4, "վ�㿨��", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn( 5, "վ�����", LVCFMT_LEFT, 85 );
}

//��·ID ����¼�
void CStationDlg::OnCbnDropdownComboRoadId()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
	}
}
