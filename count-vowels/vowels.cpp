#include <iostream>
#include <string>

static std::wstring vowels = L"aeiouyAEIOUY";

bool is_vowel(wchar_t c)
{
	return vowels.find(c) != std::wstring::npos;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

	std::wstring s;
	std::wcin >> s;

	unsigned int num_vowels = 0;
	for (wchar_t c : s)
		if (is_vowel(c))
			num_vowels++;


	std::wcout << num_vowels << " Vowel";
	if (num_vowels != 1)
		std::wcout << "s";
	std::wcout << "\n";

	return 0;
}