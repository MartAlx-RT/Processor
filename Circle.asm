push 1000
popr rdx

;(a-50)^2 + (b-50)^2 < d


push 0
popr rax
push 0
popr rbx

    :outer_for
push 100
pushr rax
    jae :end_outer_for
    push 0
    popr rbx
        :inner_for
    push 100
    pushr rbx
        jae :end_inner_for
        
        pushr rax
        push -50
        add
        popr r8
        call :SQARE(r8)
        pushr r8
        popr r10

        pushr rbx
        push -50
        add
        popr r8
        call :SQARE(r8)
        pushr r8
        popr r11

        pushr r10
        pushr r11
        add
        pushr rdx
        jb :notdraw
        
        call :VRAM_write
        
        :notdraw
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
    call :COORDINATE(rax,rbx->rcx)
    
    push 70
    popv [rcx]

    ret

:SQARE(r8)
    pushr r8
    pushr r8
    mul
    popr r8
    ret

:COORDINATE(rax,rbx->rcx)
    push 100
    pushr rax
    mul
    pushr rbx
    add
    popr rcx
    ret