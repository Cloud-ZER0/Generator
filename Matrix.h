#pragma once

#include <vector>
#include <cassert>

class Matrix
{
public:
	using ValueType = std::vector<std::vector<double>>;
public:

	Matrix();
	~Matrix();

	Matrix(const std::initializer_list<std::initializer_list<double>>& list);
	Matrix(const ValueType& vec);

	std::vector<double> Gauss();
private:
	inline void GaussHelper(const std::vector<double>& first, std::vector<double>& second, double value);
	inline void castToZero(const std::size_t& position, std::vector<double>& matrixRow);
	
private:
	ValueType data_{};
	
};

