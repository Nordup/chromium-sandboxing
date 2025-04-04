#include <sandbox/win/src/sandbox.h>
#include <sandbox/win/src/sandbox_factory.h>
#include <iostream>

using namespace std;

int RunParent(int argc, wchar_t* argv[], sandbox::BrokerServices* broker_service) {
    if (0 != broker_service->Init()) {
        wcout << L"Failed to initialize the BrokerServices object" << endl;
        return 1;
    }

    PROCESS_INFORMATION pi;

    sandbox::TargetPolicy* policy = broker_service->CreatePolicy();

    // Here's where you set the security level of the sandbox. Doing a "goto definition" on any
    // of these symbols usually gives you a good description of their usage and alternatives.
    policy->SetJobLevel(sandbox::JOB_LOCKDOWN, 0);
    policy->SetTokenLevel(sandbox::USER_RESTRICTED_SAME_ACCESS, sandbox::USER_LOCKDOWN);
    policy->SetAlternateDesktop(true);
    policy->SetDelayedIntegrityLevel(sandbox::INTEGRITY_LEVEL_LOW);

    //Add additional rules here (ie: file access exceptions) like so:
    policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES, sandbox::TargetPolicy::FILES_ALLOW_ANY, "some/file/path");

    sandbox::ResultCode result = broker_service->SpawnTarget(argv[0], GetCommandLineW(), policy, &pi);

    policy->Release();
    policy = NULL;

    if (sandbox::SBOX_ALL_OK != result) {
        wcout << L"Sandbox failed to launch with the following result: " << result << endl;
        return 2;
    }

    // Just like CreateProcess, you need to close these yourself unless you need to reference them later
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    broker_service->WaitForAllTargets();

    return 0;
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

void TryDoingSomethingBad() {
    // Try to write to a protected directory
    FILE* file = fopen("C:\\Windows\\System32\\test.txt", "w");
    if (file) {
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

int main(int argc, char* argv[]) {
    sandbox::BrokerServices* broker_service = sandbox::SandboxFactory::GetBrokerServices();

    // A non-NULL broker_service means that we are not running the the sandbox, 
    // and are therefore the parent process
    if(NULL != broker_service) {
        return RunParent(argc, argv, broker_service);
    } else {
        return RunChild(argc, argv);
    }
}
