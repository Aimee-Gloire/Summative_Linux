global _start

SECTION .data
    filename:       db "sensor_log.txt", 0
    prefix:         db "Total sensor readings: ", 0
    newline:        db 10, 0

SECTION .bss
    buffer:     resb 1024
    count:      resq 1
    numbuf:     resb 32        ; plenty of space for integer string

SECTION .text

; ----------------------------------------------------------
; MAIN ENTRY
; ----------------------------------------------------------
_start:
    ; Open file (sys_open)
    mov rax, 2
    mov rdi, filename
    mov rsi, 0
    mov rdx, 0
    syscall
    cmp rax, 0
    jl error
    mov r12, rax          ; file descriptor

    mov qword [count], 0  ; reset count

read_loop:
    mov rax, 0            ; sys_read
    mov rdi, r12
    mov rsi, buffer
    mov rdx, 1024
    syscall
    cmp rax, 0
    je finish_reading

    mov rcx, rax          ; bytes read
    mov rbx, buffer

count_loop:
    cmp rcx, 0
    je read_loop

    mov al, [rbx]
    cmp al, 10           ; '\n'
    jne skip
    inc qword [count]

skip:
    inc rbx
    dec rcx
    jmp count_loop

finish_reading:
    mov rax, 3            ; sys_close
    mov rdi, r12
    syscall

    ; Print prefix
    mov rdi, prefix
    call print_string

    ; Convert number to string
    mov rax, [count]
    mov rdi, numbuf
    call utoa

    ; Print number
    mov rdi, numbuf
    call print_string

    ; Print newline
    mov rdi, newline
    call print_string

    jmp exit_success

; ----------------------------------------------------------
; print_string: prints null-terminated string at RDI
; ----------------------------------------------------------
print_string:
    mov rsi, rdi
.next:
    mov al, [rsi]
    cmp al, 0
    je .done
    inc rsi
    jmp .next
.done:
    ; rdi = start, rsi = end → length = rsi - rdi
    mov rdx, rsi
    sub rdx, rdi           ; length
    mov rax, 1             ; sys_write
    mov rsi, rdi
    mov rdi, 1             ; stdout
    syscall
    ret

; ----------------------------------------------------------
; utoa: unsigned integer in RAX → string at RDI
; ----------------------------------------------------------
utoa:
    mov rcx, 0             ; digit count
.conv:
    xor rdx, rdx
    mov rbx, 10
    div rbx
    add dl, '0'
    push rdx               ; save digit
    inc rcx
    cmp rax, 0
    jne .conv

    mov rbx, rdi           ; write pointer

.write:
    pop rdx
    mov [rbx], dl
    inc rbx
    loop .write

    mov byte [rbx], 0      ; null terminator
    ret

; ----------------------------------------------------------
; Exit routines
; ----------------------------------------------------------
exit_success:
    mov rax, 60
    xor rdi, rdi
    syscall

error:
    mov rax, 60
    mov rdi, 1
    syscall
