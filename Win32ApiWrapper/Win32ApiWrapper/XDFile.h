#pragma once

enum class FileAccess :DWORD
{
	GenericeRead = GENERIC_READ,
	GenericWrite = GENERIC_WRITE,
	GenericExecute = GENERIC_EXECUTE,
	GenericAll = GENERIC_ALL,
	// TODO: Add other masks
	//
};

enum class FileShareMode :DWORD
{
	None = 0,
	Delete = FILE_SHARE_DELETE,
	Read = FILE_SHARE_READ,
	Write = FILE_SHARE_WRITE
};

enum class FileCreationDisposition :DWORD
{
	CreateAlways = CREATE_ALWAYS,
	CreateNew = CREATE_NEW,
	OpenAlways = OPEN_ALWAYS,
	OpenExisting = OPEN_EXISTING,
	TruncateExisting = TRUNCATE_EXISTING
};

enum class FileFlagsAndAttributes :DWORD
{
	// Attributes
	//
	AttributeArchive = FILE_ATTRIBUTE_ARCHIVE,
	AttributeEncrypted = FILE_ATTRIBUTE_ENCRYPTED,
	AttributeHidden = FILE_ATTRIBUTE_HIDDEN,
	AttributeNormal = FILE_ATTRIBUTE_NORMAL,
	AttributeOffline = FILE_ATTRIBUTE_OFFLINE,
	AttributeReadOnly = FILE_ATTRIBUTE_READONLY,
	AttributeSystem = FILE_ATTRIBUTE_SYSTEM,
	AttributeTemporary = FILE_ATTRIBUTE_TEMPORARY,

	// Flags
	//
	FlagBackupSemantics = FILE_FLAG_BACKUP_SEMANTICS,
	FlagDeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
	FlagNoBuffering = FILE_FLAG_NO_BUFFERING,
	FlagOpenNoRecall = FILE_FLAG_OPEN_NO_RECALL,
	FlagOpenReparsePoint = FILE_FLAG_OPEN_REPARSE_POINT,
	FlagOverlapped = FILE_FLAG_OVERLAPPED,
	FlagPosixSemantics = FILE_FLAG_POSIX_SEMANTICS,
	FlagRandomAccess = FILE_FLAG_RANDOM_ACCESS,
	FlagSessionAware = FILE_FLAG_SESSION_AWARE,
	FlagSequentialScan = FILE_FLAG_SEQUENTIAL_SCAN,
	FlagWriteThrough = FILE_FLAG_WRITE_THROUGH,

	// SQOS information
	//
	SecurityAnonymous = SECURITY_ANONYMOUS,
	SecurityContextTracking = SECURITY_CONTEXT_TRACKING,
	SecurityDelegation = SECURITY_DELEGATION,
	SecurityEffectiveOnly = SECURITY_EFFECTIVE_ONLY,
	SecurityIdentification = SECURITY_IDENTIFICATION,
	SecurityImpersonation = SECURITY_IMPERSONATION,
};

class XDFile
	:public MinusOneInvalidHandle
{
public:
	XDFile() = delete;
	XDFile(
		const std::wstring			strFileName,
		const FileAccess				desiredAccess,
		const FileShareMode           shareMode,
		SecurityAttributes*		lpSecurityAttributes,
		const FileCreationDisposition	creationDisposition,
		const FileFlagsAndAttributes	dwFlagsAndAttributes,
		const XDFile&			fileTemplate)
	{
		HANDLE hFile = CreateFileW(strFileName.c_str(),
			(DWORD)desiredAccess,
			(DWORD)shareMode,
			lpSecurityAttributes,
			(DWORD)creationDisposition,
			(DWORD)dwFlagsAndAttributes,
			fileTemplate.GetHandle());

		if(hFile == INVALID_HANDLE_VALUE)
		{
			throw Win32ErrorException();
		}
		SetHandle(hFile);
	}
	XDFile(HANDLE hFile) :XDHandle(hFile) {}
	XDFile(const XDFile& other) :XDHandle(other) {}
	XDFile(XDFile&& other) :XDHandle(other) {}
	~XDFile() {}

public:
	static const XDFile InvalidFile;
};
