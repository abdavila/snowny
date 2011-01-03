#pragma once
#include "Keycmd.h"
#include "Player.h"
#include "Config.h"
#include "../../Common/DirectInput-Hook/DirectInput-Hook.h"
#include "../../Common/UIShow/Fairy.h"

#define MOST_SIMPLE (-3)
#define VERY_SIMPLE (-2)
#define SIMPLE (-1)
#define MIDDLE 0
#define LITTLE_HARD 1
#define HARD 2
#define VERY_HARD 3
#define MOST_HARD 4
//��� �ǳ���  �� �е� ƫ�� ���� �ǳ����� ����


#define STANDBYBG 0
#define LOADINGBG 1
#define CREDIT	2
#define IPSTART	3
#define INSERTCOIN 4
#define LOADING 5


VOID KeyProc(BYTE, KeyState&);
static class CContrlCenter
{
public:
	CContrlCenter(void);
	~CContrlCenter(void);
	bool Init();
	void Run();
	 CConfig m_Config;
	 CKeycmd m_cmdDemo2SelectChar;  //��Ϸ��ʼ����demo���浽��ɫѡ��
	 CKeycmd m_cmdTitle2SelectChar; //��Ϸ��ʼ����Title���浽��ɫѡ��
	 CKeycmd m_cmdSetting;          //��Ϸ��ʼ����
	 CKeycmd m_cmdMainmenu2Title;   //��Ϸ������ص�title����
	 CPlayer m_Players[2];
	 BOOL m_bStart;                 //�ж�����Ƿ�����ʼ
	 BOOL m_bInsertCoin;
	 BOOL m_bIsBusy;                //�ж���Ϸ�Ƿ������ã�״̬�л���æµ״̬

	 //CScreen m_CoverScreen;
	 //CSTreetFightScene m_Scene;
	 CFairy m_Fairy;
	 HWND m_GameWnd;
};
extern CContrlCenter g_ContrlCenter;