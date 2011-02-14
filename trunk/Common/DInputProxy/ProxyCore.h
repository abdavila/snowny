#pragma once

#include "DInputProxy.h"

struct DeviceInfo
{
	LPVOID		devobj;				// �豸���������һһ��
	KeyID		emuKID;				// ����ģ��İ���
	KeyID		prevPressKID;		// ��¼�ϴεİ���״̬�����ظ�����
	KeyID		lockedKID;			// �����İ���
};

extern DeviceInfo g_DeviceTbl[2];
extern DInputProxyConfig g_ProxyConfig;

void RealityKeyDown(LPVOID devobj, DWORD size, LPVOID data);
void SimulateKeyDown(LPVOID devobj, DWORD size, LPVOID data);
void CreateDevice(LPVOID devobj, DWORD devid);