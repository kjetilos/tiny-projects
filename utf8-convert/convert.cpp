#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include "table.h"

static int count_leading_bits(uint8_t value)
{
	return count_table[value];
}

static int required_octets(uint32_t value)
{
	if (value <= 0x7f)
		return 1;
	if (value <= 0x7ff)
		return 2;
	if (value <= 0xffff)
		return 3;
	return 4;
}

static int header_table[] = {
	0x00, // 0 octets  xxxx xxxx
	0x00, // 1 octet   xxxx xxxx
	0xc0, // 2 octets  110x xxxx
	0xe0, // 3 octets  1110 xxxx
	0xf0, // 4 octets  1111 0xxx
};

static int mask_table[] = {
	0x7f, // 0 octets  0111 1111
	0x3f, // 1 octet   0011 1111
	0x1f, // 2 octets  0001 1111
	0x0f, // 3 octets  0000 1111
	0x07, // 4 octets  0000 0111
};

uint32_t decode(const uint8_t* a)
{
	uint32_t value = 0;
	uint8_t b = *a++;
	int n = count_leading_bits(b);

	if (n == 0)
		return b;

	int mask = mask_table[n];

	for (int octet = n - 1; octet >= 0; octet--)
	{
		value |= (b & mask) << (6 * octet);
		b = *a++;
		mask = 0x3f;
	}
	return value;
}

void encode(uint32_t value, uint8_t* out)
{
	int n = required_octets(value);

	for (int octet = n-1; octet > 0; octet--)
	{
		out[octet] = 0x80 | (value & 0x3f);
		value = value >> 6;
	}
	
	out[0] = header_table[n] | value;
}

int main(int argc, char* argv[])
{
	if (argc != 2) 
	{
		std::cerr << "Usage ./convert file\n";
		std::exit(1);
	}

	std::string line = "";
	std::ifstream in(argv[1], std::ios::in | std::ios::binary);

	uint8_t buffer[4] = {0};

	for (int i=0; i<0x10ffff; i++)
	{
		getline(in, line);
		if (line.empty())
		{
			continue;
		}
		uint32_t value = decode((const uint8_t*)line.c_str());
		encode(i, buffer);
		bool match = std::memcmp(line.c_str(), buffer, required_octets(i)) == 0;

		assert (i == value);
		assert (match);
	}

	std::cout << "\n";
	return 0;
}
