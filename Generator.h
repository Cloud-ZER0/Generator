#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <charconv>

class Generator
{
public:
	Generator() = default;

	~Generator() = default;

	Generator(const Generator& other) = delete;

	Generator& operator = (const Generator& other) = delete;

	//Generator(const std::string& filename, const std::string& output, const std::string& offset, const std::string& quantity);
	
	Generator(const std::string& filename, const std::string& output);

	void clear();


	void preProcessor();

	void generate();


private:
	std::ifstream file{};
	std::ofstream outputFile{};
	int offset{78};
	int quantityOfelem{1000};
	std::string str{};
	std::vector<int> grandCollumn{};
	std::vector<std::vector<int>> borders{};
	int rows = 0;

};

