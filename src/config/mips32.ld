
OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradbigmips", "elf32-tradlittlemips")
OUTPUT_ARCH(mips)

SECTIONS
{
    . = 0x80000000;

    .text :
    {
         _ftext = ABSOLUTE(.);
        *(.text)
        *(.text.*)
        . = ALIGN(8);
        _etext = ABSOLUTE(.);
    }=0

    .data :
    {
        _fdata = ABSOLUTE(.);
        *(.rodata)
        *(.rodata.*)
        *(.data)
        . = ALIGN(8);
        _gp = ABSOLUTE(. + 0x7ff0);
        *(.lit8)
        *(.lit4)
        *(.sdata)
        . = ALIGN(8);
        _edata = ABSOLUTE(.);
    }=0

    .bss :
    {
        _fbss = ABSOLUTE(.);
        *(.sbss)
        *(.scommon)
        *(.bss)
        *(COMMON)
        . = ALIGN(8);
        _ebss = ABSOLUTE(.);
    }

    /DISCARD/ :
    {
        *(.reginfo)
        *(.MIPS.abiflag)
    }
}