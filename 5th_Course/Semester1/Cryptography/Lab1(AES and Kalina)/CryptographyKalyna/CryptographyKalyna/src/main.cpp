#include "kalyna.h"




int main()
{
	std::vector<uint64_t> key =
	{
		0x0001020304050607, 0x08090A0B0C0D0E0F
	};

	Kalyna kalyna(2, 2, key);
	kalyna.Encrypt("../../TestFiles/testFileSmall.txt", "../../Results/kalynaFileSmallEncrypted.txt");
	kalyna.Decrypt("../../Results/kalynaFileSmallEncrypted.txt", "../../Results/kalynaFileSmallDecrypted.txt");

	return 0;
}