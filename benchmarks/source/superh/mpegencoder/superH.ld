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
