#ifndef _CPUID_H_
#define _CPUID_H_

typedef union _CpuId
{
	int i[4];
	struct {
		int eax;
		int ebx;
		int ecx;
		int edx;
	};
} CpuId_t;

#endif