#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
{
	CoinMode = 0;
	UnitCoin = 2;

	//��������ֵ�������ϷĬ�����õ�λ�ã�-1��ʾĬ�����õ���һ�����ã�1��ʾĬ�����õ���һ������
	Difficulty = 3;
	Rounds = -1;
	TimeLimit = -2;
}

CConfig::~CConfig(void)
{
}
BOOL CConfig::ReadConfig(LPCWSTR fileName)
{
	CoinMode=GetPrivateProfileInt(L"CoinMode",L"Sel",0,fileName);
	//m_UnitCoin=������+1
	UnitCoin=GetPrivateProfileInt(L"UnitCoin",L"Sel",0,fileName);
	UnitCoin++;
	//Difficuty��Ĭ��ֵ�ǡ��еȡ�������Ϊ3
	Difficulty=GetPrivateProfileInt(L"Difficulty",L"Sel",0,fileName);
	Difficulty=Difficulty-3;
	Rounds=GetPrivateProfileInt(L"Rounds",L"Sel",0,fileName);
	Rounds=Rounds-1;
	TimeLimit=GetPrivateProfileInt(L"TimeLimit",L"Sel",0,fileName);
	TimeLimit=TimeLimit-2;
	//printf("StreetFighter CoinMode:%d\n",m_CoinMode);
	//printf("StreetFighter m_UnitCoin:%d\n",m_UnitCoin);
	return TRUE;
} 