OUTPUT_FORMAT("elf32-littleriscv")
OUTPUT_ARCH(riscv)
SECTIONS
{
	.text . :
	{
			_text = . ;
			*(.text)
			*(.strings)
			_etext = . ;
	}
	.tors :
	{
		___ctors = . ;
		*(.ctors)
		___ctors_end = . ;
		___dtors = . ;
		*(.dtors)
		___dtors_end = . ;
	}
	.data . :
	{
			_data = . ;
			*(.data)
			*(.gcc_exc*)
			___EH_FRAME_BEGIN__ = . ;
			*(.eh_fram*)
			___EH_FRAME_END__ = . ;
			LONG(0);
			_edata = . ;
	}
	.bss . :
	{
			_bss = . ; 
			*(.bss)
			*(COMMON)
			_ebss = . ;
			_end = . ;
	}
}
