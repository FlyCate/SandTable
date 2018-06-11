// SerialSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SandTable.h"
#include "SerialSetting.h"
#include "afxdialogex.h"

#define BUF_LEN 1024
// SerialSetting �Ի���

IMPLEMENT_DYNAMIC(SerialSetting, CDialogEx)

SerialSetting::SerialSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(SerialSetting::IDD, pParent)
	,m_port(0)
	,m_bund(115200)
	,m_com_isopen(false)
	,m_runing(false)
	,m_pThread(NULL)
{

}

SerialSetting::~SerialSetting()
{
}

void SerialSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_com_box);
	DDX_Control(pDX, IDC_COMBO_BAUD_RATE, m_com_bund);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_open);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_clos_wind);
}


BEGIN_MESSAGE_MAP(SerialSetting, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &SerialSetting::OnCbnSelchangeComboCom)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD_RATE, &SerialSetting::OnCbnSelchangeComboBaudRate)
	ON_BN_CLICKED(IDC_BUTTON2, &SerialSetting::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &SerialSetting::OnBnClickedButton1)
//	ON_MESSAGE(Wm_SerialPort_RXCHAR, &SerialSetting::OnReceiveChar)
//	ON_MESSAGE(Wm_SerialPort_RXSTR, &SerialSetting::OnReceiveStr)
	
	ON_BN_CLICKED(IDC_BUTTON3, &SerialSetting::OnBnClickedButton3)
END_MESSAGE_MAP()


// SerialSetting ��Ϣ�������

//����ѡ��ı�
void SerialSetting::OnCbnSelchangeComboCom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_com_box.GetLBText(m_com_box.GetCurSel(),str);
	if (str.GetLength()>3)
	{
		str = str.Mid(3);
		m_port = atoi(str.GetString())	;
	}
}

//�����ʸı�
void SerialSetting::OnCbnSelchangeComboBaudRate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str;
	m_com_bund.GetLBText(m_com_bund.GetCurSel(),str);
	if (str.GetLength()>0)
	{
		m_bund = atoi(str.GetString())	;
	}
}


void SerialSetting::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	this->ShowWindow(FALSE);

}


BOOL SerialSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton *)GetDlgItem(IDC_BUTTON3))->ShowWindow(false);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSerialDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// ��ʼ�����ڶԻ���
void SerialSetting::InitSerialDlg(void)
{
	m_serial.Hkey2ComboBox(m_com_box);
	CString str;
	if (m_com_box.GetCount()>0)
	{
		m_com_box.SetCurSel(0);

		m_com_box.GetLBText(m_com_box.GetCurSel(),str);
		if (str.GetLength()>3)
		{
			str = str.Mid(3);
			m_port = atoi(str.GetString())	;
		}
	}

	m_com_bund.SetCurSel(7);

	m_com_bund.GetLBText(m_com_bund.GetCurSel(),str);
	if (str.GetLength()>0)
	{
		m_bund = atoi(str.GetString())	;
	}

	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CRect mainrt,newrct;
	GetWindowRect(mainrt);
	newrct.left = nScreenWidth/2 - mainrt.Width()/2;
	newrct.right += newrct.left + mainrt.Width();
	newrct.top =  nScreenHeight/2 - mainrt.Height()/2;
	newrct.bottom =  newrct.top +  mainrt.Height();
	this->MoveWindow(newrct);

}
UINT Serial_hander_thread(LPVOID pParam);
//���������߳�
void SerialSetting::start_serial_thread()
{
	if (m_pThread==NULL)
	{
		m_runing = true;
		m_pThread = AfxBeginThread(Serial_hander_thread, this); //�����߳�
	}
}
void SerialSetting::stop_serial_thread()
{
		m_runing = false;
		m_pThread =NULL;
}
 byte buf[BUF_LEN]={0};
