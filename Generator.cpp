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
	str.resize(size);
	file.read(str.data(), size);
	file.close();
	str.push_back(0x0a);
}


void Generator::clear() 
{

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
	
	for (auto i = 0; i < str.size(); ++i) {
		if ((i == 0 and str[i] == '.') or (i == str.size() - 2 and str[i] == '.')) str[i] = ' ';
		else if (i != 0 and i != str.size() - 1 and str[i] == '.') {
			if (/*str[i - 1] == ' '*/
				str[i - 1] == '.'
				or str[i + 1] == ' '
				or str[i + 1] == '.') 
				str[i] = ' ';
		}
	}

	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == ' ') {
			if (*(i - 1) == ' '
				or *(i + 1) == ' '
				/* 
				or *(i+1) == '.'*/
				or *(i - 1) == '.') {
				str.erase(i);
				--i;
			}
		}
	}
	if (str[0] == ' ') str.erase(str.begin());

	for (auto i = 0; i < str.size(); ++i) {
		if (i != 0 and i != str.size() - 1) {
			if ((str[i + 1] == 0x0a and str[i] == ' ')
				or (str[i + 1] == ' ' and str[i] == 0x0a)) {
				str[i] = 0x0a;
			}
		}
	}

	for (auto i = str.begin(); i != str.end(); ++i) {
		if (i != str.begin() and i != (str.end() - 1) and *i == 0x0a) {
			if (*(i - 1) == 0x0a or *(i + 1) == 0x0a) {
				str.erase(i);
				--i;
			}
		}
	}

	for (auto it = ++str.begin(); it != str.end(); ++it) {
		if (*(it - 1) == 0x0a and *it == ' ') str.erase(it);
	}

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


	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.' and it == str.begin()) {
			str.insert(it, '0');
		}
		if (*it == '.' and it != str.begin() and (*(it - 1) == ' ' or *(it-1) == 0x0a)) {
			str.insert(it, '0');
			++it;
		}
	}

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

	// add 0 before the value
	// add 0 after the value


	std::cout << str << '\n';

}


void Generator::preProcessor() {
	int row{};
	for (auto i : str) {
		if (i == 0x0a) ++row;
	}
	rows = row;
	std::vector<int> collumn(row, 0);
	
	auto counter = 0;
	for (auto i = 0; i < str.size(); ++i) {
		if (str[i] == 0x20) {
			collumn[counter] += 1;
		}
		if (str[i] == 0x0a) ++counter;
	}
	grandCollumn = collumn;
	auto spaces_counter = collumn[0];

	for (auto i : collumn) {
		if (i != spaces_counter) {
			std::cout << "Different values quantity\n";
		}
	}
	
	/*for (auto i = 0; i < str.size(); ++i) {
		str[i] += 22 * !(str[i] - 0x0a);
	}*/ // делает матрицу линейной
	
	if (row != collumn[0] + 1) {
		std::cout << "Not squared Matrix\n";
		//exit(-156);
	}

	int summBuffer{ 0 };
	int stepen{ 0 };
	std::string buffer{};
	
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it == '.') {
			auto from = it;

			while (*it != ' ' and *it != 0x0a) {
				++it;
			}

			auto to = it;
			std::copy(++from, to, std::back_inserter(buffer));
			summBuffer += std::atoi(buffer.data());
			stepen = buffer.size();
			buffer.clear();
		}
		if (*it == 0x0a) {
			if (summBuffer != pow(10, stepen)) {
				std::cout << "!= 1\n";
			}
			summBuffer = 0;
		}
	}
	
	if (offset < 0 or offset - row > 255) {
		std::cout << "WRONG OFFSET\n";
	}
	if (quantityOfelem < 0) {
		std::cout << "WRONG QAUNTITY\n";
	}

}

void Generator::generate() 
{
	srand(time(NULL));
	
	std::vector<std::vector<double>> probabilities{};
	probabilities.resize(rows);
	
	std::string buffer{};
	std::size_t k = 0;

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

	/*for (auto i = 0; i < 100; ++i) {
		auto ch = rand() % 255;
		outputFile.put(ch);

	}*/

	if (!outputFile.is_open()) std::cout << "ne otkrut\n";
	for (auto i = 0; i < quantityOfelem; ++i) {
		auto symbol = rand() % 12;
		for (auto j = 0; j < grandCollumn[0]; ++j) {
			if (symbol < probabilities[0].at(j)) {
				outputFile.put(offset + j);
				j = grandCollumn[0];
			}
		}
	}
	outputFile.close();

}
