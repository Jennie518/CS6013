; Example assembly program to print an integer and exit

section .text
global _start

_start:
    mov rdi, 1234       ; Move the integer to be printed into rdi
    call print_int      ; Call the print_int function
    mov rax, 60         ; sys_exit system call number
    xor rdi, rdi        ; Exit code 0
    syscall             ; Invoke the system call to exit

print_int:
    push rbp            ; Save the base pointer
    mov rbp, rsp        ; Set base pointer to current stack pointer
    sub rsp, 20         ; Allocate 20 bytes on stack for character storage

    mov rax, rdi        ; Copy the integer to rax for manipulation
    mov rbx, 1          ; Set rbx to 1, used as a counter/index
    mov byte [rsp+1], 10; Store a line feed character at the beginning of the buffer

    ; Main loop to extract digits
    dividing:
        cmp rax, 0
        je print
        mov rdx, 0
        mov rcx, 10
        div rcx
        add rdx, 48
        inc rbx
        mov [rsp+rbx], dl
        jmp dividing

    ; Procedure to print the extracted digits
    print:
        mov rax, 1       ; sys_write system call number
        mov rdi, 1       ; File descriptor for stdout
        lea rsi, [rsp+1] ; Address of the buffer to write
        mov rdx, rbx     ; Number of digits to print
        syscall          ; Invoke the system call to print
        add rsp, 20      ; Cleanup the allocated stack space
        pop rbp          ; Restore the base pointer
        ret              ; Return from the function

