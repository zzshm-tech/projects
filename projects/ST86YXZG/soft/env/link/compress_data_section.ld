
_ram_bss_overlay_start = .;
OVERLAY . : NOCROSSREFS AT (_compressed_code_load_start)
{
    .ram_bss_overlay_array
    {
        _ram_bss_overlay_array_start = .;
		KEEP(*(.overlyarray))
		 LONG(0)
		. = ALIGN(0x10);
        _ram_bss_overlay_array_end = .;
    }

    .ram_bss_overlay_java_common
    {
        _ram_bss_overlay_java_common_start = .;
        _CTORS_START_CORE = .;
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .ctors))
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .ctors.*))
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .ctor))
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .ctor.*))
        _CTORS_END_CORE = .;

        _JCR_START_CORE = .;
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .jcr))
        KEEP(*(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .jcr.*))
        LONG(0)
        gcj_*.o(.data .data.*)
        *.oo(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .data)
        *.oo(EXCLUDE_FILE(EXCLUDE_JAVA_APP_LIST) .data.*)
        LONG(0)
        . = ALIGN(0x10);
        _ram_bss_overlay_java_common_end = .;
    }
}

/* GNU ld: The current output location counter between OVERLAY end and next output section 
 * is set to the end of last OVERLAY section. However, if it is specified as VMA of next output
 * section implicitly or explicitly, its value is what you expect,
 * OVERLAY start + size of largest OVERLAY section. */
.dummy_ram_bss_overlay_end . (NOLOAD) :
{
    _ram_bss_overlay_end = ABSOLUTE(.);
}

