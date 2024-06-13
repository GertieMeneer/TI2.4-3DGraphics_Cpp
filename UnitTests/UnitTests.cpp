#include "pch.h"
#include "CppUnitTest.h"

#include <fstream>
#include "FileIO.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point endTime = startTime + std::chrono::seconds(10);
			std::string reason = "angry unit test";

			FileIO::saveScore(startTime, endTime, reason);

			std::ifstream file("scores.txt");		//input file stream
			Assert::IsTrue(file.is_open());

			std::string expectedReason = "Reason of death: " + reason;

			bool foundExpectedReason = false;

			std::string line;		//store read line in here
			while (std::getline(file, line)) {		//store every line inside line
				if (line.find(expectedReason) != std::string::npos) {		//when line found != false, npos = no position
					foundExpectedReason = true;
				}
			}

			file.close();

			std::ofstream outputFile("FileIO_UnitTest_output.txt");		//output file stream
			Assert::IsTrue(outputFile.is_open());

			//write output to file
			outputFile << "FileIO UnitTest output:" << std::endl;
			outputFile << "Found expected reason in file: " << (foundExpectedReason ? "true" : "false") << std::endl;
			outputFile.close();

			Assert::IsTrue(foundExpectedReason, L"Expected reason not found in file.");		//requires wide-character string (L) as parameter
		}
	};
}
