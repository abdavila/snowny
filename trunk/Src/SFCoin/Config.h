#pragma once
#include <windows.h>

#define COINMODE_SINGLE 0
#define COINMODE_DOUBLE 1

class CConfig
{
public:
	CConfig(void);
public:
	~CConfig(void);
	BOOL ReadConfig(LPCWSTR fileName);
	LONG CoinMode;		// Ͷ��ģʽ��0-��ʽ��1-˫ʽ
	LONG Difficulty;
	LONG Rounds;
	LONG TimeLimit;
	LONG UnitCoin;
	LONG IsFree;
};
