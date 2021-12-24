#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Includes.h"

#pragma warning (disable: 4996)

struct Settings
{
	Settings()
	{};
	std::string settingsFile = "config.JSON";

	bool autoRename = false;
	std::string leaguePath = "D:/Games/Riot Games/League of Legends/";
	std::vector<std::string>vFonts;
	int selectedFont = 0;
	bool bAddFont = false;
	bool streamProof = false;
	bool debugger = false;
	std::string currentDebugger;

	struct
	{
		int width = 1207;
		int height = 633;
		bool resize = false;
	}Window;

	struct
	{
		std::string playerName;
	}infoTab;

	struct
	{
		std::string method;
		std::string urlText;
		std::string requestText;
	}customTab;

	struct
	{
		std::string destination;
		std::string method;
		std::string args;
	}invokeTab;

	struct
	{
		std::string language = "tr_TR";
		std::string leagueArgs = "--locale=tr_TR";
	}loginTab;

	struct
	{
		bool autoAcceptEnabled = false;
		bool instalockEnabled = false;
		int instalockId = 0;
		int instalockDelay = 0;
		std::string instantMessage;
		int instantMessageDelay = 0;
		int autoBanId = 0;
		bool dodgeOnBan = false;
		int backupId = 0;
	}gameTab;
};
extern Settings S;

class CSettings
{
public:

	static void Save()
	{
		// dosya yoksa, ayr��t�r�labilmesi i�in {} ile yeni bir tane olu�turun
		if (!std::filesystem::exists(S.settingsFile))
		{
			std::ofstream file(S.settingsFile);
			file << "{}";
			file.close();
		}

		Json::Reader reader;
		Json::Value root;

		std::ifstream iFile(S.settingsFile);

		if (iFile.good())
		{
			if (reader.parse(iFile, root, false))
			{
				root["autoRename"] = S.autoRename;
				root["leaguePath"] = S.leaguePath;
				root["debugger"] = S.debugger;
				root["window"]["width"] = S.Window.width;
				root["window"]["height"] = S.Window.height;
				root["selectedFont"] = S.selectedFont;
				root["loginTab"]["language"] = S.loginTab.language;
				root["loginTab"]["leagueArgs"] = S.loginTab.leagueArgs;
				root["streamProof"] = S.streamProof;
				root["infoTab"]["playerName"] = S.infoTab.playerName;
				root["customTab"]["method"] = S.customTab.method;
				root["customTab"]["urlText"] = S.customTab.urlText;
				root["customTab"]["requestText"] = S.customTab.requestText;
				root["invokeTab"]["destination"] = S.invokeTab.destination;
				root["invokeTab"]["method"] = S.invokeTab.method;
				root["invokeTab"]["args"] = S.invokeTab.args;
				root["gameTab"]["autoAcceptEnabled"] = S.gameTab.autoAcceptEnabled;
				root["gameTab"]["instalockEnabled"] = S.gameTab.instalockEnabled;
				root["gameTab"]["instalockDelay"] = S.gameTab.instalockDelay;
				root["gameTab"]["instalockId"] = S.gameTab.instalockId;
				root["gameTab"]["instantMessage"] = S.gameTab.instantMessage;
				root["gameTab"]["instantMessageDelay"] = S.gameTab.instantMessageDelay;
				root["gameTab"]["autoBanId"] = S.gameTab.autoBanId;
				root["gameTab"]["dodgeOnBan"] = S.gameTab.dodgeOnBan;
				root["gameTab"]["backupId"] = S.gameTab.backupId;

				if (S.bAddFont)
				{
					S.bAddFont = false;
					if (!root["fonts"].isArray())
						root["fonts"] = Json::Value(Json::arrayValue);
					Json::Value fontsArray = root["fonts"];

					// temizle, b�ylece ayn� yaz� tiplerini tekrar eklemeyiz
					fontsArray.clear();
					for (std::string font : S.vFonts)
						fontsArray.append(font);
					root["fonts"] = fontsArray;
				}

				if (!root.toStyledString().empty())
				{
					std::ofstream oFile(S.settingsFile);
					oFile << root.toStyledString() << std::endl;
					oFile.close();
				}
			}
		}
		iFile.close();
	}

