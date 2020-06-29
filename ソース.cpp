#include <urlmon.h>
#include <fstream>
#include<string>
#include<ctime>

#pragma comment(lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

using namespace std;

#define IPnet "https://www.ip.cn/"
#define FILENAME "GetInternetIP.txt"
#define IPFILE "IP.log"
#define MAXPATH 1024

int GetInternetIP();//获取IP/現在IPを獲得する
int SaveIp();//将获取的IP写入IP.txt/獲得したIPをIP.txtに書き込む
int PutsIP_Time();//写入IP和时间/IPと現在時刻を書き込む

char IP[15] = { 0 };
char str[MAXPATH];
fstream in;
FILE* fp = NULL;

int main()
{
	GetInternetIP();
	
	//如果与之前IP相同则不写入/前のIPと同じ場合は何も書き込まない
	if (SaveIp() == 0)
	{		
		goto close;
	}
	PutsTime();
close:
	remove(FILENAME);
	return 0;
}

int GetInternetIP()
{
	URLDownloadToFile(0, TEXT(IPnet), TEXT(FILENAME), 0, NULL);

	in.open(FILENAME);
	
	//提取IP/IPを絞り出す
	for (int i = 0; i < 37; i++)
	{
		in.getline(str, MAXPATH);
	}
	for (int i = 62; i < 77; i++)
	{
		if (str[i] == '<')
			break;
		IP[i - 62] = str[i];
	}
	in.close();
	return 0;
}

int SaveIp()
{	
	int rt = 0;
	fstream fin(IPFILE, ios::ate | ios::in);

	//核对之前的IP/前のIPと照合
	fin.seekg(-3, fin.cur);

	int state = 0;

	while (fin.peek() != fin.widen('\n'))
	{
		fin.seekg(-1, fin.cur);
		if (fin.tellg() == -1)
		{
			state = 1;
			break;
		}
	}
	fin.seekg(2, fin.cur);

	char str2[MAXPATH];
	fin.getline(str2, MAXPATH);
	char oldIP[15] = { 0 };
	for (int i = 0; i < 15; i++)
	{
		if (str2[i] == ' ')
		{
			break;
		}
		oldIP[i] = str2[i];
	}
	rt = strcmp(oldIP, IP);

	fin.close();
	return rt;
}
int PutsTime()
{
	errno_t err;
	err = fopen_s(&fp, IPFILE, "a");

	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);

	fputs(IP, fp);

	fputs("    ", fp);

	fprintf(fp, "%d", t.tm_year + 1900);
	fputs("/", fp);

	fprintf(fp, "%d", t.tm_mon + 1);
	fputs("/", fp);

	fprintf(fp, "%d", t.tm_mday);
	fputs(" ", fp);

	fprintf(fp, "%d", t.tm_hour);
	fputs(":", fp);

	fprintf(fp, "%d", t.tm_min);
	fputs(":", fp);

	fprintf(fp, "%d", t.tm_sec);
	fputs("\n", fp);

	fclose(fp);
	return 0;
}
