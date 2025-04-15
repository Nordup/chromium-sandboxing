#include <sandbox/win/src/sandbox.h>
#include <sandbox/win/src/sandbox_factory.h>
#include <iostream>
#include <shellapi.h>  // For CommandLineToArgvW
#include "BrokerServicesDelegateImpl.h"

wchar_t* GetChildExecutablePath(wchar_t* argv0) {
    static wchar_t exe_path[MAX_PATH];
    wcscpy_s(exe_path, argv0);
    wchar_t* last_slash = wcsrchr(exe_path, L'\\');
    if (last_slash) {
        *(last_slash + 1) = L'\0';
        wcscat_s(exe_path, MAX_PATH, L"sandbox_parent.exe");
    }
    return exe_path;
}

int RunParent(int argc, wchar_t* argv[], sandbox::BrokerServices* broker_service) {
    std::unique_ptr<BrokerServicesDelegateImpl> delegate = std::make_unique<BrokerServicesDelegateImpl>();
    if (sandbox::SBOX_ALL_OK != broker_service->Init(std::move(delegate))) {
        std::wcout << L"Failed to initialize the BrokerServices object" << std::endl;
        return 1;
    }

    PROCESS_INFORMATION pi;

    std::unique_ptr<sandbox::TargetPolicy> policy = broker_service->CreatePolicy();
    sandbox::TargetConfig* config = policy->GetConfig();

    sandbox::ResultCode ret = config->SetJobLevel(sandbox::JobLevel::kLockdown, 0);
    if (ret != sandbox::SBOX_ALL_OK) {
        std::wcout << L"Failed to set job level" << std::endl;
        return 1;
    }

    ret = config->SetTokenLevel(sandbox::TokenLevel::USER_RESTRICTED_SAME_ACCESS, sandbox::TokenLevel::USER_LOCKDOWN);
    if (ret != sandbox::SBOX_ALL_OK) {
        std::wcout << L"Failed to set token level" << std::endl;
        return 1;
    }

    
    ret = broker_service->CreateAlternateDesktop(sandbox::Desktop::kAlternateDesktop);
    if (ret != sandbox::SBOX_ALL_OK) {
        std::wcout << L"Failed to create alternate desktop" << std::endl;
        return 1;
    }

    config->SetDesktop(sandbox::Desktop::kAlternateDesktop);
    config->SetDelayedIntegrityLevel(sandbox::IntegrityLevel::INTEGRITY_LEVEL_LOW);

    //Add additional rules here (ie: file access exceptions) like so:
    ret = config->AllowFileAccess(sandbox::FileSemantics::kAllowAny, L"some/file/path");
    if (ret != sandbox::SBOX_ALL_OK) {
        std::wcout << L"Failed to set file access" << std::endl;
        return 1;
    }

    wchar_t* exe_path = GetChildExecutablePath(argv[0]);
    std::wcout << L"Spawning target with path: " << exe_path << std::endl;

    DWORD error_code = 0;
    sandbox::ResultCode result = broker_service->SpawnTarget(exe_path, GetCommandLineW(), std::move(policy), &error_code, &pi);
    if (sandbox::SBOX_ALL_OK != result) {
        std::wcout << L"Sandbox failed to launch with the following result: " << result << std::endl;
        return 2;
    }

    // Just like CreateProcess, you need to close these yourself unless you need to reference them later
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    // broker_service->WaitForAllTargets();

    std::wcout << L"Successfully launched sandboxed process" << std::endl;

    Sleep(30000);
    return 0;
}

int wmain(int argc, wchar_t* argv[]) {
    sandbox::BrokerServices* broker_service = sandbox::SandboxFactory::GetBrokerServices();
    return RunParent(argc, argv, broker_service);
}
