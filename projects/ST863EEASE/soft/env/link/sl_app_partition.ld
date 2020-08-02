/* ======================================================================= */
/*                  TARGET CONFIGURATION SECTION                           */
/* ======================================================================= */
/* The following values are defined in ff_target.def and exported by the Makefile */
/*  FLASH_SIZE              */
/*  RAM_SIZE                */
/*  CALIB_BASE              */
/*  FACT_SETTINGS_BASE      */
/*  CODE_BASE               */
/*  USER_DATA_BASE          */
/*  USER_DATA_SIZE          */
/* ======================================================================= */
/*                  END CONFIGURATION SECTION                              */
/* ======================================================================= */
/* The linker script depends on the following defines */
/*  - CONST_IN_RAM : all constants are placed in RAM, this is useful for ram_run */
/*  - CODE_IN_RAM : All code is executed from RAM, this is useful for ram_run or code running from RAM on romulator */
/*  - PRELOADED_INT_SRAM : The internal SRAM are already preloaded, useful for ram_run */
/*  - PRELOADED_RAM : The external RAM is already preloaded, useful for ram_run or simu */
/*  - __NATIVE_JAVA__ :  The Native Java need  reload it's data and bss section every time when app start*/
/* Those are constants of the system */


/* ROM content selection: things to include
    WARNING: the DISCARD section uses the opposite -->
    do all inclusion before discard !*/


/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

#define MEM_CACHED_UNCACHED_OFFSET 0x20000000

/*Mandatory to have the BB side irq handler linked */
ENTRY (SL_Entry)

SECTIONS
{
	/* ----------------------------------------------- */
	/* FUNCTAB IN EXTERNAL RAM */
	/* ----------------------------------------------- */

	. = SL_FUNCTAB_BASE;
    _ram_start = .;
	_ram_functab_start = SL_FUNCTAB_BASE;
	.functab . : AT (ADDR(.functab))
	{
		*(.functab)
        . = ALIGN(0x10);
	}
	_ram_functab_end = .;
	_ram_functab_size = _ram_functab_end - _ram_functab_start;
	/* Check that functab on external RAM is not over stuffed (partition size is 2K bytes) */
	LD_ASSERT(ABSOLUTE(SL_APP_BASE) > ABSOLUTE(_ram_functab_end), "EXTS", "Functab on External RAM overflow !")

	/* ----------------------------------------------- */
	/* TEXT IN EXTERNAL RAM */
	/* ----------------------------------------------- */

	. = SL_APP_BASE;
    .rotext . : AT (ADDR(.rotext))
    {
		*(.app_entry)
		*(.rotext)
        . = ALIGN(0x10);
    }

     . = ALIGN(0x20); 
    .rodata . : AT (ADDR(.rodata))
    {
		*(.rodata)
        . = ALIGN(0x20);
    }

    /* ------------------------------------------------ */
    /* Ext RAM CACHED  - SECTIONS ALWAYS IN RAM */
    /* ------------------------------------------------ */
    .ram . : AT (ADDR(.ram))
    {
        _ram_special_cached_start = .;
        _ram_txt_start = .;
        *(.ram)
        . = ALIGN(0x10);
    }
    _ram_special_cached_end = .;
    _ram_special_cached_size = _ram_special_cached_end - _ram_special_cached_start;

    /*---------------------------- */
    /* Ext RAM CACHED TXT          */
    /*---------------------------- */
    .ramtext  _ram_special_cached_end  : AT (ADDR(.ramtext))
    {
        _ram_cached_txt_start = .;
        *(.text .text.* .stub .gnu.linkonce.t.*)
        . = ALIGN(0x10);
    } = 0
    _ram_cached_txt_end = .;
    ram_cached_txt_size = _ram_cached_txt_end - _ram_cached_txt_start;
    _ram_txt_end = .;

    /*---------------------- */
    /* Ext RAM UNCACHED Data */
    /*---------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .ucdata  . : AT (ADDR(.ucdata))
    {
        _ram_uncached_data_start = .;
        *(.ucdata)
        *(*.globalvars.uc) 
        . = ALIGN(0x10);
    }
    _ram_uncached_data_end = .;
    _ram_uncached_data_size = _ram_uncached_data_end - _ram_uncached_data_start;

    /*---------------------- */
    /* Ext RAM CACHED Data */
    /*---------------------- */
    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;
    .data   . : AT (ADDR(.data))
    {
        _ram_cached_data_start = . ;
        *(.rdata)
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
        *(.ctors) 
        . = ALIGN(0x10);

        /* NOTE: globalvars.c sections are to be removed.
         * They are kept here for backward compatibility of SVC/VPP codes.
         * These codes are linked from Total_BJ branch and the special global
         * variable sections will be removed in the future. */
        *(*.globalvars.c *.globalvars.c.*)

        *(EXCLUDE_FILE(gcj_*.o *.oo) .data)
        *(EXCLUDE_FILE(gcj_*.o *.oo) .data.*)
        *(EXCLUDE_FILE(gcj_*.o *.oo) .gnu.linkonce.d.*) 
        *(.data1)
        _gp = ALIGN(16) + 0x7ff0;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.lit8)
        *(.lit4)
        . = ALIGN(0x4);
        /***barrier : do not remove or edit */
        . = ALIGN(0x10);
    }
    _ram_cached_data_end = . ;
    _ram_cached_data_size = _ram_cached_data_end - _ram_cached_data_start;
    _ram_end = .;
    _ram_size = _ram_end - _ram_start;

    /*-------------------------------------------------------  */
    /* Uninitialized section in RAM (content kept after reboot)*/
    /*-------------------------------------------------------  */
    .backup . : AT (ADDR(.backup))
    {
        *(.backup)
        . = ALIGN(0x10);
    }

    /*-------------------------------------------------------  */
    /* RAM BSS (content reset to 0 )                           */
    /*-------------------------------------------------------  */
    _ram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .ucbss  . (NOLOAD) : AT (ADDR(.ucbss))
    {
        *(.ucbss)
        *(.ramucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bss    . (NOLOAD) : AT (ADDR(.bss))
    {
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }

    _ram_bss_end = .;
    _ram_bss_size = _ram_bss_end - _ram_bss_start;

    /*----------- */
    /* End of RAM */
    /*----------- */
    /* sky 20161024 modify for add cos heap size */
    /*_ram_top = SL_APP_BASE + SL_APP_SIZE; */
    /*_heap_size = _ram_top - _ram_bss_end; */
    /*_heap_start =  _ram_bss_end; */
    /*_heap_end =  _ram_bss_end + _heap_size; */
    _ram_top = SL_APP_BASE + SL_APP_SIZE + SL_APP_SIZE; 
    _heap_size = _ram_top - _ram_bss_end; 
    _heap_start =  _ram_bss_end; 
    _heap_end =  _ram_bss_end + _heap_size; 

    /* Check that the external RAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_ram_top) > ABSOLUTE(_ram_bss_end), "EXTS", "APP External RAM overflow !")
}

