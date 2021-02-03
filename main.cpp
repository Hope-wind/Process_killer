/*************************************************************************
	> File Name: main.cpp
	> Created Time: Wed 03 Feb 2021 10:37:01 AM CST
 ************************************************************************/

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

// 需要关闭进程名字所在文件路径
#define CLOSE_FILE_PATH "names.txt"
// 每CHECK_INTERVAL时间(毫秒)后检查并结束一次进程
#define CHECK_INTERVAL 3000

class Controller {

private:
	
	// 进程信息结构体，包含进程id和进程名
	struct PInfo {
		long pId;
		wchar_t *pName;

		PInfo(long pId, wchar_t *pName) : pId(pId), pName(pName) {}
	};

	// 根据pid关闭进程
	static int closeProcess(unsigned long pid) {

		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	
		if (hProcess) {
			TerminateProcess(hProcess, -1);
		}
		
		return 0;
	}

	// 获取所有进程信息
	static map<wstring, long> getProcessInfo() {
		HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 currentProcess;
		currentProcess.dwSize = sizeof(currentProcess);

		map<wstring, long> pInfosMap = map<wstring, long>();

		Process32First(hProcess, &currentProcess);

		wchar_t *pNameStr;
		bool flag = true;

		while (flag) {
			pNameStr = new WCHAR[MAX_PATH];

			// wcscpy(pNameStr, currentProcess.szExeFile);
			wcscpy_s(pNameStr, wcslen(currentProcess.szExeFile) + 1, currentProcess.szExeFile);

			pInfosMap.insert(pair<wstring, long>(pNameStr, currentProcess.th32ProcessID));
			flag = Process32Next(hProcess, &currentProcess);
		}

		return pInfosMap;
	}

	// char*转wchar*
	static wchar_t * charToWchar(const char* cchar)
	{
		wchar_t *m_wchar;
		int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
		m_wchar = new wchar_t[len + 1];
		MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	// 循环获取需要关闭的进程名
	template <typename Callback>
	static void forClosePName(Callback callback) {
		ifstream examplefile(CLOSE_FILE_PATH);
		if (!examplefile.is_open()) {
			cout << "Error opening file";
			exit(1);
		}

		char buffer[260];
		
		while (!examplefile.eof()) {

			examplefile.getline(buffer, 260);

			callback(buffer);
		}
	}

	// 将需要关闭的进程名保存到vector<wchar_t*>中返回
	vector<wchar_t*> getClosePName() {
		vector<wchar_t*> closePNames =  vector<wchar_t*>();
		forClosePName([&](auto pName) {
			closePNames.push_back(charToWchar(pName));
		});
		return closePNames;
	}

public:
	Controller() {

		// 需要结束的进程名
		vector<wchar_t*> closePNames = getClosePName();
		// 所有进程信息map
		map<wstring, long> processMap;

		while (true) {

			processMap = getProcessInfo();

			for (auto pName : closePNames) {
				
				//printf("%ls \n", pName);

				long closePId = processMap[pName];

				if (closePId == 0 || wstring(pName) == L"系统空闲进程")
					continue;

				closeProcess(closePId);
			}

			Sleep(CHECK_INTERVAL);
		}
	}
};

int main() {

	Controller();

	return 0;
}
		

