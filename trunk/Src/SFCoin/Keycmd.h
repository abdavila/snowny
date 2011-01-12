#pragma once
#include <vector>
#include "GlobalVar.h"

class CKeycmd
{
public:
	// ģ�����й��Ĵ��ĸ�״̬fromFlow���䵽�ĸ�״̬toFlow��
	// ��Ϊ�����ǽ�����״̬fromFlow�ϵģ��Ҵﵽ��toFlow�Ž�������ʹ��
	// �뿪״̸̬ģ�ⲻ�������Ͷ���
	// ����ְ�𻮷ָ���
	CKeycmd(GAMEFLOW fromFlow, GAMEFLOW toFlow,	
		CString name=L"No name");
	~CKeycmd(void);
	typedef struct _Key 
	{
		DWORD interval;
		DWORD key_id;
	}KEYCMD;
	bool ReadCmdFile(const char* fileName);
	bool SaveCmdFile(const char* fileName);
	DWORD InsertCmd(DWORD key,DWORD interval,DWORD pos=-1);
	void Excute(BYTE deviceID);// ��ָ�����豸ģ�⣬�����ģ�ֱ�����䵽toFlow
private:
	std::vector<KEYCMD> m_vCmds;
	CString m_Name;

	GAMEFLOW m_FromFlow;
	GAMEFLOW m_ToFlow;
};
