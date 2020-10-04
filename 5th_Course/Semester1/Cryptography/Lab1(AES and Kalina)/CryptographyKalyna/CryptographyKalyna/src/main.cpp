#include "kalyna.h"

int main()
{
	std::vector<uint64_t> key =
	{
		0x0001020304050607, 0x08090A0B0C0D0E0F
	};

	Kalyna kalyna(2, 2, key);
	kalyna.Encrypt("../../TestFiles/testFile100Mb.txt", "../../Results/kalynaFile100MbEncrypted.txt");
	kalyna.Decrypt("../../Results/kalynaFile100MbEncrypted.txt", "../../Results/kalynaFile100MbDecrypted.txt");

	return 0;
}