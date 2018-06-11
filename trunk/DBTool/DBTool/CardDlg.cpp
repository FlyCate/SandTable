// CardDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DBTool.h"
#include "CardDlg.h"
#include "afxdialogex.h"
#include "DBToolDlg.h"

// CCardDlg �Ի���

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


// CCardDlg ��Ϣ�������
BOOL CCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListControl();
	OnBnClickedButtonSelect();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//��
void CCardDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ӳɹ���");
		OnBnClickedButtonSelect();
		UpdateData(TRUE);
		int nNumber = atoi(m_strCardNum.GetBuffer());
		nNumber++;
		m_strCardNum = itoaa(nNumber);
		UpdateData(FALSE);
	}
	else
	{
		MessageBox("���ʧ�ܣ�");
	}
}

//ɾ
void CCardDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//��
void CCardDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѯʧ�ܣ�");
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
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)itoaa(tCardInfo.nID);//��һ��

	//�����һ�в����¼ֵ.
	m_listData.InsertItem(&lvItem);

	//����������
	m_listData.SetItemText(nIndex, 1, itoaa(tCardInfo.nCardNum));
	m_listData.SetItemText(nIndex, 2, itoaa(tCardInfo.nXpos));
	m_listData.SetItemText(nIndex, 3, itoaa(tCardInfo.nYpos));
}

void CCardDlg::InitListControl()
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
	m_listData.InsertColumn( 1, "Card(����)", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn( 2, "Xpos(X����)", LVCFMT_LEFT, 200 );
	m_listData.InsertColumn( 3, "Ypos(Y����)", LVCFMT_LEFT, 200 );
}


void CCardDlg::OnEnChangeEditRealHeigh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nTotal = atoi(m_strTotalHeigh.GetBuffer());
	int nReal = atoi(m_strRealHeight.GetBuffer());

	m_strYPos = itoaa(nTotal - nReal);
	UpdateData(FALSE);
}
