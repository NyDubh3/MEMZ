OUTPUT_FORMAT("binary")

ENTRY(start)
codea = 0x7c00;
dataa = 0x0000;
SECTIONS
{
  .text : AT(codea) 
  {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(dataa)
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  end = .;
}
