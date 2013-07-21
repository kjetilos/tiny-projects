#include <iostream>
#include <string>
#include <unordered_set>
#include <locale>

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

unsigned int count_vowels(const std::wstring& word)
{
	unsigned int count = 0;
	for (wchar_t c : word)
		if (is_vowel(c))
			count++;

	return count;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	std::locale::global(std::locale(""));
	init_vowels();

	unsigned int num_vowels = 0;

	std::wstring s;
	while (std::wcin >> s)
	{
		num_vowels += count_vowels(s);
	}

	std::wcout << num_vowels << " Vowel";
	if (num_vowels != 1)
		std::wcout << "s";
	std::wcout << "\n";

	return 0;
}