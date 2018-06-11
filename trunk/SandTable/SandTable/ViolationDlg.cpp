// ViolationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "ViolationDlg.h"
#include "afxdialogex.h"
#include "SandTableDB.h"
#include "SandTableDlg.h"

const char * itoaa(const int nNum);

// CViolationDlg 对话框

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


// CViolationDlg 消息处理程序
BOOL CViolationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ViolationType.InsertString(0, "所有");
	m_ViolationType.InsertString(1, "超速");
	m_ViolationType.InsertString(2, "超重");
	m_ViolationType.InsertString(3, "闯红灯");
	m_ViolationType.SetCurSel(0);

	LONG lStyle; 
	lStyle = GetWindowLong(m_listViolation.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_REPORT; // 设置style 
	SetWindowLong(m_listViolation.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_listViolation.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_listViolation.SetExtendedStyle(dwStyle); // 设置扩展风格 

	m_listViolation.InsertColumn( 0, "序号", LVCFMT_LEFT, 40 );// 插入列 
	m_listViolation.InsertColumn( 1, "违法类型", LVCFMT_LEFT, 80 );
	m_listViolation.InsertColumn( 2, "违法图片名称", LVCFMT_LEFT, 200 );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CViolationDlg::InsertData(const char * lID, const char * strType, const char * picName)
{
	int nIndex = m_listViolation.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT ; 
	lvItem.iItem = nIndex;     //行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)lID;   //第一列

	//在最后一行插入记录值.
	m_listViolation.InsertItem(&lvItem); 

	//插入其它列
	m_listViolation.SetItemText(nIndex, 1, strType); 
	m_listViolation.SetItemText(nIndex, 2, picName); 
}

void CViolationDlg::OnNMClickListVolite(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
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

//刷新
void CViolationDlg::OnBnClickedButtonFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;

	m_listViolation.DeleteAllItems();

	CSandTableDB tDB;
	bool bRet = tDB.OpenDB(SANDTABLE_VIOLATION);
	if (bRet == false)
	{
		MessageBox("打开违法DB文件失败!");
		return;
	}

	list<tagViolation> listViolation;

	CString strVioType;
	m_ViolationType.GetWindowTextA(strVioType);

	if (strVioType.Compare("所有") == 0)
	{
		bRet = tDB.ReadDB(listViolation);
	}
	else
	{
		bRet = tDB.ReadDB(strVioType.GetBuffer(), listViolation);
	}
	
	if (bRet == false)
	{
		MessageBox("读取违法信息失败！");
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

//绘图
void CViolationDlg::DrawPic(const char * szPicName)
{
	if (IsFileExist(szPicName) == FALSE)
	{
		MessageBox("图片文件不存在！");
		return;
	}

	int cx, cy;  
	CImage  image;  
	CRect   rect;  

	//根据路径载入图片  
	image.Load(szPicName);  
	//获取图片的宽 高度  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();  

	//获取Picture Control控件的大小  
	GetDlgItem(IDC_STATIC_PICSHOW)->GetWindowRect(&rect);  
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);  
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_STATIC_PICSHOW)->MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_STATIC_PICSHOW);//获取控件句柄  
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  

	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//获取picture的DC  

	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  

	ReleaseDC(pDc); 
}

void CViolationDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strID;
	CString strPic;

	POSITION pos=m_listViolation.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if(pos==NULL)
		MessageBox("没有选中行");
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
				MessageBox("打开违法DB文件失败!");
				return;
			}

			tagViolation tVio;
			tVio.nID = atoi(strID.GetBuffer());

			bRet = tDB.DeleteDB(tVio);
			if (bRet == false)
			{
				MessageBox("删除违法失败!");
			}

			tDB.CloseDB();
		}
	}

	OnBnClickedButtonFresh();
}
