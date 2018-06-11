// PathPlanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "PathPlanDlg.h"
#include "afxdialogex.h"
#include "SandTableDlg.h"
#include "MapDlg.h"
#include "ChooseRoad.h"
#include <math.h>

// CPathPlanDlg 对话框

#define SERIAL_OPEN()\
	if (dlg->m_Serial_dlg->m_com_isopen == false)\
{\
	MessageBox("请打开串口");\
	return;\
}

const char * itoaa(const int nNum);

IMPLEMENT_DYNAMIC(CPathPlanDlg, CDialogEx)

CPathPlanDlg::CPathPlanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPathPlanDlg::IDD, pParent)
	, m_strStart(_T(""))
	, m_strEnd(_T(""))
{
	m_parent = (void *)pParent;
}

CPathPlanDlg::~CPathPlanDlg()
{
}

void CPathPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATH, m_PathList);
	DDX_Text(pDX, IDC_EDIT_START, m_strStart);
	DDX_Text(pDX, IDC_EDIT_END, m_strEnd);
}


BEGIN_MESSAGE_MAP(CPathPlanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CPathPlanDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_START, &CPathPlanDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_END, &CPathPlanDlg::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CPathPlanDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CPathPlanDlg 消息处理程序

BOOL CPathPlanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG lStyle; 
	lStyle = GetWindowLong(m_PathList.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_LIST; // 设置style 
	SetWindowLong(m_PathList.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_PathList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_PathList.SetExtendedStyle(dwStyle); // 设置扩展风格 
	//m_PathList.InsertColumn( 0, "序号", LVCFMT_LEFT, 40 );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//路径搜索
void CPathPlanDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PathList.DeleteAllItems();

	bool bRet = m_roadPlan.StartCodeCardArriveEndCard(m_nStartCardID, m_nEndCardID);
	if (bRet == false)
	{
		MessageBox("起始点不能到达终点！请重新选择！");
		return;
	}

	long lRet = m_roadPlan.RoadPlan(m_nStartCardID, m_nEndCardID);
	if (lRet != 0)
	{
		CString str;
		str.Format("路径规划失败！ 错误信息：%s", m_roadPlan.GetErrStr());
		MessageBox(str);
		return;
	}

	auto & lstPlan = m_roadPlan.GetPlan();
	for (auto it = lstPlan.begin(); it != lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_CROSSING)
		{
			TRACE("%s\n", it->tPlanCrossing.strDescription.c_str());
			InsertPathList(it->tPlanCrossing.strDescription.c_str());
		}

		if (it->type == PLAN_TYPE_ROAD)
		{
			TRACE("%s\n", it->tPlanRoad.strDescription.c_str());
			InsertPathList(it->tPlanRoad.strDescription.c_str());
		}
	}
}

const char * CPathPlanDlg::GetExePath()
{
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	return dlg->m_strExePath.GetBuffer();
}


void CPathPlanDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CMapDlg * map = new CMapDlg(this);

	map->DoModal();
	m_StartPos = map->GetPos();

	delete map;
	map = NULL;

	list<tagCard> lstCard;
	m_roadPlan.GetCardFromPos(m_StartPos, lstCard);

	list<tagRoad> lstRoad;
	for (auto it = lstCard.begin(); it != lstCard.end(); it++)
	{
		tagRoad tRoad;
		long lRet = m_roadPlan.GetRoadInfoFromCardID(it->nCardNum, tRoad);
		if (lRet == 0)
		{
			lstRoad.push_back(tRoad);
		}
	}

	CChooseRoad * pChooseDlg = new CChooseRoad(this);
	pChooseDlg->SetRoadList(lstRoad);
	pChooseDlg->DoModal();

	for (auto it = lstRoad.begin(); it != lstRoad.end(); it++)
	{
		if (it->nID == pChooseDlg->m_nRoadID)
		{
			for (auto itCard = lstCard.begin(); itCard != lstCard.end(); itCard++)
			{
				bool bRet = m_roadPlan.EndPointIsInThisRoad(itCard->nCardNum, it->nID);
				if (bRet == true)
				{
					m_strStart.Format("%s:%s(%d)", it->strRoadName.c_str(), it->nDirection == 0? "上行": "下行", itCard->nID);
					m_nStartCardID = itCard->nID;
				}
			}
			break;
		}
	}

	delete pChooseDlg;
	pChooseDlg = NULL;

	UpdateData(FALSE);
}


void CPathPlanDlg::OnBnClickedButtonEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	CMapDlg * map = new CMapDlg(this);

	map->DoModal();
	m_EndPos = map->GetPos();

	delete map;
	map = NULL;

	list<tagCard> lstCard;
	m_roadPlan.GetCardFromPos(m_EndPos, lstCard);

	list<tagRoad> lstRoad;
	for (auto it = lstCard.begin(); it != lstCard.end(); it++)
	{
		tagRoad tRoad;
		long lRet = m_roadPlan.GetRoadInfoFromCardID(it->nCardNum, tRoad);
		if (lRet == 0)
		{
			lstRoad.push_back(tRoad);
		}
	}

	CChooseRoad * pChooseDlg = new CChooseRoad(this);
	pChooseDlg->SetRoadList(lstRoad);
	pChooseDlg->DoModal();

	for (auto it = lstRoad.begin(); it != lstRoad.end(); it++)
	{
		if (it->nID == pChooseDlg->m_nRoadID)
		{
			for (auto itCard = lstCard.begin(); itCard != lstCard.end(); itCard++)
			{
				bool bRet = m_roadPlan.EndPointIsInThisRoad(itCard->nCardNum, it->nID);
				if (bRet == true)
				{
					m_strEnd.Format("%s:%s(%d)", it->strRoadName.c_str(), it->nDirection == 0? "上行": "下行", itCard->nID);
					m_nEndCardID = itCard->nID;
				}
			}
			break;
		}
	}

	delete pChooseDlg;
	pChooseDlg = NULL;

	UpdateData(FALSE);
}

void CPathPlanDlg::InsertPathList(const char * str)
{
	int nIndex = m_PathList.GetItemCount();

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nIndex;//行数
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)str;//第一列

	//在最后一行插入记录值.
	m_PathList.InsertItem(&lvItem);
}


void CPathPlanDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CPathPlanDlg::SendCmd()
{
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;

	SERIAL_OPEN();

	int nNum = 1;
	auto & lstPlan = m_roadPlan.GetPlan();
	if (lstPlan.size() == 0)
	{
		MessageBox("请路径规划后，选择确定！");
		return;
	}

	char * pCmdBuff = new char[lstPlan.size() * 3];
	if (pCmdBuff == NULL)
	{
		MessageBox("系统错误！请稍后再试！");
		OnOK();
		return;
	}
	pCmdBuff[nNum-1] = m_nStartCardID;

	for (auto it = lstPlan.begin(); it != lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_CROSSING)
		{
			nNum++;
			pCmdBuff[nNum-1] = it->tPlanCrossing.nCrossingID;
			nNum++;
			pCmdBuff[nNum-1] = it->tPlanCrossing.nDirectLine;
		}
	}
	nNum++;
	pCmdBuff[nNum-1] = m_nEndCardID;

	dlg->m_Serial_dlg->Send_car_ControlRoute_CtrlCMD(dlg->m_CComoBoxCar.GetCurSel()+1, (byte *)pCmdBuff, nNum);

	delete pCmdBuff;
	pCmdBuff = NULL;

}
