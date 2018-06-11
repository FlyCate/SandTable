
// SandTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "SandTableDlg.h"
#include "afxdialogex.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SERIAL_OPEN()\
	if (m_Serial_dlg->m_com_isopen == false)\
{\
	MessageBox("请打开串口");\
	return;\
}

/*int TrafficPos1[4][2] = {{605,81},{556,124},{498,81},{-1,-1}};
int TrafficPos2[4][2] = {{859,81},{805,124},{750,81},{-1,-1}};
int TrafficPos3[4][2] = {{-1,-1},{1217,268},{1162,237},{1208,176}};
int TrafficPos4[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
int TrafficPos5[4][2] = {{986,837},{-1,-1},{886,837},{-1,-1}};
int TrafficPos6[4][2] = {{606,81},{-1,-1},{502,81},{-1,-1}};
int TrafficPos7[4][2] = {{341,81},{-1,-1},{239,81},{-1,-1}};
int TrafficPos8[4][2] = {{133,490},{66,560},{-1,-1},{66,418}};
int TrafficPos9[4][2] = {{612,490},{556,541},{503,490},{-555,541}};
int TrafficPos10[4][2] = {{607,323},{572,352},{-1,-1},{543,289}};
int TrafficPos11[4][2] = {{878,300},{826,356},{753,321},{803,260}};*/

/*int TrafficPos[11][4][2] = {{{605,81},{556,124},{498,81},{-1,-1}},
{{859,81},{805,124},{750,81},{-1,-1}},
{{-1,-1},{1217,268},{1162,237},{1208,176}},
{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
{{986,837},{-1,-1},{886,837},{-1,-1}},
{{606,837},{-1,-1},{502,837},{-1,-1}},
{{341,837},{-1,-1},{239,837},{-1,-1}},
{{133,490},{66,560},{-1,-1},{66,418}},
{{612,490},{556,541},{503,490},{555,442}},
{{607,323},{572,352},{-1,-1},{543,289}},
{{878,300},{826,356},{753,321},{803,260}}};*/

int g_RedLightCardID = 194;		//闯红灯卡号
int g_OverSpeedCardID = 153;	//超速卡号

int TrafficPos[11][4][2] = {{{605,81},{-1,-1},{498,81},{-1,-1}},
{{859,81},{-1,-1},{750,81},{-1,-1}},
{{-1,-1},{1217,268},{-1,-1},{1208,176}},
{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
{{986,837},{-1,-1},{886,837},{-1,-1}},
{{606,837},{-1,-1},{502,837},{-1,-1}},
{{341,837},{-1,-1},{239,837},{-1,-1}},
{{-1,-1},{66,560},{-1,-1},{66,418}},
{{612,490},{556,541},{503,490},{555,442}},
{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
{{878,300},{826,356},{753,321},{803,260}}};

const char * itoaa(const int nNum);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSandTableDlg 对话框

CSandTableDlg::CSandTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSandTableDlg::IDD, pParent)
	, m_strRTCCar(_T(""))
	, m_strRTCMoney(_T(""))
	, m_strRTCCar2(_T(""))
	, m_strRTCMoney2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lightCtrlDlg = NULL;
	m_rechargeDlg = NULL;
	m_violationDlg = NULL;
	m_pathplanDlg = NULL;
	m_cameraSettingDlg = NULL;
}

void CSandTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RTC_CAR, m_strRTCCar);
	DDX_Text(pDX, IDC_RTC_MONEY, m_strRTCMoney);
	DDX_Control(pDX, IDC_COMBO_CAR_LIST, m_CComoBoxCar);
	DDX_Text(pDX, IDC_RTC_CAR2, m_strRTCCar2);
	DDX_Text(pDX, IDC_RTC_MONEY2, m_strRTCMoney2);
}

BEGIN_MESSAGE_MAP(CSandTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_SETTINGS, &CSandTableDlg::OnBnClickedButtonSerialSettings)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_CONTROL, &CSandTableDlg::OnBnClickedButtonLightControl)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGE, &CSandTableDlg::OnBnClickedButtonRecharge)
	ON_BN_CLICKED(IDC_BUTTON_LLLEGAL_INQUIREIES, &CSandTableDlg::OnBnClickedButtonLllegalInquireies)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CSandTableDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSandTableDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OUT_GARAGE, &CSandTableDlg::OnBnClickedButtonOutGarage)
	ON_BN_CLICKED(IDC_BUTTON_IN_GARAGE, &CSandTableDlg::OnBnClickedButtonInGarage)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDING, &CSandTableDlg::OnBnClickedButtonSpeeding)
	ON_BN_CLICKED(IDC_BUTTON_RUNNING_RED_LIGHT, &CSandTableDlg::OnBnClickedButtonRunningRedLight)
	ON_BN_CLICKED(IDC_BUTTON_ROUTE_PLAN, &CSandTableDlg::OnBnClickedButtonRoutePlan)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA, &CSandTableDlg::OnBnClickedButtonCamera)
	ON_MESSAGE(WM_SERIAL_PACKET, &CSandTableDlg::OnSerialPacket) 
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_CAR_LIST, &CSandTableDlg::OnCbnSelchangeComboCarList)
END_MESSAGE_MAP()


