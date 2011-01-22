#include "stdafx.h"
#include <Winbase.h>
// IOCTL ָ���
#define DFP_GET_VERSION   0x00074080
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088
// ���� IDEREGS �ṹ bCommandReg �����Чֵ
#define IDE_ATAPI_IDENTIFY  0xA1
#define IDE_ATA_IDENTIFY  0xEC
#define IDENTIFY_BUFFER_SIZE 512

// �ṹ�嶨��
// �����������������Ϣ
typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;
	BYTE bRevision;
	BYTE bReserved;
	BYTE bIDEDeviceMap;
	DWORD fCapabilities;
	DWORD dwReserved[4];
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;
// IDE �Ĵ���
typedef struct _IDEREGS
{
	BYTE bFeaturesReg;
	BYTE bSectorCountReg;
	BYTE bSectorNumberReg;
	BYTE bCylLowReg;
	BYTE bCylHighReg;
	BYTE bDriveHeadReg;
	BYTE bCommandReg;
	BYTE bReserved;
} IDEREGS, *PIDEREGS, *LPIDEREGS;
// ���ʹ���ָ����������
typedef struct _SENDCMDINPARAMS
{
	DWORD cBufferSize;
	IDEREGS irDriveRegs;
	BYTE bDriveNumber;
	BYTE bReserved[3];
	DWORD dwReserved[4];
	BYTE bBuffer[1];
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
// ����״̬
typedef struct _DRIVERSTATUS
{
	BYTE bDriverError;
	BYTE bIDEStatus;
	BYTE bReserved[2];
	DWORD dwReserved[2];
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
// ���ʹ���ָ����������
typedef struct _SENDCMDOUTPARAMS
{
	DWORD cBufferSize;
	DRIVERSTATUS DriverStatus;
	BYTE bBuffer[1];
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;


//��ȡӲ�����к�
void GetDiskPhysicalSN(char pchDiskPhysicalSN[14])
{
	BYTE IdOutCmd[530];
	HANDLE drive=CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ |
		GENERIC_WRITE,

		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,

		OPEN_EXISTING, 0, NULL);
	if (drive == INVALID_HANDLE_VALUE)
	{
		// ������
		return ;
	}
	GETVERSIONOUTPARAMS VersionParams;
	DWORD cbBytesReturned = 0;
	memset ((void*) &VersionParams, 0, sizeof(VersionParams));
	if (!DeviceIoControl(drive, DFP_GET_VERSION, NULL, 0,
		&VersionParams,
		sizeof
		(VersionParams), &cbBytesReturned, NULL))
	{
		// ������
		return ;
	}
	if (VersionParams.bIDEDeviceMap<=0)
	{
		// ������
		return ;
	}
	BYTE bIDCmd = 0;
	SENDCMDINPARAMS scip;
	bIDCmd = (VersionParams.bIDEDeviceMap >> 0 & 0x10) ?
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
	memset (&scip, 0, sizeof(scip));
	memset (IdOutCmd, 0, sizeof(IdOutCmd));
	scip.cBufferSize=IDENTIFY_BUFFER_SIZE;
	scip.irDriveRegs.bFeaturesReg=0;
	scip.irDriveRegs.bSectorCountReg=1;
	scip.irDriveRegs.bSectorNumberReg=1;
	scip.irDriveRegs.bCylLowReg=0;
	scip.irDriveRegs.bCylHighReg=0;
	scip.irDriveRegs.bDriveHeadReg=0xA0 | (((BYTE) drive & 1) << 4);
	scip.irDriveRegs.bCommandReg=bIDCmd;
	scip.bDriveNumber=(BYTE) drive;
	scip.cBufferSize=IDENTIFY_BUFFER_SIZE;
	if (!DeviceIoControl(drive, DFP_RECEIVE_DRIVE_DATA, &scip, sizeof
		(SENDCMDINPARAMS) - 1,
		(LPVOID)&IdOutCmd,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		&cbBytesReturned,
		NULL))
	{
		// ������
		return ;
	}
	USHORT *pIdSector = (USHORT *)((PSENDCMDOUTPARAMS) IdOutCmd) ->
		bBuffer;
	int nIndex=0, nPosition=0;
	for (nIndex=13; nIndex<20; nIndex++)
	{
		char ch = (unsigned char)(pIdSector[nIndex]/256);
		if(ch>='a'&&ch<='z' || ch>='A'&&ch<='Z' || ch>='0'&&ch<='9')
		{
			pchDiskPhysicalSN[nPosition++]=(unsigned char)(pIdSector
			[nIndex]/256);
		}
		ch = (unsigned char)(pIdSector[nIndex]%256);
		if(ch>='a'&&ch<='z' || ch>='A'&&ch<='Z' || ch>='0'&&ch<='9')
		{
			pchDiskPhysicalSN[nPosition++]=ch;
		}
	}
	pchDiskPhysicalSN[nPosition++]=0;
}