#pragma once

#include <windows.h>

class CPlayer
{
public:
	CPlayer(DWORD id=0);
	DWORD GetCoinNumber(){return m_dCoins;}
	VOID SetCoinNumber(DWORD number){m_dCoins=number;}

	BOOL ClickStart() const { return m_ClickStart; }
	void ClickStart(BOOL val) { m_ClickStart = val; }
private:
	BOOL m_ClickStart;	//�ж�����Ƿ�����ʼ
	DWORD m_dCoins;
	DWORD m_dId;
};