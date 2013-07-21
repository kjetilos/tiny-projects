#include <iostream>
#include <string>
#include <unordered_set>
#include <locale>
#include <iterator>

std::unordered_set<wchar_t> vowels;

void init_vowels()
{
	std::locale loc;
	std::wstring chars = L"aeiouyæøå";
	for (wchar_t c : chars)
	{
		wchar_t lower = c;
		wchar_t upper = std::toupper(c, loc);

		vowels.insert(lower);
		vowels.insert(upper);
	}
}

bool is_vowel(wchar_t c)
{
	return vowels.find(c) != vowels.end();
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	std::locale::global(std::locale(""));
	init_vowels();

	unsigned int num_vowels = 0;

	std::istreambuf_iterator<wchar_t> in(std::wcin);
	std::istreambuf_iterator<wchar_t> eos;

	while (in != eos)
	{
		wchar_t c = *in;
		if (is_vowel(c))
			num_vowels++;
		++in;
	}

	std::wcout << num_vowels << " Vowel";
	if (num_vowels != 1)
		std::wcout << "s";
	std::wcout << "\n";

	return 0;
}