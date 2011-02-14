#pragma once

// ��ID��֧�����������Ҫ����DIPInputLock��
enum KeyID{
	DIP_START	= 1,		// ��ʼ��
	DIP_A		= 1 << 1,	// ��Ϸ��ȷ�ϼ���A
	DIP_B		= 1 << 2,	// ��Ϸ�л��˼���B
	DIP_UP		= 1 << 3,	// ��
	DIP_DOWN	= 1 << 4,	// ��
	DIP_LEFT	= 1 << 5,	// ��
	DIP_RIGHT	= 1 << 6,	// ��
	DIP_COIN	= 1 << 7,	// ������Ҫ��
	DIP_ALL		= 1 << 8	// �ṩһ��ȫ����ʶ
};

// ��ұ�ʶ
enum PlayID{P1 = 0, P2};

struct KeyMapping
{
	PlayID	pid;			// ��ұ�ʶ
	DWORD	devid;			// ��ð󶨵��豸guid��ǰ4�ֽ�
	BYTE	keyvalSTART;	// ��ʼ����ֵ�������A����DIK_A���ֱ�2�ż���1����ͬ
	BYTE	keyvalA;		// ȷ�ϼ���A
	BYTE	keyvalB;		// ��Ϸ�л��˼���B
	BYTE	keyvalUP;		// ��
	BYTE	keyvalDOWN;		// ��
	BYTE	keyvalLEFT;		// ��
	BYTE	keyvalRIGHT;	// ��
	BYTE	keyvalCOIN;		// ������Ҫ��
};

typedef void (*KDPROC)(const PlayID &pid, const KeyID &kid);
struct DInputProxyConfig
{
	KeyMapping	KeyMapping[2];	// P1��P2
	KDPROC		KeyDownProc;
};

// ��ʼ��������
/*
// ��ʼ��ʵ��
DInputProxyConfig config;
memset(&config, 0, sizeof(config));
config.KeyMapping[0].pid = P1;
config.KeyMapping[1].pid = P2;
config.KeyDownProc = KeyDownProc;

config.KeyMapping[0].devid = 0x6F1D2B61;// ����id
config.KeyMapping[0].keyvalA = 0x24;	// J
config.KeyMapping[0].keyvalB = 0x25;	// K
config.KeyMapping[0].keyvalCOIN = 0x04;	// 3
config.KeyMapping[0].keyvalDOWN = 0xD0;	// down
config.KeyMapping[0].keyvalLEFT = 0xCB;	// left
config.KeyMapping[0].keyvalRIGHT= 0xCD; // RIGHT
config.KeyMapping[0].keyvalSTART= 0x26; // L
config.KeyMapping[0].keyvalUP	= 0xC8; // up

config.KeyMapping[1].devid = 0x6101f3a70;// �ֱ�id
config.KeyMapping[1].keyvalCOIN = 0x05;	// 4
config.KeyMapping[1].keyvalA = 0x00;	// 1
config.KeyMapping[1].keyvalB = 0x01;	// 2
config.KeyMapping[1].keyvalSTART = 0x07;	// 7

InitDInputProxy(config);
*/
BOOL InitDInputProxy(const DInputProxyConfig &config);

// ģ�ⰴ���¼�
// �� 1P ���� start��DIPKeyDown(P1, DIP_START) 
void DIPInput(const PlayID &pid, const KeyID &kid);

// �����������û����루ģ�����벻��Ӱ�죩
void DIPInputLock(const PlayID &pid, const KeyID &kid);
