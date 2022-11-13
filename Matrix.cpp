#include "Matrix.h"
#include <iostream>

Matrix::Matrix() {}

Matrix::~Matrix() {}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>>& list)
{
	data_.reserve(list.size());
	for (auto i = list.begin(); i != list.end(); ++i) {
		std::vector<double> buffer_;
		buffer_.reserve(i->size());
		for (auto j = i->begin(); j != i->end(); ++j) {
			buffer_.push_back(*j);
		}
	}
}

Matrix::Matrix(const ValueType& vec) : data_(vec) {}

inline void Matrix::GaussHelper(const std::vector<double>& first, std::vector<double>& second, double value)
{
	assert(first.size() == second.size());
	for (auto i = 0; i < first.size(); ++i) {
		if (value < 0) {
			second[i] = (second[i] == 0) ? 0 :
				second[i] + (first[i] * abs(value));
		}
		else {
			second[i] = (second[i] == 0) ? 0 :
				second[i] - (first[i] * value);
		}

	}
}

inline void Matrix::castToZero(const std::size_t& position, std::vector<double>& matrixRow)
{
	assert(position < matrixRow.size());
	auto devisor = matrixRow[position];
	for (auto i = 0; i < matrixRow.size(); ++i) {
		matrixRow[i] = (matrixRow[i] == 0) ? 0 : matrixRow[i] / devisor;
	}
}


std::vector<double> Matrix::Gauss()
{
	std::vector<std::vector<double>> localCopy{ this->data_ };

	std::vector<double> vectorOfFree{};
	vectorOfFree.reserve(localCopy.size() - 1);

	// вектор свободных членов 
	for (auto i = 0; i < data_.size() - 1; ++i) {
		vectorOfFree.push_back(localCopy[i].back());
		//удаляем из локальной копии матрицы последний элемент, тк как запушили его в специальный вектор
		localCopy[i].pop_back();
	}
	// удаляем последний вектор из локальной матрицы, тк как для рассчёта достаточно matrix.size() - 1 уравнений
	localCopy.pop_back();

	// построение системы уравнений
	for (auto i = 0; i < localCopy.size(); ++i) {
		for (auto j = 0; j < localCopy[i].size(); ++j) {

			localCopy[i].operator[](j) = (i == j) ?
				1.0 + vectorOfFree[i] - localCopy[i].operator[](j)
				: vectorOfFree[i] - localCopy[i].operator[](j);	
		}
	}

	// приведение к простой матрице
	for (auto i = 0; i < localCopy.size(); ++i) {
		localCopy[i].push_back(vectorOfFree[i]);
	}

	// реализация метода Гаусса
	for (std::size_t i = 0; i < localCopy.size(); ++i) {
		castToZero(i, localCopy[i]);

		if (i == 0) {
			for (auto j = i + 1; j < localCopy.size(); ++j) {
				GaussHelper(localCopy[i], localCopy[j], localCopy[j].operator[](i));
			}
		}

		else if (0 < i and i < localCopy.size()) {
			for (int j = i - 1; j >= 0; --j) {
				GaussHelper(localCopy[i], localCopy[j], localCopy[j].operator[](i));
			}
			for (auto k = i + 1; k < localCopy.size(); ++k) {
				GaussHelper(localCopy[i], localCopy[k], localCopy[k].operator[](i));
			}
		}
	}

	// создаем вектор с ответами и инициализируем результатом, полученным по гаусу
	std::vector<double> result{};
	result.reserve(localCopy.size());
	for (auto&& row : localCopy) {
		result.push_back(row.back());
	}
	double third = 0;
	for (auto i : result) {
		third += i;
	}
	result.push_back(1 - third);

	return result;
}
