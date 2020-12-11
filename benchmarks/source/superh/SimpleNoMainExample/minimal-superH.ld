OUTPUT_FORMAT("coff-sh")
OUTPUT_ARCH(sh)
SECTIONS
{
	.text . :
	{
			_text = . ;
			*(.text)
			_etext = . ;
	}
	.data . :
	{
			_data = . ;
			*(.data)
			_edata = . ;
	}
	.bss . :
	{
			_bss = . ; 
			*(.bss)
			_ebss = . ;
			_end = . ;
	}
}
