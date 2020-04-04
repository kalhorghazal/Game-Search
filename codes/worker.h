#ifndef WORKER_H
#define WORKER_H

#include "extra_methods.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define EQUAL_SIGN '='
#define FILTER_SEPARATOR '$'
#define PART_SEPARATOR '|'
#define FILE_SEPARATOR '*'
#define EMPTY ""
#define FIELD_NAME 0
#define QUERY_VALUE 1
#define TWO 2
#define FILTERS 0
#define FILE_PATHS 1
#define ONLY_FILES 0
#define TABLE_HEADER 0
#define TRUE true
#define FALSE false
#define FIELD_SEPARATOR "-"
#define RESULT_SEPARATOR "\n"
#define COMMAND 1
#define READ_WRITE 0666
#define EXE_SIGN "./"
#define ONE 1
#define ZERO 0

typedef vector<vector<string> > Datasets;
typedef vector<string> Dataset;
typedef char* Message;

using namespace std;

class Worker
{
public:
	Worker() {}
	void handle_command(string command);
	void get_info_from_command_line_arg(string command);
	void decode_filters(string encoded_filters);
	void decode_file_path(string encoded_files);
	void read_files();
	bool has_condition(Dataset dataset, string field_name, string query_value);
	void apply_filters();
	string encode_result(Dataset result);
	void fill_presenter_message();
	void send_results_to_presenter();
private:
	Dataset field_names;
	Dataset query_values;
	Dataset file_paths;
	Datasets datasets;
	Datasets results;
	string presenter_message;
};
#endif