
// SandTableDlg.cpp : ʵ���ļ�
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
	MessageBox("��򿪴���");\
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

int g_RedLightCardID = 194;		//����ƿ���
int g_OverSpeedCardID = 153;	//���ٿ���

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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSandTableDlg �Ի���

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


// CSandTableDlg ��Ϣ�������

BOOL CSandTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CString str = "VioPic";  
	if (!PathIsDirectory(str))  
	{  
		::CreateDirectory(str, NULL);//����Ŀ¼,���еĻ���Ӱ��  
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

	m_CComoBoxCar.InsertString(0, "С��1");
	m_CComoBoxCar.InsertString(1, "С��2");
	m_CComoBoxCar.InsertString(2, "С��3");
	m_CComoBoxCar.InsertString(3, "С��4");
	m_CComoBoxCar.InsertString(4, "С��5");
	m_CComoBoxCar.InsertString(5, "����1");
	m_CComoBoxCar.InsertString(6, "����2");
	m_CComoBoxCar.InsertString(7, "����3");
	m_CComoBoxCar.InsertString(8, "����4");
	m_CComoBoxCar.InsertString(9, "����5");
	m_CComoBoxCar.InsertString(10, "����1");
	m_CComoBoxCar.InsertString(11, "����2");
	m_CComoBoxCar.InsertString(12, "����3");
	m_CComoBoxCar.InsertString(13, "����4");
	m_CComoBoxCar.InsertString(14, "����5");
	m_CComoBoxCar.SetCurSel(0);

	//��ȡϵͳ�������߶�
	int taskbar_height = 10;
	CWnd * hWnd = FindWindow("Shell_TrayWnd", 0);
	RECT rect;
	::GetWindowRect(hWnd->GetSafeHwnd(), &rect);
	taskbar_height = rect.bottom - rect.top;

	//����������λ�úʹ�С
	MoveWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - taskbar_height);

	//���õ�ͼͼƬ
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_LOW); 
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//��ʾͼƬ
	CString strPicPath = m_strExePath;
	strPicPath += "res/MapLow.bmp";
	m_big_pic_path =strPicPath;
	::GetWindowRect(GetDlgItem(IDC_STATIC_MAP_LOW)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //��Դ�Ż򱾵��ļ���
		IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
		rect.right - rect.left,                  //��� 0ΪĬ�ϴ�С
		rect.bottom - rect.top,                  //�߶� ����Ϊ��λ
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nMapLowWidth = rect.right - rect.left;
	m_nMapLowHeight = rect.bottom - rect.top;

	//���õ�ͼͼƬ
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_HIGH); 
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	pWnd->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
	//��ʾͼƬ
	strPicPath = m_strExePath;
	strPicPath += "res/MapHigh.bmp";
	m_high_pic_path = strPicPath;
	::GetWindowRect(GetDlgItem(IDC_STATIC_MAP_HIGH)->GetSafeHwnd(), &rect);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		strPicPath,  //��Դ�Ż򱾵��ļ���
		IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
		rect.right - rect.left,                  //��� 0ΪĬ�ϴ�С
		rect.bottom - rect.top,                  //�߶� ����Ϊ��λ
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	m_nMapHighWidth = rect.right - rect.left;
	m_nMapHighHeight = rect.bottom - rect.top;

	//��ʼ������ͷ
	bool bRet = m_db.OpenDB(SANDTABLE_CAMERA);
	if (bRet == true)
	{
		list<tagCamera> listCamera;
		bRet = m_db.ReadDB(listCamera);
		if (bRet == true)
		{
			for (auto it = listCamera.begin(); it != listCamera.end(); it++)
			{
				if (it->strCameraType.compare("�����") == 0)
				{
					m_RedCamera.SetCameraID(it->nCameraID);
				}
				else if (it->strCameraType.compare("����") == 0)
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
		MessageBox("�򿪴��������ͷʧ�ܣ���������������ͷ��");
	}

	lRet = m_SpeedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("�򿪳�������ͷʧ�ܣ����鲢������������ͷ��");
	}

	SetTimer(0, 100, NULL);
	TrafficLightInit();
    SetTimer(159, 1000, NULL);//����ˢ��ͼƬ
	DrawCarStatusPicture();
	
	GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(FALSE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSandTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		DrawCarStatusPicture();
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSandTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��������
void CSandTableDlg::OnBnClickedButtonSerialSettings()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Serial_dlg->ShowWindow(TRUE);
}

//·�ƿ���
void CSandTableDlg::OnBnClickedButtonLightControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_lightCtrlDlg == NULL)
	{
		m_lightCtrlDlg = new CLightCtrl(this);
	}

	m_lightCtrlDlg->DoModal();
}

//��ֵ
void CSandTableDlg::OnBnClickedButtonRecharge()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		if ((strSelectCar.Compare("С��1") == 0)||(strSelectCar.Compare("����1") == 0))
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


// �����Ӵ���
void CSandTableDlg::CreateChildWindow(void)
{
	m_Serial_dlg = new SerialSetting(this);
	m_Serial_dlg->Create(IDD_DIALOG_SERIAL_CONTROL, this);
	m_Serial_dlg->ShowWindow(false);

	m_questionDlg = new CQuestion(this);
	m_questionDlg->Create(IDD_DIALOG_OK, this);
	m_questionDlg->ShowWindow(FALSE);
}

//��ȡ���ŵ�  ԭʼ��ͼ����
bool CSandTableDlg::GetCardPos(const int nCardID, TAGPOS & pos)
{
	long lRet = m_roadPlan.GetCardPos(nCardID, pos);
	if (lRet != 0)
	{
		TRACE("��������ʧ��!\n");
		return false;
	}

	return true;
}

//ԭʼ��ͼ���� ת��Ϊ ��ͼ�ؼ�����
void CSandTableDlg::MapLowPosTransforDrawPicPos(TAGPOS & pos)
{
	pos.x = pos.x * m_nMapLowWidth / MAP_LOW_WIDTH;
	pos.y = pos.y * m_nMapLowHeight / MAP_LOW_HEIGHT;
	pos.y = m_nMapLowHeight - pos.y;
}

//ԭʼ��ͼ���� ת��Ϊ ��ͼ�ؼ�����
void CSandTableDlg::MapHeightPosTransforDrawPicPos(TAGPOS & pos)
{
	pos.x = pos.x * m_nMapHighWidth / MAP_HIGH_WIDTH;
	pos.y = pos.y * m_nMapHighHeight / MAP_HIGH_HEIGHT;
	pos.y = m_nMapLowHeight - pos.y;
}

//����ת��
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

//Υ�²�ѯ
void CSandTableDlg::OnBnClickedButtonLllegalInquireies()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_violationDlg == NULL)
	{
		m_violationDlg = new CViolationDlg(this);
	}

	m_violationDlg->DoModal();
}

//��ʻ
void CSandTableDlg::OnBnClickedButtonRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Driving_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//ֹͣ
void CSandTableDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Stop_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//����
void CSandTableDlg::OnBnClickedButtonOutGarage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Out_warehouse_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//���
void CSandTableDlg::OnBnClickedButtonInGarage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_Back_warehouse_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	SetTimer(0, 100, NULL);
}

//����
void CSandTableDlg::OnBnClickedButtonSpeeding()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->Send_car_OverSpeeding_CtrlCMD(m_CComoBoxCar.GetCurSel()+1);
	m_bOverSpeedCapFlag = true;
	SetTimer(0, 100, NULL);
}

//�����
void CSandTableDlg::OnBnClickedButtonRunningRedLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SERIAL_OPEN();
	KillTimer(0);
	Sleep(5);
	m_Serial_dlg->SendRunLedLightCMD();
	m_bRedLightCapFlag = true;
	SetTimer(0, 100, NULL);
}

//·���滮
void CSandTableDlg::OnBnClickedButtonRoutePlan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//ץ������
void CSandTableDlg::OnBnClickedButtonCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
		MessageBox("�򿪴��������ͷʧ�ܣ���������������ͷ��");
	}

	lRet = m_SpeedCamera.OpenCamera();
	if (lRet != OPENCV_CAMERA_SUCCESS)
	{
		MessageBox("�򿪳�������ͷʧ�ܣ����鲢������������ͷ��");
	}
}