	static void Load()
	{
		std::fstream file(S.settingsFile, std::ios_base::in);
		if (file.good())
		{
			std::string config;
			std::string temp;
			while (std::getline(file, temp))
			{
				config += temp + "\n";
			}

			Json::Value root;
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			JSONCPP_STRING err;

			if (reader->parse(config.c_str(), config.c_str() + static_cast<int>(config.length()), &root, &err))
			{
				if (auto t = root["autoRename"]; !t.empty()) S.autoRename = t.asBool();
				if (auto t = root["leaguePath"]; !t.empty()) S.leaguePath = t.asString();
				if (auto t = root["debugger"]; !t.empty()) S.debugger = t.asBool();
				if (auto t = root["window"]["width"]; !t.empty()) S.Window.width = t.asInt();
				if (auto t = root["window"]["height"]; !t.empty()) S.Window.height = t.asInt();
				if (auto t = root["selectedFont"]; !t.empty()) S.selectedFont = t.asInt();
				if (auto t = root["loginTab"]["language"]; !t.empty()) S.loginTab.language = t.asString();
				if (auto t = root["loginTab"]["leagueArgs"]; !t.empty()) S.loginTab.leagueArgs = t.asString();
				if (auto t = root["streamProof"]; !t.empty()) S.streamProof = t.asBool();
				if (auto t = root["infoTab"]["playerName"]; !t.empty()) S.infoTab.playerName = t.asString();
				if (auto t = root["customTab"]["method"]; !t.empty()) S.customTab.method = t.asString();
				if (auto t = root["customTab"]["urlText"]; !t.empty()) S.customTab.urlText = t.asString();
				if (auto t = root["customTab"]["requestText"]; !t.empty()) S.customTab.requestText = t.asString();
				if (auto t = root["invokeTab"]["destination"]; !t.empty()) S.invokeTab.destination = t.asString();
				if (auto t = root["invokeTab"]["method"]; !t.empty()) S.invokeTab.method = t.asString();
				if (auto t = root["invokeTab"]["args"]; !t.empty()) S.invokeTab.args = t.asString();
				if (auto t = root["gameTab"]["autoAcceptEnabled"]; !t.empty()) S.gameTab.autoAcceptEnabled = t.asBool();
				if (auto t = root["gameTab"]["instalockEnabled"]; !t.empty()) S.gameTab.instalockEnabled = t.asBool();
				if (auto t = root["gameTab"]["instalockDelay"]; !t.empty()) S.gameTab.instalockDelay = t.asInt();
				if (auto t = root["gameTab"]["instalockId"]; !t.empty()) S.gameTab.instalockId = t.asInt();
				if (auto t = root["gameTab"]["instantMessage"]; !t.empty()) S.gameTab.instantMessage = t.asString();
				if (auto t = root["gameTab"]["instantMessageDelay"]; !t.empty()) S.gameTab.instantMessageDelay = t.asInt();
				if (auto t = root["gameTab"]["autoBanId"]; !t.empty()) S.gameTab.autoBanId = t.asInt();
				if (auto t = root["gameTab"]["dodgeOnBan"]; !t.empty()) S.gameTab.dodgeOnBan = t.asBool();
				if (auto t = root["gameTab"]["backupId"]; !t.empty()) S.gameTab.backupId = t.asInt();

				if (root["fonts"].isArray() && !root["fonts"].empty())
				{
					for (Json::Value::ArrayIndex i = 0; i < root["fonts"].size(); i++)
					{
						S.vFonts.emplace_back(root["fonts"][i].asString());
					}
				}
			}
		}
		file.close();
	}
};