// CSandTableDlg 消息处理程序

BOOL CSandTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString str = "VioPic";  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//创建目录,已有的话不影响  
	}  

	m_bRedLightCapFlag = false;
	m_bOverSpeedCapFlag = false;

	char buf[256];
	GetCurrentDirectory(sizeof(buf),buf);
	m_strExePath = buf;
	m_strExePath += "\\";

	CreateChildWindow();
	m_bOpenSerial = false;
	//::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU);

	m_CComoBoxCar.InsertString(0, "小车1");
	m_CComoBoxCar.InsertString(1, "小车2");
	m_CComoBoxCar.InsertString(2, "小车3");
	m_CComoBoxCar.InsertString(3, "小车4");
	m_CComoBoxCar.InsertString(4, "小车5");
	m_CComoBoxCar.InsertString(5, "公交1");
	m_CComoBoxCar.InsertString(6, "公交2");
	m_CComoBoxCar.InsertString(7, "公交3");
	m_CComoBoxCar.InsertString(8, "公交4");
	m_CComoBoxCar.InsertString(9, "公交5");
	m_CComoBoxCar.InsertString(10, "货车1");
	m_CComoBoxCar.InsertString(11, "货车2");
	m_CComoBoxCar.InsertString(12, "货车3");
	m_CComoBoxCar.InsertString(13, "货车4");
	m_CComoBoxCar.InsertString(14, "货车5");
	m_CComoBoxCar.SetCurSel(0);

	//获取系统任务栏高度
	int taskbar_height = 10;
	CWnd * hWnd = FindWindow("Shell_TrayWnd", 0);
	RECT rect;
	::GetWindowRect(hWnd->GetSafeHwnd(), &rect);
	taskbar_height = rect.bottom - rect.top;

	//设置主窗口位置和大小
	MoveWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - taskbar_height);

	//设置地图图片
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_LOW); 
	//设置静态控件窗口风格为位图居中显示
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//显示图片
	CString strPicPath = m_strExePath;
	strPicPath += "res/MapLow.bmp";
	m_big_pic_path =strPicPath;
	::GetWindowRect(GetDlgItem(IDC_STATIC_MAP_LOW)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
		rect.right - rect.left,                  //宽度 0为默认大小
		rect.bottom - rect.top,                  //高度 像素为单位
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nMapLowWidth = rect.right - rect.left;
	m_nMapLowHeight = rect.bottom - rect.top;

	//设置地图图片
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_HIGH); 
	//设置静态控件窗口风格为位图居中显示
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//显示图片
	strPicPath = m_strExePath;
	strPicPath += "res/MapHigh.bmp";
	m_high_pic_path = strPicPath;
	::GetWindowRect(GetDlgItem(IDC_STATIC_MAP_HIGH)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
		rect.right - rect.left,                  //宽度 0为默认大小
		rect.bottom - rect.top,                  //高度 像素为单位
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nMapHighWidth = rect.right - rect.left;
	m_nMapHighHeight = rect.bottom - rect.top;

	//初始化摄像头
	bool bRet = m_db.OpenDB(SANDTABLE_CAMERA);
	if (bRet == true)
	{
		list<tagCamera> listCamera;
		bRet = m_db.ReadDB(listCamera);
		if (bRet == true)
		{
			for (auto it = listCamera.begin(); it != listCamera.end(); it++)
			{
				if (it->strCameraType.compare("闯红灯") == 0)
				{
					m_RedCamera.SetCameraID(it->nCameraID);
				}
				else if (it->strCameraType.compare("超速") == 0)
				{
					m_SpeedCamera.SetCameraID(it->nCameraID);
				}
			}
		}

		m_db.CloseDB();
	}

	long lRet = m_RedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("打开闯红灯摄像头失败！请重新设置摄像头！");
	}

	lRet = m_SpeedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("打开超速摄像头失败！请检查并重新设置摄像头！");
	}

	SetTimer(0, 100, NULL);
	TrafficLightInit();
    SetTimer(159, 1000, NULL);//用于刷新图片
	DrawCarStatusPicture();
	
	GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSandTableDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	/*else if (nID == SC_MOVE || nID == 0xF012)
	{
		return;
	}*/
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSandTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		DrawCarStatusPicture();
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSandTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//串口设置
void CSandTableDlg::OnBnClickedButtonSerialSettings()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Serial_dlg->ShowWindow(TRUE);
}

