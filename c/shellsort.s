.att_syntax

.section .rodata
    /* Gap sequence of 57, 23, 10, 4, 1; due to Ciura, 2005 */
    GAPS:
        .string "\X39\X17\X0A\X04\X01\X00"
    ELEMENT:
        .string "%d \n"
    NEWLINE:
        .string "\n"
    DATA:
        .string "r13: 0x%X\nr13 + rax: 0x%X\n"

.text
.globl  shellsort_int

shellsort_int:
    /* Exit if there are one or fewer elements (already sorted) */
    ; mov     $0x1, %rax
    ; cmp     %rax, %rsi
    ; jle     exit    

    mov     $GAPS, %rbx         /* Initialize gap sequence array */
    offset_loop:
        movb    (%rbx), %r8b    /* Move the next gap into r8 */
        cmp     %r8, %rsi       /* Check if gap is larger than # of elements */
        setge   %r9b            /* Set r9's L.O. byte to r8 > rdx */
        lea     (%rbx, %r9), %rbx   /* Increment gap pointer */
        jge     offset_loop     /* Restart loop if incremented */

        mov     %rdi, %r13      /* Initialize data pointer (r13) */
        mov     %rdx, %r14      /* Move datum size into r14 */
        mov     %rcx, %r15      /* Move cmp function pointer into r15 */
        movb    $0x40, %cl      /* Put bus size into L.O. bits of rcx */
        lea     (, %r14, 0x8), %r14
        sub     %r14, %rcx      /* Subtract datum size from bus size */

        mov     %rsi, %rax      /* Move # of elements into rax */
        mul     %r14            /* Multiply # of elements by datum size */
        lea     (%rdi, %rax), %r12  /* Compute array boundary memory address */

        mov     (%rbx), %rax       /* Move current gap into rax */

    gap_loop:
        mul     %r14            /* Multiply gap by datum size */
        add     %rax, %r13      /* Offset data pointer by gap elements */

        sort_loop:
            mov     (%r13), %r9 /* Read next 64 bits into r9 */
            shr     %cl, %r9    /* Shift size H.O. bits of r9 into L.O. bits */

            push    %rax
            push    %rcx
            push    %rsi
            push    %rdi
            mov     $ELEMENT, %rdi
            mov     %r9, %rsi
            movq    $0, %rax
            call    printf
            pop     %rdi
            pop     %rsi
            pop     %rcx
            pop     %rax

            add     %rax, %r13  /* Step data pointer */
            cmp     %r12, %r13  /* Check if the pointer is still in bounds */
            jg      sort_loop   /* Restart loop if so */

        push    %rax
        push    %rcx
        push    %rdi
        mov     $NEWLINE, %rdi
        movq    $0, %rax
        call    printf
        pop     %rdi
        pop     %rcx
        pop     %rax

        inc     %rbx            /* Increment gap pointer by one byte */
        movb    (%rbx), %al     /* Load next element into L.O. byte of rax */
        test    %rax, %rax      /* Check if the new gap is zero */
        jnz     gap_loop        /* Restart loop if not */

    exit:
        ret
