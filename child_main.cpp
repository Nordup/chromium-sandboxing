#include <sandbox/win/src/sandbox.h>
#include <sandbox/win/src/sandbox_factory.h>
#include <iostream>
#include <shellapi.h>  // For CommandLineToArgvW

void TryDoingSomethingBad() {
    // Try to write to a protected directory
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "C:\\Windows\\System32\\test.txt", "w");
    if (err == 0 && file) {
        fprintf(file, "This should be blocked by the sandbox\n");
        fclose(file);
        std::wcout << L"Successfully wrote to protected directory (sandbox failed!)" << std::endl;
    } else {
        std::wcout << L"Could not write to protected directory (sandbox working!)" << std::endl;
    }
    
    // Try to modify registry
    HKEY key;
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                               0, KEY_WRITE, &key);
    if (result == ERROR_SUCCESS) {
        std::wcout << L"Successfully opened registry key for writing (sandbox failed!)" << std::endl;
        RegCloseKey(key);
    } else {
        std::wcout << L"Could not open registry key for writing (sandbox working!)" << std::endl;
    }
}

int RunChild(int argc, wchar_t* argv[]) {
    sandbox::TargetServices* target_service = sandbox::SandboxFactory::GetTargetServices();

    if (NULL == target_service) {
        std::wcout << L"Failed to retrieve target service" << std::endl;
        return 1;
    }

    if (sandbox::SBOX_ALL_OK != target_service->Init()) {
        std::wcout << L"failed to initialize target service" << std::endl;
        return 2;
    }

    // Do any "unsafe" initialization code here, sandbox isn't active yet

    target_service->LowerToken(); // This locks down the sandbox

    // Any code executed at this point is now sandboxed!

    TryDoingSomethingBad();

    std::wcout << L"Successfully lower token" << std::endl;
    return 0;
}

int wmain(int argc, wchar_t* argv[]) {
    return RunChild(argc, argv);
}
