#include "stdafx.h"
#include "CppUnitTest.h"
#include <XDWin32.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Win32UtilitiesUnitTest
{
	TEST_CLASS(XDFileMappingUnitTest)
	{
	public:
		XDFileMappingUnitTest()
		{
			Logger::WriteMessage("In XDFileMappingUnitTest()");
		}

		~XDFileMappingUnitTest()
		{
			Logger::WriteMessage("In ~XDFileMappingUnitTest()");
		}

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In Class XDFileMappingUnitTest Initialize");
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class XDFileMappingUnitTest Cleanup");
		}

	public:
		BEGIN_TEST_METHOD_ATTRIBUTE(TestFileMappingConstructor)
			TEST_OWNER(L"Xiang Dong")
			TEST_PRIORITY(1)
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(TestFileMappingConstructor)
		{
			std::wstring fileMappingName = L"Test File Mapping Name";
			XDFileMapping fileMapping(
				const_cast<XDFile&>(XDFile::InvalidFile),
				nullptr,
				PageProtection::ReadWrite,
				12 * 1024 * 1024,
				fileMappingName
				);
			Assert::AreNotEqual((HANDLE)NULL, (HANDLE)fileMapping);

			XDFileMapping secondFileMapping(FileMappingAccess::Read | FileMappingAccess::Write, true, fileMappingName);
			Assert::AreNotEqual((HANDLE)NULL, (HANDLE)secondFileMapping);

			XDFileMappingView<char> view(fileMapping, FileMappingViewAccess::AllAccess, 0, 1024 * 1024);

			TestAccessFileMapppingView(view);
		}

		void TestAccessFileMapppingView(XDFileMappingView<char>& view)
		{
			view[1024 * 1024 - 1] = 100;

			__try
			{
				view[1024 * 1024] = 100;
				Assert::Fail(L"Should not be here.");
			}
			CATCH_FILE_MAP_VIEW_ACCESS_EXCEPTION(EXCEPTION_ACCESS_VIOLATION)
			{
				Logger::WriteMessage(L"Exception Occured here. it is correct");
			}
		}
	};
}