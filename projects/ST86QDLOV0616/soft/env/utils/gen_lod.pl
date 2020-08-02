#!/usr/bin/perl -w

$reserved_len = 1024;
$bin_file=$ARGV[0];
$start_addr=$ARGV[1];
$max_size=hex($ARGV[2]);
$flash_type=$ARGV[3];
$loadinfo_addr=$ARGV[4];
$loadinfo_size=hex($ARGV[5]);

@tmp_array=split(/FFFFFFFF8/, uc $start_addr);
$start_addr=hex($tmp_array[1]);

@tmp_array=split(/FFFFFFFF8/, uc $loadinfo_addr);
$loadinfo_addr=hex($tmp_array[1]);

@path_array=split(/.bin/, $bin_file);
$final_path=$path_array[0];
$hex_file=$final_path.".hex";
$lod_file=$final_path.".lod";


#add crc code to bin file
@args=stat($bin_file);
$bin_size=$args[7];
if ($bin_size > ($max_size - $reserved_len))
{
	print "[Error]: bin file size overflow!\n";
	exit(1);
}
if (0 != ($bin_size % 4))
{
	print "[Error]: bin file size is not word align!\n";
	exit(1);
}
$crc_calc=0;
open(BINFILE, $bin_file) || die "can not open $bin_file:$!";
binmode(BINFILE);
read(BINFILE, $bin_hex, $bin_size);
$bin_hex_str=unpack("H*", $bin_hex);
for ($i=0; $i<($bin_size*2); $i=$i+8)
{
	$a=substr $bin_hex_str,$i,2;
	$b=substr $bin_hex_str,$i+2,2;	
	$c=substr $bin_hex_str,$i+4,2;	
	$d=substr $bin_hex_str,$i+6,2;
	$bin_hex_str_word=sprintf("%02s%02s%02s%02s", $d, $c, $b, $a);
	$crc_calc=$crc_calc^(hex($bin_hex_str_word));
}
close(BINFILE);

open(BINFILE, ">>$bin_file") || die "can not open $bin_file:$!";
binmode(BINFILE);
$crc_calc_str_1=sprintf("%08x", $crc_calc);
$a=substr $crc_calc_str_1,0,2;
$b=substr $crc_calc_str_1,2,2;
$c=substr $crc_calc_str_1,4,2;
$d=substr $crc_calc_str_1,6,2;
$crc_calc_str_word=sprintf("%02s%02s%02s%02s", $d, $c, $b, $a);
$crc_calc_str=pack("H8", $crc_calc_str_word);
print BINFILE $crc_calc_str;
close(BINFILE);
@args=stat($bin_file);
$bin_size=$args[7];
if ($bin_size > ($max_size - $reserved_len))
{
	print "[Error]: crc bin file size overflow!\n";
	exit(1);
}
if (0 != ($bin_size % 4))
{
	print "[Error]: crc bin file size is not word align!\n";
	exit(1);
}

$bin_file_tmp=$final_path.".tmp";
open(BINFILE, $bin_file) || die "can not open $bin_file:$!";
open(BINFILE_TMP, ">$bin_file_tmp") || die "can not open $bin_file_tmp:$!";
binmode(BINFILE);
binmode(BINFILE_TMP);
$bin_size_str_1=sprintf("%08x", $bin_size);
$a=substr $bin_size_str_1,0,2;
$b=substr $bin_size_str_1,2,2;
$c=substr $bin_size_str_1,4,2;
$d=substr $bin_size_str_1,6,2;
$bin_size_str_word=sprintf("%02s%02s%02s%02s", $d, $c, $b, $a);
$bin_size_str=pack("H8", $bin_size_str_word);
print BINFILE_TMP $bin_size_str;
read(BINFILE, $bin_contant, $bin_size);
print BINFILE_TMP $bin_contant;
close(BINFILE);
close(BINFILE_TMP);
system("mv $bin_file_tmp $bin_file");
@args=stat($bin_file);
$bin_size=$args[7];
if ($bin_size > ($max_size - $reserved_len))
{
	print "[Error]: final bin file size overflow!\n";
	exit(1);
}
if (0 != ($bin_size % 4))
{
	print "[Error]: final bin file size is not word align!\n";
	exit(1);
}

