#include <iostream>
#include <string>

static std::wstring vowels = L"aeiouyAEIOUY";

bool is_vowel(wchar_t c)
{
	return vowels.find(c) != std::wstring::npos;
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