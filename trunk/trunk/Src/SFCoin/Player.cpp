#include "stdafx.h"
#include "Player.h"

DWORD CPlayer::m_UnionCoins = 0;
DWORD CPlayer::m_OldUnionCoins = 0;

CPlayer::CPlayer(DWORD id)
: m_dId(id)
, m_Status(PS_IDLE)
, m_OldCoins(0)
, m_dCoins(0)
{

}

void CPlayer::RefreshStatus( GAMEFLOW gameFlow )
{
	if (m_Status != PS_IDLE)
	{
		if (gameFlow == flow_mainmenu && m_Status == PS_CLICKSTART)
		{
			m_Status = PS_STARTTING;
		}
		else if (gameFlow == flow_game && m_Status == PS_STARTTING)
		{
			m_Status = PS_GAMING;
		}
		else if (gameFlow == flow_continue && m_Status == PS_GAMING)
		{
			m_Status = PS_DEAD; // maybe winner
		}
		else if (gameFlow == flow_mainmenu && m_Status == PS_DEAD)
		{
			m_Status = PS_GAMEOVER;
		}
		else if (gameFlow == flow_titlemenu && m_Status == PS_GAMEOVER)
		{
			m_Status = PS_IDLE;
		}
	}
}

BOOL CPlayer::ClickStart()
{
	if (m_Status == PS_IDLE)
	{
		DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins; // 单式、双式
		if (*pCoins >= g_Config.UnitCoin)
		{
			*pCoins -= g_Config.UnitCoin;
			m_Status = PS_CLICKSTART;

			TRACE(TEXT("SF4 player[%d] start success"), m_dId);
			return TRUE;
		}
		else
		{
			TRACE(TEXT("SF4 player[%d] start fail: has no coins %d, %d"), m_dId, *pCoins, g_Config.UnitCoin);
		}
	}
	else
	{
		TRACE(TEXT("SF4 player[%d] start fail: error status %d"), m_dId, m_Status);
	}

	return FALSE;
}

void CPlayer::IncrementCoin()
{
	g_Config.CoinMode == 0 ? m_UnionCoins ++ : m_dCoins ++; // 单式、双式
}

BOOL CPlayer::CoinsChanged()
{
	BOOL changed = FALSE;
	DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins;			// 单式、双式
	DWORD *pOldCoins = g_Config.CoinMode == 0 ? &m_OldUnionCoins : &m_OldCoins; // 单式、双式

	changed = *pCoins != *pOldCoins;
	*pOldCoins = *pCoins;

	return changed;
}

DWORD CPlayer::GetCoinNumber()
{
	return g_Config.CoinMode == 0 ? m_UnionCoins : m_dCoins;// 单式、双式
}
