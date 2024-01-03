PUBLIC Enable_VMX_Operation
PUBLIC ReadMsr
PUBLIC ReadCr4
PUBLIC Breakpoint

.code _text

;------------------------------------------------------------------------

; Intel Software Developer’s Manual (23.7 ENABLING AND ENTERING VMX OPERATION)
; enables VMX by setting CR4.VMXE[bit 13] = 1
; NOTE:	VMXON is also controlled by the IA32_FEATURE_CONTROL MSR (MSR address 3AH). 
; 		This MSR is cleared to zero when a logical processor is reset.
; 		0/1/2 bits (BIOS\in SMX\out SMX) might cause exceptions on VMXON.
Enable_VMX_Operation PROC PUBLIC
push rax			; Save the state

xor rax,rax			; Clear the RAX
mov rax,cr4
or rax,02000h		; Set the 14th bit
mov cr4,rax

pop rax				; Restore the state
ret
Enable_VMX_Operation ENDP

;------------------------------------------------------------------------

ReadMsr PROC
rdmsr				; MSR.index(ecx) -> edx:eax
shl		rdx, 32
or		rax, rdx
ret
ReadMsr ENDP

;------------------------------------------------------------------------

ReadCr4 PROC PUBLIC
mov		rax, cr4
ret
ReadCr4 ENDP

;------------------------------------------------------------------------

Breakpoint PROC PUBLIC
int 3
ret
Breakpoint ENDP 

;------------------------------------------------------------------------

END                                                                                                                                                                                                                   