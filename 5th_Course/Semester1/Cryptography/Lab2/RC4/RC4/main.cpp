#include "RC4.h"
#include <iostream>

int main()
{
	std::string key = "Wiki";
	std::string text = "pedia";

	RC4 rc4(key);

	rc4.Encrypt("../../../TestFiles/testFile100Mb.txt", "../../../Results/rc4Encrypted100Mb.txt");
	rc4.Decrypt("../../../Results/rc4Encrypted100Mb.txt", "../../../Results/rc4Decrypted100Mb.txt");

	return 0;
}