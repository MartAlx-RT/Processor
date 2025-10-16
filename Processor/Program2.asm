; factorial
; rax - argument
; rbx - returning value

;----------
push 1
popr rbx
;----------

push 15          ;calculates (rax)!
popr rax


;----------

call :11

push 0
popr rcx

:101
pushr rax
pushr rcx
jbe :100
    hlt
:100
    pushm [rcx]
    out

    push 1
    pushr rcx
    add
    popr rcx

    jmp :101

hlt


:1              ;factorial function
    pushr rax
    pushr rbx
    mul
    popr rbx

    pushr rax
    push 1
    sub
    popr rax
    
    push 1
    pushr rax
    ja :2

    ret
    :2
        call :1
        ret

;----------------------


:11             ;factorialS function
    push 1
    push 1
    popr rcx
    popm [rcx]

    :12
        pushr rax
        pushr rcx
        jb :13
            pushm [rcx]
            popr rbx
            ret

        :13
            pushm [rcx]

                push 1
                pushr rcx
                add
                popr rcx
            
            pushr rcx
            mul
            

            popm [rcx]
            jmp :12