//路灯控制
void CSandTableDlg::OnBnClickedButtonLightControl()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_lightCtrlDlg == NULL)
	{
		m_lightCtrlDlg = new CLightCtrl(this);
	}

	m_lightCtrlDlg->DoModal();
}

//充值
void CSandTableDlg::OnBnClickedButtonRecharge()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_rechargeDlg == NULL)
	{
		m_rechargeDlg = new CRechargeDlg(this);
	}

	if (m_rechargeDlg->DoModal() == IDOK)
	{
		TRACE("money %d\n", m_rechargeDlg->m_lMoney);
		SERIAL_OPEN();
		KillTimer(0);
		Sleep(5);
		CString strSelectCar;
		 m_CComoBoxCar.GetWindowTextA(strSelectCar);
		if ((strSelectCar.Compare("小车1") == 0)||(strSelectCar.Compare("货车1") == 0))
		{
			m_Serial_dlg->Send_car_ChargeMoney_CtrlCMD(0, m_CComoBoxCar.GetCurSel()+1, m_rechargeDlg->m_lMoney);
		}
		else
		{
			m_Serial_dlg->Send_car_ChargeMoney_CtrlCMD(1, m_CComoBoxCar.GetCurSel()+1, m_rechargeDlg->m_lMoney);
		}
		SetTimer(0, 100, NULL);
	}
}


// 创建子窗口
void CSandTableDlg::CreateChildWindow(void)
{
	m_Serial_dlg = new SerialSetting(this);
	m_Serial_dlg->Create(IDD_DIALOG_SERIAL_CONTROL, this);
	m_Serial_dlg->ShowWindow(false);

	m_questionDlg = new CQuestion(this);
	m_questionDlg->Create(IDD_DIALOG_OK, this);
	m_questionDlg->ShowWindow(FALSE);
}

//获取卡号的  原始地图坐标
bool CSandTableDlg::GetCardPos(const int nCardID, TAGPOS & pos)
{
	long lRet = m_roadPlan.GetCardPos(nCardID, pos);
	if (lRet != 0)
	{
		TRACE("查找坐标失败!\n");
		return false;
	}

	return true;
}

//原始地图坐标 转换为 绘图控件坐标
void CSandTableDlg::MapLowPosTransforDrawPicPos(TAGPOS & pos)
{
	pos.x = pos.x * m_nMapLowWidth / MAP_LOW_WIDTH;
	pos.y = pos.y * m_nMapLowHeight / MAP_LOW_HEIGHT;
	pos.y = m_nMapLowHeight - pos.y;
}

//原始地图坐标 转换为 绘图控件坐标
void CSandTableDlg::MapHeightPosTransforDrawPicPos(TAGPOS & pos)
{
	pos.x = pos.x * m_nMapHighWidth / MAP_HIGH_WIDTH;
	pos.y = pos.y * m_nMapHighHeight / MAP_HIGH_HEIGHT;
	pos.y = m_nMapLowHeight - pos.y;
}

//坐标转换
void CSandTableDlg::MapPosTrans(const int nCardID, const TAGPOS & pos, REAL_POS & realPos)
{
	if ((pos.x == -1) && (pos.y == -1))
	{
		realPos.pos.x = pos.x;
		realPos.pos.y = pos.y;
		return;
	}

	if (nCardID >= 144 && nCardID <= 193)
	{
		realPos.pos.x = pos.x;
		realPos.pos.y = MAP_HIGH_HEIGHT - pos.y;
		realPos.flag = 1;
	}
	else
	{
		realPos.pos.x = pos.x;
		realPos.pos.y = MAP_LOW_HEIGHT - pos.y;
		realPos.flag = 0;
	}
}

//违章查询
void CSandTableDlg::OnBnClickedButtonLllegalInquireies()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_violationDlg == NULL)
	{
		m_violationDlg = new CViolationDlg(this);
	}

	m_violationDlg->DoModal();
}

//行驶
void CSandTableDlg::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Driving_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//停止
void CSandTableDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Stop_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//出库
void CSandTableDlg::OnBnClickedButtonOutGarage()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Out_warehouse_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//入库
void CSandTableDlg::OnBnClickedButtonInGarage()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Back_warehouse_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//超速
void CSandTableDlg::OnBnClickedButtonSpeeding()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_OverSpeeding_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	m_bOverSpeedCapFlag = true;
	SetTimer(0, 100, NULL);
}

