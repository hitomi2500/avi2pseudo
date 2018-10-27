..\c8080\c8080.exe avplay.c unpack_btree1.c>errors.txt
if errorlevel 1 goto err
..\c8080\tasm -gb -b -85 avplay.asm avplay.bin >errors.txt
if errorlevel 1 goto err
-make-rka.js
copy avplay.rka avplay.rkr
goto end
:err
type errors.txt
pause
:end