//���ڴ����߳�
UINT Serial_hander_thread(LPVOID pParam)
{
	SerialSetting *dlg=(SerialSetting*)pParam;
	 int num=0;
	 int len= 0;
	 int length =0;

	 while (dlg->m_runing)
	{
		len = dlg->m_serial.Read((char*)(buf+num),BUF_LEN-num);
		if (len <= 0)
		{
			Sleep(10);
			continue;
		}
		while (1)
		{
			num += len;
			if(num > BUF_LEN)
			{
				num = 0;
				length =0;
				break;
			}

			if(num >= 1)
			{
				if ( buf[0] != FRAME_HEADER)
				{
					num = 0;
					length =0;
					break;
				}
			}
			if(num >=  2)
			{
				length = buf[1];//����������Ϣ
				if ((length > 6)||(length < 1))
				{
					num = 0;
					length =0;
					break;
				}
			}
			if(num >= 4)
			{
				if ( (length + 3) <= num)
				{
					//PKT *pkt = new PKT;
					//if (pkt!=NULL)
					//{
						PKT pkt;
						memcpy((void *)&pkt,buf,(length + 3));//�����������ݰ�
						::SendMessage(dlg->GetParent()->GetSafeHwnd(), WM_SERIAL_PACKET, (WPARAM)0, (LPARAM)&pkt);
					//}
					//����������Ϣ
					int remain = num - (length + 3);
					int index = (length + 3);
					//���³���
					num = remain;
					length = 0;
					//���������ֽڵ�ǰ��
					for (int i =0;i< remain;i++)
					{
						buf[i] = buf[index+i];
					}
					len =0;
					continue;
					//�ǵ��ͷ��ڴ�
				}
			}//if(num >= 4)
			//������ɽ��յ������ݣ�δ���ܵ����������ݰ����˳�ѭ������������
			break;

		}//end while(1)

	}//end while(m_runing)

	return 0;
}
void SerialSetting::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_com_isopen = !m_com_isopen;
	if(m_com_isopen)
	{
		if(m_serial.OpenCOM(m_port,m_bund,0)==TRUE)
		{
		
			//���������߳�
			m_btn_open.SetWindowTextA(_T("�رմ���"));
			m_com_box.EnableWindow(FALSE);
			m_com_bund.EnableWindow(FALSE);
			start_serial_thread();
		}else
		{
			m_runing =false;
			m_com_box.EnableWindow(TRUE);
			m_com_bund.EnableWindow(TRUE);
			MessageBoxA("û�з��ִ˴��ڻ��߱�ռ��","����",MB_ICONQUESTION+MB_OK);  
			m_com_isopen=FALSE;
			m_btn_open.SetWindowTextA(_T("�򿪴���"));
			stop_serial_thread();
		}
	}else
	{
		m_btn_open.SetWindowTextA(_T("�򿪴���"));
		m_com_box.EnableWindow(TRUE);
		m_com_bund.EnableWindow(TRUE);
	
		stop_serial_thread();
		m_serial.CloseCOM();
	}
}

//�������
void SerialSetting::MakeCMDPKT(PKT *pkt,byte cmd,byte  *payload,byte paylen,byte node)
{
	if (pkt == NULL)
		return;
	pkt->header = FRAME_HEADER;
	pkt->length = 1 + paylen;
	pkt->nodeid = node;
	pkt->cmd    = cmd;
	memcpy(pkt->payload,payload,paylen);
}
//���ڷ�������
void SerialSetting::send_cmd_data_pkt(byte cmd,byte  *payload,byte paylen,byte node)
{
	PKT pkt;
	MakeCMDPKT(&pkt,cmd ,payload, paylen, node); 
	m_serial.Write((char*)&pkt,pkt.length + 3);
	
}
//���Ͳɼ��ڵ���Ϣ node �ڵ��ַ �� NODE_ADDR �е�ֵ
void SerialSetting::SendCollectNodeCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 0; 
	send_cmd_data_pkt(COLL_CMD,pay, paylen, node);
}
//���ʹ���ƿ��� ��Խڵ� CROSS_9 0x19
void SerialSetting::SendRunLedLightCMD(void)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x0a;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, CROSS_9);
}
//���͸�����վ������·ģʽΪ�Զ������ֶ�ģʽ true�Զ�ģʽ false�ֶ�ģʽ
void SerialSetting::SendBusStop_light_Auto_ManualMode_CMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 1; 
	if (mode== true)
		pay[0] = 0x30;
	else
		pay[0] = 0x31;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//���͸�����վ���򿪻�ر�·�� true��·�� false�ر�·��
