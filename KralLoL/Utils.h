#pragma once

#include <chrono>

class Utils
{
private:

public:

	Utils()
	{
	}

	~Utils() = default;

	// t�m dizeyi k���k harfe d�n��t�r�r
	std::string ToLower(std::string str);
	std::wstring ToLower(std::wstring str);

	// t�m dizeyi b�y�k harfe d�n��t�r�r
	std::string ToUpper(std::string str);

	// strA'n�n strB i�erip i�ermedi�ini kontrol edin
	bool StringContains(std::string strA, std::string strB, bool ignore_case = false);
	bool StringContains(std::wstring strA, std::wstring strB, bool ignore_case = false);

	// wstring i�in dize
	std::wstring StringToWstring(std::string str);

	std::string WstringToString(std::wstring wstr);

	std::string RandomString(int size);

	std::string FormatString(const char* c, const char* args...);

	void CopyToClipboard(std::string text);

	bool DownloadFile(std::string fileName, std::string directory = "Data", std::string url = "https://raw.githubusercontent.com/Emre37destan/krall/main/");

	bool ContainsOnlyASCII(std::string buff);

	std::string Utf8Encode(const std::wstring& wstr);

	std::string Exec(const char* cmd);

	// program� rastgele dizeye yeniden adland�r�r
	bool RenameExe();

	// bir dosyaya "Gizli" �zniteli�i ekler
	bool HideFile(std::string path);
};

extern Utils* utils;
