/*
* bpknock.cpp
* based on joanna rutkowska's bpknock (bluepill knock)
* this utility communicates to the VMM via calls to CPUID
* (which can be run in ring 3)
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*
* NOTE: VMCALL is the trivial way to implement that mechanism.
* Instead, we may trigger VM-Exit, for example, with specific instruction execution such as CPUID.
* CPUID gets DWORD and return 4 registers, so we can implement full send/recv using that opcode.
* In this POC, we are just sending a DWORD and getting a DWORD.
*/


ULONG32
__declspec(naked) /*<- indicates no compiler generated prolog/epilog */
NBPCall(ULONG32 knock) {
	__asm {
		push    ebp
		mov     ebp, esp
		push    ebx
		push    ecx
		push    edx
		cpuid
		pop     edx
		pop     ecx
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret
	}
}


int __cdecl main(int argc, char** argv) {
	ULONG32 knock;

	if (argc != 2)
	{
		printf("bpknock <magic knock>\n");
		return 1;
	}

	knock = strtoul(argv[1], 0, 0);
	__try
	{
		printf("knock answer: %#x\n", NBPCall(knock));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("CPUDID caused exception\n");
		return 2;
	}

	return 0;

}
