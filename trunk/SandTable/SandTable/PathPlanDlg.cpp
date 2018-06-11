// PathPlanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "PathPlanDlg.h"
#include "afxdialogex.h"
#include "SandTableDlg.h"
#include "MapDlg.h"
#include "ChooseRoad.h"
#include <math.h>

// CPathPlanDlg �Ի���

#define SERIAL_OPEN()\
	if (dlg->m_Serial_dlg->m_com_isopen == false)\
{\
	MessageBox("��򿪴���");\
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


// CPathPlanDlg ��Ϣ�������

BOOL CPathPlanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG lStyle; 
	lStyle = GetWindowLong(m_PathList.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_LIST; // ����style 
	SetWindowLong(m_PathList.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_PathList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_PathList.SetExtendedStyle(dwStyle); // ������չ��� 
	//m_PathList.InsertColumn( 0, "���", LVCFMT_LEFT, 40 );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//·������
void CPathPlanDlg::OnBnClickedButtonSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_PathList.DeleteAllItems();

	bool bRet = m_roadPlan.StartCodeCardArriveEndCard(m_nStartCardID, m_nEndCardID);
	if (bRet == false)
	{
		MessageBox("��ʼ�㲻�ܵ����յ㣡������ѡ��");
		return;
	}

	long lRet = m_roadPlan.RoadPlan(m_nStartCardID, m_nEndCardID);
	if (lRet != 0)
	{
		CString str;
		str.Format("·���滮ʧ�ܣ� ������Ϣ��%s", m_roadPlan.GetErrStr());
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
					m_strStart.Format("%s:%s(%d)", it->strRoadName.c_str(), it->nDirection == 0? "����": "����", itCard->nID);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
					m_strEnd.Format("%s:%s(%d)", it->strRoadName.c_str(), it->nDirection == 0? "����": "����", itCard->nID);
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
	lvItem.iItem = nIndex;//����
	lvItem.iSubItem = 0;
	lvItem.pszText = (char *)str;//��һ��

	//�����һ�в����¼ֵ.
	m_PathList.InsertItem(&lvItem);
}


void CPathPlanDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��·���滮��ѡ��ȷ����");
		return;
	}

	char * pCmdBuff = new char[lstPlan.size() * 3];
	if (pCmdBuff == NULL)
	{
		MessageBox("ϵͳ�������Ժ����ԣ�");
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
