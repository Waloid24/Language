global _start

section .text

buf: times 128 db 0

_start:
    ; mov rax, buf
    ; mov rcx, buf
    ; mov rdx, buf
    ; mov rbx, buf
    ; mov rsp, buf
    ; mov rbp, buf
    ; mov rsi, buf
    ; mov rdi, buf
    ; mov r8, buf
    ; mov r9, buf
    ; mov r10, buf
    ; mov r11, buf
    ; mov r12, buf
    ; mov r13, buf
    ; mov r14, buf
    ; mov r15, buf

    push QWORD [r15 + 0]
    push QWORD [r15 + 1]
    push QWORD [r15 + 2]
    push QWORD [r15 + 3]
    push QWORD [r15 + 4]
    push QWORD [r15 + 5]
    push QWORD [r15 + 6]
    push QWORD [r15 + 7]
    push QWORD [r15 + 8]
    push QWORD [r15 + 9]
    push QWORD [r15 + 10]
    push QWORD [r15 + 257]
    push QWORD [r15 + 34563342]
    push QWORD [r15 + 345633428]


    mov rax, 0x3C		;exit64(rdi)
    xor rdi, rdi
    syscall




