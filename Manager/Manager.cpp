// Manager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

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
	
	std::fill(res_xstr.begin(), res_xstr.end(), "0");
	std::fill(res_ystr.begin(), res_ystr.end(), "0");
	std::fill(res_zstr.begin(), res_zstr.end(), "0");

	for (int j = 1; j <= count; j++)
	{
		std::ifstream IFILE;
		IFILE.open(getExeDir() + "/exe/" + std::to_string(j) + "/out.ppm");

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
			if (std::isdigit(xstr[i].front()))
			{
				 float val = atof(res_xstr[i].c_str()) + atof(xstr[i].c_str()) / count;
				res_xstr[i] = std::to_string(val);
			}
			else
			{
				res_xstr[i] = xstr[i];
			}			
		}

		for (int i = 0; i < ystr.size(); i++)
		{
			if (std::isdigit(ystr[i].front()))
			{
				float val = atof(res_ystr[i].c_str()) + atof(ystr[i].c_str()) / count;
				res_ystr[i] = std::to_string(val);
			}
			else
			{
				res_ystr[i] = ystr[i];
			}
		}

		for (int i = 0; i < zstr.size(); i++)
		{
			if (std::isdigit(zstr[i].front()))
			{
				float val = atof(res_zstr[i].c_str()) + atof(zstr[i].c_str()) / count;
				res_zstr[i] = std::to_string(val);
			}
			else
			{
				res_zstr[i] = zstr[i];
			}
		}

		IFILE.close();
	}

	std::ofstream OF(getExeDir() + "/combine.ppm");

	for (int i = 0; i < res_xstr.size(); i++)
	{
		if (std::isdigit(res_xstr[i].front()))
		{
			int val = atof(res_xstr[i].c_str());
			OF << std::to_string(val);
		}
		else
		{
			OF << res_xstr[i];
		}

		OF << " ";

		if (std::isdigit(res_ystr[i].front()))
		{
			int val = atof(res_ystr[i].c_str());
			OF << std::to_string(val);
		}
		else
		{
			OF << res_ystr[i];
		}

		OF << " ";

		if (std::isdigit(res_zstr[i].front()))
		{
			int val = atof(res_zstr[i].c_str());
			OF << std::to_string(val);
		}
		else
		{
			OF << res_zstr[i];
		}

		OF << "\n";
	}

	OF.close();
	system("pause");
}