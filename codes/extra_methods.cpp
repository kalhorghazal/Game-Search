#include "extra_methods.h"
vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
      tokens.push_back(token);
   return tokens;
}

void get_info_from_command_line(vector<string> &info)
{
	string line;
	getline(cin, line);
	for (int i = ONE; i < split(line, SPACE).size(); ++i)
	{
		info.push_back(split(line, SPACE)[i]);
	}
}

string ignore_space(string s)
{
	string s2;
	
	if(s[ZERO] == SPACE)
	{
		for (int i = ONE; i < s.size(); ++i)
	    {
		    s2.push_back(s[i]);
	    }
	    s = s2;
	}

	if(s[s.size()-ONE] == SPACE)
	{
		s.pop_back();
	}
	return s;
}

void get_encoded_info_from_file(string file_path, vector<string>& encoded_info)
{
	string line;
	ifstream file(file_path);
	while (getline(file, line))
	{
		encoded_info.push_back(line);
	}

	file.close();
}

void get_info_from_file(string file_path, vector<vector<string> > &info)
{
	string line;
	ifstream file(file_path);
	while (getline(file, line))
	{
		vector<string> data;
		for (int i = ZERO; i < split(line, DASH).size(); ++i)
		{
			data.push_back(ignore_space(split(line, DASH)[i]));
		}
		info.push_back(data);
	}

	file.close();
}