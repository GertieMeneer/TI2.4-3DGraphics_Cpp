#pragma once

#include <chrono>
#include <iostream>

class FileIO {
public:
	FileIO();
	~FileIO();

	static void saveScore(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, std::string reason);
};