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
/*  - PRELOADED_RAM : The external RAm is already preloaded, useful for ram_run or simu */

/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

#define MEM_CACHED_UNCACHED_OFFSET 0x20000000

EXTERN (__bcpu_start)

EXTERN (bcpu_romCsString bcpu_Monitor g_bcpuRomVersionNumber)

EXTERN (spp_SearchAmrMarker  spp_CipherCountInit spp_FcchDetectAsm spp_FcchDetect
        spal_IrqSetMask spp_DecodeBlock spp_Open
        spp_EqualizeNBurst spp_SearchFcch spp_EncodeBlock
        spal_ItlvItlv spp_10log10 g_sppNbFwequStartSt
        __bb_int_handler 
        )

EXTERN (spal_InvalidateDcache 
        spal_Open
        spal_VitacBusy
        spal_XcorTscxcNb
        spal_DbgHstSendEvent
        )

EXTERN (spc_AmrDecode spc_UpdateAmrCfg g_mailbox 
        spc_RxProcess g_spcCtxStruct AFSfilterCI
        spc_MboxToSppSpeechMode spc_IrqHandler)

EXTERN (atoi
        bsearch
        _ctype
        memchr
        memcmp
        memcpy
        memmove
        memscan
        memset
        qsort
        strcat
        strchr
        strrchr
        strcmp
        strcpy
        strcspn
        strlen
        strnlen
        strncat
        strncmp
        strncpy
        strnicmp
        strsep
        strspn
        strstr
        strtok
        strtok_r
        strtol
        strtoul
        vsnprintf
        vsprintf
        sprintf
        snprintf
        vsscanf
        sscanf)

EXTERN (g_spalMapVersion g_spcMapVersion g_sppMapVersion)

SECTIONS
{
    /* ========================================================= */
    /* INT ROM                                                   */
    /* ========================================================= */

    . = BCPU_ROM_BASE;
    _bcpu_rom_start = .;

    /* Go uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_rom_text . : AT(ADDR(.bcpu_rom_text) - MEM_CACHED_UNCACHED_OFFSET)
    {
        /* Specifies a section for the functions to put
           in ROM. */
        __bcpu_rom_main = .;
 #define ROTEXT_CONTENT
#if (CHIP_ASIC_ID == 9)
        #include "8809_bcpu_rom_content"
#else
#error "Unsupported Chip Id"
#endif
#undef ROTEXT_CONTENT
        . = ALIGN(0x10);
    }

    /* Go cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_rom_rodata . : AT(ADDR(.bcpu_rom_rodata))
    {
        /* Specifies a section for the functions to put
           in ROM. */
 #define RODATA_CONTENT
#if (CHIP_ASIC_ID == 9)
        #include "8809_bcpu_rom_content"
#else
#error "Unsupported Chip Id"
#endif
#undef RODATA_CONTENT
        . = ALIGN(0x10);
    }

    __bcpu_rom_end = .;

    /* ========================================================= */
    /* BB SRAM                                                   */
    /* ========================================================= */

    . = BB_SRAM_BASE;
    _bb_sram_romed_start = .;

    .bbsram . : AT(ADDR(.bbsram))
    {
        _bb_sram_romed_cached_start = .;
        *(BB_ITLV_SECTION)     
        . = ALIGN(0x4);
        _bb_sram_romed_itlv_buf_end = .;
        *(BB_SPC_CTX_SECTION)
        . = ALIGN(0x10);
    }
    _bb_sram_romed_cached_end = .;
    _bb_sram_romed_cached_size = _bb_sram_romed_cached_end - _bb_sram_romed_cached_start;

    . += 0x20000000;

    .bbsramu . : AT(ADDR(.bbsramu))
    {
        _bb_sram_romed_uncached_start = .;
        . = ALIGN(0x10);
        _mbbsramu = .;
        *(BB_STATIC_SECTION)
        . = ALIGN(0x10);
    } 
    _bb_sram_romed_uncached_end = .;
    _bb_sram_romed_uncached_size = _bb_sram_romed_uncached_end - _bb_sram_romed_uncached_start;

    _bb_sram_romed_size = _bb_sram_romed_cached_size + _bb_sram_romed_uncached_size;
    _bb_sram_romed_end = _bb_sram_romed_start + _bb_sram_romed_size;

    .bbsram_globals (BB_SRAM_BASE + BB_SRAM_SIZE - RESERVED_PATCH_SIZE - BB_SRAM_GLOBALS_SIZE) : AT(ADDR(.bbsram_globals))
    {
        _bb_sram_romed_globals_start = .;
        *(BB_SRAM_GLOBALS)
        *(.bbsram_globals)
        _bb_sram_romed_globals_end = .;
    }

    /* BBSRAM overflow */
    LD_ASSERT(_bb_sram_romed_end <= _bb_sram_romed_globals_start, "BBSO", "BBSRAM Overflozed !!")
    LD_ASSERT(SIZEOF(.bbsram_globals) <= BB_SRAM_GLOBALS_SIZE, "BBGO", "BBSRAM  Globals Overflozed !!")

    /* ========================================================= */
    /* DUALPORT SRAM                                             */
    /* ========================================================= */
    .mailbox (DUALPORT_SRAM_BASE + DUALPORT_SRAM_SIZE - RESERVED_MBX_SIZE + 0x20000000) : AT(ADDR(.mailbox))
    {
        _dualport_sram_romed_mbx_start = .;
        *spc_mailbox.o(.mailbox)
        . = ALIGN(0x10);
    } 

    /* Mailbox overflow */
    LD_ASSERT(SIZEOF(.mailbox) <= RESERVED_MBX_SIZE, "MBXO", "Mailbox Overlfowed !!")

    /* ========================================================= */
    /* Back in the INT ROM                                       */
    /* ========================================================= */

    .bcpu_rom_version_number (BCPU_ROM_END - 4) : AT(ADDR(.bcpu_rom_version_number))
    {
        _bcpu_rom_version_number = .;
        *(.bcpu_rom_version_number)
    }

    .bcpu_rom.crc BCPU_ROM_END : AT(ADDR(.bcpu_rom.crc))
    {
        __bcpu_rom_crc = . ;
        LONG(0xc001c001)
    }

    /* All other sections (including .bss and .data) or files should NOT exist in BCPU ROM.
     * TIP:
     * The global or static variables can be in BB_SRAM_GLOBALS (for asm files) or
     * .bbsram_globals section (for C files), and their initialization codes can be put
     * in bcpu_main or spc_GnrlReset.
     */
    /DISCARD/ :
    {
        *(*)
    }
}

