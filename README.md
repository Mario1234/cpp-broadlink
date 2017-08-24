# cpp-broadlink

This Windows application only shows info about broadlink type SP2 devices attached to a Wi-Fi.

To try it, download it and install Code::Blocks with TDM compiler, create new project and paste all this projects files.

Install visualcppbuildtools_full.exe or winsdksetup.exe and configure as linked libraries iphlpapi.lib and WS2_32.Lib in the code::blocks project settings.

To do it, open code::blocks, go to settings->Compiler...->Linker Settings

Click Add button in Link libraries and add each location of the two libraries.

In my case locations are:

C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\um\x64\iphlpapi.lib

C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\um\x64\WS2_32.Lib

Main code file to execute is PruebaDescubrimiento.cpp
