#pragma once
#include <windows.h>
#define DEBUG true

void MovAddrReg6Byte(DWORD instruction, char* const reg, void* pValue, HANDLE process = (HANDLE)-1);
void RETHook6Byte(DWORD instruction, void* function, HANDLE process = (HANDLE)-1);
void NOP(DWORD instruction, int len, HANDLE process = (HANDLE)-1);