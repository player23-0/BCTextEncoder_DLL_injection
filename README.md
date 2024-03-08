# BCTextEncoder_DLL_injection_x86

## Description
I made this tool after completing the HTB Analysis room.
I took inspiration from these two great repos:
https://github.com/0x09AL/RdpThief
https://github.com/milkdevil/injectAllTheThings
So definitely check them out.

This toolset comprises two main components designed for educational and ethical hacking purposes to demonstrate the process of DLL injection and password retrieval from a running process. 

- `injector.exe`: A utility that injects `PasswordThief.dll` into the `TextEncode.exe` process (a spawn process of BCTextEncoder).
- `PasswordThief.dll`: A dynamic link library (DLL) crafted to intercept and log passwords through API hooking of the WideCharToMultiByte function.

I have also included debugging tools:
- Api Monitor
- Process Hacker

As well as BCTextEncoder.exe


## Warning

This software is intended for educational purposes only. Unauthorized use of these tools to invade privacy or engage in illegal activities strictly goes against the intended use. By using this software, you agree to use it responsibly and ethically.


## Installation

1. Clone this repository to your local machine.
2. Navigate to the cloned directory for Injector and PasswordThief, respectively.
3. Open the .sln file in Visual Studio, make sure x86 and Release is checked, then build.
4. Ensure `injector.exe` and `PasswordThief.dll` are generated.

5. Or navigate to the respective folders ../Release and get the precompiled binaries


## Usage

- Usage: injector.exe <FULL Path to DLL> <Process_PID>

BCTextEncoder will spawn two TextEncode.exe processes.
Either of them could hold the Thread with the plaintext password. So inject into both processes.

- After the password has been entered. It will spawn the file in %TEMP%\data.bin

- For a walkthrough, go to my Analysis Writeup https://github.com/player23-0/Analysis_Writeup_HTB/blob/main/Analysis%20-%20Writeup.pdf
  



## Testing
This has only been tested on Windows 11, Server 2016, Server 2019

## Disclaimer

This project is a demonstration and should not be used for malicious purposes. The developers assume no liability and are not responsible for any misuse or damage caused by this program.
