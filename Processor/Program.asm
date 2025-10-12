; solves square equation x^2-5x+6=0
;5

;6
push 1
popr rax; a=1

;10
push -198
popr rbx; b=-198
;25
push 6885
popr rcx; c=6885

;------------------

:5
    pushr rbx
    pushr rbx
    mul

push 4
pushr rax
pushr rcx

:10
    mul
    mul
    sub

sqrt
popr rdx; d=sqrt(b^2-4ac)


;-------x1----------

pushr rdx
pushr rbx
sub

jmp :5

push 2
div
pushr rax
div

popr r8; r8=x1

;------x2-----------

pushr rdx
pushr rbx
add

push 2
pushr rax
mul
div

push -1
mul

popr r9; r9=x2

;-------out----------

pushr r8
out

pushr r9
out


hlt



;7
    ;here we can do something
    ;for exaample, 
    pushr rax
    push 10

    popr r15

;9
    ;here we can do something
    ;for exaample, 
    pushr rax
    push 10

    popr r15

