# Safenet Client-Server - PBL project

Application for secure image sharing developed on PBL classes. Encryptrion algorithm uses chaotic magic transform based on 2D sine logistic modulation map. Images are shared via server with REST api.

## Directory structure
- client - client application with gui in Qt. it's written in c++ and qml.
- server - java application that's build upon Jersey https server.
- windows - wix project creating windows msi installer. it wraps server.jar and registers it as windows service using apache procrun.

