#pragma once

struct SecurityAttributes :SECURITY_ATTRIBUTES
{
public:
	SecurityAttributes()
	{
		nLength = sizeof(SECURITY_ATTRIBUTES);
		bInheritHandle = false;
		lpSecurityDescriptor = nullptr;
	}

	~SecurityAttributes()
	{
		if(lpSecurityDescriptor != nullptr)
		{
			delete lpSecurityDescriptor;
		}
	}
};