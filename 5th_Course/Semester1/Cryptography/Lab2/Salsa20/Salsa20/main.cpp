#include "Salsa20.h"
#include <iostream>

int main()
{
	auto key = "qwertyuioplkjhfm";
	auto salsa20 = Salsa20(key);

	salsa20.Encrypt("../../../TestFiles/testFile10Mb.txt", "../../../Results/salsa20Encrypted10Mb.txt");
	salsa20.Decrypt("../../../Results/salsa20Encrypted10Mb.txt", "../../../Results/salsa20Decrypted10Mb.txt");
	return 0;
}