
if not "%1"=="-s" windeployqt.exe --qmldir ..\client ..\build\release\safenet.exe

set NAME=Safenet.x64

if exist %NAME%.msi del %NAME%.msi

candle.exe -arch x64 -ext WixUtilExtension %NAME%.wxs 
light.exe -ext WixUIExtension -ext WixUtilExtension %NAME%.wixobj

if exist %NAME%.wixobj del %NAME%.wixobj
if exist %NAME%.wixpdb del %NAME%.wixpdb

