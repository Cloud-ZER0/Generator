#include "Generator.h"

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

	Generator gena("C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\input.txt", "C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\output.txt");
	//Generator gena(nm[1],nm[2],nm[3],nm[4]);
	gena.clear();
	gena.preProcessor();
	gena.generate();


	return 0;
}