//闯红灯
void CSandTableDlg::OnBnClickedButtonRunningRedLight()
{
	// TODO: 在此添加控件通知处理程序代码
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->SendRunLedLightCMD();
	m_bRedLightCapFlag = true;
	SetTimer(0, 100, NULL);
}

//路径规划
void CSandTableDlg::OnBnClickedButtonRoutePlan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pathplanDlg == NULL)
	{
		m_pathplanDlg = new CPathPlanDlg(this);
	}

	if (m_pathplanDlg->DoModal() == IDOK)
	{

		KillTimer(0);
		Sleep(5);
		m_pathplanDlg->SendCmd();
		SetTimer(0, 100, NULL);
	}
}

//抓拍设置
void CSandTableDlg::OnBnClickedButtonCamera()
{
	// TODO: 在此添加控件通知处理程序代码

	m_RedCamera.CloseCamera();
	m_SpeedCamera.CloseCamera();

	if (m_cameraSettingDlg == NULL)
	{
		m_cameraSettingDlg = new CCameraSettingDlg(this);
	}

	m_cameraSettingDlg->DoModal();

	long lRet = m_RedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("打开闯红灯摄像头失败！请重新设置摄像头！");
	}

	lRet = m_SpeedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("打开超速摄像头失败！请检查并重新设置摄像头！");
	}
}

