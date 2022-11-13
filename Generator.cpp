#include "Generator.h"

//Generator::Generator(const std::string& filename, const std::string& output, const std::string& offset, const std::string& quointity )
//	:file(filename, std::ios::binary),
//	outputFile(output, std::ios::binary),
//	offset(std::atoi(offset.data())),
//	quantityOfelem(std::atoi(quointity.data()))
//{
//	if (!file.is_open()) {
//		std::cerr << "cannot open the file\n";
//		exit(-1);
//	}
//	if (!outputFile.is_open()) std::cout << "fuck\n";
//	file.seekg(0, std::ios::end);
//	int size = static_cast<int>(file.tellg());
//	file.seekg(0, std::ios::beg);
//	str.resize(size);
//	file.read(str.data(), size);
//	file.close();
//	str.push_back(0x0a);
//}

Generator::Generator(const std::string& filename, const std::string& output)
	:file(filename, std::ios::binary),
	outputFile(output, std::ios::binary)
{
	if (!file.is_open()) {
		std::cerr << "cannot open the file\n";
		exit(-1);
	}
	file.seekg(0, std::ios::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);
	str.resize(size+1);
	file.read(str.data(), size);
	file.close();
	str.push_back(0x0a);
}

void Generator::Clear() 
{
	/*Убираем все символы кроме цифр, запятых и точек*/ 
	for (auto& i : str) {
		switch (i)
		{
		case 0x09: 
			i = 0x20;
			break;
		case 0x2c:
			i = 0x2e;
			break;
		}
		if (i != '.' 
			and (i | 7) != '7'
			and (i | 9) != '9'
			and i != ' '
			and i != 0x0a)
			i = ' ';
	}

	/*Меняем точки на пробелы*/ 
	for (auto i = 0; i < str.size(); ++i) {
		if ((i == str.size() - 2 and str[i] == '.')) str[i] = ' ';
		else if (i != 0 and i != str.size() - 1 and str[i] == '.') {
			if (str[i - 1] == '.'
				or str[i + 1] == ' '
				or str[i + 1] == '.')
				str[i] = ' ';
		}
	}

	/*Удаляем лишнее пробелы*/
	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == ' ') {
			if (*(i - 1) == ' '
				or *(i + 1) == ' '
				or *(i - 1) == '.') {
				str.erase(i);
				--i;
			}
		}
	}
	/*Удаляем пробел в начале строки, если он есть*/
	if (str[0] == ' ') str.erase(str.begin());

	/*Заменяем пробелы после новой строки на переносы строки*/
	for (auto i = 0; i < str.size(); ++i) {
		if (i != 0 and i != str.size() - 1) {
			if ((str[i + 1] == 0x0a and str[i] == ' ')
				or (str[i + 1] == ' ' and str[i] == 0x0a)) {
				str[i] = 0x0a;
			}
		}
	}

	/*Удаляем два и более идущих подряд переноса строки*/ 
	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == 0x0a) {
			if (*(i - 1) == 0x0a or *(i + 1) == 0x0a) {
				str.erase(i);
				--i;
			}
		}
	}
	/*Удаляем пробелы после знака переноса строки*/
	for (auto it = ++str.begin(); it != str.end(); ++it) {
		if (*(it - 1) == 0x0a and *it == ' ') str.erase(it);
	}

	/*Находим масимальное количество символов после запятой*/  
	int max{};
	int k{};
	for (auto i = 1; i < str.size(); ++i) {
		if (str[i] == '.') {
			[ss = str](int ii, int& kk) {
				while (ii < ss.size() and (ss[ii] != 0x20 and ss[ii] != 0x0a)) {
					++kk;
					++ii;
				}
			}(i + 1, k);
		}
		if (str[i] == 0x20 or str[i] == 0x0a) {
			max = (max > k) ? max : k;
			k = 0;
		}
	}

	str.reserve(str.size() * 2);
	// добавляем 0 перед точкой, если необходимо.
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.' and it == str.begin()) {
			str.insert(it, '0');
		}
		if (*it == '.' and it != str.begin() and (*(it - 1) == ' ' or *(it-1) == 0x0a)) {
			str.insert(it, '0');
			++it;
		}
	}

	/*Добавляем нули перед точкой и после точки, если необходимо*/
	// doesn't work well if the bigest number has 7 numbers after the dot
	int ctr{};
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.') {
			auto copy = it;
			while (*it != ' ' and *it != 0x0a and it != str.end()) {
				++ctr;
				++it;
			}
			for (auto j = 0; j <= max - ctr; ++j) {
				str.insert(it, '0');
				++it;
			}
			it = copy;
			ctr = 0;

		}
	}

	str.shrink_to_fit();
	std::cout << str << '\n';

}


