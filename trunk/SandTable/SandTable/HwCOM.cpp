// COM.cpp: implementation of the CHwCOM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HwCOM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int m_nComArrayh[20];
CHwCOM::CHwCOM ()
	: m_bDeviceGotway ( FALSE )
	, m_pCSFor_OpenCOM ( NULL )
{
	m_nComNo = INVALID_COM_NO;
	ZeroMemory(&m_CommProp,sizeof(COMMPROP));
	m_handleCOM = NULL;
}

CHwCOM::~CHwCOM()
{
	CloseCOM();
}

/********************************************************************************
* Function Type	:	Public
* Parameter		:	buf			-	读数据缓冲
*					dwSize		-	要读的字节数
* Return Value	:	>=0			-	读到的字节数
*					-1			-	串口丢失
* Description	:	从串口中读数据, 该类无法判断接在串口上的设备被移走的情况，必须
*					由调用者根据不同的通信协议来判断设备被移走的情况
*********************************************************************************/
int CHwCOM::Read(char *data, int size)
{
	if ( GetCommInputBuffLen () <= 0 ) return 0;

	int nRet = Transmit ( data, size, TRUE );
	return nRet;
}
/********************************************************************************
* Function Type	:	Public
* Parameter		:	buf			-	写数据缓冲
*					dwSize		-	要写的字节数
* Return Value	:	>=0			-	写出去的字节数
*					-1			-	串口丢失
* Description	:	往串口中写数据, 该类无法判断接在串口上的设备被移走的情况，必须
*					由调用者根据不同的通信协议来判断设备被移走的情况
*********************************************************************************/
int CHwCOM::Write(char *data, int size)
{
	if ( size < 1 ) return 0;
	return Transmit ( data, size, FALSE );
}

/********************************************************************************
* Function Type	:	Public
* Parameter		:	buf			-	写数据缓冲
*					dwSize		-	要写的字节数
* Return Value	:	>=0			-	传输成功的字节数
*					-1			-	串口丢失
* Description	:	与串口数据通信, 该类无法判断接在串口上的设备被移走的情况，必须
*					由调用者根据不同的通信协议来判断设备被移走的情况
*********************************************************************************/
int CHwCOM::Transmit(char *data, int size, BOOL bRead)
{
	if ( m_bDeviceGotway ) return 0;

	DWORD dwTransmitBytes = 0;
	m_CSFor_TransmitCOM.Lock();

	BOOL bRet = FALSE;
	for ( int i=0; i< COM_REMOVE_MAX_ERROR; i++ )
	{
		if ( bRead )
			bRet = ::ReadFile ( m_handleCOM, data, size, &dwTransmitBytes, NULL );
		else
			bRet = ::WriteFile ( m_handleCOM, data, size, &dwTransmitBytes, NULL );
		if ( bRet ) break;
		Sleep ( 1 );
	}
	m_CSFor_TransmitCOM.Unlock();

	// 串口通信成功
	if ( bRet )
	{
		return (int)dwTransmitBytes;
	}

	// 串口丢失
	m_bDeviceGotway = TRUE;
	return -1;
}