//串口回调消息相应
LRESULT CSandTableDlg::OnSerialPacket(WPARAM wParam, LPARAM lParam)
{
	PKT * pkt = (PKT *) lParam;
	CString strMsg;

	if (pkt != NULL)
	{
		if (pkt->header == FRAME_HEADER)
		{
			switch(pkt->cmd)
			{
			case CTRL_CMD: //控制返回
				{
					strMsg = GetNodeidStr(pkt->nodeid);

					if (strMsg.GetLength() != 0)
					{
						if (pkt->payload[0] == 'O' && pkt->payload[1] == 'K')
						{
							strMsg += " 控制命令发送成功。";
						}
						else
						{
							strMsg += " 节点没有组网。";
						}

						m_questionDlg->SetQuestion(strMsg.GetBuffer());
						m_questionDlg->ShowWindow(TRUE);
					}
				}
				break;
			case COLL_CMD: //采集
				{
					switch (pkt->payload[0])
					{
					case 0x55://红绿灯
						{
							switch (pkt->nodeid)
							{
							case CROSS_1 :
							case CROSS_2 :
							case CROSS_3 :
							case CROSS_4 :
							case CROSS_5 :
							case CROSS_6 :
							case CROSS_7 :
							case CROSS_8 :
							case CROSS_9 :
							case CROSS_10:
							case CROSS_11:
								SetTrafficLight(pkt->nodeid - CROSS_1, pkt->payload[1]);
								break;
							default:
								break;
							}
						}
						break;
					case 0x06://小车位置信息
						{
							switch (pkt->nodeid)
							{
							case CAR_1:
								{
									//超速
									if (m_bOverSpeedCapFlag == true)
									{
										m_bOverSpeedCapFlag = false;
										if (pkt->payload[1] == g_OverSpeedCardID) 
										{
											//超速
											memset(m_SpeedBuff, 0, 1024);
											SYSTEMTIME t;
											GetLocalTime(&t);
											sprintf_s(m_SpeedBuff, "VioPic/Speed%d%d%d%d%d%d_%d.jpg", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
											long lRet = m_SpeedCamera.SnapshotCamera(m_SpeedBuff);
											if (OPENCV_CAMERA_SUCCESS == lRet)
											{
												InsertVioData(m_SpeedBuff, "超速");
											}
										}
									}
								}
							case CAR_2:
							case CAR_3:
							case CAR_4:
							case CAR_5:
							case BUS_1:
							case BUS_2:
							case BUS_3:
							case BUS_4:
							case BUS_5:
							case Truck_1:
							case Truck_2:
							case Truck_3:
							case Truck_4:
							case Truck_5:
								{
									//闯红灯 卡号为194
									if (pkt->payload[1] == g_RedLightCardID)
									{
										if (m_bRedLightCapFlag == true) 
										{
											m_bRedLightCapFlag = false;
											//闯红灯
											memset(m_RedBuff, 0, 1024);
											SYSTEMTIME t;
											GetLocalTime(&t);
											sprintf_s(m_RedBuff, "VioPic/Red%d%d%d%d%d%d_%d.jpg", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
											long lRet = m_RedCamera.SnapshotCamera(m_RedBuff);
											if (OPENCV_CAMERA_SUCCESS == lRet)
											{
												InsertVioData(m_RedBuff, "闯红灯");
											}
										}
										break;
									}

									//显示小车位置
									SetCarPos(pkt->nodeid - CAR_1, pkt->payload[1]);
								}
								break;
							default:
								break;
							}
						}
						break;
					case 0x07://ETC 余额汇报
						{
							switch (pkt->nodeid)
							{
							case ETC_1:
								{
									m_strRTCCar = GetNodeidStr(pkt->payload[1]);
									m_strRTCMoney = itoaa((int)(pkt->payload[2]));
									UpdateData(FALSE);
								}
								break;
							case ETC_2:
								{
									m_strRTCCar2 = GetNodeidStr(pkt->payload[1]);
									m_strRTCMoney2 = itoaa((int)(pkt->payload[2]));
									UpdateData(FALSE);
								}
								break;
							default:
								break;
							}
						}
						break;
					case 0x0b://小车重量
						{
							if (pkt->payload[4] == 1)
							{
								CString strErrMsg;
								strErrMsg.Format("超重车辆：%s 重量：%d", GetNodeidStr(pkt->payload[1]), itoaa(pkt->payload[2]*256 + pkt->payload[3]));
								InsertVioData(strErrMsg.GetBuffer(), "超重");
							}
						}
						break;
					case 'n'://节点组网不存在
						{

							strMsg = GetNodeidStr(pkt->nodeid);
							strMsg += " 节点没组网";
							OutputDebugString(strMsg);
						}
						break;
					default:
						{
						}
						break;
					}
				}
				break;
			default:
				break;
			}
		}

		/*if (pkt != NULL)
		{
			delete[] pkt;
			pkt = NULL;
		}*/
	}

	return 0;
}

const char * CSandTableDlg::GetNodeidStr(const int nNodeID)
{
	static CString strMsg = "";

	switch (nNodeID)
	{
	case CAR_1:	strMsg = "小车1";
		break;
	case CAR_2:	strMsg = "小车2";
		break;
	case CAR_3:	strMsg = "小车3";
		break;
	case CAR_4:	strMsg = "小车4";
		break;
	case CAR_5:	strMsg = "小车5";
		break;
	case BUS_1:	strMsg = "公交1";
		break;
	case BUS_2:	strMsg = "公交2";
		break;
	case BUS_3:	strMsg = "公交3";
		break;
	case BUS_4:	strMsg = "公交4";
		break;
	case BUS_5:	strMsg = "公交5";
		break;
	case Truck_1:	strMsg = "货车1";
		break;
	case Truck_2:	strMsg = "货车2";
		break;
	case Truck_3:	strMsg = "货车3";
		break;
	case Truck_4:	strMsg = "货车4";
		break;
	case Truck_5:	strMsg = "货车5";
		break;
	case CROSS_1 :	strMsg = "路口1";
		break;
	case CROSS_2 :	strMsg = "路口2";
		break;
	case CROSS_3 :	strMsg = "路口3";
		break;
	case CROSS_4 :	strMsg = "路口4";
		break;
	case CROSS_5 :	strMsg = "路口5";
		break;
	case CROSS_6 :	strMsg = "路口6";
		break;
	case CROSS_7 :	strMsg = "路口7";
		break;
	case CROSS_8 :	strMsg = "路口8";
		break;
	case CROSS_9 :	strMsg = "路口9";
		break;
	case CROSS_10:	strMsg = "路口10";
		break;
	case CROSS_11:	strMsg = "路口11";
		break;
	case ETC_1:	strMsg = "ETC1";
		break;
	case ETC_2:	strMsg = "ETC2";
		break;
	case Truck_weighing:	strMsg = "货车称重站";
		break;
	case BUS_STOP:	strMsg = "1号公交站";
		break;
	case Parking_lot:	strMsg = "停车场道闸";
		break;
	default:
		strMsg = "";
		break;
	}

	return strMsg.GetBuffer();
}


void CSandTableDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 0:
		{
			if (m_Serial_dlg->m_com_isopen == true)
			{
// 				for (int i = CAR_1; i <= Parking_lot; i++)
// 				{
// 					m_Serial_dlg->SendCollectNodeCMD(i);
// 				}
				static int i = 1;
				m_Serial_dlg->SendCollectNodeCMD(i++);
				if (i == 7)
				{
					i = 1;
				}
			}
		}
		break;
	case 159:
		{
	  //  	static int step=0;
			//step +=1;
			//if (step >800)
			//{
			//	step =0;
			//}
			//m_pos[0].pos.x += step;
			//m_pos[0].pos.y += step;
			//m_pos[6].pos.x += step+30;
			//m_pos[6].pos.y += step+30;
			//m_pos[13].flag =true;
			//m_pos[13].pos.x += step;
			//m_pos[13].pos.y += step;			
			//m_TrafficLight[1].Light[0].pos.x = 100;
			//m_TrafficLight[1].Light[0].pos.y= 100;
			//m_TrafficLight[1].Light[0].bFlag = true;
			//m_TrafficLight[1].Light[0].LightColor = TRAFFIC_COLOR_RED;
			//m_TrafficLight[1].Light[2].pos.x = 120;
			//m_TrafficLight[1].Light[2].pos.y= 120;
			//m_TrafficLight[1].Light[2].bFlag =true;
			//m_TrafficLight[1].Light[2].LightColor = TRAFFIC_COLOR_YELLOW;
			//m_TrafficLight[10].Light[2].pos.x = 700;
			//m_TrafficLight[10].Light[2].pos.y= 700;
			//m_TrafficLight[10].Light[2].bFlag =true;
			//m_TrafficLight[10].Light[2].LightColor = TRAFFIC_COLOR_GREEN;
			DrawMapPicture(m_big_pic_path);
			DrawhighMapPicture( m_high_pic_path);
			//DrawCarStatusPicture();
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSandTableDlg::InsertVioData(const char * pURL, const char * pVioType)
{
	bool bRet = m_db.OpenDB(SANDTABLE_VIOLATION);
	if (bRet == false)
	{
		TRACE("打开违法DB文件失败!\n");
		return;
	}

	tagViolation tViolation;
	tViolation.strURL = pURL;
	tViolation.strVioType = pVioType;
	bRet = m_db.WriteDB(tViolation);
	if (bRet == true)
	{
		//MessageBox("写入违法成功！");
		TRACE("写入违法成功！\n");
	}
	else
	{
		//MessageBox("写入违法失败！");
		TRACE("写入违法失败！\n");
	}

	m_db.CloseDB();
}


// 双缓冲绘制地图
void CSandTableDlg::DrawMapPicture(CString name)
{
	CString dstname;
	if (name== "")return;

	dstname =  name;
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		dstname, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if (!hbmp)return;
	CBitmap bitmap;
	bitmap.Attach(hbmp); //关联位图对象  
	CDC *pDC = GetDC();
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(&bitmap);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	CRect dstrc;


	real_draw_map_inf(memdc);
	Draw_coress_light(memdc,8);
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_LOW); 


	pWnd->GetWindowRect(&dstrc);//获取控件的屏幕坐标
	ScreenToClient(&dstrc);//转换为对话框上的客户坐标

	pDC->StretchBlt(dstrc.left,dstrc.top,dstrc.Width(),dstrc.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	bitmap.DeleteObject();
	ReleaseDC(pDC);
	memdc.DeleteDC();
}
// 高架双缓冲绘制地图
void CSandTableDlg::DrawhighMapPicture(CString name)
{
	CString dstname;
	if (name== "")return;

	dstname =  name;
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		dstname, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if (!hbmp)return;
	CBitmap bitmap;
	bitmap.Attach(hbmp); //关联位图对象  
	CDC *pDC = GetDC();
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(&bitmap);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	CRect dstrc;
	real_draw_high_map_inf(memdc);
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_HIGH); 
	pWnd->GetWindowRect(&dstrc);//获取控件的屏幕坐标
	ScreenToClient(&dstrc);//转换为对话框上的客户坐标
	pDC->StretchBlt(dstrc.left,dstrc.top,dstrc.Width(),dstrc.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	//pDC->BitBlt(dstrc.left,dstrc.top,dstrc.Width(),dstrc.Height(),&memdc,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	bitmap.DeleteObject();
	ReleaseDC(pDC);
	memdc.DeleteDC();
}
void CSandTableDlg::draw_circular(CPoint point ,CDC &memdc,COLORREF rgb)
{
	CPen *oldpen;
	CPen pen(PS_SOLID,2,rgb);
	oldpen = memdc.SelectObject(&pen);
	CBrush *oldbrush;
	CBrush pBrush(rgb);
	oldbrush = memdc.SelectObject(&pBrush);

	memdc.Ellipse(point.x-20,point.y-20,point.x+20,point.y+20);
	memdc.SelectObject(pen);
	memdc.SelectObject(oldbrush);
}

// 固定位置写入数字
void CSandTableDlg::WriteNumberTOpicture(CPoint point ,CDC &memdc,COLORREF rgb,CString str)
{
	CFont font, *pOldFont= NULL;//定义字体对象及其指针 ，你可以根据需求选择你要的字体类型和大小
	// font.CreatePointFont(120, "楷体", &dc);   
	font.CreatePointFont(180, "system", &memdc);

	pOldFont = memdc.SelectObject(&font);   

	memdc.SetBkMode(TRANSPARENT);//透明   
	memdc.SetTextColor(RGB(255,255,255)); 
	memdc.TextOut(point.x-8, point.y-15,str, str.GetLength());

	memdc.SelectObject(pOldFont);

	if(pOldFont)//释放资源避免内存泄露
	font.DeleteObject(); 
}

void CSandTableDlg::real_draw_map_inf(CDC &memdc)
{
	int i =0;
	COLORREF rgb;
	for (i=0;i<15;i++)
	{	
		if (i<5)
		{
			rgb = RGB(220,68,65);
		}else if (i<10)
		{
			rgb = RGB(82,71,214);
		}else
		{
			rgb = RGB(55,157,157);
		}
		if (  m_pos[i].flag == false )//大图
		{   //X坐标小于0或者y坐标小于0 则认为是无效的数据，或者已经超时离线数据
			if( m_pos[i].pos.x <0 || m_pos[i].pos.y <0 )
				continue;
			CPoint point;
			point.x = m_pos[i].pos.x;
			point.y = m_pos[i].pos.y;
			draw_circular( point , memdc, rgb);
			CString str;
			str.Format("%d",i%5+1);
			WriteNumberTOpicture( point , memdc, rgb,str);
		}
	}
}
void CSandTableDlg::real_draw_high_map_inf(CDC &memdc)
{
	int i =0;
	COLORREF rgb;
	for (i=0;i<15;i++)
	{	
		if (i<5)
		{
			rgb = RGB(220,68,65);
		}else if (i<10)
		{
			rgb = RGB(82,71,214);
		}else
		{
			rgb = RGB(105,92,120);
		}
		if (  m_pos[i].flag == true )//高价绘图
		{
			//X坐标小于0或者y坐标小于0 则认为是无效的数据，或者已经超时离线数据
			if( m_pos[i].pos.x <0 || m_pos[i].pos.y <0 )
				continue;
			CPoint point;
			point.x = m_pos[i].pos.x;
			point.y = m_pos[i].pos.y;
			//draw_circular( point , memdc, rgb);
			draw_circular( point , memdc, rgb,15);
			CString str;
			str.Format("%d",i%5+1);
			WriteNumberTOpicture( point , memdc, rgb,str);
		}
	}

}

void CSandTableDlg::TrafficLightInit()
{
	//遍历所有路口
	for (int i = 0; i < 11; i++)
	{
		//遍历四个方向
		for (int j = 0; j < 4; j++)
		{
			//如果x坐标不是 -1  说明这个方向存在红绿灯
			if (TrafficPos[i][j][0] != -1)
			{
				//标志位置位
				m_TrafficLight[i].Light[j].bFlag = true;
				//设置坐标
				m_TrafficLight[i].Light[j].pos.x = TrafficPos[i][j][0];
				m_TrafficLight[i].Light[j].pos.y = TrafficPos[i][j][1];
			}
		}
	}
}

void CSandTableDlg::SetTrafficLight(const int nCrossing, const int nLightStatus)
{
	switch (nLightStatus)
	{
	case 1://东西红灯，南北绿灯
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_RED;//东
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_RED;//西
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_GREEN;//南
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_GREEN;//北
		}
		break;
	case 2://东西红灯，南北黄灯
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_RED;//东
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_RED;//西
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_YELLOW;//南
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_YELLOW;//北
		}
		break;
	case 3://东西绿灯，南北红灯
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_GREEN;//东
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_GREEN;//西
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_RED;//南
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_RED;//北
		}
		break;
	case 4://东西黄灯，南北红灯
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_YELLOW;//东
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_YELLOW;//西
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_RED;//南
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_RED;//北
		}
		break;
	default:
		break;
	}
}

