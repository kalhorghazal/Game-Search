#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "extra_methods.h"

#include <iostream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h> 
#include <dirent.h>
#include <fcntl.h> 

#define SPACE ' '
#define EQUAL_SIGN "="
#define NEW_LINE '\n'
#define DIR_OFFSET -1
#define PROCESSES_OFFSET -5
#define SORT_DIRECTION_OFFSET -9
#define SORT_FIELD_OFFSET -11
#define NON_SPECIFIED_LEN 7
#define ASCENDING "ascending"
#define DESCENDING "descending"
#define TWO 2
#define READ_END 0
#define WRITE_END 1
#define NEW_LINE '\n'
#define ONE 1
#define ZERO 0
#define UNSUCCESSFUL -1
#define DIR_SEPARATOR "/"
#define FILTER_SEPARATOR "$"
#define PART_SEPARATOR "|"
#define FILE_SEPARATOR "*"
#define ID_SEPARATOR "&"
#define EMPTY ""
#define MAXLINE 2048
#define READ_WRITE 0666
#define NONE ""
#define PATH_NAME "presenter"
#define FORK_FAILED_MESSAGE "Fork Failed!"
#define PIPE_FAILED_MESSAGE "Pipe Failed!"
#define DIRECTORY_FAILED_MESSAGE "Read Directory Failed!"
#define QUIT "quit"
#define TRUE true
#define FALSE false
#define DATASET "dataset"
#define WORKER_DIR "./worker.out"
#define PRESENTER_DIR "./presenter.out"

typedef vector<string> Dataset;
typedef char* Message;
typedef vector<pid_t> Pids;
typedef pid_t Pid;

using namespace std;

class Load_Balancer
{
public:
	Load_Balancer() {}
	void handle_command(string command);
	int extract_info_to_handle_command(Dataset info);
	int fill_file_paths();
	void fill_worker_messages();
	void fill_presenter_message();
	void evaluate_load_balance();
	string encode_ids();
	string encode_filters();
	int hire_workers();
	int hire_presenter();
	void wait_for_staff();

private:
	Dataset field_names;
	Dataset query_values;
	string sort_field;
	string sort_direction;
	int processes;
	string dir_name;
	Dataset file_paths;
	int load_quotient;
	int load_remainder;
	Dataset worker_messages;
	string presenter_message;
	Pids worker_ids;
	Pid presenter_id;
};
#endif