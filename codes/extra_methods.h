#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#define SPACE ' '
#define DASH '-'
#define ONE 1
#define ZERO 0

using namespace std;

vector<string> split(const string& s, char delimiter);
void get_info_from_command_line(vector<string> &info);
void get_info_from_file(string file_path, vector<vector<string> > &info);
void get_encoded_info_from_file(string file_path, vector<string>& encoded_info);
string ignore_space(string s);