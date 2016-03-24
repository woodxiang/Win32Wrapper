#pragma once
#include <exception>
#include <string>
#include <Windows.h>

template <typename T>
inline T operator | (T a, T b)
{
	return static_cast<T>(static_cast<int>(a) | static_cast<int>(b));
}

template <typename T>
inline T operator & (T a, T b)
{
	return static_cast<T>(static_cast<int>(a) & static_cast<int>(b));
}

template <typename T>
inline T operator ^ (T a, T b)
{
	return static_cast<T>(static_cast<int>(a) ^ static_cast<int>(b));
}

class InvalidOperationException
	: public std::exception
{
public:
	InvalidOperationException() = delete;

	InvalidOperationException(const std::string msg)
		:std::exception(msg.c_str())
	{
	}
	InvalidOperationException(const char * const msg)
		:std::exception(msg)
	{

	}

	~InvalidOperationException()
	{

	}
};

class ArgumentException
	: public std::exception
{
public:
	ArgumentException() = delete;

	ArgumentException(const std::string msg)
		:std::exception(msg.c_str())
	{
	}
	ArgumentException(const char * const msg)
		:std::exception(msg)
	{

	}

	~ArgumentException()
	{

	}
};

class Win32ErrorException
	:public std::exception
{
public:
	Win32ErrorException()
	{
	}

	Win32ErrorException(const unsigned int uiErrCode)
		:std::exception(GetWin32ErrorMessage(uiErrCode).c_str())
	{
	}

	Win32ErrorException(const std::string msg)
		:std::exception(msg.c_str())
	{
	}
	Win32ErrorException(const char * const msg)
		:std::exception(msg)
	{

	}

	~Win32ErrorException()
	{

	}

private:

	static std::string GetWin32ErrorMessage(const DWORD dwErrCode)
	{
		LPSTR lpMsgBuf = NULL;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErrCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpMsgBuf,
			0, NULL);
		auto msg = std::string(lpMsgBuf);
		LocalFree(lpMsgBuf);
		return msg;
	}

private:

};