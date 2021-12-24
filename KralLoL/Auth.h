#pragma once

#include <string>
#include "base64.h"

class Auth
{
public:
	int riotPort = 0;
	std::string riotToken;
	std::string riotHeader;

	int leaguePort = 0;
	std::string leagueToken;
	std::string leagueHeader;

	Base64 base64;

	// ba�ar�yla ba�lant� noktas� ve belirte� al�nd���nda 1 d�nd�r�r
	bool GetRiotClientInfo();

	void MakeRiotHeader();

	// ba�ar�yla ba�lant� noktas� ve belirte� al�nd���nda 1 d�nd�r�r
	bool GetLeagueClientInfo();

	void MakeLeagueHeader();
private:
	std::wstring GetProcessCommandLine(std::string sProcessName);
};

extern Auth* auth;