void Generator::validChecking() {

	for (std::size_t cheker = 0; cheker < str.size(); ++cheker) {
		if (str[cheker] != '0' and str[cheker + 1] == '.') {
			std::cerr << "Invalid probabilities\n";
			exit(-6);
		}
	}

	/*Считаем количество строк*/
	for (auto i : str) {
		if (i == 0x0a) ++rows;
	}

	assert(rows >= 1);
	grandCollumn.resize(rows,1);
	
	/*Считаем количество коллонок в каждой строке*/
	auto counter = 0;
	for (auto i = 0; i < str.size(); ++i) {
		if (str[i] == 0x20) {
			grandCollumn[counter] += 1;
		}
		if (str[i] == 0x0a) ++counter;
	}

	/*Проверяем одинаковое ли каличество вероятностей в каждой строке*/
	for (auto i : grandCollumn) {
		if (i != grandCollumn[0]) {
			std::cerr << "Different values quantity\n";
			exit(-7);
		}
	}
	
	/*Проверяем квадратная ли матрица*/
	if (rows != grandCollumn[0]  and rows != 1 and grandCollumn.size() != 1) {
		std::cerr << "Not squared Matrix\n";
		exit(-8);
	}

	int summBuffer{ 0 };
	std::size_t stepen{ 0 };
	std::string buffer{};
	std::size_t k = 0;

	borders.resize(rows);

	for (auto i = 0; i < borders.size(); ++i) {
		borders[i].reserve(grandCollumn[0]);
	}

	/*переводим вероятности в инты и инициализируем границы*/
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.') {
			auto from = it;

			while (*it != ' ' and *it != 0x0a) {
				++it;
			}

			auto to = it;
			std::copy(++from, to, std::back_inserter(buffer));
			borders[k].push_back(std::atoi(buffer.data()));
			summBuffer += std::atoi(buffer.data());
			stepen = buffer.size();
			buffer.clear();
		}
		if (*it == 0x0a) {
			if (summBuffer != pow(10, stepen)) {
				std::cerr << "PROBABILITIES SUMM DOESN'T EQUAL TO 1\n";
				exit(-9);
			}
			summBuffer = 0;
			++k;
		}
	}
	
	/*проверка отступа*/
	if (offset < 0 or offset - rows > 255) {
		std::cerr << "WRONG OFFSET\n";
		exit(-10);
	}
	/*Проверка количества элементов*/
	if (quantityOfelem < 0) {
		std::cerr << "WRONG QAUNTITY\n";
		exit(-11);
	}

}

void Generator::castToDoubles() {
	std::string buffer{};
	std::size_t k = 0;
	probabilities.resize(rows);

	/*переводим вероятности в даблы*/
	for (auto it = str.begin(); it != str.end() and k < rows; ++it) {

		if (it == str.begin() or *it != ' ' and *it != 0x0a) {
			auto from = it;

			while (*it != ' ' and *it != 0x0a) {
				++it;
			}

			auto to = it;
			std::copy(from, to, std::back_inserter(buffer));
			double value{};
			std::from_chars(buffer.data(), buffer.data() + buffer.size(), value);
			buffer.clear();
			probabilities[k].push_back(value);
			--it;
		}
		else if (*it == ' ') {
			auto from = ++it;

			while (*it != ' ' and *it != 0x0a) {
				++it;
			}

			auto to = it;
			std::copy(from, to, std::back_inserter(buffer));
			double value{};
			std::from_chars(buffer.data(), buffer.data() + buffer.size(), value);
			buffer.clear();
			probabilities[k].push_back(value);
			--it;

		}
		else if (*it = 0x0a) ++k;
	}
}

