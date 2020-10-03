#include "kalyna.h"
#include "tables.h"

Kalyna::Kalyna(const int Nb, const int Nk) :
	Nb(Nb), Nk(Nk), Nr(NULL)
{
	if (Nb == 2)
	{
		if (Nk == 2)
			Nr = 10;
		if (Nk == 4)
			Nr = 14;
	}
	else if (Nb == 4)
	{
		if (Nk == 4)
			Nr = 14;
		if (Nk == 8)
			Nr = 18;
	}
	else if (Nb == 8)
	{
		if (Nk == 8)
			Nr = 18;
	}
}

void Kalyna::SubBytes(std::vector<uint64_t>& state)
{
	for (int i = 0; i < Nb; ++i)
	{
		state[i] = sboxes_enc[0][state[i] & 0x00000000000000FFULL] |
			((uint64_t)sboxes_enc[1][(state[i] & 0x000000000000FF00ULL) >> 8] << 8) |
			((uint64_t)sboxes_enc[2][(state[i] & 0x0000000000FF0000ULL) >> 16] << 16) |
			((uint64_t)sboxes_enc[3][(state[i] & 0x00000000FF000000ULL) >> 24] << 24) |
			((uint64_t)sboxes_enc[0][(state[i] & 0x000000FF00000000ULL) >> 32] << 32) |
			((uint64_t)sboxes_enc[1][(state[i] & 0x0000FF0000000000ULL) >> 40] << 40) |
			((uint64_t)sboxes_enc[2][(state[i] & 0x00FF000000000000ULL) >> 48] << 48) |
			((uint64_t)sboxes_enc[3][(state[i] & 0xFF00000000000000ULL) >> 56] << 56);
	}
}

void Kalyna::InvSubBytes(std::vector<uint64_t>& state)
{
	for (int i = 0; i < Nb; ++i)
	{
		state[i] = sboxes_dec[0][state[i] & 0x00000000000000FFULL] |
			((uint64_t)sboxes_dec[1][(state[i] & 0x000000000000FF00ULL) >> 8] << 8) |
			((uint64_t)sboxes_dec[2][(state[i] & 0x0000000000FF0000ULL) >> 16] << 16) |
			((uint64_t)sboxes_dec[3][(state[i] & 0x00000000FF000000ULL) >> 24] << 24) |
			((uint64_t)sboxes_dec[0][(state[i] & 0x000000FF00000000ULL) >> 32] << 32) |
			((uint64_t)sboxes_dec[1][(state[i] & 0x0000FF0000000000ULL) >> 40] << 40) |
			((uint64_t)sboxes_dec[2][(state[i] & 0x00FF000000000000ULL) >> 48] << 48) |
			((uint64_t)sboxes_dec[3][(state[i] & 0xFF00000000000000ULL) >> 56] << 56);
	}
}


void Kalyna::ShiftRows(std::vector<uint64_t>& state)
{
	int shift = -1;
	auto temp_state = state;

	for (int row = 0; row < sizeof(uint64_t); ++row)
	{
		if (row % (sizeof(uint64_t) / Nb) == 0)
			shift += 1;
		for (int col = 0; col < Nb; ++col)
			temp_state[row + sizeof(uint64_t) * ((col + shift) % Nb)] = state[row + sizeof(uint64_t) * col];
	}
}

void Kalyna::InvShiftRows(std::vector<uint64_t>& state)
{
	int shift = -1;
	auto temp_state = state;

	for (int row = 0; row < sizeof(uint64_t); ++row)
	{
		if (row % (sizeof(uint64_t) / Nb) == 0)
			shift += 1;
		for (int col = 0; col < Nb; ++col)
			temp_state[row + sizeof(uint64_t) * col] = state[row + sizeof(uint64_t) * ((col + shift) % Nb)];
	}
}

uint8_t Kalyna::MultGF(uint8_t f, uint8_t s)
{
	int i;
	uint8_t r = 0;
	uint8_t hbit = 0;
	for (i = 0; i < 8; ++i) {
		if ((s & 0x1) == 1)
			r ^= f;
		hbit = f & 0x80;
		f <<= 1;
		if (hbit == 0x80)
			f ^= 0x011d;
		s >>= 1;
	}
	return r;
}

