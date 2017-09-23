# Building WiX Installer

- install Wix Toolset
- add bin folder of your Qt installation to path (use right version for your compiler)
- build client
- build server
- run 
```
build.bat
```

The first set of build script is to add all qml files and qt dependencies to build folder. If you already did it and want to skip this setp run:
```
build.bat -s
```