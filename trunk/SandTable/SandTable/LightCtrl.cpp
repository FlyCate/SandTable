// LightCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTableDlg.h"
#include "LightCtrl.h"
#include "afxdialogex.h"

#define SERIAL_OPEN()\
	if (dlg->m_Serial_dlg->m_com_isopen == false)\
{\
	MessageBox("请打开串口");\
	return;\
}

// CLightCtrl 对话框

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


// CLightCtrl 消息处理程序

BOOL CLightCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO: 在此添加额外的初始化代码
	OnBnDropDownRadioControl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLightCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

//手动
void CLightCtrl::OnBnDropDownRadioControl()
{
	// TODO: 在此添加控件通知处理程序代码
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

//自动
void CLightCtrl::OnBnClickedRadioAutoControl()
{
	// TODO: 在此添加控件通知处理程序代码
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

//路灯开
void CLightCtrl::OnBnClickedButtonRoadLight()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Roadlight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//路灯关
void CLightCtrl::OnBnClickedButtonRoadLightClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->m_Serial_dlg->SendBusStop_Roadlight_CtrlCMD(false);
}

//景观灯开
void CLightCtrl::OnBnClickedButtonLandscapeLight()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Landscapelight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//景观灯关
void CLightCtrl::OnBnClickedButtonLandscapeLightClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Landscapelight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}

//建筑灯开
void CLightCtrl::OnBnClickedButtonBuildingLight()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Buildinglight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//建筑灯关
void CLightCtrl::OnBnClickedButtonBuildingLightClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Buildinglight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}

//所有灯开
void CLightCtrl::OnBnClickedButtonAllLight()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->m_Serial_dlg->SendBusStop_Alllight_CtrlCMD(true);
	dlg->SetTimer(0, 100, NULL);
}

//所有灯关
void CLightCtrl::OnBnClickedButtonAllLightClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CSandTableDlg * dlg = (CSandTableDlg *) m_parent;
	SERIAL_OPEN();
	dlg->KillTimer(0);
	Sleep(5);
	dlg->m_Serial_dlg->SendBusStop_Alllight_CtrlCMD(false);
	dlg->SetTimer(0, 100, NULL);
}
