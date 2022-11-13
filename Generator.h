#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <charconv>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include "Matrix.h"



class Generator
{
public:
	Generator() = default;

	~Generator() = default;

	Generator(const Generator& other) = delete;

	Generator& operator = (const Generator& other) = delete;

	//Generator(const std::string& filename, const std::string& output, const std::string& offset, const std::string& quantity);
	
	Generator(const std::string& filename, const std::string& output);

	void Generate();

	void Clear();

private:

	void validChecking();

	void castToDoubles();

	void generateByRow(int64_t K);

	void generateByMatrix(int64_t K);

	void makeRowGen(int64_t K);



private:
	std::ifstream file{};
	std::ofstream outputFile{};
	std::string str{};
	std::vector<int> grandCollumn{};
	std::vector<std::vector<int>> borders{};
	std::vector<std::vector<double>> probabilities{};
	std::vector<std::vector<char>> buffer; // GUARDED BY LOCALMUTEX
	std::mutex localMutex;
	int offset{ 48 };
	int quantityOfelem{ 1'000'000 };
	int rows{ 0 };
};

// строка 
// матрица
// равновероятный 
// неравновероятный
// зависимые и равновероятные
// 3 символы
// 4 символа

