#pragma once

#include <fstream>
#include <thread>
#include <filesystem>
#include <tlhelp32.h>

#include "Auth.h"
#include "HTTP.h"
#include "Settings.h"

#define ICON_FA_LINK "\xef\x83\x81"	// U+f0c1

class Misc
{
public:

	static void LaunchLegacyClient()
	{
		if (!std::filesystem::exists(std::format("{}LoL Arkadas", S.leaguePath)))
		{
			std::filesystem::create_directory(std::format("{}LoL Arkadas", S.leaguePath));
		}
		if (!std::filesystem::exists(std::format("{}LoL Arkadas/system.yaml", S.leaguePath)))
		{
			std::ifstream infile(std::format("{}system.yaml", S.leaguePath));
			std::ofstream outfile(std::format("{}LoL Arkadas/system.yaml", S.leaguePath));
			std::string content = "";

			std::string temp;
			while (std::getline(infile, temp))
				content += temp + "\n";

			infile.close();
			size_t pos = content.find("riotclient:");
			content = content.substr(0, pos + 11);

			outfile << content;
			outfile.close();
		}

		if (::FindWindowA("RCLIENT", "League of Legends"))
		{
			http->Request("POST", "https://127.0.0.1/process-control/v1/process/quit", "", auth->leagueHeader, "", "", auth->leaguePort);

			// m��terinin kapanmas�n� bekleyin (belki bunu yapman�n daha iyi bir y�ntemi vard�r)
			std::this_thread::sleep_for(std::chrono::milliseconds(4500));
		}

		ShellExecuteA(NULL, "open", std::format("{}LeagueClient.exe", S.leaguePath).c_str(),
			std::format("--system-yaml-override=\"{}LoL Arkadas/system.yaml\"", S.leaguePath).c_str(), NULL, SW_SHOWNORMAL);
	}

	static void CheckVersion()
	{
		std::string getLatest = http->Request("GET", "https://api.github.com/repos/Emre37destan/KralLoL/releases/latest");

		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		JSONCPP_STRING err;
		Json::Value root;
		if (reader->parse(getLatest.c_str(), getLatest.c_str() + static_cast<int>(getLatest.length()), &root, &err))
		{
			std::string latestName = root["tag_name"].asString();
			if (latestName != "1.9.0")
			{
				if (MessageBoxA(0, "�ndirme sitesi a��ls�n m�?", "Yeni s�r�m mevcut", MB_YESNO | MB_SETFOREGROUND) == IDYES)
				{
					ShellExecute(0, 0, L"https://github.com/Emre37destan/KralLoL/releases/latest", 0, 0, SW_SHOW);
				}
			}
		}
	}

	static std::string GetCurrentPatch()
	{
		std::string result = http->Request("GET", "https://ddragon.leagueoflegends.com/api/versions.json");
		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		JSONCPP_STRING err;
		Json::Value root;
		if (reader->parse(result.c_str(), result.c_str() + static_cast<int>(result.length()), &root, &err))
		{
			return root[0].asString();
		}
		return "0.0.0";
	}

	static void TaskKillLeague()
	{
		Misc::TerminateProcessByName("RiotClientServices.exe");
		Misc::TerminateProcessByName("RiotClientCrashHandler.exe");
		Misc::TerminateProcessByName("RiotClientUx.exe");
		Misc::TerminateProcessByName("RiotClientUxRender.exe");

		Misc::TerminateProcessByName("LeagueClient.exe");
		Misc::TerminateProcessByName("LeagueCrashHandler.exe");
		Misc::TerminateProcessByName("LeagueClientUx.exe");
		Misc::TerminateProcessByName("LeagueClientUxRender.exe");
	}

	static std::string ChampIdToName(int id)
	{
		if (!id)
		{
			return "Hicbiri";
		}
		else if (champSkins.empty())
		{
			return "Veri yok";// "�ampiyon verileri hala indiriliyor";
		}
		{
			for (auto &c : champSkins)
			{
				if (c.key == id)
					return c.name;
			}
		}
		return "";
	}

