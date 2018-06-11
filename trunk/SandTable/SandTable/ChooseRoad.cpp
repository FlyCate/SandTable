// ChooseRoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "ChooseRoad.h"
#include "afxdialogex.h"


// CChooseRoad �Ի���

IMPLEMENT_DYNAMIC(CChooseRoad, CDialogEx)

CChooseRoad::CChooseRoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooseRoad::IDD, pParent)
{

}

CChooseRoad::~CChooseRoad()
{
}

void CChooseRoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROAD, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CChooseRoad, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChooseRoad::OnBnClickedOk)
END_MESSAGE_MAP()

void CChooseRoad::SetRoadList(list<tagRoad> lstRoad)
{
	m_lstRoad = lstRoad;
}


// CChooseRoad ��Ϣ�������
BOOL CChooseRoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;
	int i = 0;
	for (auto it = m_lstRoad.begin(); it != m_lstRoad.end(); it++)
	{
		str.Format("%s:%s(%d)", it->strRoadName.c_str(), it->nDirection == 0? "����" : "����", it->nID);
		int nRet = m_ComboBox.FindString(0, str.GetBuffer());
		if (nRet == -1)
		{
			m_ComboBox.InsertString(i, str);
		}
	}

	m_ComboBox.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChooseRoad::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_ComboBox.GetWindowTextA(m_strRoadName);
	int nPos = m_strRoadName.Find('(');
	CString str = m_strRoadName.Right(m_strRoadName.GetLength() - nPos - 1);
	str = str.Left(str.GetLength() - 1);
	m_nRoadID = atoi(str.GetBuffer());

	CDialogEx::OnOK();
}