void CSandTableDlg::SetCarPos(const int nCarID, const int nCardID)
{
	TAGPOS pos;
	bool bRet = GetCardPos(nCardID, pos);
	MapPosTrans(nCardID, pos, m_pos[nCarID]);
}


// 绘制红路灯
void CSandTableDlg::Draw_coress_light(CDC &memdc,int r)
{
	int i=0,j=0;
	for (i=0;i<11;i++)
	{
		//确定有此路口
		for (j=0;j<4;j++)
		{
			//确定有此路口
			if (m_TrafficLight[i].Light[j].bFlag == true)
			{
				CPoint point;
				point.x = m_TrafficLight[i].Light[j].pos.x;
				point.y = m_TrafficLight[i].Light[j].pos.y;
				if (point.x <0 || point.y<0)
				{
					continue;
				}
				if (m_TrafficLight[i].Light[j].LightColor == TRAFFIC_COLOR_RED )
				{
					draw_circular( point , memdc, RGB(255,0,0), r);
				}else if (m_TrafficLight[i].Light[j].LightColor == TRAFFIC_COLOR_YELLOW )
				{
					draw_circular( point , memdc, RGB(255,255,77), r);
				}else if (m_TrafficLight[i].Light[j].LightColor == TRAFFIC_COLOR_GREEN )
				{	
					draw_circular( point , memdc, RGB(0,255,0), r);
			    }
			} 
		}	
	}
}

