#include "kalyna.h"

int main()
{
	std::vector<uint64_t> key =
	{
		0x0001020304050607, 0x08090A0B0C0D0E0F,
		0x0001020304050607, 0x08090A0B0C0D0E0F,
		0x0001020304050607, 0x08090A0B0C0D0E0F,
		0x0001020304050607, 0x08090A0B0C0D0E0F
	};

	Kalyna kalyna(256, 512, key);
	kalyna.Encrypt("../../TestFiles/testFile1Mb.txt", "../../Results/kalynaFile1MbEncrypted.txt");
	kalyna.Decrypt("../../Results/kalynaFile1MbEncrypted.txt", "../../Results/kalynaFile1MbDecrypted.txt");

	return 0;
}