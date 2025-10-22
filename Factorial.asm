
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

call :Factorials

;push rax

push 0
popr rcx

:condition
pause
pushr rax
pushr rcx
jbe :while
    hlt
:while
    pushm [rcx]
    out

    push 1
    pushr rcx
    add
    popr rcx

    jmp :condition

hlt
;govnocod

:Factorial              ;factorial function
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
    ja :if(rax>1)

    ret
    :if(rax>1)
        call :Factorial
        ret

;----------------------


:Factorials             ;factorialS function
    push 1
    push 1
    popr rcx
    popm [rcx]

    :Factorials_condition
        pushr rax
        pushr rcx
        jb :Factorials_while
            pushm [rcx]
            popr rbx
            ret

        :Factorials_while
            pushm [rcx]


            ;pushm [rbx]

                push 1
                pushr rcx
                add
                popr rcx
            
            pushr rcx
            mul
            

            popm [rcx]
            jmp :Factorials_condition