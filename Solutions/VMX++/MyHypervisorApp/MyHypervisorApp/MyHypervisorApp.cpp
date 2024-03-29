// MyHypervisorApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>  
#include <vector>  
#include <bitset>  
#include <array>  
#include <string>  
#include <intrin.h>  

using namespace std;



string GetCpuID()
{
	//Initialize used variables
	char SysType[13]; //Array consisting of 13 single bytes/characters
	string CpuID; //The string that will be used to add all the characters to
				  //Starting coding in assembly language
	_asm
	{
		//Execute CPUID with EAX = 0 to get the CPU producer
		XOR EAX, EAX
		CPUID
		//MOV EBX to EAX and get the characters one by one by using shift out right bitwise operation.
		MOV EAX, EBX
		MOV SysType[0], al
		MOV SysType[1], ah
		SHR EAX, 16
		MOV SysType[2], al
		MOV SysType[3], ah
		//Get the second part the same way but these values are stored in EDX
		MOV EAX, EDX
		MOV SysType[4], al
		MOV SysType[5], ah
		SHR EAX, 16
		MOV SysType[6], al
		MOV SysType[7], ah
		//Get the third part
		MOV EAX, ECX
		MOV SysType[8], al
		MOV SysType[9], ah
		SHR EAX, 16
		MOV SysType[10], al
		MOV SysType[11], ah
		MOV SysType[12], 00
	}
	CpuID.assign(SysType, 12);
	return CpuID;
}

// Intel Software Developer’s Manual volume 3C in section 23.6 DISCOVERING SUPPORT FOR VMX
// CPUID.1:ECX.VMX[bit 5] = 1
bool VMX_Support_Detection()
{

	bool VMX = false;
	__asm {
		xor    eax, eax
		inc    eax
		cpuid
		bt     ecx, 0x5
		jc     VMXSupport
		VMXNotSupport :
		jmp     NopInstr
		VMXSupport :
		mov    VMX, 0x1
		NopInstr :
		nop
	}

	return VMX;
}

int main()
{
	string CpuID;

	CpuID = GetCpuID();
	cout << "[*] The CPU Vendor is : " << CpuID << endl;

	if (CpuID == "GenuineIntel")
	{
		cout << "[*] The Processor virtualization technology is VT-x. \n";
	}
	else
	{
		cout << "[*] This program is not designed to run in a non-VT-x environemnt !\n";
		return 1;
	}
	
	if (VMX_Support_Detection())
	{
		cout << "[*] VMX Operation is supported by your processor .\n";
	}
	else
	{
		cout << "[*] VMX Operation is not supported by your processor .\n";
		return 1;
	}


	/* 
	* 
	* Triggering DrvCreate 
	* 
	*/
	

	_getch(); // wait before exit

    return 0;
}
