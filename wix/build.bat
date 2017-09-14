windeployqt.exe --qmldir ..\client\safenet ..\client\build\release\safenet.exe
if exist Safenet.msi del Safenet.msi
if exist Safenet.wixobj del Safenet.wixobj
if exist Safenet.wixpdb del Safenet.wixpdb
candle.exe -arch x64 Safenet.wxs 
light.exe -ext WixUIExtension Safenet.wixobj
if exist Safenet.wixobj del Safenet.wixobj
if exist Safenet.wixpdb del Safenet.wixpdb