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

#define IPSTART	3
#define INSERTCOIN 4
#define LOADING 5
#define TIMECOUNTER  6
#define CREDITTEXT0 7
#define CREDIT0	8
#define CREDITTEXT1 9
#define CREDIT1	10


VOID KeyProc(BYTE, KeyState&);
class CContrlCenter
{
public:
	CContrlCenter(void);
	~CContrlCenter(void);
	bool Init();
	void Run();

	static void KeyProc(BYTE id, KeyState& state);

	void RefreshPlayerStatus();
	void UpdateCoinShow();

	// ���ֻֽ�ģʽ�Ͷ�սģʽ�ϻغϡ�ʱ������һ��
	void Setting();	// �ֻ�ģʽ����սģʽ����

	CKeycmd m_cmdDemo2Titile;		//��Ϸ��ʼ����demo���浽title����
	CKeycmd m_cmdtitle2Mainmenu;	//��Ϸ��ʼ����title���浽Mainmenu����
	CKeycmd m_mainmenu2SelectChar;	//��Ϸ��ʼ����Mainmenu����ɫѡ��
	CKeycmd m_mainMenu2Title;		//��Ϸ��Mainmenu��title����
	CKeycmd m_mainMenuSetting2SelectChar;		//��Ϸ��ʼ����
	CKeycmd m_selectChar2mainMenu;				//��Ϸѡ��ɫ�����˵�

	CKeycmd m_VersusSetting;				// ��սģʽ����
	CKeycmd m_VersusMainmenu2SelectChar;	// ��սģʽ����ѡ��
	CKeycmd m_Game2MainMenu;				// �ֻ�ģʽ�л�����սģʽ����������Ϸ�У��˳���Ϸ
	
	//CKeycmd m_cmdDemo2SelectChar;  //��Ϸ��ʼ����demo���浽��ɫѡ��
	//CKeycmd m_cmdTitle2SelectChar; //��Ϸ��ʼ����Title���浽��ɫѡ��
	//CKeycmd m_cmdSetting;          //��Ϸ��ʼ����
	//CKeycmd m_cmdMainmenu2Title;   //��Ϸ������ص�title����
	CPlayer m_Players[2];
	BYTE	m_ActivePlayer;

	CFairy* m_Fairy;
	HWND   m_GameWnd;

	BOOL	m_IsArcadeMode;		// �ֻ�ģʽ
};
extern CContrlCenter g_ContrlCenter;