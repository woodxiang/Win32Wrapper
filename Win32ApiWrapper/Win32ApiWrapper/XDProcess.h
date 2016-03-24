#pragma once
#include <set>

enum class ProcessAccess :DWORD
{
	Delete = DELETE,
	ReadControl = READ_CONTROL,
	Synchronize = SYNCHRONIZE,
	WriteDac = WRITE_DAC,
	WriteOwner = WRITE_OWNER,

	AllAccess = PROCESS_ALL_ACCESS,
	CreateProcess = PROCESS_CREATE_PROCESS,
	CreateThread = PROCESS_CREATE_THREAD,
	DupplicateHandle = PROCESS_DUP_HANDLE,
	QueryInformation = PROCESS_QUERY_INFORMATION,
	QueryLimitedInformation = PROCESS_QUERY_LIMITED_INFORMATION,
	SetInformation = PROCESS_SET_INFORMATION,
	SetQuota = PROCESS_SET_QUOTA,
	SuspendResume = PROCESS_SUSPEND_RESUME,
	Terminate = PROCESS_TERMINATE,
	VMOperation = PROCESS_VM_OPERATION,
	VMRead = PROCESS_VM_READ,
	VMWrite = PROCESS_VM_WRITE,
};

enum class ThreadAccess :DWORD
{
	Delete = DELETE,
	ReadControl = READ_CONTROL,
	Synchronize = SYNCHRONIZE,
	WriteDac = WRITE_DAC,
	WriteOwner = WRITE_OWNER,

	AllAccess = THREAD_ALL_ACCESS,
	DirectImpersonation = THREAD_DIRECT_IMPERSONATION,
	GetContext = THREAD_GET_CONTEXT,
	Impersonate = THREAD_IMPERSONATE,
	QueryInformation = THREAD_QUERY_INFORMATION,
	QueryLimitedInformation = THREAD_QUERY_LIMITED_INFORMATION,
	SetInformation = THREAD_SET_INFORMATION,
	SetContext = THREAD_SET_CONTEXT,
	SetLimitedInformation = THREAD_SET_LIMITED_INFORMATION,
	SetThreadToken = THREAD_SET_THREAD_TOKEN,
	SuspendResume = THREAD_SUSPEND_RESUME,
	Terminate = THREAD_TERMINATE,
};

struct StartupInfoEx : STARTUPINFOEX
{
public:
	StartupInfoEx(bool useExtension = false, int attributeCount = 0)
	{
		ZeroMemory(this, sizeof(STARTUPINFOEX));
		StartupInfo.cb = useExtension ? sizeof(STARTUPINFOEX) : sizeof(STARTUPINFO);
		if(useExtension)
		{
			SIZE_T size;
			BOOL bRet = InitializeProcThreadAttributeList(NULL, attributeCount, 0, &size);
			if(!bRet)
			{
				throw Win32ErrorException();
			}

			lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size));
			bRet = InitializeProcThreadAttributeList(lpAttributeList, attributeCount, 0, &size);
		}
	}

	~StartupInfoEx()
	{
		if(StartupInfo.cb == sizeof(STARTUPINFOEX))
		{
			DeleteProcThreadAttributeList(lpAttributeList);
			HeapFree(GetProcessHeap(), 0, lpAttributeList);
			lpAttributeList = nullptr;
		}
	}

	void UpdateProcThreadAttribute()
	{
		// TODO update process thread attribute.
		//
	}
};
class XDThread
	:public ZeroInvalidHandle
{
public:
	XDThread()
	{
		HANDLE hHandel = ::GetCurrentThread();
		SetHandle(hHandel);
	}

	XDThread(XDThread& other) :XDHandle(other) {}
	XDThread(XDThread&& other) :XDHandle(other) {}
	XDThread(ThreadAccess desiredAccess, bool isInheritHandle, DWORD dwThreadId)
	{
		HANDLE hHandle = OpenThread((DWORD)desiredAccess, isInheritHandle ? TRUE : FALSE, dwThreadId);
		if(hHandle == NULL)
		{
			throw Win32ErrorException();
		}
		SetHandle(hHandle);
	}

	~XDThread() {}

public:

	// if return STILL_ACTIVE(259), the thread is still running
	//
	DWORD GetExitCodeThread()
	{
		DWORD dwExitCode;
		BOOL bRet = ::GetExitCodeThread(GetHandle(), &dwExitCode);
		if(!bRet)
		{
			throw Win32ErrorException();
		}

		return dwExitCode;
	}
};

enum class ProcessCreateFlags
{
	CreateAlwaysFromJob = CREATE_BREAKAWAY_FROM_JOB,
	CreateDefaultErrorMode = CREATE_DEFAULT_ERROR_MODE,
	CreateNewConsole = CREATE_NEW_CONSOLE,
	CreateNewProcessGroup = CREATE_NEW_PROCESS_GROUP,
	CreateNoWindow = CREATE_NO_WINDOW,
	CreateProtectedProcess = CREATE_PROTECTED_PROCESS,
	CreatePreservedCodeAuthzLevel = CREATE_PRESERVE_CODE_AUTHZ_LEVEL,
	CreateSeperateWowVdm = CREATE_SEPARATE_WOW_VDM,
	CreateSharedWowVdm = CREATE_SHARED_WOW_VDM,
	CreateSuspended = CREATE_SUSPENDED,
	CreateUnicodeEnvironment = CREATE_UNICODE_ENVIRONMENT,
	DebugOnlyThisProcess = DEBUG_ONLY_THIS_PROCESS,
	DebugProcess = DEBUG_PROCESS,
	DetachedProcess = DETACHED_PROCESS,
	ExterdedStartupInfoPresent = EXTENDED_STARTUPINFO_PRESENT,
	InheritParentAffinity = INHERIT_PARENT_AFFINITY,
};

class XDProcess
	:public ZeroInvalidHandle
{
public:
	XDProcess()
	{
		HANDLE hHandle = GetCurrentProcess();
		SetHandle(hHandle);
	}

	XDProcess(XDProcess& other) :XDHandle(other) {}
	XDProcess(XDProcess&& other) :XDHandle(other) {}

	XDProcess(ProcessAccess desiredAccess, bool isInheritHandle, DWORD dwProcessId)
	{
		HANDLE hHandle = OpenProcess((DWORD)desiredAccess, isInheritHandle ? TRUE : FALSE, dwProcessId);
		if(hHandle == NULL)
		{
			throw Win32ErrorException();
		}
		SetHandle(hHandle);
	}

	XDProcess(std::wstring strApplicationName,
		std::wstring& strCommandLine,
		SecurityAttributes processAttribute,
		SecurityAttributes threadAttribute,
		bool bInheritHandles,
		ProcessCreateFlags createFlags,
		std::set<std::wstring> environments,
		std::wstring strCurrentDirectory,
		StartupInfoEx startupInfo,
		XDThread& targetThread)
	{

	}


	~XDProcess() {}

public:
	inline static DWORD GetCurrentProcessId()
	{
		return ::GetCurrentProcessId();
	}
};