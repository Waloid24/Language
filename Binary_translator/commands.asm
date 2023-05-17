global _start
section .text

_start:
    mov rax, rax
    mov rax, 1
    mov rax, -1
    mov rax, 4853245
    mov rax, -4853245
    mov rbx, 234654
    mov rcx, 234654
    mov rdx, 234654


    push rax
    push rcx
    push rbx
    push rdx


    push 5

    push QWORD [r15 + 5]

    ;CVTSI2SD_XMM0_RAX
    cvtsi2sd xmm0, rax


    jmp _start

    mov rax, 0x3C		;exit64(rdi)
    xor rdi, rdi
    syscall

