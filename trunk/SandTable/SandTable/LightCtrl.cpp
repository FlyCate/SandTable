// LightCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTableDlg.h"
#include "LightCtrl.h"
#include "afxdialogex.h"

#define SERIAL_OPEN()\
	if (dlg->m_Serial_dlg->m_com_isopen == false)\
{\
	MessageBox("��򿪴���");\
	return;\
}

// CLightCtrl �Ի���

IMPLEMENT_DYNAMIC(CLightCtrl, CDialogEx)

CLightCtrl::CLightCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightCtrl::IDD, pParent)
	, m_RadioValue(0)
{
	m_parent = pParent;
}

CLightCtrl::~CLightCtrl()
{
}

void CLightCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MANUAL_CONTROL, m_RadioValue);
}


BEGIN_MESSAGE_MAP(CLightCtrl, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_MANUAL_CONTROL, &CLightCtrl::OnBnDropDownRadioControl)
	ON_BN_CLICKED(IDC_RADIO_AUTO_CONTROL, &CLightCtrl::OnBnClickedRadioAutoControl)
	ON_BN_CLICKED(IDC_BUTTON_ROAD_LIGHT, &CLightCtrl::OnBnClickedButtonRoadLight)
	ON_BN_CLICKED(IDC_BUTTON_ROAD_LIGHT_CLOSE, &CLightCtrl::OnBnClickedButtonRoadLightClose)
	ON_BN_CLICKED(IDC_BUTTON_LANDSCAPE_LIGHT, &CLightCtrl::OnBnClickedButtonLandscapeLight)
	ON_BN_CLICKED(IDC_BUTTON_LANDSCAPE_LIGHT_CLOSE, &CLightCtrl::OnBnClickedButtonLandscapeLightClose)
	ON_BN_CLICKED(IDC_BUTTON_BUILDING_LIGHT, &CLightCtrl::OnBnClickedButtonBuildingLight)
	ON_BN_CLICKED(IDC_BUTTON_BUILDING_LIGHT_CLOSE, &CLightCtrl::OnBnClickedButtonBuildingLightClose)
	ON_BN_CLICKED(IDC_BUTTON_ALL_LIGHT, &CLightCtrl::OnBnClickedButtonAllLight)
	ON_BN_CLICKED(IDC_BUTTON_ALL_LIGHT_CLOSE, &CLightCtrl::OnBnClickedButtonAllLightClose)
END_MESSAGE_MAP()


// CLightCtrl ��Ϣ�������

BOOL CLightCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OnBnDropDownRadioControl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLightCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

//�ֶ�
void CLightCtrl::OnBnDropDownRadioControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON_ROAD_LIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ROAD_LIGHT_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LANDSCAPE_LIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LANDSCAPE_LIGHT_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BUILDING_LIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BUILDING_LIGHT_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ALL_LIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ALL_LIGHT_CLOSE)->EnableWindow(TRUE);

	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_light_Auto_ManualMode_CMD(false);
	dlg->SetTimer(0, 100, NULL);
}

//�Զ�
void CLightCtrl::OnBnClickedRadioAutoControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON_ROAD_LIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ROAD_LIGHT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LANDSCAPE_LIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LANDSCAPE_LIGHT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BUILDING_LIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BUILDING_LIGHT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ALL_LIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ALL_LIGHT_CLOSE)->EnableWindow(FALSE);

	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_light_Auto_ManualMode_CMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//·�ƿ�
void CLightCtrl::OnBnClickedButtonRoadLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Roadlight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//·�ƹ�
void CLightCtrl::OnBnClickedButtonRoadLightClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->m_Serial_dlg->SendBusStop_Roadlight_CtrlCMD(false);
}

//���۵ƿ�
void CLightCtrl::OnBnClickedButtonLandscapeLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Landscapelight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//���۵ƹ�
void CLightCtrl::OnBnClickedButtonLandscapeLightClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Landscapelight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}

//�����ƿ�
void CLightCtrl::OnBnClickedButtonBuildingLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Buildinglight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//�����ƹ�
void CLightCtrl::OnBnClickedButtonBuildingLightClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Buildinglight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}

//���еƿ�
void CLightCtrl::OnBnClickedButtonAllLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->m_Serial_dlg->SendBusStop_Alllight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//���еƹ�
void CLightCtrl::OnBnClickedButtonAllLightClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Alllight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}
