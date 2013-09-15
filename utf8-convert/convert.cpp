#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include "table.h"

int count_leading_bits(uint8_t value)
{
	return count_table[value];
}

uint32_t decode_switch(const uint8_t* a)
{
	uint32_t value = 0;
	uint8_t b = *a++;
	int num_octets = count_leading_bits(b);

	switch (num_octets)
	{
	case 0:
		value = b;
		break;
	case 2:
		value = (b & 0x1f) << (6*1);
		b = *a++;
		value += b & 0x3f;
		break;
	case 3:
		value = (b & 0xf) << (6*2);
		b = *a++;
		value += (b & 0x3f) << (6*1);
		b = *a++;
		value += (b & 0x3f) << 0;
		break;
	case 4:
		value = (b & 0x7) << (6*3);
		b = *a++;
		value += (b & 0x3f) << (6*2);
		b = *a++;
		value += (b & 0x3f) << (6*1);		
		b = *a++;
		value += (b & 0x3f) << 0;
		break;
	}
	return value;
}

int mask_table[] = {
	0x7f, // 0 octets
	0x3f, // 1 octet
	0x1f, // 2 octets
	0x0f, // 3 octets
	0x07, // 4 octets
};

uint32_t decode_loop(const uint8_t* a)
{
	uint32_t value = 0;
	uint8_t b = *a++;
	int num_octets = count_leading_bits(b);
	if (num_octets == 0)
		return b;

	int mask = mask_table[num_octets];

	for (int octet = num_octets - 1; octet >= 0; octet--)
	{
		value |= (b & mask) << (6 * octet);
		b = *a++;
		mask = 0x3f;
	}
	return value;
}

uint32_t decode_switch_reverse(const uint8_t* a)
{
	uint32_t value = 0;
	int num_octets = count_leading_bits(a[0]);

	switch (num_octets)
	{
	case 0:
		value = a[0];
		break;
	case 2:
		value = a[1] & 0x3f;
		value |= (a[0] & 0x1f) << (6*1);
		break;
	case 3:
		value = a[2] & 0x3f;
		value |= (a[1] & 0x3f) << (6*1);
		value |= (a[0] & 0xf) << (6*2);
		break;
	case 4:
		value = a[3] & 0x3f;
		value |= (a[2] & 0x3f) << (6*1);
		value |= (a[1] & 0x3f) << (6*2);
		value |= (a[0] & 0x7) << (6*3);
		break;
	}
	return value;
}


uint32_t decode_loop_reverse(const uint8_t* in)
{
	uint32_t value = 0;
	int n = count_leading_bits(in[0]);
	int i = 0;

	for (int octet = n - 1; octet > 0; octet--)
	{
		value |= (in[octet] & 0x3f) << (6 * i);
		i++;
	}

	value |= (in[0] & mask_table[n]) << (6 * i);

	return value;
}

int required_octets(uint32_t value)
{
	if (value <= 0x7f)
		return 1;
	if (value <= 0x7ff)
		return 2;
	if (value <= 0xffff)
		return 3;
	return 4;
}

void encode(uint32_t value, uint8_t* out)
{
	int n = required_octets(value);
	switch (n) {
	case 1:
		out[0] = value;
		break;
	case 2:
		out[1] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[0] = 0xc0 | value;
		break;
	case 3:
		out[2] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[1] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[0] = 0xe0 | value;
		break;
	case 4:
		out[3] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[2] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[1] = 0x80 | (value & 0x3f);
		value = value >> 6;
		out[0] = 0xf0 | value;
		break;
	}
}

int header_table[] = {
	0x00, // 0 octets
	0x00, // 1 octet
	0xc0, // 2 octets  110x xxxx
	0xe0, // 3 octets  1110 xxxx
	0xf0, // 4 octets  1111 0xxx
};

void encode_loop(uint32_t value, uint8_t* out)
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
		uint32_t value = decode_loop_reverse((const uint8_t*)line.c_str());
//		uint32_t value = decode_switch_reverse((const uint8_t*)line.c_str());
//		std::cout << "[" << i << "] decode " << value << "\n";
//		uint32_t value = decode_loop((const uint8_t*)line.c_str());
		encode_loop(i, buffer);
		bool match = std::memcmp(line.c_str(), buffer, required_octets(i)) == 0;

		assert (i == value);
		assert (match);
	}

	std::cout << "\n";
	return 0;
}