#add lib version at the end of bin file
open(LIBVERFILE, "./lib/lib_version.txt") || die "can not open ./lib/lib_version.txt!";
$lib_ver_num=int(<LIBVERFILE>);
close(LIBVERFILE);
open(BINFILE, ">>$bin_file") || die "can not open $bin_file:$!";
binmode(BINFILE);
$lib_version_str=sprintf("%08x", $lib_ver_num);
$a=substr $lib_version_str,0,2;
$b=substr $lib_version_str,2,2;
$c=substr $lib_version_str,4,2;
$d=substr $lib_version_str,6,2;
$lib_version_str_word=sprintf("%02s%02s%02s%02s", $d, $c, $b, $a);
$lib_version=pack("H8", $lib_version_str_word);
print BINFILE $lib_version;
close(BINFILE);
@args=stat($bin_file);
$bin_size=$args[7];
if ($bin_size > ($max_size - $reserved_len))
{
	print "[Error]: lib svn version bin file size overflow!\n";
	exit(1);
}
if (0 != ($bin_size % 4))
{
	print "[Error]: lib svn version bin file size is not word align!\n";
	exit(1);
}


open(OUTFILE, ">$lod_file") || die "can not open $lod_file:$!";
if ($flash_type eq "flsh_spi32m")
{
	$sector_size=64*1024;
}
else
{
	print "[Error]: Wrong input param flash type, please check again!\n";
	close(OUTFILE);
	exit(1);
}

system("xxd -c 4 -ps $bin_file > $hex_file");
open(INFILE, $hex_file) || die "can not open $hex_file:$!";
print OUTFILE "\#\$mode=flsh_spi32m\n";
print OUTFILE "\#\$sectormap=(16 x 4k, 57 x 64k, 96 x 4k)\n";
print OUTFILE "\#\$base=0x08000000\n";
print OUTFILE "\#\$spacesize=0x08000000\n";
print OUTFILE "\#\$XCV_MODEL=xcv_8809\n";
print OUTFILE "\#\$PA_MODEL=pasw_rda6625\n";
print OUTFILE "\#\$FLSH_MODEL=flsh_spi32m\n";
print OUTFILE "\#\$FLASH_SIZE=0x00400000\n";
print OUTFILE "\#\$RAM_SIZE=0x00800000\n";
print OUTFILE "\#\$RAM_PHY_SIZE=0x00800000\n";
print OUTFILE "\#\$CALIB_BASE=0x003FC000\n";
print OUTFILE "\#\$FACT_SETTINGS_BASE=0x003FE000\n";
print OUTFILE "\#\$CODE_BASE=0x00000000\n";
print OUTFILE "\#\$USER_DATA_BASE=0x00300000\n";
print OUTFILE "\#\$USER_DATA_SIZE=0x000FC000\n";
print OUTFILE "\#\$PM_MODEL=pmu_8809\n";
$count=0;
printf OUTFILE ("@%08x\n", $loadinfo_addr);
printf OUTFILE ("8%07x\n", $start_addr);
for ($i = 0; $i < ($loadinfo_size-4); $i=$i+4)
{
	print OUTFILE "ffffffff\n";
}

while (<INFILE>)
{
	if (($count % $sector_size) == 0)
	{
		printf OUTFILE ("@%08x\n", $start_addr);
		$start_addr=$start_addr+$sector_size;
	}
	$a=substr $_,0,2;
	$b=substr $_,2,2;
	$c=substr $_,4,2;
	$d=substr $_,6,2;
	print OUTFILE "$d$c$b$a\n";
	$count=$count+4;
}

if (($count % $sector_size) != 0)
{
	$dummy_count = $sector_size - ($count % $sector_size);
	for ($i = 0; $i < $dummy_count; $i=$i+4)
	{
		print OUTFILE "ffffffff\n";
	}
}

close(OUTFILE);
close(INFILE);
system("rm $hex_file");
