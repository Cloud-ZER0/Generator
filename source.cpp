#include "Generator.h"
#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string_view>
#include <system_error>
//int main(int mn, char* nm[]) {
//	if (mn != 5) { std::cout << "Error!\n"; exit(1); }
//
//	Generator gena(nm[1]);
//	gena.clear();
//
//	// input
//	// output
//	// smeshenie
//	// kolvo
//	// for(int i = 0; i < s-2;++i) {
//	// p[i] += 23*!(p[i] - 0x09);
//	//} убираем табуляциюж
//
//	return 0;
//}

int main(int mn, char* nm[]) {

	std::fstream out("C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\output.txt");
	if (!out) {
		std::cout << "!!!!!!!!!!!!!!!!1\n";
	}

	Generator gena("C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\input.txt", "C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\output.txt");
	//Generator gena(nm[1],nm[2],nm[3],nm[4]);
	gena.clear();
	gena.preProcessor();
	gena.generate();

	/*std::string str{"0.00066676767677767675"};
	char* ptr = new char[10]{ "12.4 13.4" };
	char pt[]{ "123445" };
	auto sz = std::size(pt);
	double res{};
	std::from_chars(str.data(), str.data()+str.size(), res);
	std::cout << res;*/
	//std::cout << sz;



	return 0;
}
