
XCPU_OVERLAY_BASE_FIRST = XCPU_OVERLAY_BASE;
OVERLAY XCPU_OVERLAY_BASE_FIRST : NOCROSSREFS AT (_compressed_code_load_start)
{
    /* .init section is used to list all init function which is only execlute once when power on*/
    .xcpu_overlay_init_func
    {
    	#include "compress_6464/section_1_1.ld"
	 LONG(0)
	. = ALIGN(0x20);
    }
    .xcpu_overlay_csdapp
    {
	LONG(0)
	    . = ALIGN(0x20);
    }
    .xcpu_overlay_wap
    {

    	 LONG(0)
    	. = ALIGN(0x20);
    }
    .xcpu_overlay_jmetoc
    {
        . = ALIGN(0x20);
    }
    .xcpu_overlay_other
    {
    	LONG(0)
    	 . = ALIGN(0x20);
    }
}

#define FLASH_SIZE_OF_XCPU_OVERLAY_0 0 + SIZEOF(.xcpu_overlay_init_func) + SIZEOF(.xcpu_overlay_jmetoc) + SIZEOF(.xcpu_overlay_other) + SIZEOF(.xcpu_overlay_csdapp) + SIZEOF(.xcpu_overlay_wap)
#define MAX_SIZE_OF_XCPU_OVERLAY_0 MAX(SIZEOF(.xcpu_overlay_init_func), MAX(SIZEOF(.xcpu_overlay_other), MAX(SIZEOF(.xcpu_overlay_csdapp), MAX(SIZEOF(.xcpu_overlay_wap),MAX(SIZEOF(.xcpu_overlay_jmetoc),0)))))
_xcpu_overlay_first_max_section = MAX_SIZE_OF_XCPU_OVERLAY_0;
_xcpu_overlay_first_flash_size = FLASH_SIZE_OF_XCPU_OVERLAY_0;
XCPU_OVERLAY_BASE_SECOND = XCPU_OVERLAY_BASE_FIRST + _xcpu_overlay_first_max_section;
xcpu_overlay_flash_second = _compressed_code_load_start + _xcpu_overlay_first_flash_size;
OVERLAY XCPU_OVERLAY_BASE_SECOND : NOCROSSREFS AT (xcpu_overlay_flash_second)
{
	.overlay_2_section_1
	{
		LONG(0)
		. = ALIGN(0x20);
	}
	.overlay_2_section_2
	{	
		#include "compress_6464/section_2_2.ld"	
		LONG(0)
		. = ALIGN(0x20);
	}
	.overlay_2_section_3
	{
		LONG(0)
		. = ALIGN(0x20);
	}
}
#define FLASH_SIZE_OF_XCPU_OVERLAY_1 0 + SIZEOF(.overlay_2_section_1) + SIZEOF(.overlay_2_section_2) + SIZEOF(.overlay_2_section_3)
#define MAX_SIZE_OF_XCPU_OVERLAY_1 MAX(SIZEOF(.overlay_2_section_1), MAX(SIZEOF(.overlay_2_section_2), MAX(SIZEOF(.overlay_2_section_3),0)))
_xcpu_overlay_second_max_section = MAX_SIZE_OF_XCPU_OVERLAY_1;
_xcpu_overlay_second_flash_size = FLASH_SIZE_OF_XCPU_OVERLAY_1;
XCPU_OVERLAY_BASE_THIRD = XCPU_OVERLAY_BASE_SECOND + _xcpu_overlay_second_max_section;
xcpu_overlay_flash_third = xcpu_overlay_flash_second + _xcpu_overlay_second_flash_size;
OVERLAY XCPU_OVERLAY_BASE_THIRD : NOCROSSREFS AT (xcpu_overlay_flash_third)
{
	.overlay_3_section_1
	{
		#include "compress_6464/section_3_1.ld"
		LONG(0)
		. = ALIGN(0x20);
	}
	.overlay_3_section_2
	{
	
		LONG(0)
		. = ALIGN(0x20);
	}
	.overlay_3_section_3
	{
		#include "compress_6464/section_3_3.ld"
		. = ALIGN(0x20);
	}
}						