/* JAVA APP overlay starts from the end of JAVA common codes, and it should end before the end of BSS overlay */
. = ABSOLUTE(_ram_bss_overlay_java_common_end);
_ram_bss_overlay_app_start = .;
OVERLAY . : NOCROSSREFS AT (_compressed_code_load_start + SIZEOF(.ram_bss_overlay_array) + SIZEOF(.ram_bss_overlay_java_common))
{
    .ram_bss_overlay_app_qq
    {
	KEEP(*KQQ3_Common_out.oo(.ctors .ctors.* .ctor .ctor.*))
	KEEP(*QQ2007_128x128_out.oo(.ctors .ctors.* .ctor .ctor.*))
	KEEP(*QQ2007_128x160_out.oo(.ctors .ctors.* .ctor .ctor.*))
	KEEP(*QQ2007_176x220_out.oo(.ctors .ctors.* .ctor .ctor.*))
	KEEP(*QQ2009S_240x320_out.oo(.ctors .ctors.* .ctor .ctor.*))
	LONG(0x12345678)
	KEEP(*KQQ3_Common_out.oo(.jcr .jcr.*))
	KEEP(*QQ2007_128x128_out.oo(.jcr .jcr.*))
	KEEP(*QQ2007_128x160_out.oo(.jcr .jcr.*))
	KEEP(*QQ2007_176x220_out.oo(.jcr .jcr.*))
	KEEP(*QQ2009S_240x320_out.oo(.jcr .jcr.*))
	LONG(0)
	*KQQ3_Common_out.oo(.data .data.*)
	*QQ2007_128x128_out.oo(.data .data.*)
	*QQ2007_128x160_out.oo(.data .data.*)
	*QQ2007_176x220_out.oo(.data .data.*)
	*QQ2009S_240x320_out.oo(.data .data.*)
	LONG(0)
	. = ALIGN(0x10);
    }

	.ram_bss_overlay_app_mreader
    {
        KEEP(*mreader-ziyou-01.14.10-zh_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*mreader-ziyou-01.14.10-zh_out.oo(.jcr .jcr.*))
        LONG(0)
        *mreader-ziyou-01.14.10-zh_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_ebuddy
    {
        KEEP(*ebuddy11_88z9n2z8_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*eBuddy_nokia_2600_classic_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*eBuddy_nokia_n73_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*ebuddy11_88z9n2z8_out.oo(.jcr .jcr.*))
        KEEP(*eBuddy_nokia_2600_classic_out.oo(.jcr .jcr.*))
        KEEP(*eBuddy_nokia_n73_out.oo(.jcr .jcr.*))
        LONG(0)
        *ebuddy11_88z9n2z8_out.oo(.data .data.*)
        *eBuddy_nokia_2600_classic_out.oo(.data .data.*)
        *eBuddy_nokia_n73_out.oo(.data .data.*)        	
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_fruitgame
    {
        KEEP(*qiexigua_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*fruitNinja_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*qiexigua_out.oo(.jcr .jcr.*))
        KEEP(*fruitNinja_out.oo(.jcr .jcr.*))
        LONG(0)
        *qiexigua_out.oo(.data .data.*)
        *fruitNinja_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_skype
    {
        KEEP(*Skype_lite_1.2.11_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Skype_lite_1.2.11_out.oo(.jcr .jcr.*))
        *Skype_lite_1.2.11_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_msncn
    {
        KEEP(*msn_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*msn_out.oo(.jcr .jcr.*))
        LONG(0)
        *msn_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_facebook
    {
        KEEP(*Facebook_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*snaptu_1.7.1_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Facebook_out.oo(.jcr .jcr.*))
        KEEP(*snaptu_1.7.1_out.oo(.jcr .jcr.*))
        LONG(0)
        *Facebook_out.oo(.data .data.*)
        *snaptu_1.7.1_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_sinawb
    {
        KEEP(*weibo_1025001010_3333_unsigned_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*weibo_1025001010_3333_unsigned_out.oo(.jcr .jcr.*))
        LONG(0)
        *weibo_1025001010_3333_unsigned_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_mxit
    {
        KEEP(*mxit0_sunsang_other_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*mxit0_sunsang_other_out.oo(.jcr .jcr.*))
        *mxit0_sunsang_other_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    } 


    .ram_bss_overlay_app_operamini
    {
        KEEP(*opera-mini-6.5.26955-advanced-zh_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*opera-mini-6.5.26955-advanced-en_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*UCBrowser_V8.0.3.99_Java_pf71_Build11092616_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*UCBrowser_en_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*opera-mini-6.5.26955-advanced-zh_out.oo(.jcr .jcr.*))
        KEEP(*opera-mini-6.5.26955-advanced-en_out.oo(.jcr .jcr.*))
        KEEP(*UCBrowser_V8.0.3.99_Java_pf71_Build11092616_out.oo(.jcr .jcr.*))
        KEEP(*UCBrowser_en_out.oo(.jcr .jcr.*))
        *opera-mini-6.5.26955-advanced-zh_out.oo(.data .data.*)
        *opera-mini-6.5.26955-advanced-en_out.oo(.data .data.*)
        *UCBrowser_V8.0.3.99_Java_pf71_Build11092616_out.oo(.data .data.*)
        *UCBrowser_en_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    } 

    .ram_bss_overlay_app_tencentwb
    {
        KEEP(*Microblog_2.1.0.224_Java_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*tencentweibo_1.0.0.45_java_mini_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Microblog_2.1.0.224_Java_out.oo(.jcr .jcr.*))
        KEEP(*tencentweibo_1.0.0.45_java_mini_out.oo(.jcr .jcr.*))
        LONG(0)
        *Microblog_2.1.0.224_Java_out.oo(.data .data.*)
        *tencentweibo_1.0.0.45_java_mini_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_angrybirds
    {
        KEEP(*Angrybirds_*.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Angrybirds_*.oo(.jcr .jcr.*))
        LONG(0)
        *Angrybirds_*.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }
    
    .ram_bss_overlay_app_buyudaren
    {
        KEEP(*buyudaren-shlq_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*SHLQ_S700_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*buyudaren-shlq_out.oo(.jcr .jcr.*))
        KEEP(*SHLQ_S700_out.oo(.jcr .jcr.*))
        LONG(0)
        *buyudaren-shlq_out.oo(.data .data.*)
        *SHLQ_S700_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

    .ram_bss_overlay_app_nimbuzz
    {
        KEEP(*Nimbuzz_lite_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*Nimbuzz_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Nimbuzz_lite_outt.oo(.jcr .jcr.*))
        KEEP(*Nimbuzz_out.oo(.jcr .jcr.*))
        LONG(0)
        *Nimbuzz_lite_out.oo(.data .data.*)
        *Nimbuzz_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }
    .ram_bss_overlay_app_gmail
    {
        KEEP(*Gmail_2.06_tbmh_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*Gmail_2.06_tbmh_out.oo(.jcr .jcr.*))
        LONG(0)
        *Gmail_2.06_tbmh_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }
    .ram_bss_overlay_app_twitter
    {
        KEEP(*twitter_out.oo(.ctors .ctors.* .ctor .ctor.*))
        KEEP(*Twim_out.oo(.ctors .ctors.* .ctor .ctor.*))
        LONG(0x12345678)
        KEEP(*twitter_out.oo(.jcr .jcr.*))
        KEEP(*Twim_out.oo(.jcr .jcr.*))
        LONG(0)
        *twitter_out.oo(.data .data.*)
        *Twim_out.oo(.data .data.*)
        LONG(0)
        . = ALIGN(0x10);
    }

}

/* GNU ld: The current output location counter between OVERLAY end and next output section 
 * is set to the end of last OVERLAY section. However, if it is specified as VMA of next output
 * section implicitly or explicitly, its value is what you expect,
 * OVERLAY start + size of largest OVERLAY section. */
.dummy_ram_bss_overlay_app_end . (NOLOAD) :
{
    _ram_bss_overlay_app_end = ABSOLUTE(.);
}

_gcj_max_ram_size = SIZEOF(.ram_bss_overlay_java_common) + MAX(SIZEOF(.ram_bss_overlay_app_qq),MAX(SIZEOF(.ram_bss_overlay_app_mreader),MAX(SIZEOF(.ram_bss_overlay_app_ebuddy),MAX(SIZEOF(.ram_bss_overlay_app_fruitgame),MAX(SIZEOF(.ram_bss_overlay_app_skype),MAX(SIZEOF(.ram_bss_overlay_app_msncn),MAX(SIZEOF(.ram_bss_overlay_app_facebook),MAX(SIZEOF(.ram_bss_overlay_app_sinawb),MAX(SIZEOF(.ram_bss_overlay_app_mxit),MAX(SIZEOF(.ram_bss_overlay_app_operamini),MAX(SIZEOF(.ram_bss_overlay_app_tencentwb),MAX(SIZEOF(.ram_bss_overlay_app_angrybirds),MAX(SIZEOF(.ram_bss_overlay_app_buyudaren),MAX(SIZEOF(.ram_bss_overlay_app_nimbuzz),MAX(SIZEOF(.ram_bss_overlay_app_gmail),MAX(SIZEOF(.ram_bss_overlay_app_twitter),0))))))))))))))));

_gcj_flash_overly_size = SIZEOF(.ram_bss_overlay_array) + SIZEOF(.ram_bss_overlay_java_common) + SIZEOF(.ram_bss_overlay_app_qq) + SIZEOF(.ram_bss_overlay_app_mreader) + SIZEOF(.ram_bss_overlay_app_ebuddy) + SIZEOF(.ram_bss_overlay_app_fruitgame) + SIZEOF(.ram_bss_overlay_app_skype) + SIZEOF(.ram_bss_overlay_app_msncn) + SIZEOF(.ram_bss_overlay_app_facebook) + SIZEOF(.ram_bss_overlay_app_sinawb) + SIZEOF(.ram_bss_overlay_app_mxit) + SIZEOF(.ram_bss_overlay_app_operamini) + SIZEOF(.ram_bss_overlay_app_tencentwb) +SIZEOF(.ram_bss_overlay_app_angrybirds) + SIZEOF(.ram_bss_overlay_app_buyudaren) + SIZEOF(.ram_bss_overlay_app_nimbuzz) + SIZEOF(.ram_bss_overlay_app_gmail) + SIZEOF(.ram_bss_overlay_app_twitter);

/* JAVA APP overlay should end before the end of BSS overlay */
LD_ASSERT(ABSOLUTE(_ram_bss_overlay_end) >= ABSOLUTE(_ram_bss_overlay_app_end), "EXTS", "Native Java bss  overlay  overflow !")

/* Update the address of _compressed_code_load_start for code compression */
_compressed_code_load_start = _compressed_code_load_start + _gcj_flash_overly_size;

/* Set the location counter to the end of BSS overlay */
. = ABSOLUTE(_ram_bss_overlay_end);

