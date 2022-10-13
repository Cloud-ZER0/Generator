#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <charconv>
#include <random>
#include <cassert>


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
	std::vector<double> Gauss(const std::vector<std::vector<double>>& matrix);

	inline void castToZero(const std::size_t& position, std::vector<double>& matrixRow);

	inline void GaussHelper(const std::vector<double>& first, std::vector<double>& second, double value);

private:
	std::ifstream file{};
	std::ofstream outputFile{};
	int offset{48};
	int quantityOfelem{1000};
	std::string str{};
	std::vector<int> grandCollumn{};
	std::vector<std::vector<int>> borders{};
	int rows = 0;

};

