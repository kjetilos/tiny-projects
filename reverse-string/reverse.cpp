#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	// Sets the default environment locale since the startup
	// locale is "C" locale which does not handle wcin correctly
	setlocale(LC_ALL, "");

	std::wstring s;
	std::wcin >> s;
	for (auto i = s.rbegin(); i != s.rend(); ++i) {
		std::wcout << *i ;
	}
	std::wcout << "\n";

	return 0;
}
