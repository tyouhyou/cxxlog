REM call winsdk_env.cmd

if not exist lib mkdir lib
if not exist bin mkdir bin
if not exist test mkdir test

rem wmake.cmd clean
nmake /f Makefile.nmake %1