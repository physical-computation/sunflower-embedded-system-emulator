OUTPUT_FORMAT("coff-sh")
OUTPUT_ARCH(sh)
SECTIONS
{
  	.text . :
  	{
     		_text = . ;
    		*(.text)
    		*(.strings)
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
    		*(COMMON)
     		_ebss = . ;
     		_end = . ;
  	}
}