void CSandTableDlg::draw_circular(CPoint point ,CDC &memdc,COLORREF rgb,int r)
{
	CPen *oldpen;
	CPen pen(PS_SOLID,2,rgb);
	oldpen = memdc.SelectObject(&pen);
	CBrush *oldbrush;
	CBrush pBrush(rgb);
	oldbrush = memdc.SelectObject(&pBrush);

	memdc.Ellipse(point.x-r,point.y-r,point.x+r,point.y+r);
	memdc.SelectObject(pen);
	memdc.SelectObject(oldbrush);
}
// 绘制左侧状态图
void CSandTableDlg::DrawCarStatusPicture(void)
{

	CBitmap bitmap,*OldBitmap;
	CRect rect;
	CDC memdc ;
	CDC *pDC = GetDC();

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_ST_CAR_PIC); 
	pWnd->GetWindowRect(&rect);//获取控件的屏幕坐标
	rect.left  += 50; 
	rect.right -= 10;
	rect.top   += 20;
	rect.bottom  -= 10; 
	ScreenToClient(&rect);//转换为对话框上的客户坐标
    memdc.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	OldBitmap=memdc.SelectObject(&bitmap);
	//memdc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(123,123,123));
	memdc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(240,240,240));
	draw_left_car_status(memdc);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memdc,0,0,rect.Width(),rect.Height(),SRCCOPY);
    memdc.SelectObject(OldBitmap);
	ReleaseDC(pDC);
	memdc.DeleteDC();
	//Invalidate();
}
void CSandTableDlg::real_draw_CarStatus(CDC &memdc,COLORREF rgb ,CPoint point,int num)
{
	int i =0,count=0;
	int index = num*5;
	int r =15;
	for (i=0;i<5;i++)
	{	

		//if (  m_pos[i+index].flag == false )//大图
		//{   //X坐标小于0或者y坐标小于0 则认为是无效的数据，或者已经超时离线数据
			//if( m_pos[i+index].pos.x <0 || m_pos[i+index].pos.y <0 )
			//	continue;
			point.x= 20+count*r*2+count*5;
		    count++;
			draw_circular( point , memdc, rgb,r);
			CString str;
			str.Format("%d",i%5+1);
			WriteNumberTOpicture( point , memdc, rgb,str);
		//}
	}
}
void CSandTableDlg::draw_left_car_status(CDC &memdc)
{
	int i =0,num =0;
	COLORREF rgb;
	CPoint point(0,0);
	for (i=0;i<15;i++)
	{	
		if (i<5)
		{
			rgb = RGB(220,68,65);
			point.y=30;
			num=0;
		}else if (i<10)
		{

			point.y=110;
			rgb = RGB(82,71,214);
			num=1;
		}else
		{
			point.y=200;
			rgb = RGB(105,92,120);
			num=2;
		}
		real_draw_CarStatus(memdc,	 rgb , point, num);
	}
}

BOOL CSandTableDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DrawCarStatusPicture();
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CSandTableDlg::OnCbnSelchangeComboCarList()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSelectCar;
	m_CComoBoxCar.GetWindowText(strSelectCar);

	if (strSelectCar.Compare("小车1") == 0)
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("小车1") == 0)||(strSelectCar.Compare("小车2") == 0)||(strSelectCar.Compare("货车1") == 0)||(strSelectCar.Compare("货车2") == 0))
	{
		GetDlgItem(IDC_BUTTON_RECHARGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_RECHARGE)->EnableWindow(FALSE);
	}

	if (strSelectCar.Compare("小车1") == 0)
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("小车1") == 0)||(strSelectCar.Compare("小车2") == 0)||(strSelectCar.Compare("小车3") == 0))
	{
		GetDlgItem(IDC_BUTTON_OUT_GARAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_IN_GARAGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OUT_GARAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IN_GARAGE)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("小车4") == 0)||(strSelectCar.Compare("小车5") == 0)
		||(strSelectCar.Compare("公交4") == 0)||(strSelectCar.Compare("公交5") == 0)
		||(strSelectCar.Compare("货车4") == 0)||(strSelectCar.Compare("货车5") == 0))
	{
		GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(FALSE);
	}
}