//���ڻص���Ϣ��Ӧ
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
			case CTRL_CMD: //���Ʒ���
				{
					strMsg = GetNodeidStr(pkt->nodeid);

					if (strMsg.GetLength() != 0)
					{
						if (pkt->payload[0] == 'O' && pkt->payload[1] == 'K')
						{
							strMsg += " ��������ͳɹ���";
						}
						else
						{
							strMsg += " �ڵ�û��������";
						}

						m_questionDlg->SetQuestion(strMsg.GetBuffer());
						m_questionDlg->ShowWindow(TRUE);
					}
				}
				break;
			case COLL_CMD: //�ɼ�
				{
					switch (pkt->payload[0])
					{
					case 0x55://���̵�
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
					case 0x06://С��λ����Ϣ
						{
							switch (pkt->nodeid)
							{
							case CAR_1:
								{
									//����
									if (m_bOverSpeedCapFlag == true)
									{
										m_bOverSpeedCapFlag = false;
										if (pkt->payload[1] == g_OverSpeedCardID) 
										{
											//����
											memset(m_SpeedBuff, 0, 1024);
											SYSTEMTIME t;
											GetLocalTime(&t);
											sprintf_s(m_SpeedBuff, "VioPic/Speed%d%d%d%d%d%d_%d.jpg", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
											long lRet = m_SpeedCamera.SnapshotCamera(m_SpeedBuff);
											if (OPENCV_CAMERA_SUCCESS == lRet)
											{
												InsertVioData(m_SpeedBuff, "����");
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
									//����� ����Ϊ194
									if (pkt->payload[1] == g_RedLightCardID)
									{
										if (m_bRedLightCapFlag == true) 
										{
											m_bRedLightCapFlag = false;
											//�����
											memset(m_RedBuff, 0, 1024);
											SYSTEMTIME t;
											GetLocalTime(&t);
											sprintf_s(m_RedBuff, "VioPic/Red%d%d%d%d%d%d_%d.jpg", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
											long lRet = m_RedCamera.SnapshotCamera(m_RedBuff);
											if (OPENCV_CAMERA_SUCCESS == lRet)
											{
												InsertVioData(m_RedBuff, "�����");
											}
										}
										break;
									}

									//��ʾС��λ��
									SetCarPos(pkt->nodeid - CAR_1, pkt->payload[1]);
								}
								break;
							default:
								break;
							}
						}
						break;
					case 0x07://ETC ���㱨
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
					case 0x0b://С������
						{
							if (pkt->payload[4] == 1)
							{
								CString strErrMsg;
								strErrMsg.Format("���س�����%s ������%d", GetNodeidStr(pkt->payload[1]), itoaa(pkt->payload[2]*256 + pkt->payload[3]));
								InsertVioData(strErrMsg.GetBuffer(), "����");
							}
						}
						break;
					case 'n'://�ڵ�����������
						{

							strMsg = GetNodeidStr(pkt->nodeid);
							strMsg += " �ڵ�û����";
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
	case CAR_1:	strMsg = "С��1";
		break;
	case CAR_2:	strMsg = "С��2";
		break;
	case CAR_3:	strMsg = "С��3";
		break;
	case CAR_4:	strMsg = "С��4";
		break;
	case CAR_5:	strMsg = "С��5";
		break;
	case BUS_1:	strMsg = "����1";
		break;
	case BUS_2:	strMsg = "����2";
		break;
	case BUS_3:	strMsg = "����3";
		break;
	case BUS_4:	strMsg = "����4";
		break;
	case BUS_5:	strMsg = "����5";
		break;
	case Truck_1:	strMsg = "����1";
		break;
	case Truck_2:	strMsg = "����2";
		break;
	case Truck_3:	strMsg = "����3";
		break;
	case Truck_4:	strMsg = "����4";
		break;
	case Truck_5:	strMsg = "����5";
		break;
	case CROSS_1 :	strMsg = "·��1";
		break;
	case CROSS_2 :	strMsg = "·��2";
		break;
	case CROSS_3 :	strMsg = "·��3";
		break;
	case CROSS_4 :	strMsg = "·��4";
		break;
	case CROSS_5 :	strMsg = "·��5";
		break;
	case CROSS_6 :	strMsg = "·��6";
		break;
	case CROSS_7 :	strMsg = "·��7";
		break;
	case CROSS_8 :	strMsg = "·��8";
		break;
	case CROSS_9 :	strMsg = "·��9";
		break;
	case CROSS_10:	strMsg = "·��10";
		break;
	case CROSS_11:	strMsg = "·��11";
		break;
	case ETC_1:	strMsg = "ETC1";
		break;
	case ETC_2:	strMsg = "ETC2";
		break;
	case Truck_weighing:	strMsg = "��������վ";
		break;
	case BUS_STOP:	strMsg = "1�Ź���վ";
		break;
	case Parking_lot:	strMsg = "ͣ������բ";
		break;
	default:
		strMsg = "";
		break;
	}

	return strMsg.GetBuffer();
}


void CSandTableDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
		TRACE("��Υ��DB�ļ�ʧ��!\n");
		return;
	}

	tagViolation tViolation;
	tViolation.strURL = pURL;
	tViolation.strVioType = pVioType;
	bRet = m_db.WriteDB(tViolation);
	if (bRet == true)
	{
		//MessageBox("д��Υ���ɹ���");
		TRACE("д��Υ���ɹ���\n");
	}
	else
	{
		//MessageBox("д��Υ��ʧ�ܣ�");
		TRACE("д��Υ��ʧ�ܣ�\n");
	}

	m_db.CloseDB();
}


// ˫������Ƶ�ͼ
void CSandTableDlg::DrawMapPicture(CString name)
{
	CString dstname;
	if (name== "")return;

	dstname =  name;
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		dstname, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if (!hbmp)return;
	CBitmap bitmap;
	bitmap.Attach(hbmp); //����λͼ����  
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


	pWnd->GetWindowRect(&dstrc);//��ȡ�ؼ�����Ļ����
	ScreenToClient(&dstrc);//ת��Ϊ�Ի����ϵĿͻ�����

	pDC->StretchBlt(dstrc.left,dstrc.top,dstrc.Width(),dstrc.Height(),&memdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	bitmap.DeleteObject();
	ReleaseDC(pDC);
	memdc.DeleteDC();
}
// �߼�˫������Ƶ�ͼ
void CSandTableDlg::DrawhighMapPicture(CString name)
{
	CString dstname;
	if (name== "")return;

	dstname =  name;
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		dstname, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if (!hbmp)return;
	CBitmap bitmap;
	bitmap.Attach(hbmp); //����λͼ����  
	CDC *pDC = GetDC();
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(&bitmap);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	CRect dstrc;
	real_draw_high_map_inf(memdc);
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_HIGH); 
	pWnd->GetWindowRect(&dstrc);//��ȡ�ؼ�����Ļ����
	ScreenToClient(&dstrc);//ת��Ϊ�Ի����ϵĿͻ�����
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

// �̶�λ��д������
void CSandTableDlg::WriteNumberTOpicture(CPoint point ,CDC &memdc,COLORREF rgb,CString str)
{
	CFont font, *pOldFont= NULL;//�������������ָ�� ������Ը�������ѡ����Ҫ���������ͺʹ�С
	// font.CreatePointFont(120, "����", &dc);   
	font.CreatePointFont(180, "system", &memdc);

	pOldFont = memdc.SelectObject(&font);   

	memdc.SetBkMode(TRANSPARENT);//͸��   
	memdc.SetTextColor(RGB(255,255,255)); 
	memdc.TextOut(point.x-8, point.y-15,str, str.GetLength());

	memdc.SelectObject(pOldFont);

	if(pOldFont)//�ͷ���Դ�����ڴ�й¶
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
		if (  m_pos[i].flag == false )//��ͼ
		{   //X����С��0����y����С��0 ����Ϊ����Ч�����ݣ������Ѿ���ʱ��������
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
		if (  m_pos[i].flag == true )//�߼ۻ�ͼ
		{
			//X����С��0����y����С��0 ����Ϊ����Ч�����ݣ������Ѿ���ʱ��������
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
	//��������·��
	for (int i = 0; i < 11; i++)
	{
		//�����ĸ�����
		for (int j = 0; j < 4; j++)
		{
			//���x���겻�� -1  ˵�����������ں��̵�
			if (TrafficPos[i][j][0] != -1)
			{
				//��־λ��λ
				m_TrafficLight[i].Light[j].bFlag = true;
				//��������
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
	case 1://������ƣ��ϱ��̵�
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_GREEN;//��
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_GREEN;//��
		}
		break;
	case 2://������ƣ��ϱ��Ƶ�
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_YELLOW;//��
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_YELLOW;//��
		}
		break;
	case 3://�����̵ƣ��ϱ����
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_GREEN;//��
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_GREEN;//��
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_RED;//��
		}
		break;
	case 4://�����Ƶƣ��ϱ����
		{
			m_TrafficLight[nCrossing].Light[0].LightColor = TRAFFIC_COLOR_YELLOW;//��
			m_TrafficLight[nCrossing].Light[2].LightColor = TRAFFIC_COLOR_YELLOW;//��
			m_TrafficLight[nCrossing].Light[1].LightColor = TRAFFIC_COLOR_RED;//��
			m_TrafficLight[nCrossing].Light[3].LightColor = TRAFFIC_COLOR_RED;//��
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


// ���ƺ�·��
void CSandTableDlg::Draw_coress_light(CDC &memdc,int r)
{
	int i=0,j=0;
	for (i=0;i<11;i++)
	{
		//ȷ���д�·��
		for (j=0;j<4;j++)
		{
			//ȷ���д�·��
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
// �������״̬ͼ
void CSandTableDlg::DrawCarStatusPicture(void)
{

	CBitmap bitmap,*OldBitmap;
	CRect rect;
	CDC memdc ;
	CDC *pDC = GetDC();

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_ST_CAR_PIC); 
	pWnd->GetWindowRect(&rect);//��ȡ�ؼ�����Ļ����
	rect.left  += 50; 
	rect.right -= 10;
	rect.top   += 20;
	rect.bottom  -= 10; 
	ScreenToClient(&rect);//ת��Ϊ�Ի����ϵĿͻ�����
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

		//if (  m_pos[i+index].flag == false )//��ͼ
		//{   //X����С��0����y����С��0 ����Ϊ����Ч�����ݣ������Ѿ���ʱ��������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawCarStatusPicture();
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CSandTableDlg::OnCbnSelchangeComboCarList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSelectCar;
	m_CComoBoxCar.GetWindowText(strSelectCar);

	if (strSelectCar.Compare("С��1") == 0)
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("С��1") == 0)||(strSelectCar.Compare("С��2") == 0)||(strSelectCar.Compare("����1") == 0)||(strSelectCar.Compare("����2") == 0))
	{
		GetDlgItem(IDC_BUTTON_RECHARGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_RECHARGE)->EnableWindow(FALSE);
	}

	if (strSelectCar.Compare("С��1") == 0)
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SPEEDING)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("С��1") == 0)||(strSelectCar.Compare("С��2") == 0)||(strSelectCar.Compare("С��3") == 0))
	{
		GetDlgItem(IDC_BUTTON_OUT_GARAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_IN_GARAGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OUT_GARAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IN_GARAGE)->EnableWindow(FALSE);
	}

	if ((strSelectCar.Compare("С��4") == 0)||(strSelectCar.Compare("С��5") == 0)
		||(strSelectCar.Compare("����4") == 0)||(strSelectCar.Compare("����5") == 0)
		||(strSelectCar.Compare("����4") == 0)||(strSelectCar.Compare("����5") == 0))
	{
		GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ROUTE_PLAN)->EnableWindow(FALSE);
	}
}
