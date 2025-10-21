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

; call :11

; ;push rax

; push 0
; popr rcx

; :101
; pushr rax
; pushr rcx
; jbe :100
;     hlt
; :100
;     pushm [rcx]
;     out

;     push 1
;     pushr rcx
;     add
;     popr rcx

;     jmp :101

; hlt
; ;govnocod

; :1              ;factorial function
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
;     ja :2

;     ret
;     :2
;         call :1
;         ret

; ;----------------------


; :11             ;factorialS function
;     push 1
;     push 1
;     popr rcx
;     popm [rcx]

;     :12
;         pushr rax
;         pushr rcx
;         jb :13
;             pushm [rcx]
;             popr rbx
;             ret

;         :13
;             pushm [rcx]


;             ;pushm [rbx]

;                 push 1
;                 pushr rcx
;                 add
;                 popr rcx
            
;             pushr rcx
;             mul
            

;             popm [rcx]
;             jmp :12


push 10
popr rax

push 9999
popr rbx

push 52
popv [rax]

push 34
popv [rbx]

pushv [rax]
pushv [rbx]

out
out

hlt