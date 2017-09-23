# Safenet Client

## Building 

Open `safenet.pro` with Qt Creator. In project configuration select only release build and set build folder's name to `build`.
On windows before running application you need to manually copy content of `external` folder to the folder containing `client.exe` file.

We built and tested with Qt 5.9. On linux we compiled with GCC 7.2 and on windows with MSVC++ 14.1.


## Running Android emulator on linux

To use andoird emulator download Andoid Studio and use it to create virutal andoird devices.

Then to run emulator go to:
```
<andoid-sdk-dir>/tools/
```
and type in konsole:
```
emulator -use-system-libs  @<device-name> 
```

