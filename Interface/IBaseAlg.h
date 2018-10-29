/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IBaseAlg.h
* Description: Algorithm 
*
* Author:      liuguoxun
* Date:        2015-04-12
*
*****************************************************************************/


#ifndef _IALG_H_20150412
#define _IALG_H_20150412


#include "IBaseSystem.h"


#define  ALG_MODULE_VER          PLATFORM_VER
  

#ifdef __cplusplus
extern "C" {
#endif


long  WINAPI Soft_Hash(   EHASH eHashAlg, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);
long  WINAPI Soft_Crypt(    ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut, bool bEncrypt = true);
long  WINAPI Soft_MAC(         EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
long  WINAPI Soft_GenSig(   ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
long  WINAPI Soft_VerifySig(ECRYPT eMode, LPCSTR lpDataIn, DWORD dwInLen,     LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen);

DWORD WINAPI Soft_CalcSM3(LPCSTR lpData, DWORD dwDataLen, char *lpHash);
long  WINAPI Soft_CalcZ(LPCSTR lpPK, WORD wPKLen, LPCSTR lpID, WORD &wInOutLen, char *pZ, WORD wIDType = 0);
long  WINAPI Soft_SM2ExchangeA(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		            LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKa, WORD wSKaLen, LPCSTR RandomSKa, WORD wRandomSKaLen, 
					char *Sa, WORD &wSaLen, char *S1, WORD &wS1Len, char *TargetKey, WORD wKeyLen = 16);
long  WINAPI Soft_SM2ExchangeB(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		            LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKb, WORD wSKbLen, LPCSTR RandomSKb, WORD wRandomSKbLen, 
					char *Sb, WORD &wSbLen, char *S2, WORD &wS2Len, char *TargetKey, WORD wKeyLen = 16);


long  WINAPI RSA_GenKeyPair(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen, int nBits = 2048, UINT nExponentValue = 65537);
long  WINAPI RSA_ConvertSK(LPCSTR pSKDer, WORD &wInOutLen, char *pCvt);
long  WINAPI SM2_GenKeyPair(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen);
long  WINAPI Soft_ECC_SignData(char *szData, long nDlen, char *szSK, long nKLen, char *szResult, int iNID = 415); //NID_X9_62_prime256v1


long  WINAPI hrng_pokerm2_test(DWORD n, BYTE *hdata_buf);
long  WINAPI hrng_pokerm4_test(DWORD n, BYTE *hdata_buf);
long  WINAPI hrng_pokerm8_test(DWORD n, BYTE *hdata_buf);
long  WINAPI fre256_test(int n, BYTE *hdata_buf);


#ifdef __cplusplus
}
#endif


#endif //_IALG_H_20150412

