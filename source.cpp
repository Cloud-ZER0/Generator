#include "Generator.h"

int main(int mn, char* nm[]) {

	Generator gena("C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\inputNeZavisimNeRavnoveroiat.txt", "C:\\Users\\Dima$\\source\\repos\\gen\\x64\\Debug\\testOutput.txt");
	//Generator gena(nm[1],nm[2],nm[3],nm[4]);
	
	gena.Clear();
	gena.Generate();
	return 0;
}
