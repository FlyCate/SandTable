
// SandTableDlg.h : 头文件
//

#pragma once

#include "LightCtrl.h"
#include "RechargeDlg.h"
#include "SerialSetting.h"
#include "ViolationDlg.h"
#include "PathPlanDlg.h"
#include "CameraSettingDlg.h"
#include "OpencvCamera.h"
#include "RoadPlan.h"
#include "afxwin.h"
#include "Question.h"

//交通灯颜色
enum TRAFFIC_COLOR
{
	TRAFFIC_COLOR_RED = 1,      //红
	TRAFFIC_COLOR_YELLOW = 2,   //黄
	TRAFFIC_COLOR_GREEN = 3,    //绿
	TRAFFIC_COLOR_OTHER = 99    //其它
};

//灯结构
typedef struct TagLight
{
	TRAFFIC_COLOR LightColor;//灯颜色
	TAGPOS pos;//灯位置
	bool bFlag;//是否有这个路口
	TagLight()
	{
		LightColor = TRAFFIC_COLOR_OTHER;
		bFlag = false;
	}
}TAGLIGHT;

//十字口四个方向 结构
typedef struct TagTrafficLight 
{
	TAGLIGHT Light[4];//0东 1南 2西 3北
}TAGTRAFFICLIGHT;

typedef struct  _real_pos_
{
	TAGPOS pos;
	bool flag;//0 大图 1 高架桥图
	_real_pos_()
	{
		flag =0;
	}
}REAL_POS;
// CSandTableDlg 对话框
class CSandTableDlg : public CDialogEx
{
// 构造
public:
	CSandTableDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SANDTABLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSerialSettings();


public:
	CLightCtrl * m_lightCtrlDlg;
	CRechargeDlg * m_rechargeDlg;
	CViolationDlg * m_violationDlg;
	CPathPlanDlg * m_pathplanDlg;
	CCameraSettingDlg * m_cameraSettingDlg;
	CQuestion * m_questionDlg;
	COpencvCamera m_RedCamera;
	COpencvCamera m_SpeedCamera;
	char m_RedBuff[1024];
	char m_SpeedBuff[1024];

	map<long, string> m_mapCameraList;
	bool m_bOpenSerial;
	CString m_strExePath;

public:
	afx_msg void OnBnClickedButtonLightControl();
	afx_msg void OnBnClickedButtonRecharge();
	// 创建子窗口
	void CreateChildWindow(void);

	SerialSetting *m_Serial_dlg;
	CString m_strRTCCar;
	CString m_strRTCMoney;
	CComboBox m_CComoBoxCar;
	int m_nMapLowWidth;
	int m_nMapLowHeight;
	int m_nMapHighWidth;
	int m_nMapHighHeight;
	TAGTRAFFICLIGHT m_TrafficLight[11];//交通灯 缓存
	bool m_bRedLightCapFlag;
	bool m_bOverSpeedCapFlag;

	CRoadPlan m_roadPlan;
	CSandTableDB m_db;
	bool GetCardPos(const int nCardID, TAGPOS & pos); //获取卡号原始地图坐标
	void MapLowPosTransforDrawPicPos(TAGPOS & pos);	//原始地图坐标 转换为 绘图控件坐标
	void MapHeightPosTransforDrawPicPos(TAGPOS & pos); //原始地图坐标 转换为 绘图控件坐标
	void MapPosTrans(const int nCardID, const TAGPOS & pos, REAL_POS & realPos);	//坐标转换
	const char * GetNodeidStr(const int nNodeID);
	void InsertVioData(const char * pURL, const char * pVioType);
	void TrafficLightInit();
	void SetTrafficLight(const int nCrossing, const int nLightStatus);
	void SetCarPos(const int nCarID, const int nCardID);

	afx_msg void OnBnClickedButtonLllegalInquireies();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOutGarage();
	afx_msg void OnBnClickedButtonInGarage();
	afx_msg void OnBnClickedButtonSpeeding();
	afx_msg void OnBnClickedButtonRunningRedLight();
	afx_msg void OnBnClickedButtonRoutePlan();
	afx_msg void OnBnClickedButtonCamera();
	afx_msg LRESULT OnSerialPacket(WPARAM wParam, LPARAM lParam);//串口回调消息相应
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 双缓冲绘制地图
	void DrawMapPicture(CString name);
	void DrawhighMapPicture(CString name);
	REAL_POS m_pos[15];//保存实时坐标信息
	void draw_circular(CPoint point ,CDC &memdc,COLORREF rgb);
	// 固定位置写入数字
	void WriteNumberTOpicture(CPoint point ,CDC &memdc,COLORREF rgb,CString str);
	void real_draw_map_inf(CDC &memdc);
	void real_draw_high_map_inf(CDC &memdc);
	CString m_big_pic_path;
	CString m_strRTCCar2;
	CString m_strRTCMoney2;

    CString m_high_pic_path;
	// 绘制红路灯
	void Draw_coress_light(CDC &memdc,int r);
	void draw_circular(CPoint  ,CDC &memdc,COLORREF rgb,int r);
	void DrawCarStatusPicture(void);
	void real_draw_CarStatus(CDC &memdc,COLORREF rgb ,CPoint point,int num);
	void draw_left_car_status(CDC &memdc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeComboCarList();
};
