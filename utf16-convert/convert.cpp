#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <cstdio>

uint16_t swap(uint16_t n)
{
	return __builtin_bswap16(n);
}

uint32_t decode(const uint16_t* in)
{
	uint16_t w1 = *in++;
	if (w1 < 0xd800 || w1 > 0xdfff)
	{
		return w1;
	}
	else
	{
		uint16_t w2 = *in++;
		uint32_t u = w2 & 0x3ff;
		u |= (w1 & 0x3ff) << 10;
		return u + 0x10000;
	}
}

void encode(uint32_t value, uint16_t* out)
{
	if (value < 0x10000)
	{
		*out++ = value;
	}
	else
	{
		uint32_t u = value - 0x10000;

		uint16_t w1 = 0xd800 | (u >> 10);
		uint16_t w2 = 0xdc00 | (u & 0x3ff);

		*out++ = w1;
		*out++ = w2;
	}
}

size_t read_line(FILE* file, uint16_t* buffer)
{
	uint16_t d = 0;
	size_t count = 0;

	while (d != '\n')
	{
		size_t n = std::fread(&d, sizeof(d), 1, file);
		if (n < 1)
			throw std::runtime_error("eof");
//		d = swap(d); // only when reading different byte order
		*buffer++ = d;
		count++;
	}
	return count;
}

int main(int argc, char* argv[])
{
	if (argc != 2) 
	{
		std::cerr << "Usage ./convert file\n";
		std::exit(1);
	}

	FILE* file = std::fopen(argv[1], "rb");
	if (! file)
	{
		std::perror("fopen");
		std::exit(1);
	}

	std::string line = "";

	uint16_t decode_buffer[4] = {0};
	uint16_t encode_buffer[2] = {0};

	for (int i=0; i<0x10ffff; i++)
	{
		size_t n = read_line(file, decode_buffer);

		if (n == 1)
		{
			continue;
		}
		uint32_t value = decode(decode_buffer);
		encode(i, encode_buffer);
		int words = i < 0x10000 ? 1 : 2;
		bool equal = std::memcmp(decode_buffer, encode_buffer, words) == 0;

		assert (value == i);
		assert (equal);
	}

	return 0;
}