#define FLASH_SIZE_OF_XCPU_OVERLAY_2 0 + SIZEOF(.overlay_3_section_1) + SIZEOF(.overlay_3_section_2) + SIZEOF(.overlay_3_section_3)
#define MAX_SIZE_OF_XCPU_OVERLAY_2 MAX(SIZEOF(.overlay_3_section_1), MAX(SIZEOF(.overlay_3_section_2), MAX(SIZEOF(.overlay_3_section_3),0)))
_xcpu_overlay_third_max_section = MAX_SIZE_OF_XCPU_OVERLAY_2;
_xcpu_overlay_third_flash_size = FLASH_SIZE_OF_XCPU_OVERLAY_2;
XCPU_OVERLAY_BASE_FOURTH = XCPU_OVERLAY_BASE_THIRD + _xcpu_overlay_third_max_section;
xcpu_overlay_flash_fourth = xcpu_overlay_flash_third + _xcpu_overlay_third_flash_size;
OVERLAY XCPU_OVERLAY_BASE_FOURTH : NOCROSSREFS AT (xcpu_overlay_flash_fourth)
{
	.overlay_4_section_1
	{		
        	*KQQ3_Common_out.oo(.text .text.* .rodata .rodata.*)
        	*QQ2007_128x128_out.oo(.text .text.* .rodata .rodata.*)
        	*QQ2007_128x160_out.oo(.text .text.* .rodata .rodata.*)
        	*QQ2007_176x220_out.oo(.text .text.* .rodata .rodata.*)
        	*QQ2009S_240x320_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_2
	{
		*mreader-ziyou-01.14.10-zh_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_3
	{
		*qiexigua_out.oo(.text .text.* .rodata .rodata.*)
		*fruitNinja_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_4
	{
		*msn_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_5
	{
		*eBuddy_nokia_2600_classic_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_6
	{
		*Skype_lite_1.2.11_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_7
	{
		*Facebook_out.oo(.text .text.* .rodata .rodata.*)
		*snaptu_1.7.1_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_8
	{
		*weibo_1025001010_3333_unsigned_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_9
	{
		*mxit0_sunsang_other_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}	
	.overlay_4_section_10
	{
		*opera-mini-6.5.26955-advanced-zh_out.oo(.text .text.* .rodata .rodata.*)
		*opera-mini-6.5.26955-advanced-en_out.oo(.text .text.* .rodata .rodata.*)
		*UCBrowser_V8.0.3.99_Java_pf71_Build11092616_out.oo(.text .text.* .rodata .rodata.*)
		*UCBrowser_en_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}	
	.overlay_4_section_11
	{
		*Microblog_2.1.0.224_Java_out.oo(.text .text.* .rodata .rodata.*)
		*tencentweibo_1.0.0.45_java_mini_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_12
	{
		*buyudaren-shlq_out.oo(.text .text.* .rodata .rodata.*)
		*SHLQ_S700_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_13
	{
		*Nimbuzz_lite_out.oo(.text .text.* .rodata .rodata.*)
		*Nimbuzz_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_14
	{
		*Gmail_2.06_tbmh_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
	.overlay_4_section_15
	{
		*twitter_out.oo(.text .text.* .rodata .rodata.*)
		*Twim_out.oo(.text .text.* .rodata .rodata.*)
		. = ALIGN(0x20);
	}
}
#define FLASH_SIZE_OF_XCPU_OVERLAY_3 0 + SIZEOF(.overlay_4_section_1) + SIZEOF(.overlay_4_section_2) + SIZEOF(.overlay_4_section_3) + SIZEOF(.overlay_4_section_4) + SIZEOF(.overlay_4_section_5) + SIZEOF(.overlay_4_section_6) + SIZEOF(.overlay_4_section_7) + SIZEOF(.overlay_4_section_8) + SIZEOF(.overlay_4_section_9) + SIZEOF(.overlay_4_section_10) + SIZEOF(.overlay_4_section_11) + SIZEOF(.overlay_4_section_12) + SIZEOF(.overlay_4_section_13) +SIZEOF(.overlay_4_section_14) + SIZEOF(.overlay_4_section_15)
#define MAX_SIZE_OF_XCPU_OVERLAY_3 MAX(SIZEOF(.overlay_4_section_1), MAX(SIZEOF(.overlay_4_section_2), MAX(SIZEOF(.overlay_4_section_3),MAX(SIZEOF(.overlay_4_section_4),MAX(SIZEOF(.overlay_4_section_5),MAX(SIZEOF(.overlay_4_section_6),MAX(SIZEOF(.overlay_4_section_7),MAX(SIZEOF(.overlay_4_section_8), MAX(SIZEOF(.overlay_4_section_9),MAX(SIZEOF(.overlay_4_section_10),MAX(SIZEOF(.overlay_4_section_11),MAX(SIZEOF(.overlay_4_section_12),MAX(SIZEOF(.overlay_4_section_13),MAX(SIZEOF(.overlay_4_section_14),MAX(SIZEOF(.overlay_4_section_15),0)))))))))))))))
_xcpu_overlay_fourth_max_section = MAX_SIZE_OF_XCPU_OVERLAY_3;
_xcpu_overlay_fourth_flash_size = FLASH_SIZE_OF_XCPU_OVERLAY_3;

_xcpu_overlay_secton_sizelimit =  RAM_PHY_SIZE - RAM_SIZE;

LD_ASSERT(ABSOLUTE(_xcpu_overlay_first_max_section + _xcpu_overlay_second_max_section + _xcpu_overlay_third_max_section + _xcpu_overlay_fourth_max_section) < ABSOLUTE(_xcpu_overlay_secton_sizelimit), "FLAS", "_xcpu_overlay section overflow: Please check compress_code_section_6464.ld.")
_xcpu_overlay_flash_end = xcpu_overlay_flash_fourth  + _xcpu_overlay_fourth_flash_size;
