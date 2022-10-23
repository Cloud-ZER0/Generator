#include "Generator.h"

int main(int mn, char* nm[]) {

	Generator gena("C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\input1.txt", "C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\output1.txt");
	//Generator gena(nm[1],nm[2],nm[3],nm[4]);
	
	gena.generate();


	return 0;
}
