#pragma once

/*
Function: InstallHook
	Installs the hook. The name of the target process should be in
	HKEY_CURRENT_USER\Software\DInput-Hook before calling this function.

Remarks:
	The __stdcall convention is used so that the function is callable from VisualBasic.
	Even though __stdcall does some name-mangling, DInput-Hook.DEF exports the symbol
	'InstallHook' for this function, so you don't need to reference the mangled symbol.
*/
void InstallHook();

/*
Function: RemoveHook
	Uninstalls the hook - call this when you no longer want to search launched processes
	for the target process. Typically called after you've closed the target process and
	are exiting.

Remarks:
	The __stdcall convention is used so that the function is callable from VisualBasic.
	Even though __stdcall does some name-mangling, DInput-Hook.DEF exports the symbol
	'RemoveHook' for this function, so you don't need to reference the mangled symbol.
*/
void RemoveHook();

struct KeyState
{
	BYTE coin;
	BYTE start;
	BYTE esc;
	BYTE backspace;
	BYTE enter;
};

// ģ�ⰴ���¼�
// id:		�豸��ʶ����1P��2P��Ӧ���豸
// keyID:	����DInput�ļ���ʶ,��A:DIK_A
void DIHKeyDown(BYTE id, BYTE keyID);

typedef void (*KDPROC)(BYTE, KeyState&);
// ���ð����ص�������
void DIHSetKDProc(KDPROC kdProc);
