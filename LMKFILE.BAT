del ds_show.obj
cc1 ds_show -i\ds\ -i\include\
if not errorlevel 1 cc2 ds_show -o\ds\
link @disk.lnk,\ds\ds,nul,\lib\lcs;
