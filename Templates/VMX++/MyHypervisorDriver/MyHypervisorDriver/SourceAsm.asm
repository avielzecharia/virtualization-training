PUBLIC Breakpoint

.code _text

;------------------------------------------------------------------------

; CODE HERE

;------------------------------------------------------------------------

Breakpoint PROC PUBLIC
int 3
ret
Breakpoint ENDP 

;------------------------------------------------------------------------

END                                                                                                                                                                                                                   