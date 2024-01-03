#include <stdio.h>
#include <stdlib.h>
#include <intrin.h> // for __cpuid, __bittest

#include "cpuid.h"


/*
* NOTE:	You may use sysinternals\coreinfo.exe -v in order to 
*		Dump only virtualization-related features including
*   	support for second level address translation.
*/


#define CPUID_0x01_ECX_VNX_SUPPORTED (1 << 5)


void PrintCPUSig();
void PrintCpuBrandString();
void PrintVMXSupport();
void PrintCPUIDRange(int start, int end);
char SizeOfPhysicalAddressRange();


int main (int argc, char* argv[]) 
{
	int eax;
	CpuId_t regs; // eax, ebx, exc, edx
	int max_eax = 0x10;
	char cpusig[16];
	memset(cpusig, 0, sizeof(cpusig));

	printf("========================\n");
	printf("CPUID from userspace\n");
	printf("========================\n");
	printf("\n");

	PrintCPUSig();
	PrintCpuBrandString();
	PrintVMXSupport();
	PrintCPUIDRange(0x01, 0x10);

	printf("SizeOfPhysicalAddressRange: %d-bit\n", SizeOfPhysicalAddressRange());
	printf("\n");
	
	getchar(); // keep console open

	return 0;
}


void PrintCPUSig()
{
	CpuId_t regs; // eax, ebx, exc, edx
	char cpusig[16] = { 0 };
	
	__cpuid(regs.i, 0x00);
	memcpy(cpusig, (void*) & (regs.ebx), 0x04);
	memcpy(cpusig + 4, (void*) & (regs.edx), 0x04);
	memcpy(cpusig + 8, (void*) & (regs.ecx), 0x04);


	printf_s("CPU Signature: %s\n\n", cpusig);
}


void PrintCpuBrandString()
{
	int eax;
	CpuId_t regs; // eax, ebx, exc, edx
	char brand[64] = { 0 };

	/* See Vol. 2A 3-225 for information on brand string
	*/
	eax = 0x80000000;
	__cpuid(regs.i, eax);


	if (regs.eax & 0x80000000) {

		char* brand_offset = brand;
		for (eax = 0x80000002; eax < 0x80000005; eax++)
		{
			__cpuid(regs.i, eax);
			memcpy(brand_offset + 0x00, (void*) & (regs.eax), 0x10);
			brand_offset += 0x10;
		}
	}

	printf_s("CPU Brand String: %s\n\n", brand);
}


void PrintVMXSupport()
{
	CpuId_t regs; // eax, ebx, exc, edx

	__cpuid(regs.i, 0x01);

	if (regs.ecx & CPUID_0x01_ECX_VNX_SUPPORTED)
	{
		printf_s("VMX is supported ! \n\n");
	}
	else
	{
		printf_s("VMX is not supported ! \n\n");
	}

}


void PrintCPUIDRange(int start, int end)
{
	CpuId_t regs; // eax, ebx, exc, edx

	for (int eax = start; eax < end; eax++)
	{
		__cpuid(regs.i, eax);
		printf("CPUID[%.8X]  %#010x %#010x %#010x %#010x\n", \
			eax, regs.eax, regs.ebx, regs.ecx, regs.edx);
	}

	printf("\n");
}


char SizeOfPhysicalAddressRange()
{
	int eax;
	CpuId_t regs; // eax, ebx, exc, edx
	char bitres;

	/* 
	* first check for 64-bit: CPUID.80000001:EDX[29] = 1
	* See 3.3.1 of Volume 3  (System Programming Guide)
	*/
	eax = 0x80000001;
	__cpuid(regs.i, eax);
	printf("CPUID[%.8X]  %#010x %#010x %#010x %#010x", \
		eax, regs.eax, regs.ebx, regs.ecx, regs.edx);
	bitres = _bittest((long*) &regs.edx, 29);
	if (bitres > 0) {

		/* 
		* physical address range is implementation-specific and 
		* indicated by CPUID.80000008H:EAX[bits 7-0]
		* Section 3.3.1 of Volume 3 (System Programming Guide)
		*/
		eax = 0x80000008;
		__cpuid(regs.i, eax);
		printf("\nCPUID[%.8X]  %#010x %#010x %#010x %#010x\n\n", \
			eax, regs.eax, regs.ebx, regs.ecx, regs.edx);

		return regs.eax & 0xff;
	}
	else 
	{
		printf("not 64-bit!\n\n");
	}

	return 0;
}