/********************************************************************************
* Function Type	:	Private
* Parameter		:	bClearRead	-	TRUE  : 终止读操作并清除读缓冲
*									FALSE : 终止写操作并清除写缓冲
* Return Value	:	None
* Description	:	清空串口所有缓冲和字符
*********************************************************************************/
void CHwCOM::ClearComm(BOOL bClearRead)
{
	m_CSFor_TransmitCOM.Lock();
	if(bClearRead)
		PurgeComm(m_handleCOM, PURGE_RXABORT | PURGE_RXCLEAR);
	else
		PurgeComm(m_handleCOM, PURGE_TXABORT | PURGE_TXCLEAR);
	m_CSFor_TransmitCOM.Unlock();
}
/********************************************************************************
* Function Type	:	Public
* Parameter		:	None
* Return Value	:	长度
* Description	:	获得串口输入缓冲区的长度
*********************************************************************************/
DWORD CHwCOM::GetCommInputBuffLen ()
{
	DWORD Errors;
	COMSTAT Stat;
	m_CSFor_TransmitCOM.Lock();
	BOOL bRet = ClearCommError(m_handleCOM,&Errors,&Stat);
	m_CSFor_TransmitCOM.Unlock();
	return Stat.cbInQue;
}
/********************************************************************************
* Function Type	:	Public
* Parameter		:	None
* Return Value	:	长度
* Description	:	获得串口输出缓冲区中还有多少字节数据未发送
*********************************************************************************/
DWORD CHwCOM::GetCommOutBufRemainBytes()
{
	DWORD Errors;
	COMSTAT Stat;
	m_CSFor_TransmitCOM.Lock();
	ClearCommError(m_handleCOM,&Errors,&Stat);
	m_CSFor_TransmitCOM.Unlock();
	return Stat.cbOutQue;
}
BOOL CHwCOM::COM_is_Open()
{
   if(m_handleCOM==NULL)
   	return FALSE;
   else
   return TRUE;


}
/********************************************************************************
* Function Type	:	Public
* Parameter		:	nComNO				-	串口号(如：2,表示COM2)
*					nBaudRate			-	波特率
* Return Value	:	None
* Description	:	初始化串口
*********************************************************************************/
BOOL CHwCOM::OpenCOM(int nComNO, int nBaudRate/*=576000*/, BOOL bQuiet/*=FALSE*/)
{
	if ( m_handleCOM && m_handleCOM != INVALID_HANDLE_VALUE )
		return TRUE;

	ASSERT ( nComNO > 0 && nComNO < 255 );
	m_nComNo = nComNO;
	TCHAR ucFileName[20];
	//打开串口
	//_snwprintf_s(ucFileName,sizeof(ucFileName), _T("\\\\.\\COM%d"),m_nComNo);
	sprintf_s(ucFileName,"\\\\.\\COM%d",m_nComNo);
	if ( m_pCSFor_OpenCOM )
		m_pCSFor_OpenCOM->Lock();
	m_handleCOM = CreateFile((LPCTSTR)ucFileName,
		GENERIC_READ | GENERIC_WRITE,				//打开类型为可读写
		0,											//以独占模式打开串口
		NULL,										//不设置安全属性
		OPEN_EXISTING,								//设备不存在时打开失败
		0,											//不需设置文件属性
		NULL);										// 不需参照模板文件
	if ( m_pCSFor_OpenCOM )
		m_pCSFor_OpenCOM->Unlock();

	if ( !m_handleCOM || m_handleCOM == INVALID_HANDLE_VALUE )
	{
		if ( !bQuiet )
		{
			TRACE ( L"Open(#2) [COM%d] failed\n",m_nComNo );
		}
		m_handleCOM = NULL;
		return FALSE;
	}

	// 设置DCB
	DCB MyDcb;
	if(GetCommState(m_handleCOM,&MyDcb))
	{
		MyDcb.BaudRate = nBaudRate;
		MyDcb.ByteSize = 8;
		MyDcb.Parity = NOPARITY;
		MyDcb.StopBits = ONESTOPBIT;
		MyDcb.fBinary=1;
		SetCommState(m_handleCOM,&MyDcb);
	}
	SetupComm(m_handleCOM,MAX_COM_BUFSIZE,MAX_COM_BUFSIZE/2);

	// 进行超时设置
	COMMTIMEOUTS CommTimeouts =
	{
		100,				// 读字符间隔超时时间: 100 ms
		2,					// 读操作时每字符的时间: 2 ms (n个字符总共为2*n ms)
		1000,				// 基本的(额外的)读超时时间: 1000 ms
		2,					// 写操作时每字符的时间: 2 ms (n个字符总共为2*n ms)
		500,				// 基本的(额外的)写超时时间: 500 ms
	};
	if ( !SetCommTimeouts(m_handleCOM, &CommTimeouts) )
	{
		TRACE ( L"Set [COM%d] timeout parameter failed\n", nComNO );
		CloseCOM ();
		return FALSE;
	}

	if ( !EscapeCommFunction ( m_handleCOM,SETDTR ) )
	{
		TRACE ( L"[COM%d] EscapeCommFunction failed\n", nComNO );
	}

	BOOL bRet = GetCommProperties ( m_handleCOM, &m_CommProp );
	if( !bRet || !(m_CommProp.dwProvCapabilities&PCF_INTTIMEOUTS) )
	{
		TRACE ( L"[COM%d] not supported capabilities\n", nComNO );
	}

	ClearCommBufByRead();

	m_bDeviceGotway = FALSE;
	if ( !bQuiet )
	{
		TRACE ( L"Open(#2) [COM%d] successfully\n", nComNO );
	}
	return TRUE;
}

