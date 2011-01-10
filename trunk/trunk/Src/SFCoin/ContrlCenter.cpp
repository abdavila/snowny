#include "StdAfx.h"
#include "ContrlCenter.h"
#include "GlobalVar.h"

#pragma data_seg (".HookSection")
// Shared instance for all processes.
HHOOK g_hMouse = NULL;
#pragma data_seg ()

CContrlCenter g_ContrlCenter;


CContrlCenter::CContrlCenter(void):m_cmdDemo2Titile(L"m_cmdDemo2Titile")
,m_cmdtitle2Mainmenu(L"m_cmdtitle2Mainmenu")
,m_mainmenu2SelectChar(L"m_mainmenu2SelectChar")
,m_mainMenu2Title(L"m_mainMenu2Title")
,m_mainMenuSetting2SelectChar(L"m_mainMenuSetting2SelectChar")
,m_selectChar2mainMenu(L"m_selectChar2mainMenu")
,m_bCoinsChanged(FALSE)
,m_ActivePlayer(0)
{
	TRACE(L"StreetFighter CContrlCenter::CContrlCenter\n");
}

CContrlCenter::~CContrlCenter(void)
{
	UnhookWindowsHookEx(g_hMouse);
}
WNDPROC OldWindowProc = NULL;
LRESULT CALLBACK NewWindowProc(
							HWND hwnd, 
							UINT uMsg, 
							WPARAM wParam, 
							LPARAM lParam 
							)
{
	//TRACE("aaaa NewWindowProc WM_LBUTTONDBLCLK before");
	static BOOL inited = FALSE;
	static CWnd parentWnd;

	if (inited == FALSE)
	{
		TRACE("aaaa NewWindowProc WM_LBUTTONDBLCLK after");
		ShowCursor(FALSE);
		parentWnd.Attach(hwnd);
		int screenWidth =GetSystemMetrics(SM_CXSCREEN);
		int screentHeight =GetSystemMetrics(SM_CYSCREEN);
		// 这里要用正确的width和height计算坐标，不然窗口可能不显示，cxb
		g_ContrlCenter.m_Fairy.CreateTransparentWnd(&parentWnd,STANDBYBG,L"SF4Con\\StandbyBG.jpg", CPoint(0,0), screenWidth, screentHeight);
		g_ContrlCenter.m_Fairy.CreateTransparentWnd(&parentWnd,INSERTCOIN,L"SF4Con\\InsertCoin.png",CPoint(screenWidth/2-80, screentHeight - 200));
		g_ContrlCenter.m_Fairy.CreateTransparentWnd(&parentWnd,TIMECOUNTER,L"SF4Con\\TimeCounter.gif",CPoint(screenWidth/2-20, 200));
		g_ContrlCenter.m_Fairy.CreateTransparentWnd(&parentWnd,CREDITTEXT,L"SF4Con\\CREDIT.png",CPoint(screenWidth/2-150, screentHeight - 100));
		g_ContrlCenter.m_Fairy.CreateTransparentWnd(&parentWnd,LOADING,L"SF4Con\\Loading.png",CPoint(screenWidth/2-20, screentHeight - 200));
		g_ContrlCenter.m_Fairy.CreateCoinInsert(CREDIT, L"SF4Con\\Num.png", 0, 0, g_ContrlCenter.m_Config.UnitCoin,CPoint(screenWidth / 2 - 20, screentHeight - 100));
		inited = TRUE;
	}

	return CallWindowProc(OldWindowProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc( int nCode,
								    WPARAM wParam,
								    LPARAM lParam
								   )
{
	return 1;
}


bool CContrlCenter::Init()
{
	TRACE(L"StreetFighter CContrlCenter::Init()\n");
	m_Config.ReadConfig(L"config.ini");
	TRACE("StreetFighter GAME WND:%d\n",0);
	HWND wnd=0;
	while(0==wnd)
	{
		wnd=::FindWindow(L"STREET FIGHTER IV",L"STREET FIGHTER IV");
		Sleep(100);
	}
	m_GameWnd.Attach(wnd);
	m_GameWnd.SetWindowPos(NULL,-(GetSystemMetrics(SM_CXBORDER) + 2),-(GetSystemMetrics(SM_CYCAPTION) + 3),0,0,SWP_NOSIZE);
	OldWindowProc = (WNDPROC)SetWindowLong(wnd, GWL_WNDPROC, (LONG)NewWindowProc);

	HMODULE handle = GetModuleHandleA("SFCoin.dll");
	g_hMouse = SetWindowsHookEx(WH_MOUSE, LowLevelMouseProc, handle, 0);
	TRACE("StreetFighter GAME WND:%d\n",wnd);
	DIHSetKDProc(KeyProc);

	int interval;

	{
		interval = 400;
		m_cmdDemo2Titile.InsertCmd(IDK_OK, interval);
	}
	{
		interval = 400;
		m_cmdtitle2Mainmenu.InsertCmd(IDK_OK, interval);
		m_cmdtitle2Mainmenu.InsertCmd(IDK_DOWN, interval);
		m_cmdtitle2Mainmenu.InsertCmd(IDK_OK, interval);
	}
	{
		m_mainmenu2SelectChar.InsertCmd(IDK_OK, 1000);
		m_mainmenu2SelectChar.InsertCmd(IDK_OK, 1000);
		m_mainmenu2SelectChar.InsertCmd(IDK_OK, 1000);
		m_mainmenu2SelectChar.InsertCmd(IDK_OK, 1000);
	}
	{
		interval = 600;
		m_mainMenu2Title.InsertCmd(IDK_BACKSPACE,interval);
		m_mainMenu2Title.InsertCmd(IDK_DOWN,interval);
		m_mainMenu2Title.InsertCmd(IDK_OK,interval);
	}

	m_mainMenuSetting2SelectChar.InsertCmd(IDK_OK,interval+1000);//进入setting
	interval= 500;

	int j=0;
	for( j=0;j<abs(m_Config.Difficulty);j++)
	{
		if(m_Config.Difficulty>0)
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_RIGHT,interval);
		else
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_LEFT,interval);
	}
	m_mainMenuSetting2SelectChar.InsertCmd(IDK_DOWN,interval);
	for(j=0;j<abs(m_Config.Rounds);j++)
	{
		if(m_Config.Rounds>0)
		{
			TRACE("StreetFighter IDK_RIGHT\n");
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_RIGHT,interval);
		}
		else
		{
			TRACE("StreetFighter IDK_LEFT\n");
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_LEFT,interval);
		}
	}
	m_mainMenuSetting2SelectChar.InsertCmd(IDK_DOWN,interval);
	for(j=0;j<abs(m_Config.TimeLimit);j++)
	{
		if(m_Config.TimeLimit>0)
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_RIGHT,interval);
		else
			m_mainMenuSetting2SelectChar.InsertCmd(IDK_LEFT,interval);
	}
	//进入select
	interval=1000;
	m_mainMenuSetting2SelectChar.InsertCmd(IDK_OK,interval);
	m_mainMenuSetting2SelectChar.InsertCmd(IDK_OK,interval);
	m_mainMenuSetting2SelectChar.InsertCmd(IDK_OK,interval);

	//返回到mainmenu
	//interval=1000;
	m_selectChar2mainMenu.InsertCmd(IDK_BACKSPACE,4000);
	m_selectChar2mainMenu.InsertCmd(IDK_UP,interval);
	m_selectChar2mainMenu.InsertCmd(IDK_OK,interval);
	TRACE(L"StreetFighter CContrlCenter::Init() compelet\n");
	return true;
}
void CContrlCenter::Run()
{
	GAMEFLOW oldGameFlow=flow_start;
	GAMEFLOW oldStartGameFlow=flow_mainmenu;
	m_Setted=FALSE;
	Sleep(2000);
	//快速跨越开机动画
	while(flow_titlemenu!=g_GameFlow)
	{
		DIHKeyDown(0,IDK_OK);
		Sleep(500);
	}
	// 锁定1p 2p 游戏手柄
	DIHLockInput(0);
	DIHLockInput(1);
	while(1)
	{
		SetActiveWindow(m_GameWnd);
		//检测是否gameover状态
		GameFlowUpdate();
		
		//检测状态切换
		if(g_GameFlow!=oldGameFlow)
		{
			TRACE(L"StreetFighter GameFlow change to :%d\n",g_GameFlow);
			switch(g_GameFlow)
			{
			case flow_start:
				break;
			case flow_titlemenu:
				//初始设置
				if(!m_Setted)
				{
					m_Fairy.ShowWnd(LOADING);
					TRACE("aaaa bSetted m_cmdtitle2Mainmenu");
					m_cmdtitle2Mainmenu.Excute(m_ActivePlayer);
				}
				if(m_Setted)
				{
					m_Fairy.HideWnd(LOADING);
					m_Fairy.ShowWnd(STANDBYBG);
					m_Fairy.ShowWnd(INSERTCOIN);
					m_Fairy.ShowWnd(CREDITTEXT);
					m_Fairy.ShowWnd(CREDIT);
				}
				break;
			case flow_demo:
				if(m_Setted)
				{
					m_Fairy.HideWnd(STANDBYBG);
					m_Fairy.ShowWnd(INSERTCOIN);
					m_Fairy.ShowWnd(CREDITTEXT);
					m_Fairy.ShowWnd(CREDIT);
				}
				break;
			case flow_mainmenu:
				Sleep(500);
				if (!m_Setted)
				{
					if(oldGameFlow == flow_titlemenu)
					{
						TRACE("aaaa bSetted m_mainMenuSetting2SelectChar");
						m_mainMenuSetting2SelectChar.Excute(m_ActivePlayer);
					}
					else if (oldGameFlow == flow_selectchar)
					{
						TRACE("aaaa bSetted m_mainMenu2Title");
						m_mainMenu2Title.Excute(m_ActivePlayer);
						m_Setted = TRUE;
					}
				}
				//游戏结束后回到mainmenu
				else if(oldGameFlow==flow_continue)
				{
					m_Fairy.ShowWnd(STANDBYBG);
					m_Fairy.ShowWnd(CREDITTEXT);
					m_Fairy.ShowWnd(CREDIT);
					Sleep(1500);
					DIHKeyDown(0,IDK_OK);      //画廊里有很多画
					m_mainMenu2Title.Excute(m_ActivePlayer);
				}
				break;
			case flow_selectchar:
				Sleep(100);
				TRACE("aaaa flow_selectchar bSetted %d", m_Setted);
				if (!m_Setted)
				{
					TRACE("aaaa bSetted m_selectChar2mainMenu");
					m_selectChar2mainMenu.Excute(m_ActivePlayer);
				}
				else
				{
					m_Fairy.HideWnd(INSERTCOIN);
					m_Fairy.HideWnd(STANDBYBG);
					m_Fairy.HideWnd(LOADING);
					m_Fairy.ShowWnd(CREDITTEXT);
					m_Fairy.ShowWnd(CREDIT);
					m_Fairy.ShowWnd(TIMECOUNTER);

					DWORD time = GetTickCount();
					while (GetTickCount() - time < 22000)
					{
						Sleep(1);
						UpdateCoinShow();// 循环里头无法及时响应投币动作，故作此重构，cxb

						//游戏开始
						if(g_GameFlow==flow_game) break;
					}
					m_Fairy.HideWnd(TIMECOUNTER);
					//默认角色
					if(g_GameFlow!=flow_game)
					{
						TRACE(TEXT("SF4 select default char"));
						DIHKeyDown(m_ActivePlayer, IDK_OK);
						Sleep(1000);
						DIHKeyDown(m_ActivePlayer, IDK_OK);
					}
				}
				break;
			default:
				break;
			}
			oldGameFlow = g_GameFlow;
		}//IF
		
		UpdateCoinShow();
		
		// 遍历玩家输入
		for (int i = 0; i < 2; i ++)
		{
			if (m_Players[i].ClickStart())
			{
				int coinsOfThePlayer = m_Players[i].GetCoinNumber();
				if (m_Config.CoinMode == 0) // 单式
				{
					coinsOfThePlayer = m_Players[0].GetCoinNumber();
				}
				TRACE(L"SF4 Start game,player[%d] %d coins and unit coins is %d gameflow %d",
				i, coinsOfThePlayer, m_Config.UnitCoin, g_GameFlow);

				if(coinsOfThePlayer >= m_Config.UnitCoin &&  g_GameFlow != oldStartGameFlow)//避免重复检测相同场景，执行相同操作
				{
					m_Fairy.ShowWnd(STANDBYBG);
					m_Fairy.ShowWnd(LOADING);
					m_Fairy.HideWnd(INSERTCOIN);

					switch(g_GameFlow)
					{
					case flow_titlemenu:
						TRACE(L"aaaa SF4 m_bStart is flow_titlemenu");
						m_cmdtitle2Mainmenu.Excute(i);
						break;

					case flow_demo:
						TRACE(L"aaaa SF4 m_bStart is flow_demo");
						m_cmdDemo2Titile.Excute(i);
						/*m_cmdDemo2SelectChar.Excute();*/
						break;
					case flow_mainmenu:
						Sleep(400);
						m_mainmenu2SelectChar.Excute(i);
						TRACE(L"aaaa SF4 m_bStart is flow_mainmenu");
						if (m_Config.CoinMode == 0) // 单式
						{
							m_Players[0].SetCoinNumber(m_Players[0].GetCoinNumber()-m_Config.UnitCoin);
						}
						else
						{
							m_Players[i].SetCoinNumber(m_Players[i].GetCoinNumber()-m_Config.UnitCoin);
						}
						m_bCoinsChanged=TRUE;
						m_Players[i].ClickStart(FALSE);
						break;
					}
					oldStartGameFlow = g_GameFlow;
					m_Fairy.HideWnd(INSERTCOIN);
				}
				else if (coinsOfThePlayer < m_Config.UnitCoin)
				{
					TRACE(TEXT("SF4 player[%d] start fail"), i);
					m_Players[i].ClickStart(FALSE);
				}
			}
		}

		// 根据是否在游戏中来锁定和解锁输入
		// 选人界面中通过截获并模拟操作即不会受锁定的影响，表现良好
		// 把选人纳入锁定范围的诱因：游戏逻辑中选人界面可以回退
		if(flow_game==g_GameFlow)
		{
			DIHUnlockInput(m_ActivePlayer);
		}
		else
		{
			DIHLockInput(m_ActivePlayer);
		}
		Sleep(1);
	}
}

