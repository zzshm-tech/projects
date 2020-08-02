@echo off
C:
set CYGWIN_PATH=C:\CSDTK\cygwin
chdir "%CYGWIN_PATH%\bin"

mount -bfu "%CYGWIN_PATH%\bin" "/usr/bin"
mount -bfu "%CYGWIN_PATH%\lib" "/usr/lib"
mount -bfu "%CYGWIN_PATH%" "/"
bash --login -i