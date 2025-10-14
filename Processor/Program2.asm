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


call :1
pushr rbx
out

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
    ja :1

    ret