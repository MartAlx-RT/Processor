;gradient picture


; int vram[100 * 100] = {};

;     for (int a = 0; a < 100; a++)
;     {
;         for (int b = 0; b < 100; b++)
;         {
;             vram[100 * a + b] = 70 * a / 100;
;         }
;     }



; rax - a
; rbx - b

push 0
popr rax
push 0
popr rbx

    :outer_for
    ;pushr rax
    ;out
push 100
pushr rax
    jae :end_outer_for
    push 0
    popr rbx
        :inner_for
    push 100
    pushr rbx
        jae :end_inner_for
        call :VRAM_write
        push 1
        pushr rbx
        add
        popr rbx
        jmp :inner_for
        :end_inner_for
    push 1
    pushr rax
    add
    popr rax
    jmp :outer_for
    :end_outer_for

drw
pause
hlt











:VRAM_write ;uses r8 (can change), rax, rbx
    push 100
    pushr rax
    mul
    pushr rbx
    add
    popr r8

    ; pushr r8
    ; out

    
    push 70
    pushr rax
    mul
    push 100
    div
    popr r9

    ; pushr r9
    ; out
    
    pushr r9
    popv [r8]


    ; pushr rax
    ; pushr rbx
    ; pushr r8
    ; pushr r9
    ; out
    ; out
    ; out
    ; out
    
    ;drw
    ;pause
    ret
















