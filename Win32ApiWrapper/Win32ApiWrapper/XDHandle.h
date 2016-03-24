#pragma once

template<HANDLE IHV = INVALID_HANDLE_VALUE>
class XDHandle
{
protected:
	XDHandle() :m_hHandle(IHV) {}
	XDHandle(const HANDLE hHandle);
	XDHandle(const XDHandle& other);
	XDHandle(XDHandle&& other);
public:
	virtual ~XDHandle();

	inline operator HANDLE() { return m_hHandle; }

	inline bool IsValidHandle() { return m_hHandle != IHV; }

	XDHandle &operator=(const XDHandle& other);
	XDHandle &operator=(XDHandle&& other);

	inline bool operator==(const XDHandle& other)
	{
#if _WIN32_NT >= _WIN32_WINNT_WIN10
		return CompareObjectHandles(m_hHandle, other.m_hHandle) != FALSE;
#else
		return m_hHandle == other.m_hHandle;
#endif
	}

	inline bool IsInherit()
	{
		return VerifyHandleInformation(m_hHandle, HANDLE_FLAG_INHERIT);
	}

	inline void SetInherit(bool isInherit)
	{
		SetHandleInformation(m_hHandle, HANDLE_FLAG_INHERIT, isInherit ? HANDLE_FLAG_INHERIT : 0);
	}

	inline bool IsProtectedFromClose()
	{
		return VerifyHandleInformation(m_hHandle, HANDLE_FLAG_PROTECT_FROM_CLOSE);
	}

	inline void SetProtectedFromClose(bool isProtectedFromClose)
	{
		SetHandleInformation(m_hHandle, HANDLE_FLAG_PROTECT_FROM_CLOSE, isProtectedFromClose ? HANDLE_FLAG_PROTECT_FROM_CLOSE : 0);
	}

protected:
	inline HANDLE GetHandle() const { return m_hHandle; }
	inline void SetHandle(const HANDLE hHandle) { m_hHandle = hHandle; }

private:
	static HANDLE CloneHandle(HANDLE hOtherHandle);
	static bool VerifyHandleInformation(HANDLE handle, DWORD flagToVerify)
	{
		DWORD dwFlag;
		BOOL bRet = GetHandleInformation(handle, &dwFlag);
		if(!bRet)
		{
			throw Win32ErrorException();
		}

		return (dwFlag & flagToVerify) != 0;
	}
private:
	HANDLE m_hHandle;
};

typedef XDHandle<INVALID_HANDLE_VALUE> MinusOneInvalidHandle;
typedef XDHandle<0> ZeroInvalidHandle;

template<HANDLE IHV>
XDHandle<IHV>::XDHandle(HANDLE hHandle)
	:m_hHandle(hHandle)
{

}

template<HANDLE IHV>
XDHandle<IHV>::XDHandle(const XDHandle<IHV>& other)
{
	m_hHandle = CloneHandle(other.m_hHandle);
}

template<HANDLE IHV>
XDHandle<IHV> &XDHandle<IHV>::operator=(const XDHandle<IHV>& other)
{
	m_hHandle = CloneHandle(other.m_hHandle);
	return *this;
}

template<HANDLE IHV>
HANDLE XDHandle<IHV>::CloneHandle(HANDLE hOtherHandle)
{
	HANDLE hRet = IHV;
	DWORD dwFlag = 0;
	BOOL bRet = GetHandleInformation(hOtherHandle, &dwFlag);
	if(!bRet)
	{
		throw Win32ErrorException();
	}

	bRet = DuplicateHandle(
		GetCurrentProcess(),
		hOtherHandle,
		GetCurrentProcess(),
		&hRet,
		0,
		(dwFlag&HANDLE_FLAG_INHERIT),
		DUPLICATE_SAME_ACCESS);
	if(!bRet)
	{
		throw Win32ErrorException();
	}

	assert(hRet != IHV);

	return hRet;
}

template<HANDLE IHV>
XDHandle<IHV>::XDHandle(XDHandle<IHV>&& other)
{
	m_hHandle = other.m_hHandle;
	other.m_hHandle = IHV;
}

template<HANDLE IHV>
XDHandle<IHV>& XDHandle<IHV>::operator=(XDHandle<IHV>&& other)
{
	m_hHandle = other.m_hHandle;
	other.m_hHandle = IHV;
	return *this;
}

template<HANDLE IHV>
XDHandle<IHV>::~XDHandle()
{
	if(m_hHandle != IHV)
	{
		CloseHandle(m_hHandle);
	}
}

