#pragma once
#include "HwCOM.h"
#include "afxwin.h"
#define  FRAME_HEADER 0xfd
#define  CTRL_CMD     'K'
#define  COLL_CMD     'C'

typedef enum
{
	CAR_1 = 0x01,
	CAR_2 = 0x02,
	CAR_3 = 0x03,
	CAR_4 = 0x04,
	CAR_5 = 0x05,

	BUS_1 = 0x06,
	BUS_2 = 0x07,
	BUS_3 = 0x08,
	BUS_4 = 0x09,
	BUS_5 = 0x0A,

	Truck_1 = 0x0B,
	Truck_2 = 0x0C,
	Truck_3 = 0x0D,
	Truck_4 = 0x0E,
	Truck_5 = 0x0F,

	CROSS_1 = 0x11,
	CROSS_2 = 0x12,
	CROSS_3 = 0x13,
	CROSS_4 = 0x14,
	CROSS_5 = 0x15,
	CROSS_6 = 0x16,
	CROSS_7 = 0x17,
	CROSS_8 = 0x18,
	CROSS_9 = 0x19,
	CROSS_10 = 0x1A,
	CROSS_11 = 0x1B,

	ETC_1 = 0x1C,
	ETC_2 = 0x1D,
	Truck_weighing = 0x1E,
	BUS_STOP = 0x1F,
	Parking_lot = 0x20,
}NODE_ADDR;

typedef struct 
{
	byte header;
	byte length;
	byte nodeid;
	byte cmd;
	byte payload[1024];
	void Reset()
	{
		header = 0;
		length = 0;
		nodeid = 0;
		cmd = 0;
		memset(payload, 0, 1024);
	}
}PKT,*PPKT;
// SerialSetting 对话框

class SerialSetting : public CDialogEx
{
	DECLARE_DYNAMIC(SerialSetting)

public:
	SerialSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SerialSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERIAL_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 串口选择控件变量
	CComboBox m_com_box;
	// 波特率box
	CComboBox m_com_bund;
	// 打开串口按键
	CButton m_btn_open;
	// 关闭串口设置窗口按键
	CButton m_btn_clos_wind;

	CHwCOM m_serial;
	WORD m_port;
	int  m_bund;
	bool m_com_isopen;
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg void OnCbnSelchangeComboBaudRate();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	// 初始化串口对话框
	void InitSerialDlg(void);
	afx_msg void OnBnClickedButton1();
	//串口接收
	//LRESULT OnReceiveChar(WPARAM wparm, LPARAM lparm);

	LRESULT OnReceiveStr(WPARAM wparm, LPARAM lparm);
	void MakeCMDPKT(PKT *pkt,byte cmd,byte  *payload,byte paylen,byte node);
	void send_cmd_data_pkt(byte cmd,byte  *payload,byte paylen,byte node);
	//串口发送命令
	void SendCollectNodeCMD(byte node);//采集数据
	afx_msg void OnBnClickedButton3();
	void SendRunLedLightCMD(void);//闯红灯
	void SendBusStop_light_Auto_ManualMode_CMD(bool mode);
	void SendBusStop_Roadlight_CtrlCMD(bool mode);//路灯控制
	void SendBusStop_Landscapelight_CtrlCMD(bool mode);//景观灯
	void SendBusStop_Buildinglight_CtrlCMD(bool mode);//建筑灯
	void SendBusStop_Alllight_CtrlCMD(bool mode);//所有灯
	void Send_car_Driving_CtrlCMD(byte node);//小车行驶
	void Send_car_Stop_CtrlCMD(byte node);//小车停止
	void Send_car_Back_warehouse_CtrlCMD(byte node);//回库
	void Send_car_Out_warehouse_CtrlCMD(byte node);//出库
	void Send_car_OverSpeeding_CtrlCMD(byte node);//小车超速
	void Send_car_ChargeMoney_CtrlCMD(byte etcnum,byte carnum,byte money);	//发送ETC，etcnum etc节点地址  carname 小车地址 money 充值数字
	void Send_car_ControlRoute_CtrlCMD(byte node,byte *route,byte routelen);//小车行驶线路控制
	CWinThread *m_pThread; //线程指针
	bool m_runing;
	void start_serial_thread();
	void stop_serial_thread();
};
