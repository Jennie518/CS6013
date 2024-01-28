section .data
helloMessage db 'Hello, world!', 0  ; 字符串以 null 字节终止

section .text
global sayHello
global myPuts
extern puts  ; 声明外部函数

sayHello:
    mov rdi, helloMessage  ; 将消息的地址放入第一个参数寄存器
    call puts              ; 调用 puts 函数
    ret

myPuts:
    mov rax, 1            ; 系统调用号 1 是 sys_write
    push rsi              ; 将字符串长度保存到栈中
    mov rsi, rdi          ; 将字符串地址从 rdi 移到 rsi
    pop rdx               ; 将字符串长度从栈中恢复到 rdx
    mov rdi, 1            ; 将文件描述符 1 放入 rdi
    syscall               ; 进行系统调用
    ret

section .text
global myGTOD

myGTOD:
    ; 分配空间给 timeval 结构
    sub rsp, 16          ; timeval 结构大小是 16 字节

    ; 设置系统调用参数
    mov rax, 96          ; 系统调用号 96 是 gettimeofday
    mov rdi, rsp         ; rdi 应该包含指向 timeval 结构的指针
    mov rsi, 0           ; rsi 为 NULL，因为我们不关心时区

    ; 执行系统调用
    syscall

    ; 从 timeval 结构中提取秒数和微秒数
    mov rax, [rsp]       ; 将秒数放入 rax
    mov rdx, [rsp + 8]   ; 将微秒数放入 rdx

    ; 释放栈空间
    add rsp, 16

    ; 返回
    ret