void SerialSetting::SendBusStop_Roadlight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x01;//·��
	if (mode== true)
		pay[2] = 'N';//��
	else
		pay[2] = 'F';//�ر�
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//���͸�����վ���򿪻�رվ��۵� true�� false�ر� L
void SerialSetting::SendBusStop_Landscapelight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x02;//���۵�
	if (mode== true)
		pay[2] = 'N';//��
	else
		pay[2] = 'F';//�ر�
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//���͸�����վ���򿪻�رս����� true�� false�ر� L
void SerialSetting::SendBusStop_Buildinglight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x03;//������
	if (mode== true)
		pay[2] = 'N';//��
	else
		pay[2] = 'F';//�ر�
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//���͸�����վ���򿪻�ر����е� true�� false�ر� L
void SerialSetting::SendBusStop_Alllight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x04;//���е�
	if (mode== true)
		pay[2] = 'N';//��
	else
		pay[2] = 'F';//�ر�
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//����С������ʻָ�� node С���Ľڵ��ַ
void SerialSetting::Send_car_Driving_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x02;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//����С������ʻָ�� node С���Ľڵ��ַ
void SerialSetting::Send_car_Stop_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x03;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//����С�����ؿ� node С���Ľڵ��ַ
void SerialSetting::Send_car_Back_warehouse_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x04;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//����С�������� node С���Ľڵ��ַ
void SerialSetting::Send_car_Out_warehouse_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x05;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//����С������·���� node �ڵ��ַ
void SerialSetting::Send_car_ControlRoute_CtrlCMD(byte node,byte *route,byte routelen)
{
	byte pay[40]={0};
	int paylen = 1 + routelen; 
	pay[0] = 0x08;
	//����·����Ϣ
	memcpy(&pay[1] ,route ,routelen);
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}

//����С�������� node С���Ľڵ��ַ
void SerialSetting::Send_car_OverSpeeding_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x09;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//����ETC��etcnum==0 etc1  ���� etc2   carname С���ڵ��ַ money ��ֵ����
void SerialSetting::Send_car_ChargeMoney_CtrlCMD(byte etcnum,byte carnum,byte money)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x07;
	pay[1] = carnum;
	pay[2] = money;
	if (etcnum == 0)
		send_cmd_data_pkt(CTRL_CMD,pay, paylen, ETC_1);
	else
		send_cmd_data_pkt(CTRL_CMD,pay, paylen, ETC_2);
}

//���԰���
void SerialSetting::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//SendCollectNodeCMD(0x08);
	//SendRunLedLightCMD();
	//SendBusStop_light_Auto_ManualMode_CMD(true);
	//SendBusStop_light_Auto_ManualMode_CMD(false);
	//SendBusStop_Roadlight_CtrlCMD(true);
	//SendBusStop_Roadlight_CtrlCMD(false);
	// SendBusStop_Landscapelight_CtrlCMD(true);
	// SendBusStop_Landscapelight_CtrlCMD(false);
	//SendBusStop_Buildinglight_CtrlCMD(true);
	//SendBusStop_Buildinglight_CtrlCMD(false);
	//SendBusStop_Alllight_CtrlCMD(true);
	SendBusStop_Alllight_CtrlCMD(false);

}
