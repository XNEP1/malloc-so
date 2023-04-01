	.text
    .global	bbrk
bbrk:
    pushq   %rbp
    movq    %rsp, %rbp
    movq    $0, %rsi
    cmp     %rdi, %rsi
    je      bbrk_if_end_1
    movq    %rdi, %rsi
    movq    $0, %rdi
    movq    $12, %rax
    syscall
    movq    %rax, %rdi
    addq    %rsi, %rdi
bbrk_if_end_1:
    movq    $12, %rax
    syscall
    popq    %rbp
    ret


    .global set_bbrk
set_bbrk:
    pushq   %rbp
    movq    %rsp, %rbp
    movq    $12, %rax
    syscall
    popq    %rbp
    ret
