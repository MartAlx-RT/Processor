; gradient picture


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


:for2_condition
push 140
pushr rax

    jb :for2
pause
hlt
    :for2
        call :PrintLine

        push 1
        pushr rax
        add
        popr rax
    jmp :for2_condition




:PrintLine
    push 0
    popr rcx

    :for_condition
    push 70
    pushr rcx
        
        jb :for
    drw
    ;pause
    ret
    ;hlt
        :for
            pushr rax
            push 70
            mul
            pushr rcx
            add
            popr rdx

            pushr rcx
            popv [rdx]

            push 1
            pushr rcx
            add
            popr rcx
        jmp :for_condition
















:VRAM_write ;uses r1 (can change), rax, rbx
    pushr rbx
    push 100
    pushr rax
    mul
    add
    popr r8

    push 70
    pushr rax
    mul
    push 100
    div
    popv [r8]

















; ; factorial
; ; rax - argument
; ; rbx - returning value

; ;----------
; push 1
; popr rbx
; ;----------

; push 15          ;calculates (rax)!
; popr rax


; ;----------

; call :Factorials

; ;push rax

; push 0
; popr rcx

; :condition
; pushr rax
; pushr rcx
; jbe :while
;     hlt
; :while
;     pushm [rcx]
;     out

;     push 1
;     pushr rcx
;     add
;     popr rcx

;     jmp :condition

; hlt
; ;govnocod

; :Factorial              ;factorial function
;     pushr rax
;     pushr rbx
;     mul
;     popr rbx

;     pushr rax
;     push 1
;     sub
;     popr rax
    
;     push 1
;     pushr rax
;     ja :if(rax>1)

;     ret
;     :if(rax>1)
;         call :Factorial
;         ret

; ;----------------------


; :Factorials             ;factorialS function
;     push 1
;     push 1
;     popr rcx
;     popm [rcx]

;     :Factorials_condition
;         pushr rax
;         pushr rcx
;         jb :Factorials_while
;             pushm [rcx]
;             popr rbx
;             ret

;         :Factorials_while
;             pushm [rcx]


;             ;pushm [rbx]

;                 push 1
;                 pushr rcx
;                 add
;                 popr rcx
            
;             pushr rcx
;             mul
            

;             popm [rcx]
;             jmp :Factorials_condition