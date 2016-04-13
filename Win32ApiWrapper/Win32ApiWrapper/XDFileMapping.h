#pragma once
#include <PathCch.h>
#include <Psapi.h>

enum class PageProtection :DWORD
{
	// Page Protection
	//
	ExecuteRead = PAGE_EXECUTE_READ,
	ExecuteReadWrite = PAGE_EXECUTE_READWRITE,
	ExecuteWriteCopy = PAGE_EXECUTE_WRITECOPY,
	Read = PAGE_READONLY,
	ReadWrite = PAGE_READWRITE,
	WriteCopy = PAGE_WRITECOPY,

	// Attributes
	//
	SectionCommit = SEC_COMMIT,
	SectionImage = SEC_IMAGE,
	SectionImageNoExecute = SEC_IMAGE_NO_EXECUTE,
	SectionLargPages = SEC_LARGE_PAGES,
	SectionNoCache = SEC_NOCACHE,
	SectionReserve = SEC_RESERVE,
	SectionWriteCombine = SEC_WRITECOMBINE,
};

enum class FileMappingAccess :DWORD
{
	AllAccess = FILE_MAP_ALL_ACCESS,
	Execute = FILE_MAP_EXECUTE,
	Read = FILE_MAP_READ,
	Write = FILE_MAP_WRITE,
	TargetInvalid = FILE_MAP_TARGETS_INVALID,
};

class XDFileMapping
	:public ZeroInvalidHandle
{
protected:
	XDFileMapping() :XDHandle() {}

public:
	XDFileMapping(HANDLE hHandle) :XDHandle(hHandle) {}
	XDFileMapping(const XDFileMapping& other) :XDHandle(other) {}
	XDFileMapping(XDFileMapping&& other) :XDHandle(other) {}

	XDFileMapping(
		XDFile&					file,
		SecurityAttributes*		lpAttributes,
		PageProtection			protect,
		unsigned long long		ullMaximumSize,
		std::wstring			strName)
	{
		HANDLE hHandle = CreateFileMappingW(
			(HANDLE)file,
			lpAttributes,
			(DWORD)protect,
			(DWORD)(ullMaximumSize >> 32),
			(DWORD)(ullMaximumSize & 0xFFFFFFFFul),
			strName.length() == 0 ? NULL : strName.c_str()
			);
		if(hHandle == NULL)
		{
			throw Win32ErrorException();
		}

		SetHandle(hHandle);
	}

	XDFileMapping(
		FileMappingAccess	desiredAccess,
		bool				bInheritHandle,
		std::wstring		strName)
	{
		HANDLE hHandle = OpenFileMappingW(
			(DWORD)desiredAccess,
			bInheritHandle,
			strName.c_str());

		if(hHandle == NULL)
		{
			throw Win32ErrorException();
		}

		SetHandle(hHandle);
	}
	~XDFileMapping() {}

public:
	static const XDFileMapping InvalidFileMapping;
};

enum class FileMappingViewAccess :DWORD
{
	AllAccess = FILE_MAP_ALL_ACCESS,
	Copy = FILE_MAP_COPY,
	Read = FILE_MAP_READ,
	Write = FILE_MAP_WRITE,
	Execute = FILE_MAP_EXECUTE
};

template<typename T>
class XDFileMappingView
{
public:
	XDFileMappingView(XDFileMapping& mapping, FileMappingViewAccess desiredAccess, unsigned long long ullSize, size_t sizeToMap)
	{
		m_pMemory = reinterpret_cast<T*>(MapViewOfFile((HANDLE)mapping,
			(DWORD)desiredAccess,
			(DWORD)(ullSize >> 32),
			(DWORD)(ullSize & 0x00000000ffffffful),
			sizeToMap));
		if(m_pMemory == nullptr)
		{
			throw Win32ErrorException();
		}
	}

	XDFileMappingView() = delete;

	XDFileMappingView(XDFileMappingView& other) = delete;

	XDFileMappingView(XDFileMappingView && other)
	{
		m_pMemory = other.m_pMemory;
		other.m_pMemory = nullptr;
	}

	~XDFileMappingView()
	{
		if(m_pMemory != nullptr)
		{
			if(!UnmapViewOfFile(m_pMemory))
			{
				// TODO: LOG here
				//
			}
		}
	}

	T& operator [](size_t index)
	{
		return m_pMemory[index];
	}

	T* GetPointer()
	{
		return m_pMemory;
	}

	size_t QuerySize()
	{
		if(m_pMemory == nullptr)
		{
			throw InvalidOperationException("Invalid Object.");
		}

		MEMORY_BASIC_INFORMATION mbi;
		SIZE_T retSize = VirtualQuery(m_pMemory, &mbi, sizeof(mbi));
		if(retSize == 0)
		{
			throw Win32ErrorException();
		}

		return mbi.RegionSize;
	}

	void Flush(void* pAddress = nullptr, size_t sizeToFlush = 0)
	{
		if(pAddress == nullptr && m_pMemory == nullptr)
		{
			throw ArgumentException("Invalid pAddress Argument.");
		}

		BOOL bRet = FlushViewOfFile(pAddress == nullptr ? m_pMemory : pAddress, sizeToFlush);
		if(!bRet)
		{
			throw Win32ErrorException();
		}
	}

	std::wstring GetFileName(void* pAddress = nullptr, HANDLE hProcess = INVALID_HANDLE_VALUE)
	{
		if(pAddress == nullptr && m_pMemory == nullptr)
		{
			throw ArgumentException("Invalid pAddress Argument.");
		}

		if(hProcess == INVALID_HANDLE_VALUE)
			hProcess = GetCurrentProcess();

		wchar_t wcFileNameBuffer[PATHCCH_MAX_CCH];

		DWORD dwCopied = GetMappedFileNameW(hProcess, pAddress == nullptr ? m_pMemory : pAddress, wcFileNameBuffer, sizeof(wcFileNameBuffer) / sizeof(wchar_t));
		if(dwCopied == 0)
		{
			throw Win32ErrorException();
		}

		return std::wstring(wcFileNameBuffer);
	}

private:
	T* m_pMemory;
};

#define CATCH_FILE_MAP_VIEW_ACCESS_EXCEPTION(exception) __except(GetExceptionCode()==(exception) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
