#include <sandbox/win/src/sandbox.h>
#include <sandbox/win/src/sandbox_factory.h>
#include <iostream>
#include <shellapi.h>  // For CommandLineToArgvW

using namespace std;

int RunParent(int argc, wchar_t* argv[], sandbox::BrokerServices* broker_service) {
    if (0 != broker_service->Init(nullptr)) {
        wcout << L"Failed to initialize the BrokerServices object" << endl;
        return 1;
    }

    PROCESS_INFORMATION pi;

    std::unique_ptr<sandbox::TargetPolicy> policy = broker_service->CreatePolicy();
    sandbox::TargetConfig* config = policy->GetConfig();

    sandbox::ResultCode ret = config->SetJobLevel(sandbox::JobLevel::kLockdown, 0);
    if (ret != sandbox::SBOX_ALL_OK) {
        wcout << L"Failed to set job level" << endl;
        return 1;
    }

    ret = config->SetTokenLevel(sandbox::TokenLevel::USER_RESTRICTED_SAME_ACCESS, sandbox::TokenLevel::USER_LOCKDOWN);
    if (ret != sandbox::SBOX_ALL_OK) {
        wcout << L"Failed to set token level" << endl;
        return 1;
    }

    config->SetDesktop(sandbox::Desktop::kAlternateDesktop);
    config->SetDelayedIntegrityLevel(sandbox::IntegrityLevel::INTEGRITY_LEVEL_LOW);

    //Add additional rules here (ie: file access exceptions) like so:
    ret = config->AllowFileAccess(sandbox::FileSemantics::kAllowAny, L"some/file/path");
    if (ret != sandbox::SBOX_ALL_OK) {
        wcout << L"Failed to set file access" << endl;
        return 1;
    }

    DWORD* last_error = nullptr;
    sandbox::ResultCode result = broker_service->SpawnTarget(argv[0], GetCommandLineW(), std::move(policy), last_error, &pi);

    if (sandbox::SBOX_ALL_OK != result) {
        wcout << L"Sandbox failed to launch with the following result: " << result << endl;
        return 2;
    }

    // Just like CreateProcess, you need to close these yourself unless you need to reference them later
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    // broker_service->WaitForAllTargets();

    wcout << L"Successfully launched sandboxed process" << endl;
    return 0;
}

void TryDoingSomethingBad() {
    // Try to write to a protected directory
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "C:\\Windows\\System32\\test.txt", "w");
    if (err == 0 && file) {
        fprintf(file, "This should be blocked by the sandbox\n");
        fclose(file);
        wcout << L"Successfully wrote to protected directory (sandbox failed!)" << endl;
    } else {
        wcout << L"Could not write to protected directory (sandbox working!)" << endl;
    }
    
    // Try to modify registry
    HKEY key;
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                               0, KEY_WRITE, &key);
    if (result == ERROR_SUCCESS) {
        wcout << L"Successfully opened registry key for writing (sandbox failed!)" << endl;
        RegCloseKey(key);
    } else {
        wcout << L"Could not open registry key for writing (sandbox working!)" << endl;
    }
}

int RunChild(int argc, wchar_t* argv[]) {
    sandbox::TargetServices* target_service = sandbox::SandboxFactory::GetTargetServices();

    if (NULL == target_service) {
        wcout << L"Failed to retrieve target service" << endl;
        return 1;
    }

    if (sandbox::SBOX_ALL_OK != target_service->Init()) {
        wcout << L"failed to initialize target service" << endl;
        return 2;
    }

    // Do any "unsafe" initialization code here, sandbox isn't active yet

    target_service->LowerToken(); // This locks down the sandbox

    // Any code executed at this point is now sandboxed!

    TryDoingSomethingBad();

    return 0;
}

int wmain(int argc, wchar_t* argv[]) {
    sandbox::BrokerServices* broker_service = sandbox::SandboxFactory::GetBrokerServices();

    // A non-NULL broker_service means that we are not running in the sandbox, 
    // and are therefore the parent process
    if(NULL != broker_service) {
        return RunParent(argc, argv, broker_service);
    } else {
        return RunChild(argc, argv);
    }
}
