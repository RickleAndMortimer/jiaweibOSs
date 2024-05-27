%macro pushad 0
    push rax      
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro popad 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx      
    pop rcx
    pop rax
%endmacro

%macro isr_err_stub 1
isr_stub_%+%1:
    push %1
    jmp isr_common_stub
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

section .text
global isr_stub_table
global irq_stub_table
extern exception_handler
extern irq_handler

isr_common_stub:
    pushad
    cld 
    lea rdi, [rsp]
    call exception_handler
    popad
    add rsp, 8
    iretq

irq_common_stub:
    pushad
    cld
    lea rdi, [rsp]
    call irq_handler
    popad
    add rsp, 8
    iretq

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

%assign i 32
%rep 	223
irq_stub_%+i:
    cli
    push i
    jmp irq_common_stub
    %assign i i+1
%endrep

isr_stub_table:
%assign i 0 
%rep    32 
    DQ isr_stub_%+i 
    %assign i i+1 
%endrep

irq_stub_table:
%assign i 32
%rep 	223
    DQ irq_stub_%+i
    %assign i i+1
%endrep
