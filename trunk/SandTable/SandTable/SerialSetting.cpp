// SerialSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "SandTable.h"
#include "SerialSetting.h"
#include "afxdialogex.h"

#define BUF_LEN 1024
// SerialSetting 对话框

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


// SerialSetting 消息处理程序

//串口选择改变
void SerialSetting::OnCbnSelchangeComboCom()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	// TODO: 在此添加控件通知处理程序代码
	m_com_box.GetLBText(m_com_box.GetCurSel(),str);
	if (str.GetLength()>3)
	{
		str = str.Mid(3);
		m_port = atoi(str.GetString())	;
	}
}

//波特率改变
void SerialSetting::OnCbnSelchangeComboBaudRate()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str;
	m_com_bund.GetLBText(m_com_bund.GetCurSel(),str);
	if (str.GetLength()>0)
	{
		m_bund = atoi(str.GetString())	;
	}
}


void SerialSetting::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码	
	this->ShowWindow(FALSE);

}


BOOL SerialSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton *)GetDlgItem(IDC_BUTTON3))->ShowWindow(false);
	// TODO:  在此添加额外的初始化
	InitSerialDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 初始化串口对话框
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
//开启串口线程
void SerialSetting::start_serial_thread()
{
	if (m_pThread==NULL)
	{
		m_runing = true;
		m_pThread = AfxBeginThread(Serial_hander_thread, this); //启动线程
	}
}
void SerialSetting::stop_serial_thread()
{
		m_runing = false;
		m_pThread =NULL;
}
 byte buf[BUF_LEN]={0};
//串口处理线程
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
				length = buf[1];//保留长度信息
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
						memcpy((void *)&pkt,buf,(length + 3));//拷贝完整数据包
						::SendMessage(dlg->GetParent()->GetSafeHwnd(), WM_SERIAL_PACKET, (WPARAM)0, (LPARAM)&pkt);
					//}
					//复制余下信息
					int remain = num - (length + 3);
					int index = (length + 3);
					//更新长度
					num = remain;
					length = 0;
					//复制余下字节到前边
					for (int i =0;i< remain;i++)
					{
						buf[i] = buf[index+i];
					}
					len =0;
					continue;
					//记得释放内存
				}
			}//if(num >= 4)
			//处理完成接收到的数据，未接受到完整的数据包，退出循环，继续接收
			break;

		}//end while(1)

	}//end while(m_runing)

	return 0;
}
void SerialSetting::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_com_isopen = !m_com_isopen;
	if(m_com_isopen)
	{
		if(m_serial.OpenCOM(m_port,m_bund,0)==TRUE)
		{
		
			//开启串口线程
			m_btn_open.SetWindowTextA(_T("关闭串口"));
			m_com_box.EnableWindow(FALSE);
			m_com_bund.EnableWindow(FALSE);
			start_serial_thread();
		}else
		{
			m_runing =false;
			m_com_box.EnableWindow(TRUE);
			m_com_bund.EnableWindow(TRUE);
			MessageBoxA("没有发现此串口或者被占用","错误",MB_ICONQUESTION+MB_OK);  
			m_com_isopen=FALSE;
			m_btn_open.SetWindowTextA(_T("打开串口"));
			stop_serial_thread();
		}
	}else
	{
		m_btn_open.SetWindowTextA(_T("打开串口"));
		m_com_box.EnableWindow(TRUE);
		m_com_bund.EnableWindow(TRUE);
	
		stop_serial_thread();
		m_serial.CloseCOM();
	}
}

//打包数据
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
//串口发送数据
void SerialSetting::send_cmd_data_pkt(byte cmd,byte  *payload,byte paylen,byte node)
{
	PKT pkt;
	MakeCMDPKT(&pkt,cmd ,payload, paylen, node); 
	m_serial.Write((char*)&pkt,pkt.length + 3);
	
}
//发送采集节点信息 node 节点地址 即 NODE_ADDR 中的值
void SerialSetting::SendCollectNodeCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 0; 
	send_cmd_data_pkt(COLL_CMD,pay, paylen, node);
}
//发送闯红灯控制 针对节点 CROSS_9 0x19
void SerialSetting::SendRunLedLightCMD(void)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x0a;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, CROSS_9);
}
//发送给公交站，控制路模式为自动或者手动模式 true自动模式 false手动模式
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
//发送给公交站，打开或关闭路灯 true打开路灯 false关闭路灯
void SerialSetting::SendBusStop_Roadlight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x01;//路灯
	if (mode== true)
		pay[2] = 'N';//打开
	else
		pay[2] = 'F';//关闭
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//发送给公交站，打开或关闭景观灯 true打开 false关闭 L
void SerialSetting::SendBusStop_Landscapelight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x02;//景观灯
	if (mode== true)
		pay[2] = 'N';//打开
	else
		pay[2] = 'F';//关闭
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//发送给公交站，打开或关闭建筑灯 true打开 false关闭 L
void SerialSetting::SendBusStop_Buildinglight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x03;//建筑灯
	if (mode== true)
		pay[2] = 'N';//打开
	else
		pay[2] = 'F';//关闭
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//发送给公交站，打开或关闭所有灯 true打开 false关闭 L
void SerialSetting::SendBusStop_Alllight_CtrlCMD(bool mode)
{
	byte pay[20]={0};
	int paylen = 3; 
	pay[0] = 0x31;
	pay[1] = 0x04;//所有灯
	if (mode== true)
		pay[2] = 'N';//打开
	else
		pay[2] = 'F';//关闭
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, BUS_STOP);
}
//发送小车，行驶指令 node 小车的节点地址
void SerialSetting::Send_car_Driving_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x02;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//发送小车，行驶指令 node 小车的节点地址
void SerialSetting::Send_car_Stop_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x03;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//发送小车，回库 node 小车的节点地址
void SerialSetting::Send_car_Back_warehouse_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x04;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//发送小车，出库 node 小车的节点地址
void SerialSetting::Send_car_Out_warehouse_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x05;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//发送小车，线路控制 node 节点地址
void SerialSetting::Send_car_ControlRoute_CtrlCMD(byte node,byte *route,byte routelen)
{
	byte pay[40]={0};
	int paylen = 1 + routelen; 
	pay[0] = 0x08;
	//复制路径信息
	memcpy(&pay[1] ,route ,routelen);
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}

//发送小车，超速 node 小车的节点地址
void SerialSetting::Send_car_OverSpeeding_CtrlCMD(byte node)
{
	byte pay[20]={0};
	int paylen = 1; 
	pay[0] = 0x09;
	send_cmd_data_pkt(CTRL_CMD,pay, paylen, node);
}
//发送ETC，etcnum==0 etc1  否则 etc2   carname 小车节点地址 money 充值数字
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

//测试按键
void SerialSetting::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
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
