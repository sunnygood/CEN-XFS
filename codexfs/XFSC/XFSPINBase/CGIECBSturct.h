/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   GIECBStruct.h
* Description: GIECB implement description
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef CB_IMPLEMENT_H
#define CB_IMPLEMENT_H

class CBImplement  
{
	typedef struct D1
	{
		BYTE	bType;
		BYTE	lpSerial[12];
		BYTE	bVersion;
	} Id;
	typedef struct D2
	{
		struct D1 id;
		BYTE	 em;
		USHORT usLength;
		BYTE	 lpData[256];
	} KTK_Encrypted;
	typedef struct D3
	{
		struct D1 id;
		USHORT usMLength;
		BYTE	 lpMData[256];
		USHORT usELength;
		BYTE	 lpEData[128];
	} KGp;
	typedef struct D4
	{
		struct D1 id;
		USHORT usMLength;
		BYTE	 lpMData[256];
		USHORT usELength;
		BYTE	 lpEData[128];
	} KDp;
	typedef struct D5
	{
		BYTE	 lpData1[3];
		BYTE	 lpData2[9];
	} SnSCD;
	typedef struct D6
	{
		BYTE	 lpData[8];
	} Rand;
	typedef struct D7
	{
		USHORT usLength;
		BYTE	 lpData[256];
	} Host_Authentication;
	typedef struct D8
	{
		USHORT usLength;
		BYTE	 lpData[256];
	} KDp_Signature;
	typedef struct D9
	{
		USHORT usLength;
		BYTE	 lpData[256];
	} KGp_Signature;
	typedef struct D10
	{
		USHORT usLength;
		BYTE	 lpData[256];
	} KTP_Signature;
	typedef struct D11
	{
		BYTE	 lpData[16];
	} KT_Encrypted;
	typedef struct D12
	{
		BYTE	 lpData[16];
	} Ksc_Encrypted;
	typedef struct D13
	{
		BYTE	 lpData[8];
	} PIN_Cryptogram;
	typedef struct D14
	{
		USHORT usLength;
		BYTE	 lpData[8];
	} Seal;
	typedef struct D15
	{
		BYTE	 lpData[20];
	} Thumbrint;
	typedef struct D21
	{
		BYTE	 lpData[3];
	} Manufacturer;
	typedef struct D22
	{
		BYTE	 lpData1[6];
		BYTE	 lpData2[2];
	} SCD_Type;
	typedef struct D23
	{
		BYTE	 lpData1[6];
		BYTE	 lpData2[2];
	} Firmware_Version;
};


ULONG GetLength(LPBYTE lpvData, ULONG ulDataLength, ULONG ulCurrent)
{
	BYTE bFisrt, bBytes, i;
	ULONG nLen = 0;

	if((ulCurrent+1) >= ulDataLength)
		return 0;

	bFisrt = lpvData[ulCurrent+1];
	if(bFisrt < 0x80)
		return (ULONG)bFisrt;

	bBytes = bFisrt - 0x80;
	if((ulCurrent + 1 + bBytes) >= ulDataLength)
		return 0;
	nLen = 0;
	for(i = 0; i < bBytes; ++i)
		nLen += lpvData[ulCurrent + 1 + i] * 256
}

#endif //CB_IMPLEMENT_H