std::vector<uint64_t> Kalyna::MatrixMult(const std::vector<uint64_t>& first, const std::vector<uint64_t>& second)
{
	uint8_t product;
	uint64_t result;
	auto res = first;

	for (int col = 0; col < Nb; ++col)
	{
		uint64_t result;
		for (int row = 7; row >= 0; --row)
		{
			product = 0;
			for (int b = 7; b >= 0; --b)
				product ^= MultGF(res[b * sizeof(uint64_t) * col], second[row * sizeof(uint64_t) * b]);
		}
		res[col] = result;
	}
	return res;
}

void Kalyna::MixColumns(std::vector<uint64_t>& state)
{
	MatrixMult(state, mds_matrix);
}

void Kalyna::InvMixColumns(std::vector<uint64_t>& state)
{
	MatrixMult(state, mds_inv_matrix);
}

void Kalyna::AddRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round)
{
	for (int i = 0; i < Nb; ++i)
		state[i] += roundKeys[round][i];
}

void Kalyna::SubRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round)
{
	for (int i = 0; i < Nb; ++i)
		state[i] -= roundKeys[round][i];
}

void Kalyna::AddRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values)
{
	for (int i = 0; i < Nb; ++i)
		state[i] += values[i];
}

void Kalyna::XorRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round)
{
	for (int i = 0; i < Nb; ++i) 
		state[i] ^= roundKeys[round][i];
}

void Kalyna::XorRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values)
{
	for (int i = 0; i < Nb; ++i)
		state[i] ^= values[i];
}

void Kalyna::Rotate(std::vector<uint64_t>& state)
{
	auto temp = state[0];
	for (int i = 1; i < state.size(); ++i)
		state[i - 1] = state[i];
	state[state.size() - 1] = temp;
}

void Kalyna::ShiftLeft(std::vector<uint64_t>& state)
{
	for (int i = 0; i < state.size(); ++i)
		state[i] << 1;
}

void Kalyna::RotateLeft(std::vector<uint64_t>& state)
{
	size_t rotate_bytes = 2 * state.size() + 3;
	size_t bytes_num = state.size() * 8;

	uint8_t* bytes = reinterpret_cast<uint8_t*>(state.data());
	uint8_t* buffer = new uint8_t[rotate_bytes];

	/* Rotate bytes in memory. */
	memcpy(buffer, bytes, rotate_bytes);
	memmove(bytes, bytes + rotate_bytes, bytes_num - rotate_bytes);
	memcpy(bytes + bytes_num - rotate_bytes, buffer, rotate_bytes);
}

void Kalyna::EncipherRound(std::vector<uint64_t>& state)
{
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
}

void Kalyna::DecipherRound(std::vector<uint64_t>& state)
{
	InvMixColumns(state);
	InvShiftRows(state);
	InvSubBytes(state);
}

void Kalyna::KeyExpandKt(std::vector<uint64_t>& state, std::vector<uint64_t>& key, std::vector<uint64_t>& kt)
{
	std::vector<uint64_t> k0(Nb * sizeof(uint64_t));
	std::vector<uint64_t> k1(Nb * sizeof(uint64_t));

	state[0] += Nb + Nk + 1;

	if (Nb == Nk) {
		memcpy(k0.data(), key.data(), Nb * sizeof(uint64_t));
		memcpy(k1.data(), key.data(), Nb * sizeof(uint64_t));
	}
	else {
		memcpy(k0.data(), key.data(), Nb * sizeof(uint64_t));
		memcpy(k1.data(), key.data() + Nb, Nb * sizeof(uint64_t));
	}

	AddRoundKeyExpand(state, k0);
	EncipherRound(state);
	XorRoundKeyExpand(state, k1);
	EncipherRound(state);
	AddRoundKeyExpand(state, k0);
	EncipherRound(state);
	memcpy(kt.data(), state.data(), Nb * sizeof(uint64_t));
}

