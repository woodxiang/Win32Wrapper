#include "stdafx.h"
#include "CppUnitTest.h"
#include <XDWin32.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Win32UtilitiesUnitTest
{
	TEST_CLASS(XDFileUnitTest)
	{
	public:
		XDFileUnitTest()
		{
			Logger::WriteMessage("In XDFileUnitTest()");
		}

		~XDFileUnitTest()
		{
			Logger::WriteMessage("In ~XDFileUnitTest()");
		}

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In Class XDFileUnitTest Initialize");
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class XDFileUnitTest Cleanup");
		}

	public:
		BEGIN_TEST_METHOD_ATTRIBUTE(TestXDFileConstructor)
			TEST_OWNER(L"Xiang Dong")
			TEST_PRIORITY(1)
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestXDFileConstructor)
		{
			XDFile newFile(
				std::wstring(L"testfil.dat"),
				FileAccess::GenericeRead|FileAccess::GenericWrite,
				FileShareMode::None,
				nullptr,
				FileCreationDisposition::CreateAlways,
				FileFlagsAndAttributes::FlagDeleteOnClose,
				XDFile::InvalidFile);

			Assert::AreNotEqual(INVALID_HANDLE_VALUE, (HANDLE)newFile);
		}
	};
}