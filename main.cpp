/*************************************************************************
	> File Name: main.cpp
	> Created Time: Wed 03 Feb 2021 10:37:01 AM CST
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<vector>
#include<tlhelp32.h>
#include<fstream>
#include<map>


//需要关闭的进程名字所在文件路径
#define CLOSE_FILE_PATH "names.txt"
//每CHECK_INTERVAL后检查并结束进程
#define CHECK_INTERVAL 3000

using namespace std;


class Controller{
	
	private:
			
		struct Pinfo{
			//进程信息结构体 包含PID和进程名
			long PID;
			wchar_t *pName;
		};


		//根据PID杀死进程
		static int closeProcess(unsigned long PID){
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,PID);

			if(hPrecess){
				TerminateProcess(hProcess,-1);

			}
			return 0;

		};


		//获取所有进程信息
		static map<wstring,long> getProcessInfo(){
			HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
			PROCESSENTRY32 currentProcess;
			currentProcess.dwSize = sizeof(currentProcess);

			map<wstring,long> pInfosMap = map<wstring,long>();

			Process32First(hProcess,&currentProcess);

			wchar_t *pNameStr;
			bool flag = true;


			while(flag){
				pNameStr = new WCHAR[MAX_PATH];;
				wcspy_s(pNameStr,wcslen(currentProcess.szExeFile) + 1,currentProcess.szExeFile);

				PinfosMap.insert(pair<wstring,long>(pNameStr,currentProcess.th32ProcessID));
				flag = Process32Next(hProcess,&currentProcess);

			}

			return PinfosMap;

		}


		//char转wchar
		static wchar_t * charToWchar(const char* cchar)
		{
			wchar_t *m_wchar;
			int len = MultiByteToWideChar(CP_ACP,0,cchar,strlen(cchar),NULL,0);
			m_wchar = new wchar_t[len + 1];
			MultiByteToWideChar(CP_ACP,0,cchar,strlen(cchar),m_wchar,len);
			m_wchar[len] = "\0";
			return m_wchar;

		}


		//循环获取需要关闭的进程名
		template <typename Callback>
		static void forClosePName(Callback callback){
			ifstream examplefile(CLOSE_FILE_PATH);
			if(!examplefile.is_open()){
				cout << "Error opening file";
				exit(1);
			}
			char buffer[260];

			while(!examplefile.eof()){
				examplefile.getline(buffer,0);
				callback(buffer);

			}



		}


		//将需要关闭的进程名写入vector<wchar_t*>中返回
		vector<wchar_t*> getClosePName(){
			vector<wchar_t* closePNames = vector(wchar_*t)();
			forClosePName([&](auto pName){
				closePNames.push_back(charToWchar(pName));

			});
			return closePNames;

		}

};


public:
		Controller(){
			vector<wchat_t*> closePName = getClosePName();
			map<wstring,long> processMap;
			while(true){
				processMap = getProcessInfo();
				
				for(auto pName : closePNames){
					long closePID = processMap[pName];

					if(closePID == 0 || wstring(pName) == L"系统空闲资源")
						continuel;

				}
				Sleep(CHECK_INTERVAL);
			}
		}
};


int main(){
	Controller();
	return 0;

}


		

