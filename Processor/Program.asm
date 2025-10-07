; solves square equation x^2-5x+6=0

push 1
popr rax; a=1

push -198
popr rbx; b=-198

push 6885
popr rcx; c=6885

;------------------

pushr rbx
pushr rbx
mul

push 4
pushr rax
pushr rcx

mul
mul
sub

sqrt
popr rdx; d=sqrt(b^2-4ac)


;-------x1----------

pushr rdx
pushr rbx
sub


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