void CContrlCenter::UpdateCoinShow()
{
	if(m_bCoinsChanged)
	{
		int totalCoins = m_Players[0].GetCoinNumber();
		if (m_Config.CoinMode == 0)	// µ¥Ê½
		{
			totalCoins += m_Players[1].GetCoinNumber();
		}
		int life = totalCoins / m_Config.UnitCoin;
		int rem = totalCoins % m_Config.UnitCoin;
		TRACE(L"StreetFighter m_bCoinsChanged %d\n", totalCoins);

		m_Fairy.ResetCoinInsert(life, rem, m_Config.UnitCoin);
		m_Fairy.ShowWnd(CREDITTEXT);
		m_Fairy.ShowWnd(CREDIT);
		m_bCoinsChanged=FALSE;
	}
}
VOID KeyProc(BYTE id, KeyState& state)
{
 	if(state.coin==1)//投币,任何时候都有效
 	{
		if (g_ContrlCenter.m_Config.CoinMode == 0) // 单式，币数都算在1P头上
		{
			id = 0;
		}
		g_ContrlCenter.m_Players[id].SetCoinNumber(g_ContrlCenter.m_Players[id].GetCoinNumber() + 1);
 		g_ContrlCenter.m_bCoinsChanged = TRUE;
 		TRACE(L"StreetFighter player [%d] insert coin:%d\n", id, g_ContrlCenter.m_Players[id].GetCoinNumber());
 	}
 	else if(state.start==1)
 	{
		//开始,暂时游戏中不能开始，等待对战模式的支持
		if (flow_titlemenu == g_GameFlow || flow_demo==g_GameFlow)
		{
			TRACE(L"StreetFighter start\n");
			g_ContrlCenter.m_Players[id].ClickStart(TRUE);
		}
		else if (flow_continue==g_GameFlow)
		{
			TRACE(L"StreetFighter continue\n");
 			if(g_ContrlCenter.m_Players[id].GetCoinNumber()>=g_ContrlCenter.m_Config.UnitCoin)
 			{
 				DIHKeyDown(id, IDK_CONTINUE);
 				//减币
 				g_ContrlCenter.m_Players[id].SetCoinNumber(g_ContrlCenter.m_Players[id].GetCoinNumber()-g_ContrlCenter.m_Config.UnitCoin);
 				g_ContrlCenter.m_bCoinsChanged=TRUE;
 				TRACE(L"StreetFighter continue coin %d\n",g_ContrlCenter.m_Players[id].GetCoinNumber());
 			}
 			else
			{
 				TRACE(L"StreetFighter continue failed coin %d\n",g_ContrlCenter.m_Players[id].GetCoinNumber());
			}
		}
		else if (flow_game == g_GameFlow)	// 对战模式要修改此处
		{
			DIHLockInputOnce(id);// 暂不支持对战模式，所以不允许开始
		}
 	}
	else if (g_GameFlow == flow_selectchar && g_ContrlCenter.m_Setted)
	{
		if (state.up == 1){	DIHKeyDown(id, IDK_UP);	}
		else if (state.down == 1){	DIHKeyDown(id, IDK_DOWN);	}
		else if (state.left == 1){	DIHKeyDown(id, IDK_LEFT);	}
		else if (state.right == 1){	DIHKeyDown(id, IDK_RIGHT);	}
		else if (state.ok == 1){	DIHKeyDown(id, IDK_OK);	}
	}
}