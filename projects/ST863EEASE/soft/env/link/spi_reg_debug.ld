    /* ------------------------------------------------ */
    /* SPI REG DEBUG - Ext RAM UNCACHED */
    /* ------------------------------------------------ */

#if (CHIP_ASIC_ID == 6 || CHIP_ASIC_ID == 8) /* GALLITE or 8808 */
#define ABB_REG_SIZE 0x100
#define PMU_REG_SIZE 0x100
#else /* 8809 or later */
#define ABB_REG_SIZE 0x140
#define PMU_REG_SIZE 0x140
#endif

#if (CHIP_ASIC_ID == 6) /* GALLITE */
#define FM_REG_SIZE  0x100
#else /* 8808 or later */
#define FM_REG_SIZE  0x0
#endif

#if (CHIP_ASIC_ID == 6) /* GALLITE */
#if defined(GALLITE_IS_8805)
#define XCV_REG_SIZE 0x300
#else /* 8806 */
#define XCV_REG_SIZE 0x400
#endif
#else /* 8808 or later */
#define XCV_REG_SIZE 0x500
#endif

#ifdef LINK_BCPU_IMAGE

    /* Just leave a hole for spi reg debug */
    . += XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE + PMU_REG_SIZE;

#else

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    _ram_spi_reg_debug_uncached_start = .;

    .spi_reg_debug (_ram_spi_reg_debug_uncached_start) (NOLOAD) :
        AT (ADDR(.spi_reg_debug))
    {
        _spi_reg_debug_start = .;
        *(.xcv_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE;
        *(.abb_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE;
#if (FM_REG_SIZE > 0)
        *(.fm_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE;
#endif
        *(.pmu_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE + PMU_REG_SIZE;
    }

    _ram_spi_reg_debug_uncached_end = .;
    _ram_spi_reg_debug_uncached_size = _ram_spi_reg_debug_uncached_end - _ram_spi_reg_debug_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;
    
#endif

