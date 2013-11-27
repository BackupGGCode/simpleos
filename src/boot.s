[BITS 32]

MBOOT_PAGE_ALIGN	equ 1<<0
MBOOT_MEM_INFO		equ 1<<1
MBOOT_HEADER_MAGIC	equ 0x1BADB002 ; Magic value
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .text

[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

mboot:
	dd  MBOOT_HEADER_MAGIC
	dd  MBOOT_HEADER_FLAGS
	dd  MBOOT_CHECKSUM
	dd mboot
		dd  code
		dd  bss
		dd  end
		dd  start

[GLOBAL start]
[EXTERN kernel_main]

start:
	cli					; Disable interrupts
	mov esp, stack		; Set up stack
	push ebx			; Push a pointer to the multiboot info structure
	mov ebp, 0			; Initialise the base pointer to zero
	call kernel_main
	jmp $
.end:

section .bss
	resb 32768
stack:
