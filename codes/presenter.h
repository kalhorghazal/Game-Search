#ifndef PRESENTER_H
#define PRESENTER_H

#include "extra_methods.h"

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm> 

#define EQUAL_SIGN '='
#define PART_SEPARATOR '|'
#define ID_SEPARATOR '&'
#define EMPTY ""
#define SORT_FIELD 0
#define SORT_DIRECTION 1
#define TWO 2
#define SORT_INFO 1
#define IDS 0
#define ONLY_IDS 0
#define MAX_LEN 10
#define NEW_LINE '\n'
#define FIELD_SEPARATOR '-'
#define RESULT_SEPARATOR '%'
#define ZERO 0
#define ONE 1
#define END_CHAR '\0'
#define PATH_NAME "presenter"
#define MAX_LINE 2048
#define EXE_SIGN "./"
#define NONE ""
#define ASCENDING "ascending"
#define DESCENDING "descending"
#define TABLE_HEADER 0
#define SPACE ' '
#define TIME_DELAY 1500000

typedef vector<vector<string> > Datasets;
typedef vector<string> Dataset;
typedef vector<pid_t> Pids;
typedef pid_t Pid;

using namespace std;

class Presenter
{
public:
	Presenter() {}
	void handle_command(string command);
	void get_info_from_command_line_arg(string command);
	void get_command_from_load_balancer();
	Datasets decode_results(Dataset encoded_result);
	void decode_sort_info(string encoded_sort_info);
	void decode_ids(string encoded_ids);
	void sort_results(Datasets& results);
	Dataset get_result_from_worker(Pid id);
	void merge_results(Datasets results);
	void get_results();
	void print_results();
private:
	Pids worker_ids;
	string sort_field;
	Datasets datasets;
	Dataset table_header;
};
#endif