/*
	Разработать программу, выполняющую создание раздела
реестра «HKEY_CURRENT_USER\Software\pi» с атрибутами
безопасности для группы пользователей «Администраторы» и
правами на полный доступ, а для группы «Все» только чтение.
	Вариант 4
*/

#include "windows.h"
#include "cstdio"
#include "aclapi.h"

INT wmain(INT argc, PWSTR argv[])
{
	DWORD dwRes;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea[2];
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	SECURITY_ATTRIBUTES sa;
	HKEY hkSub = NULL;

	PWSTR sNameKey = 0;
	BOOL mode = FALSE;
	PWSTR path = 0;
	path = (PWSTR)malloc(sizeof(WCHAR) * 128);
	if (path == 0)
		return -1;

	printf("\nLab 5. Creating key with Security Attributes\n\n");

	if (argc < 2)
	{
		printf("Expected more params! Use -help for help . . .\n");
		return 0;
	}

	for (INT i = 1; i < argc; i++)
	{
		if (wcscmp(TEXT("-help"), argv[i]) == 0)
		{
			printf("Help:\n\t-help - get help about params.\n\t-reg <key> - reg key in registry\n\
\t-delete <key> - delete key in registry\n");
			return 0;
		}
		else if (wcscmp(TEXT("-reg"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected name of key! Try use \"-help\n");
				return 0;
			}
			sNameKey = argv[i];
			mode = TRUE;
		}
		else if (wcscmp(TEXT("-delete"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected name of key! Try use \"-help\n");
				return 0;
			}
			sNameKey = argv[i];
			mode = FALSE;
		}
	}

	if (mode)
	{
		// Create a well-known SID for the Everyone group.
		if (!AllocateAndInitializeSid(&SIDAuthWorld, 1,
			SECURITY_WORLD_RID,
			0, 0, 0, 0, 0, 0, 0,
			&pEveryoneSID))
		{
			printf("AllocateAndInitializeSid Error %u\n", GetLastError());
			goto Cleanup;
		}

		// Initialize an EXPLICIT_ACCESS structure for an ACE.
		// The ACE will allow Everyone read access to the key.
		ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
		ea[0].grfAccessPermissions = KEY_READ;
		ea[0].grfAccessMode = SET_ACCESS;
		ea[0].grfInheritance = NO_INHERITANCE;
		ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

		// Create a SID for the BUILTIN\Administrators group.
		if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0,
			&pAdminSID))
		{
			printf("AllocateAndInitializeSid Error %u\n", GetLastError());
			goto Cleanup;
		}

		// Initialize an EXPLICIT_ACCESS structure for an ACE.
		// The ACE will allow the Administrators group full access to
		// the key.
		ea[1].grfAccessPermissions = KEY_ALL_ACCESS;
		ea[1].grfAccessMode = SET_ACCESS;
		ea[1].grfInheritance = NO_INHERITANCE;
		ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
		ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

		// Create a new ACL that contains the new ACEs.
		dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
		if (ERROR_SUCCESS != dwRes)
		{
			printf("SetEntriesInAcl Error %u\n", GetLastError());
			goto Cleanup;
		}

		// Initialize a security descriptor.  
		pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
			SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (NULL == pSD)
		{
			printf("LocalAlloc Error %u\n", GetLastError());
			goto Cleanup;
		}

		if (!InitializeSecurityDescriptor(pSD,
			SECURITY_DESCRIPTOR_REVISION))
		{
			printf("InitializeSecurityDescriptor Error %u\n",
				GetLastError());
			goto Cleanup;
		}

		// Add the ACL to the security descriptor. 
		if (!SetSecurityDescriptorDacl(pSD,
			TRUE,     // bDaclPresent flag   
			pACL,
			FALSE))   // not a default DACL 
		{
			printf("SetSecurityDescriptorDacl Error %u\n",
				GetLastError());
			goto Cleanup;
		}

		// Initialize a security attributes structure.
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;

		HKEY hKey;

		if (sNameKey == 0)
			return -1;

		wsprintf(path, L"Software\\%ls", sNameKey);

		if (RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, REG_OPTION_VOLATILE,
			KEY_WRITE, &sa, &hKey, NULL) != ERROR_SUCCESS)
		{
			printf("Error! Cannot not create key\n");
			return -1;
		}

		RegCloseKey(hKey);

	Cleanup:

		if (pEveryoneSID)
			FreeSid(pEveryoneSID);
		if (pAdminSID)
			FreeSid(pAdminSID);
		if (pACL)
			LocalFree(pACL);
		if (pSD)
			LocalFree(pSD);
		if (hkSub)
			RegCloseKey(hkSub);

		printf("Key \"%ls\" was created!\n", path);
		return 0;
	}
	else
	{
		wsprintf(path, L"Software\\%ls", sNameKey);
		DWORD dwResult;
		dwResult = RegDeleteKey(HKEY_CURRENT_USER, path);
		if (dwResult != ERROR_SUCCESS)
		{
			printf("Cannot delete current key!\n");
			return -1;
		}
		printf("Key \"%ls\" was deleted!\n", path);
	}
}