	static std::string ClearLogs()
	{
		std::string result = "";

		TaskKillLeague();

		std::this_thread::sleep_for(std::chrono::seconds(2));

		std::error_code errorCode;

		std::string logsFolder = S.leaguePath + "Logs";
		if (std::filesystem::exists(logsFolder))
		{
			SetFileAttributesA(logsFolder.c_str(), GetFileAttributesA(logsFolder.c_str()) & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN);
			std::filesystem::remove_all(logsFolder, errorCode);
			result += logsFolder + " - " + errorCode.message() + "\n";
		}

		std::string configFolder = S.leaguePath + "Config";
		if (std::filesystem::exists(configFolder))
		{
			SetFileAttributesA(configFolder.c_str(), GetFileAttributesA(configFolder.c_str()) & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN);
			std::filesystem::remove_all(configFolder, errorCode);
			result += configFolder + " - " + errorCode.message() + "\n";
		}

		std::string programData = "C:/ProgramData/Riot Games";
		if (std::filesystem::exists(programData))
		{
			SetFileAttributesA(programData.c_str(), GetFileAttributesA(programData.c_str()) & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN);
			std::filesystem::remove_all(programData, errorCode);
			result += programData + " - " + errorCode.message() + "\n";
		}

		char* pLocal;
		size_t localLen;
		_dupenv_s(&pLocal, &localLen, "LOCALAPPDATA");
		std::string local = pLocal;
		local += "\\Riot Games";
		if (std::filesystem::exists(local))
		{
			SetFileAttributesA(local.c_str(), GetFileAttributesA(local.c_str()) & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN);
			std::filesystem::remove_all(local, errorCode);
			result += local + " - " + errorCode.message() + "\n";
		}

		int k = 0;
		for (const auto& file : std::filesystem::directory_iterator(std::filesystem::temp_directory_path()))
		{
			std::filesystem::remove_all(file, errorCode);
			k++;
		}
		result += std::to_string(k) + " kadar dosya silindi\n";
		return result;
	}

	// �zerine gelindi�inde bir ara� ipucunu g�steren k���k bir (?) i�areti g�r�nt�lemek i�in yard�mc�.
	// Birle�tirilmi� simge yaz� tipleri kullan�yorsan�z, kendi kodunuzda ger�ek bir simge g�r�nt�lemek isteyebilirsiniz (bkz. docs/FONTS.md)
	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	static void ArrowButtonDisabled(const char* id, ImGuiDir dir)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::ArrowButton(id, dir);
		ImGui::PopStyleVar();
	}

	static void AddUnderLine(ImColor col_)
	{
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}

	static void TextURL(const char* name_, const char* URL_, uint8_t SameLineBefore_ = 1, uint8_t SameLineAfter_ = 1)
	{
		if (1 == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::Text(name_);
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				ShellExecuteA(NULL, "open", URL_, NULL, NULL, SW_SHOWNORMAL);
			}
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::SetTooltip(/*ICON_FA_LINK*/ "  Tarayicida ac\n%s", URL_);
		}
		else
		{
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
		}
		if (1 == SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
	}
	// ��lev ba�ar�l� olursa, d�n�� de�eri s�f�r de�ildir.
	static bool TerminateProcessByName(std::string sProcessName)
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 entry{};
			entry.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(snapshot, &entry))
			{
				do
				{
					char temp[260];
					sprintf(temp, "%ws", entry.szExeFile);
					if (!stricmp(temp, sProcessName.c_str()))
					{
						HANDLE process = OpenProcess(PROCESS_TERMINATE, false, entry.th32ProcessID);
						bool terminate = TerminateProcess(process, 0);
						CloseHandle(snapshot);
						CloseHandle(process);
						return terminate;
					}
				} while (Process32Next(snapshot, &entry));
			}
		}
		CloseHandle(snapshot);
		return false;
	}
};