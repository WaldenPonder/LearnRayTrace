// Manager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

std::string getExeDir()
{
	char* str = 0;
	_get_pgmptr(&str);
	string ss(str);
	ss = ss.substr(0, ss.find_last_of("\\") + 1);
	return ss;
}

int main()
{	
	float count = 3.f;
	vector<string> res_xstr, res_ystr, res_zstr;
	for (int i = 1; i <= count; i++)
	{
		std::ifstream IFILE;
		IFILE.open(getExeDir() + "/exe/" + std::to_string(i) + "/out.ppm");

		string x, y, z;
		vector<string> xstr, ystr, zstr;

		while (!IFILE.eof())
		{
			IFILE >> x >> y >> z;
			xstr.push_back(x);
			ystr.push_back(y);
			zstr.push_back(z);
		}
		res_xstr.resize(xstr.size());
		res_ystr.resize(xstr.size());
		res_zstr.resize(xstr.size());

		for (int i = 0; i < xstr.size(); i++)
		{
			atoi(xstr[i]);
		}

		IFILE.close();
	}


	system("pause");
}