
// SandTableDlg.h : ͷ�ļ�
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

//��ͨ����ɫ
enum TRAFFIC_COLOR
{
	TRAFFIC_COLOR_RED = 1,      //��
	TRAFFIC_COLOR_YELLOW = 2,   //��
	TRAFFIC_COLOR_GREEN = 3,    //��
	TRAFFIC_COLOR_OTHER = 99    //����
};

//�ƽṹ
typedef struct TagLight
{
	TRAFFIC_COLOR LightColor;//����ɫ
	TAGPOS pos;//��λ��
	bool bFlag;//�Ƿ������·��
	TagLight()
	{
		LightColor = TRAFFIC_COLOR_OTHER;
		bFlag = false;
	}
}TAGLIGHT;

//ʮ�ֿ��ĸ����� �ṹ
typedef struct TagTrafficLight 
{
	TAGLIGHT Light[4];//0�� 1�� 2�� 3��
}TAGTRAFFICLIGHT;

typedef struct  _real_pos_
{
	TAGPOS pos;
	bool flag;//0 ��ͼ 1 �߼���ͼ
	_real_pos_()
	{
		flag =0;
	}
}REAL_POS;
// CSandTableDlg �Ի���
class CSandTableDlg : public CDialogEx
{
// ����
public:
	CSandTableDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SANDTABLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// �����Ӵ���
	void CreateChildWindow(void);

	SerialSetting *m_Serial_dlg;
	CString m_strRTCCar;
	CString m_strRTCMoney;
	CComboBox m_CComoBoxCar;
	int m_nMapLowWidth;
	int m_nMapLowHeight;
	int m_nMapHighWidth;
	int m_nMapHighHeight;
	TAGTRAFFICLIGHT m_TrafficLight[11];//��ͨ�� ����
	bool m_bRedLightCapFlag;
	bool m_bOverSpeedCapFlag;

	CRoadPlan m_roadPlan;
	CSandTableDB m_db;
	bool GetCardPos(const int nCardID, TAGPOS & pos); //��ȡ����ԭʼ��ͼ����
	void MapLowPosTransforDrawPicPos(TAGPOS & pos);	//ԭʼ��ͼ���� ת��Ϊ ��ͼ�ؼ�����
	void MapHeightPosTransforDrawPicPos(TAGPOS & pos); //ԭʼ��ͼ���� ת��Ϊ ��ͼ�ؼ�����
	void MapPosTrans(const int nCardID, const TAGPOS & pos, REAL_POS & realPos);	//����ת��
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
	afx_msg LRESULT OnSerialPacket(WPARAM wParam, LPARAM lParam);//���ڻص���Ϣ��Ӧ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ˫������Ƶ�ͼ
	void DrawMapPicture(CString name);
	void DrawhighMapPicture(CString name);
	REAL_POS m_pos[15];//����ʵʱ������Ϣ
	void draw_circular(CPoint point ,CDC &memdc,COLORREF rgb);
	// �̶�λ��д������
	void WriteNumberTOpicture(CPoint point ,CDC &memdc,COLORREF rgb,CString str);
	void real_draw_map_inf(CDC &memdc);
	void real_draw_high_map_inf(CDC &memdc);
	CString m_big_pic_path;
	CString m_strRTCCar2;
	CString m_strRTCMoney2;

    CString m_high_pic_path;
	// ���ƺ�·��
	void Draw_coress_light(CDC &memdc,int r);
	void draw_circular(CPoint  ,CDC &memdc,COLORREF rgb,int r);
	void DrawCarStatusPicture(void);
	void real_draw_CarStatus(CDC &memdc,COLORREF rgb ,CPoint point,int num);
	void draw_left_car_status(CDC &memdc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeComboCarList();
};
