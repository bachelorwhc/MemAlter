#ifndef PRIVILEGE_H
#define PRIVILEGE_H
#include <exception>
#include <Windows.h>

class Privilege
{
public:
	Privilege(HANDLE& request_thread) :
		thread(nullptr),
		token(nullptr)
	{
		if(!OpenThreadToken(request_thread, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
		{
			if(!ImpersonateSelf(SecurityImpersonation))
				throw std::runtime_error("ImpersonateSelf failed.");

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
				throw std::runtime_error("OpenThreadToken failed.");
		}

		LUID luid;
		if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			throw std::runtime_error("LookupPrivilegeValue failed.");
		}

		token_privilege.PrivilegeCount = 1;
		token_privilege.Privileges[0].Luid = luid;
		token_privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if(!AdjustTokenPrivileges(token, FALSE, &token_privilege, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
		{
			throw std::runtime_error("AdjustTokenPrivileges failed.");
		}
	}

private:
	HANDLE thread;
	HANDLE token;
	TOKEN_PRIVILEGES token_privilege;
};

#endif