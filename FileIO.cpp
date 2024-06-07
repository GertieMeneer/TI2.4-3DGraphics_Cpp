#include "FileIO.h"
#include <fstream>

FileIO::FileIO()
{

}

FileIO::~FileIO()
{

}

void FileIO::saveScore(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, std::string reason)
{
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	int score = duration.count();

	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);

	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now_c);
	std::string now_str(buffer);
	now_str.erase(now_str.length() - 1);

	std::fstream file;
	file.open("scores.txt", std::ios_base::app);
	if (file.is_open()) {
		file << now_str << ": " << score << " seconds" << std::endl;
		file << "Reason of death: " << reason << std::endl << std::endl;
		file.close();
	}
	else {
		std::cout << "Unable to open scores.txt for writing." << std::endl;
	}
}
