// ViolationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "ViolationDlg.h"
#include "afxdialogex.h"
#include "SandTableDB.h"
#include "SandTableDlg.h"

const char * itoaa(const int nNum);

// CViolationDlg �Ի���

IMPLEMENT_DYNAMIC(CViolationDlg, CDialogEx)

CViolationDlg::CViolationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViolationDlg::IDD, pParent)
{
	m_parent = (void *) pParent;
}

CViolationDlg::~CViolationDlg()
{
}

void CViolationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ViolationType);
	DDX_Control(pDX, IDC_LIST_VOLITE, m_listViolation);
}


BEGIN_MESSAGE_MAP(CViolationDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VOLITE, &CViolationDlg::OnNMClickListVolite)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, &CViolationDlg::OnBnClickedButtonFresh)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CViolationDlg::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CViolationDlg ��Ϣ�������
BOOL CViolationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ViolationType.InsertString(0, "����");
	m_ViolationType.InsertString(1, "����");
	m_ViolationType.InsertString(2, "����");
	m_ViolationType.InsertString(3, "�����");
	m_ViolationType.SetCurSel(0);

	LONG lStyle; 
	lStyle = GetWindowLong(m_listViolation.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_REPORT; // ����style 
	SetWindowLong(m_listViolation.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_listViolation.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_listViolation.SetExtendedStyle(dwStyle); // ������չ��� 

	m_listViolation.InsertColumn( 0, "���", LVCFMT_LEFT, 40 );// ������ 
	m_listViolation.InsertColumn( 1, "Υ������", LVCFMT_LEFT, 80 );
	m_listViolation.InsertColumn( 2, "Υ��ͼƬ����", LVCFMT_LEFT, 200 );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CViolationDlg::InsertData(const char * lID, const char * strType, const char * picName)
{
	int nIndex = m_listViolation.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT ; 
	lvItem.iItem = nIndex;     //����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)lID;   //��һ��

	//�����һ�в����¼ֵ.
	m_listViolation.InsertItem(&lvItem); 

	//����������
	m_listViolation.SetItemText(nIndex, 1, strType); 
	m_listViolation.SetItemText(nIndex, 2, picName); 
}

void CViolationDlg::OnNMClickListVolite(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR; 
	if(pNMListView->iItem != -1) 
	{
		CString strPic;
		strPic = m_listViolation.GetItemText(pNMListView->iItem, 2);
		DrawPic(strPic.GetBuffer());
		//AfxMessageBox(strPic); 
	}

	*pResult = 0;
}

//ˢ��
void CViolationDlg::OnBnClickedButtonFresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;

	m_listViolation.DeleteAllItems();

	CSandTableDB tDB;
	bool bRet = tDB.OpenDB(SANDTABLE_VIOLATION);
	if (bRet == false)
	{
		MessageBox("��Υ��DB�ļ�ʧ��!");
		return;
	}

	list<tagViolation> listViolation;

	CString strVioType;
	m_ViolationType.GetWindowTextA(strVioType);

	if (strVioType.Compare("����") == 0)
	{
		bRet = tDB.ReadDB(listViolation);
	}
	else
	{
		bRet = tDB.ReadDB(strVioType.GetBuffer(), listViolation);
	}
	
	if (bRet == false)
	{
		MessageBox("��ȡΥ����Ϣʧ�ܣ�");
	}
	else
	{
		for (auto it = listViolation.begin(); it != listViolation.end(); it++)
		{
			InsertData(itoaa(it->nID), it->strVioType.c_str(), it->strURL.c_str());
		}
	}

	tDB.CloseDB();
}

BOOL IsFileExist(const char * csFile)
{
	DWORD dwAttrib = GetFileAttributes(csFile);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

//��ͼ
void CViolationDlg::DrawPic(const char * szPicName)
{
	if (IsFileExist(szPicName) == FALSE)
	{
		MessageBox("ͼƬ�ļ������ڣ�");
		return;
	}

	int cx, cy;  
	CImage  image;  
	CRect   rect;  

	//����·������ͼƬ  
	image.Load(szPicName);  
	//��ȡͼƬ�Ŀ� �߶�  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();  

	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(IDC_STATIC_PICSHOW)->GetWindowRect(&rect);  
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);  
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(IDC_STATIC_PICSHOW)->MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_STATIC_PICSHOW);//��ȡ�ؼ����  
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С  

	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//��ȡpicture��DC  

	image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������  

	ReleaseDC(pDc); 
}

void CViolationDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strID;
	CString strPic;

	POSITION pos=m_listViolation.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if(pos==NULL)
		MessageBox("û��ѡ����");
	else
	{
		while (pos)
		{
			int nIdx=-1;
			nIdx= m_listViolation.GetNextSelectedItem(pos);

			if(nIdx >=0 && nIdx<m_listViolation.GetItemCount())
			{
				strID = m_listViolation.GetItemText(nIdx, 0);
				strPic = m_listViolation.GetItemText(nIdx, 2);
			}

			DeleteFile(strPic);
			CSandTableDlg * dlg = (CSandTableDlg *) m_parent;

			CSandTableDB tDB;
			bool bRet = tDB.OpenDB(SANDTABLE_VIOLATION);
			if (bRet == false)
			{
				MessageBox("��Υ��DB�ļ�ʧ��!");
				return;
			}

			tagViolation tVio;
			tVio.nID = atoi(strID.GetBuffer());

			bRet = tDB.DeleteDB(tVio);
			if (bRet == false)
			{
				MessageBox("ɾ��Υ��ʧ��!");
			}

			tDB.CloseDB();
		}
	}

	OnBnClickedButtonFresh();
}
