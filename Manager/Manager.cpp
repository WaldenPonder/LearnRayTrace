#include "..\LearnRayTrace\util.h"
// Manager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

void combine(int kCount);

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
	for (int kCount = 1; kCount <= 3; kCount++)
	{
		combine(kCount);
	}

	system("pause");
}

void combine(int kCount)
{
	vector<string> res_xstr, res_ystr, res_zstr;

	for (int j = 1; j <= kCount; j++)
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

		if (res_xstr.size() == 0)
		{
			res_xstr.resize(xstr.size());
			res_ystr.resize(xstr.size());
			res_zstr.resize(xstr.size());

			std::fill(res_xstr.begin(), res_xstr.end(), "0");
			std::fill(res_ystr.begin(), res_ystr.end(), "0");
			std::fill(res_zstr.begin(), res_zstr.end(), "0");
		}

		for (int i = 0; i < xstr.size(); i++)
		{
			if (xstr[i].front() >= '0' && xstr[i].front() <= '9')
			{
				float tmp = atof(xstr[i].c_str());			
				float val = atof(res_xstr[i].c_str()) + tmp;
				res_xstr[i] = std::to_string(val);
			}
			else
			{
				res_xstr[i] = xstr[i];
			}
		}

		for (int i = 0; i < ystr.size(); i++)
		{
			if (ystr[i].front() >= '0' && ystr[i].front() <= '9')
			{
				float tmp = atof(ystr[i].c_str());
				float val = atof(res_ystr[i].c_str()) + tmp;
				res_ystr[i] = std::to_string(val);
			}
			else
			{
				res_ystr[i] = ystr[i];
			}
		}

		for (int i = 0; i < zstr.size(); i++)
		{
			if (zstr[i].front() >= '0' && zstr[i].front() <= '9')
			{
				float tmp = atof(zstr[i].c_str());
				float val = atof(res_zstr[i].c_str()) + tmp;
				res_zstr[i] = std::to_string(val);
			}
			else
			{
				res_zstr[i] = zstr[i];
			}
		}

		IFILE.close();
	}

	std::ofstream OF(getExeDir() + "/combine" + std::to_string(kCount) + ".ppm");

	for (int i = 0; i < res_xstr.size(); i++)
	{
		if (res_xstr[i].front() >= '0' && res_xstr[i].front() <= '9')
		{
			int val = atof(res_xstr[i].c_str()) / (float)kCount;
			OF << std::to_string(val);
		}
		else
		{
			OF << res_xstr[i];
		}

		OF << "\t";

		if (res_ystr[i].front() >= '0' && res_ystr[i].front() <= '9')
		{
			int val = atof(res_ystr[i].c_str()) / (float)kCount;
			OF << std::to_string(val);
		}
		else
		{
			OF << res_ystr[i];
		}

		OF << "\t";

		if (res_zstr[i].front() >= '0' && res_zstr[i].front() <= '9')
		{
			int val = atof(res_zstr[i].c_str()) / (float)kCount;
			OF << std::to_string(val);
		}
		else
		{
			OF << res_zstr[i];
		}

		OF << "\n";
	}

	OF.close();
}