void Generator::makeRowGen(int64_t K) {
	assert(file);

	std::vector<char> local_buffer_;
	local_buffer_.reserve(quantityOfelem / 4);
	int counter = 0;
	int tabl = 106;
	int64_t symbol{};
	while (counter < quantityOfelem / 5) {
	symbol = rand() % K;
		for (auto i = 0; i < grandCollumn[0]; ++i) {
			if (symbol < borders[0].at(i)) {
				if (counter >= tabl) {
				local_buffer_.push_back(0x0a);
				tabl += 106;
				}
				local_buffer_.push_back(offset + i);
				i = grandCollumn[0];
				++counter;
			}
		}
	}
	std::lock_guard lock(localMutex);
	buffer.emplace_back(std::move(local_buffer_));
	

	/*int counter = 0;
	int tabl = 106;
	int64_t symbol{};
	while (counter < quantityOfelem) {
		symbol = rand() % K;
		for (auto i = 0; i < grandCollumn[0]; ++i) {
			switch (symbol < borders[0].at(i))
			{
			case true:
				if (counter >= tabl) {
					outputFile.put(0x0a);
					tabl += 106;
				}
				outputFile.put(offset + i);
				i = grandCollumn[0];
				++counter;
				break;
			default:
				break;
			}	
		}
	}*/
}
//1 209    902
//914      986
//816      1048
//875      914
//1001		892
//922		911

//	7 845		7 266
//	7 887		7 060
//  8 421		7 156
//	7 846		7 457
//  8 381		7 498


//37011ms			32521ms

void Generator::generateByRow(int64_t K) {
	// генерация по строке.
	srand(time(0));
	auto start = std::chrono::steady_clock::now();
	//makeRowGen(K);
	std::vector<std::thread> Tpool;
	buffer.reserve(5);
	for (auto i = 0; i < 5; ++i) {
		Tpool.emplace_back([this, K]() mutable {
			makeRowGen(K);
		});
	}

	for (auto&& th : Tpool) {
		th.join();
	}

	for (auto i = 0; i < buffer.size(); ++i) {
		for (auto j = 0; j < buffer[i].size(); ++j) {
			outputFile.put(buffer[i].operator[](j));
		}
	}

	auto elapsed = std::chrono::steady_clock::now() - start;

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms" << '\n';

	/*Подсчёт энтропии*/
	double entropy{ 0 };

	for (std::size_t i = 0; i < grandCollumn[0]; ++i) {
		if (probabilities[0].at(i) > 1e-9) {
			entropy -= probabilities[0].at(i) * log(probabilities[0].at(i)) / log(2.);
			std::cout << probabilities[0].operator[](i) << ' ';
		}
	}

	std::cout << '\n';

	double superfluity = 1.0 - entropy / (log(static_cast<double>(grandCollumn[0])) / log(2.));
	double compressionCaf = 1.0 / (1.0 - superfluity);

	std::cout << "H(A): " << entropy << '\n';
	std::cout << "nx: " << superfluity << '\n';
	std::cout << "n/n0: " << compressionCaf << '\n';

}
// 0.908