void Kalyna::KeyExpandEven(std::vector<uint64_t>& state, std::vector<uint64_t>& key, std::vector<std::vector<uint64_t>>& round_keys, std::vector<uint64_t>& kt) {
	int i;
	std::vector<uint64_t> initial_data(Nk * sizeof(uint64_t));
	std::vector<uint64_t> kt_round (Nb * sizeof(uint64_t));
	std::vector<uint64_t> tmv(Nb * sizeof(uint64_t));
	size_t round = 0;

	memcpy(initial_data.data(), key.data(), Nk * sizeof(uint64_t));
	for (i = 0; i < Nb; ++i) {
		tmv[i] = 0x0001000100010001;
	}

	while (true) {
		memcpy(state.data(), kt.data(), Nb * sizeof(uint64_t));
		AddRoundKeyExpand(state, tmv);
		memcpy(kt_round.data(), state.data(), Nb * sizeof(uint64_t));

		memcpy(state.data(), initial_data.data(), Nb * sizeof(uint64_t));

		AddRoundKeyExpand(state, kt_round);
		EncipherRound(state);
		XorRoundKeyExpand(state, kt_round);
		EncipherRound(state);
		AddRoundKeyExpand(state, kt_round);

		memcpy(round_keys[round].data(), state.data(), Nb * sizeof(uint64_t));

		if (Nr == round)
			break;

		if (Nk != Nb) {
			round += 2;

			ShiftLeft(tmv);

			memcpy(state.data(), kt.data(), Nb * sizeof(uint64_t));
			AddRoundKeyExpand(state, tmv);
			memcpy(kt_round.data(), state.data(), Nb * sizeof(uint64_t));

			memcpy(state.data(), initial_data.data() + Nb, Nb * sizeof(uint64_t));

			AddRoundKeyExpand(state, kt_round);
			EncipherRound(state);
			XorRoundKeyExpand(state, kt_round);
			EncipherRound(state);
			AddRoundKeyExpand(state, kt_round);

			memcpy(round_keys[round].data(), state.data(), Nb * sizeof(uint64_t));

			if (Nr == round)
				break;
		}
		round += 2;
		ShiftLeft(tmv);
		Rotate(initial_data);
	}
}

void Kalyna::KeyExpandOdd(std::vector<std::vector<uint64_t>>& round_keys)
{
	for (int i = 1; i < Nr; i += 2) 
	{
		memcpy(round_keys[i].data(), round_keys[i - 1].data(), Nb * sizeof(uint64_t));
		RotateLeft(round_keys[i]);
	}
}

void Kalyna::KalynaEncipher(std::vector<uint64_t>& state, std::vector<uint64_t>& msg, std::vector<std::vector<uint64_t>>& roundKeys, std::vector<uint64_t>& cipher)
{
	memcpy(state.data(), msg.data(), Nb * sizeof(uint64_t));

	int round = 0;
	AddRoundKey(state, roundKeys, round);
	for (round = 1; round < Nr; ++round) {
		EncipherRound(state);
		XorRoundKey(state, roundKeys, round);
	}
	EncipherRound(state);
	AddRoundKey(state, roundKeys, Nr);

	memcpy(cipher.data(), state.data(), Nb * sizeof(uint64_t));
}

void Kalyna::KalynaDecipher(std::vector<uint64_t>& state, std::vector<uint64_t>& msg, std::vector<std::vector<uint64_t>>& roundKeys, std::vector<uint64_t>& cipher)
{
	int round = Nr;
	memcpy(state.data(), cipher.data(), Nb * sizeof(uint64_t));

	SubRoundKey(state, roundKeys, round);
	for (round = Nr - 1; round > 0; --round) {
		DecipherRound(state);
		XorRoundKey(state, roundKeys, round);
	}
	DecipherRound(state);
	SubRoundKey(state, roundKeys, 0);

	memcpy(msg.data(), state.data(), Nb * sizeof(uint64_t));
}