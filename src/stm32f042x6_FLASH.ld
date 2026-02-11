_Min_Heap_Size  = 0x200;  /* required heap (bytes in hex) - Adjust to MCU! */
_Min_Stack_Size = 0x400;  /* required stack (bytes in hex) - Adjust to MCU! */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 32K   /* Adjust to MCU! */
  RAM (xrw)    : ORIGIN = 0x20000000, LENGTH = 6K    /* Adjust to MCU! */
}

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  /* Interrupt vector table - must be at start of FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))  /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* Program code and constants */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* ARM to Thumb veneer stubs */
    *(.glue_7t)        /* Thumb to ARM veneer stubs */
    *(.eh_frame)       /* Exception frames */

    KEEP (*(.init))    /* Startup routines */
    KEEP (*(.fini))    /* Cleanup routines */

    . = ALIGN(4);
    _etext = .;        /* End of code section symbol */
  } >FLASH

  /* Read-only data (constants) */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants) */
    *(.rodata*)        /* .rodata* sections (constants) */
    . = ALIGN(4);
  } >FLASH

  /* ARM exception unwinding tables (C++ exception handling) */
  .ARM.extab :
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  /* ARM exception index (required for exception unwinding) */
  .ARM :
  {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >FLASH

  /* Pre-initialization array (C++ constructors called before main) */
  .preinit_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Initialization array (C++ constructors) */
  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Finalization array (C++ destructors) */
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Load address of .data initial values in FLASH */
  _sidata = LOADADDR(.data);

  /* Initialized data - stored in FLASH, copied to RAM at startup */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* Start of .data in RAM */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* Functions in RAM */
    *(.RamFunc*)

    . = ALIGN(4);
    _edata = .;        /* End of .data in RAM */
  } >RAM AT> FLASH

  /* Uninitialized data - zeroed by startup code */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;         /* Start of .bss in RAM */
    __bss_start__ = _sbss;
    *(.bss)            /* .bss sections */
    *(.bss*)           /* .bss* sections */
    *(COMMON)          /* Common symbols */

    . = ALIGN(4);
    _ebss = .;         /* End of .bss in RAM */
    __bss_end__ = _ebss;
  } >RAM

  /* Reserve space for heap and stack - linker checks available RAM */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );   /* Heap starts here */
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* Discard standard library debug info to save space */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  /* ARM metadata (not loaded to MCU, used by debugger/toolchain) */
  .ARM.attributes 0 : { *(.ARM.attributes) }
}