void CHwCOM::CloseCOM()
{
	if ( m_handleCOM && m_handleCOM != INVALID_HANDLE_VALUE )
	{
		CloseHandle ( m_handleCOM );
		m_handleCOM = NULL;
	}
}
/********************************************************************************
* Function Type	:	Private
* Parameter		:	None
* Return Value	:	None
* Description	:	通过读操作来清空串口所有缓冲和字符
*********************************************************************************/
void CHwCOM::ClearCommBufByRead()
{
	ClearComm ( TRUE ); ClearComm ( FALSE );
	char buf[1024] = {0};
	int nReadyBytes = 0;
	while ( ( nReadyBytes = (int)GetCommInputBuffLen() ) > 0 )
	{
		Read ( buf, nReadyBytes );
	}
}
/********************************************************************************
* Function Type	:	Private
* Parameter		:	iBytes		-	[in] 等待数据字节数
*					timeout		-	[in] 等待数据的最大时间(毫秒)
* Return Value	:	TRUE		-	等到数据
*					FALSE		-	没等到
* Description	:	等到指定字节数据可读
*********************************************************************************/
BOOL CHwCOM::WaitForDataToRead(int iBytes,int timeout/* = WAIT_DATA_TIMEOUT*/)
{
	DWORD start_time = GetTickCount();
	while ( (DWORD)( GetTickCount() - start_time ) < (DWORD)timeout)
	{
		if( GetCommInputBuffLen() < (DWORD)iBytes)	//没有接收到iBytes个数据
			Sleep ( 100 );
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CHwCOM::SetCommMask(DWORD dwEvtMask)
{
	if ( m_handleCOM && m_handleCOM != INVALID_HANDLE_VALUE )
	{
		return ::SetCommMask ( m_handleCOM, dwEvtMask );
	}
	return FALSE;
}

BOOL CHwCOM::WaitCommEvent(LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped)
{
	if ( m_handleCOM && m_handleCOM != INVALID_HANDLE_VALUE )
	{
		return ::WaitCommEvent ( m_handleCOM, lpEvtMask, lpOverlapped );
	}
	return FALSE;
}
void CHwCOM::QueryKey(HKEY hKey) 
{ 
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
	//	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	//	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	for (i=0;i<20;i++)///存放串口号的数组初始化
	{
		m_nComArrayh[i] = -1;
	}

	// Enumerate the key values. 
	if (cValues > 0) {
		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) { 
			cchValue = MAX_VALUE_NAME;  achValue[0] = '\0'; 
			if (ERROR_SUCCESS == RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL))  { 
				CString szName(achValue);
				//if (-1 != szName.Find(_T("Serial")) || -1 != szName.Find(_T("VCom")) ){
					BYTE strDSName[10]; memset(strDSName, 0, 10);
					DWORD nValueType = 0, nBuffLen = 10;
					if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPCTSTR)achValue, NULL, &nValueType, strDSName, &nBuffLen)){
						int nIndex = -1;
						while(++nIndex < 200){
							if (-1 == m_nComArrayh[nIndex]) {
								m_nComArrayh[nIndex] = atoi((char*)(strDSName + 3));
								break;
							}
						}
					}
				//}
			} 
		}
	}
	else{
		AfxMessageBox(_T("本机没有串口....."));
	}

}

void CHwCOM::Hkey2ComboBox(CComboBox& m_PortNO)
{
	HKEY hTestKey;
	bool Flag = FALSE;

	///仅是XP系统的注册表位置，其他系统根据实际情况做修改
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey) ){
		QueryKey(hTestKey);
	}
	RegCloseKey(hTestKey);

	int i = 0;
	m_PortNO.ResetContent();///刷新时，清空下拉列表内容
	while(i < 200 && -1 != m_nComArrayh[i]){
		CString szCom; 
		szCom.Format(_T("COM%d"), m_nComArrayh[i]);
		m_PortNO.InsertString(i, szCom.GetBuffer(5));
		++i;
		Flag = TRUE;
		if (Flag)///把第一个发现的串口设为下拉列表的默认值
			m_PortNO.SetCurSel(0);
	}

}