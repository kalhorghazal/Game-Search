#include "load_balancer.h"

int Load_Balancer::extract_info_to_handle_command(Dataset info)
{
	int command_len = info.size();
	int filtered_len = command_len - NON_SPECIFIED_LEN;
	dir_name = info[command_len + DIR_OFFSET];
	processes = atoi(info[command_len + PROCESSES_OFFSET].c_str());

	if (command_len == NON_SPECIFIED_LEN)
	{
		return ZERO;
	}

	if (info[command_len + SORT_DIRECTION_OFFSET] == ASCENDING || 
		info[command_len + SORT_DIRECTION_OFFSET] == DESCENDING)
	{
		sort_direction = info[command_len + SORT_DIRECTION_OFFSET];
		sort_field = info[command_len + SORT_FIELD_OFFSET];
		filtered_len = command_len + SORT_FIELD_OFFSET;
	}
	else
		sort_field = NONE;

	for (int i = ZERO; i < filtered_len; ++i)
	{
		if (info[i] == EQUAL_SIGN) 
		{
			field_names.push_back(info[i-ONE]);
			query_values.push_back(info[i+ONE]);
		}
	}
}

int Load_Balancer::fill_file_paths()
{
    DIR *dir = opendir(dir_name.c_str());
    if (dir == NULL)
    {
        cout << DIRECTORY_FAILED_MESSAGE << NEW_LINE;
        return ZERO;
    }
    struct dirent *entry;
    while (entry = readdir(dir))
    {
    	string dir_path;
    	dir_path = dir_name + DIR_SEPARATOR + entry->d_name;

    	if (dir_path.find(DATASET) == string::npos)
    		continue;
    	file_paths.push_back(dir_path);
    }
    closedir(dir);
}

void Load_Balancer::evaluate_load_balance()
{
	int num_files = file_paths.size();
	load_quotient = num_files / processes;
	load_remainder = num_files - load_quotient*processes;
}

void Load_Balancer::fill_worker_messages()
{
	worker_messages.resize(processes);
	string encoded_filters = encode_filters();
	evaluate_load_balance();

	int file_index = ZERO;
	for (int i = ZERO; i < processes; ++i)
	{
		string message = encoded_filters;
		for (int i = ZERO; i < load_quotient; ++i)
		{
			message += file_paths[file_index];
			file_index++;

			if (i != (load_quotient-ONE))
				message += FILE_SEPARATOR;
		}
		if (i < load_remainder)
		{
			message += (FILE_SEPARATOR + file_paths[file_index]);
			file_index++;
		}

		worker_messages[i] = message; 
	}
}

void Load_Balancer::fill_presenter_message()
{
	string message = encode_ids();
	if (sort_field != NONE)
	{
		message += PART_SEPARATOR;
		message += (sort_field + EQUAL_SIGN + sort_direction);
	}

	presenter_message = (Message) message.c_str(); 
}

string Load_Balancer::encode_ids()
{
	string encoded_ids = EMPTY;
	for (int i = ZERO; i < worker_ids.size(); ++i)
	{
		encoded_ids += to_string(worker_ids[i]);
		if (i != (worker_ids.size() - ONE))
			encoded_ids += ID_SEPARATOR;
	}

	return encoded_ids;
}

string Load_Balancer::encode_filters()
{
	string encoded_filters = EMPTY;

	for (int i = ZERO; i < field_names.size(); ++i)
	{
		encoded_filters += (field_names[i] + EQUAL_SIGN + query_values[i]);
		if (i != (field_names.size() - ONE))
			encoded_filters += FILTER_SEPARATOR;
	}

	if (encoded_filters != EMPTY)
	{
		encoded_filters += PART_SEPARATOR;
	}
	return encoded_filters;
}

void Load_Balancer::handle_command(string command)
{
	Dataset info = split(command, SPACE);
	extract_info_to_handle_command(info);
	fill_file_paths();
	fill_worker_messages();
	hire_workers();
	fill_presenter_message();
	hire_presenter();
	wait_for_staff();
}

int Load_Balancer::hire_workers()
{
	for (int i = ZERO; i < processes; ++i)
	{
		int fd[TWO];
		Pid p;

		if (pipe(fd) == UNSUCCESSFUL)
		{
			cout << PIPE_FAILED_MESSAGE << NEW_LINE; 
			return ONE;
		}

		p = fork();

		if (p < ZERO)
		{
			cout << FORK_FAILED_MESSAGE << NEW_LINE; 
			return ONE;
		}
		else if (p > ZERO)
		{
			close(fd[READ_END]);
            write(fd[WRITE_END], (Message) worker_messages[i].c_str(), 
            	strlen((Message) worker_messages[i].c_str()) + ONE);
            close(fd[WRITE_END]);
            worker_ids.push_back(p);
		}
		else
		{
            close(fd[WRITE_END]);
            char input[MAXLINE];
            read(fd[READ_END], input, MAXLINE);
            close(fd[READ_END]);
            char *args[] = {(Message)WORKER_DIR, (Message)input, NULL};
            execv(args[ZERO], args);
		}
	}
}

int Load_Balancer::hire_presenter()
{
	unlink(PATH_NAME);
	mkfifo(PATH_NAME, READ_WRITE);
	
	int fd[TWO];	
	Pid p;

	p = fork();

	if (p < ZERO)
	{
		cout << FORK_FAILED_MESSAGE << NEW_LINE; 
		return ONE;
	}
	else if (p > ZERO)
	{
		presenter_id = p;
		close(fd[READ_END]);
	}
	else
	{
		close(fd[WRITE_END]);
		char *args[] = {(Message)PRESENTER_DIR, (Message)"1", NULL};
		execv(args[ZERO], args);
	}

	int fds = open(PATH_NAME, O_WRONLY);
    write(fds, (Message) presenter_message.c_str(), 
    	strlen((Message) presenter_message.c_str()) + ONE);
    close(fds);
}

void Load_Balancer::wait_for_staff()
{
	for (int i = ZERO; i < processes; ++i)
	{
		waitpid(worker_ids[i], NULL, WNOHANG);
	}
	waitpid(presenter_id, NULL, WNOHANG);
}

int main(int argc, char const *argv[])
{
	string command;

	while(TRUE)
	{
		getline(cin, command);
		if (command == QUIT)
		{
			break;
		}

		Load_Balancer load_balancer;

		load_balancer.handle_command(command);
	}

	return ZERO;
}