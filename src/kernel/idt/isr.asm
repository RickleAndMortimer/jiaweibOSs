extern exception_handler

isr_common_stub:
	call exception_handler
	iretq

global isr_common_stub
