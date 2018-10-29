/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IBasePort.h
* Description: the standard Communicate interface, 
*              inlcude Serial, USB, HID, LPT, TCPIP, and so on        
*
* Author:      liuguoxun
* Date:        2012-04-05
*
*****************************************************************************/


#ifndef  _IPORT_H_20120405
#define  _IPORT_H_20120405


#include "IBaseSystem.h"

#define  PORT_MODULE_VER          PLATFORM_VER


#define  TIME_OUT          3000                  //default communication timeout
#define  MAX_PACKET_LEN    (MAX_DATA_LEN + 32)   //max packet length
#define  MAX_DATA_LEN      (8 * 1024 + 128)      //max data length, not include head and tail
#define  MAX_ACK_LEN       16                    //max ack length


//callback function pointer which is used to parse packet's total length, see SetReturnFormat function
typedef long (CALLBACK *TOTALLENPROC)(LPCSTR lpcHead); 


//exchange type
typedef enum {
	 eEXCHANGE_NONE          =     0x0L //no this item(free mode, all control by user)

	,eEXCHANGE_FORMAT        =    0x10L //format command without ack
	,eEXCHANGE_VARIABLE      =    0x20L //variable command without ack
	,eEXCHANGE_FLOW          =    0x40L //control by flow without ack

	,eEXCHANGE_FORMAT_ACK    =   0x100L
	,eEXCHANGE_VARIABLE_ACK  =   0x200L
	,eEXCHANGE_FLOW_ACK      =   0x400L
} EExchange;


// Handshaking
typedef enum
{
	 eHandshakeOff			 =  0	// No handshaking
	,eHandshakeHardware		 =  1	// Hardware handshaking (RTS/CTS)
	,eHandshakeSoftware		 =  2	// Software handshaking (XON/XOFF)
	,eHandshakeRTSDTR  		 =  3	// Only use RTS and DTR (default)
}EHandshake;


// Escape Functions
typedef enum
{
	 eSETXOFF                =  1	// Simulate XOFF received
	,eSETXON                 =  2	// Simulate XON received
	,eSETRTS                 =  3	// Set RTS high
	,eCLRRTS                 =  4	// Set RTS low
	,eSETDTR                 =  5	// Set DTR high
	,eCLRDTR                 =  6	// Set DTR low
	,eRESETDEV               =  7	// Reset device if possible
	,eSETBREAK               =  8	// Set the device break line
	,eCLRBREAK               =  9	// Clear the device break line
}EESCAPE;


// modem pin
typedef enum
{
	 ePIN_CTS                =  0	// CTS
	,ePIN_DSR                =  1	// DSR
	,ePIN_RING               =  2	// RING
	,ePIN_RLSD               =  3	// RLSD
}EPIN;


//   interface in/out parameter£º
//   1¡¢hexadecimal(HEX 0x00 ~ 0xFF)
//   2¡¢viewable character string(be not split character)

INTERFACE_I  IPort
{
public:
	virtual long Open(LPCTSTR lpDesc) = 0; // lpDesc's sample list at EPORT 
	virtual long Close(void)  = 0;
	virtual long Purge(void)  = 0;
	virtual long Cancel(void) = 0; //cancel the port operate
	virtual long ReOpen(DWORD dwAfterTime = 0) = 0;
	virtual bool IsOpen(void) const = 0;
	virtual LPCTSTR GetDesc(BOOL bAll = FALSE) const = 0; //such as TRUE="COM1:9600,N,8,1",  FALSE="COM1"

	virtual long SendData(LPCSTR pData, DWORD dwDataLen, DWORD dwTimeout = TIME_OUT) = 0;
	virtual long ReceiveData(char *pBuffer, DWORD dwWaitLen, LPDWORD pdwRead, DWORD dwTimeout = TIME_OUT) = 0;	

	

	//Below is special function 

	//Before call SendReceive at least had called SetReturnFormat or SetVariable once
	//If with ACK, at least call SetAckFormat once
	//send and receive data
	virtual long SendReceive(LPCSTR pData, DWORD dwDataLen, char *pReturn, LPDWORD pdwReadLen, DWORD dwTimeout = TIME_OUT, long iMaxCnt = 1) = 0; 


	//only for eEXCHANGE_FORMAT | eEXCHANGE_FORMAT_ACK	
	//Set receive data's format, if call once it will hold to next call this, so format command only need call once.
	//    iFirstReadLen : first read length(const length or const data include length segment); 
	//    dwCmdFieldPos : command field position which must is same between send and receive format, otherwise don't check this field.
	//                    if it's 0, don't check this field, 
	//                    if it's > 0, check this field. 
	//    pFunc : callback function pointer, for parse packet's total length
	//e.g. 
	//    SetReturnFormat(N, 0): receive const length N once
	//    SetReturnFormat(N, 3): receive const length N once, and the command field is 3.
	//e.g.
	//    EPP(Hxx) : SetReturnFormat(6, 3, CPinpadBase::ParseRetLen);
	//    CPinpadBase Send: 02 00 15 0300(len) 15 3B30033893
	//    CPinpadBase Recv: 02 00 15 0100(len) 00     ...... first receive 6 char, then parse total length by call CPinpadBase::ParseRetLen,
	//               Recv2: 03 62 3D                  ...... second receive left length, 
	//                                                ...... third check command field(3) 15 whether is same between send and receive.
	virtual void SetReturnFormat(long iFirstReadLen, DWORD dwCmdFieldPos = 0, TOTALLENPROC pFunc = 0) = 0; 


	//only for eEXCHANGE_VARIABLE | eEXCHANGE_VARIABLE_ACK
	//Set begin and end marking of command protocol, dwCmdFieldPos see SetReturnFormat.
	//e.g. SetVariable("\x02", "\x03", 0): 0x02 ... 0x03
	//e.g. SetVariable("\x02", "\x03", 1): 0x02 ... 0x03 BCC
	//e.g. SetVariable("\x02", "\x03", 2): 0x02 ... 0x03 CRC
	virtual void SetVariable(LPCSTR pBegin, LPCSTR pEnd, BYTE ExtraLen = 0, DWORD dwCmdFieldPos = 0) = 0;


	//only for eEXCHANGE_FORMAT_ACK | eEXCHANGE_VARIABLE_ACK | eEXCHANGE_FLOW_ACK
	virtual long SetACK(LPCSTR pACK, WORD wACK, LPCSTR pNAK = 0, LPCSTR pEOT = 0, WORD wEOT = 0, LPCSTR pENQ = 0, WORD wENQ = 0, BOOL bEndSendACK = 1, DWORD dwTimeout = 200) = 0; 




	//Only Serial(eCOM) is effect, other type return success immediately
	virtual BOOL EscapeComm(EESCAPE eFunc) = 0; //Escape Functions
	virtual bool GetPinFlag(EPIN eFlag) = 0; //CTS DSR RING RLSD
	virtual DWORD GetBaudrate(void) const = 0;
	virtual long SetBaudrate(DWORD dwBaudrate) = 0;	
	virtual long SetHandshaking(EHandshake eShake) = 0;
	virtual long SetIntervalRead(DWORD dwIntervalTimeout) = 0;
};




//[IN] ePort:  see EPORT define,
//             when EPORT is eCOMBINE, you must input right port description to find the master device which is combined, 
//             otherwise ignore lpDesc
//[IN] eType:  see EExchange define
//[IN] lpDesc: such as "COM1:9600,N,8,1", "VID_23AB&PID_XXXX" ......
extern "C" IPort* WINAPI InstanceIPort(EPORT ePort, EExchange eType = eEXCHANGE_NONE, LPCTSTR lpDesc = 0);


extern "C" void WINAPI ReleaseIPort(IPort *p);


//[IN]  ePort:  see EPORT define
//[OUT] pPortList: return string split by '|', such as "COM1|COM6|COM9|"
extern "C" void WINAPI EnumPortList(EPORT ePort, LPTSTR pPortList);


typedef IPort* (WINAPI * IPORT_INSFUNC)(EPORT, EExchange, LPCTSTR);
typedef void (WINAPI * IPORT_DELFUNC)(IPort*);
typedef void (WINAPI * ENUM_PORTLIST)(EPORT, LPTSTR);


#endif //_IPORT_H_20120405