void Generator::generateByMatrix(int64_t K) {
	srand(time(0));
	Matrix probab(probabilities);
	std::vector<double> realProbability = probab.Gauss();
	std::vector<double> realBorders = realProbability;
	for (auto i = 1; i < realBorders.size() - 1; i++) {
		realBorders[i] += realBorders[i - 1];
	}
	if (!outputFile.is_open()) std::cout << "couldn't open the file\n";
	/*генерация первого символа*/
	bool toBreak{ false };
	for (;;) {
		for (auto i = 0; i < borders.size(); ++i) {
			int64_t symbol = rand() % K;
			if (symbol < RAND_MAX * realBorders[i]) {
				outputFile.put(offset + i);
				toBreak = true;
				break;
			}
		}
		if (toBreak) break;
	}
	/*Генерация остальных символов*/
	int tabl = 106;
	int counter = 0;
	int64_t symbol = 0;
	std::size_t left = 0 ;
	if (!outputFile.is_open()) std::cout << "couldn't open the file\n";
	while (counter != quantityOfelem) {
		symbol = rand() % K;
		for (auto j = 0; j < grandCollumn[0]; ++j) {
			switch (symbol < borders[left].at(j))
			{
			case true:
				if (counter == tabl) {
					outputFile.put(0x0a);
					tabl += 106;
				}
				++counter;
				outputFile.put(offset + j);
				left = j;
				j = grandCollumn[0] + 1;
				break;
			default:
				break;
			}
		}
	}
	/*Безусловные*/
	double entropy{ 0 };

	/*for (std::size_t i = 0; i < probabilities.size(); ++i) {
		for(std::size_t j = 0; j < grandCollumn[0]+1; ++j)
			if (probabilities[i].at(j) > 1e-9) {
				entropy -= probabilities[i].at(j) * log(probabilities[i].at(j)) / log(2.);
			}
	}*/

	/*Безусловная этропия*/
	for (std::size_t i = 0; i < grandCollumn[0]; ++i) {
		if (probabilities[0].at(i) > 1e-9) {
			entropy -= realProbability[i] * log(realProbability[i]) / log(2.);
		}
	}
	/*Безусловная избыточность*/
	double superfluity = 1.0 - entropy / (log(static_cast<double>(grandCollumn[0])) / log(2.));

	/*условная этропия*/

	double Q{ 0 };
	double buffer_{0};

	for (auto i = 0; i < realProbability.size(); ++i) {
		for (auto j = 0; j < probabilities[i].size(); ++j) {
			buffer_ += probabilities[i].operator[](j) * log(probabilities[i].operator[](j)) / log(2.);
		}
		Q -= realProbability[i] * buffer_;
		buffer_ = 0;
	}
	/*Рассчет условной избыточности*/

	double X = 1.0 - Q / (log(static_cast<double>(rows)) / log(2.));

	/*Вывод рассчетов*/
	std::cout << "H(A): " << entropy << '\n';
	std::cout << "nx: " << superfluity << '\n';
	std::cout << "Szhatie: " << 1.0 / (1.0 - superfluity) << '\n';
	std::cout << "Variable H(A): " << Q << '\n';
	std::cout << "Variable nx: " << X << '\n';
	std::cout << "Szhatie: " << 1.0 / (1.0 - X) << '\n';

	for (auto i : realProbability) {
		std::cout << i << ' ';
	}

}

void Generator::Generate() 
{
	/*Проверяем исходные данные на валидность, в соответсвии с условием*/
	validChecking();

	/*Находим вероятности*/
	castToDoubles();

	/*Находим границы*/
	for (auto i = 0; i < borders.size(); ++i) {
		for (auto j = 0; j < borders[i].size(); ++j) {
			if (j != 0) {
				borders[i].at(j) += borders[i].at(j - 1);
			}
		}
	}

	/*Генерация К*/
	int64_t k = 0;
	int64_t K = 2;
	int64_t L = 1;

	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == ' ') break;
		++k;
	}
	k -= 1;

	for (int i = 1; i < k; ++i) {
		L *= 10;
	}

	for (K = 2; K < L; K *= 2);

	switch (rows)
	{
	case 1: 
		generateByRow(K);
		break;
	default:
		generateByMatrix(K);
		break;
	}

}
