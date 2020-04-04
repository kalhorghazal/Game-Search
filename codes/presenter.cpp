#include "presenter.h"

int sort_index;
string sort_direction;

void Presenter::decode_sort_info(string encoded_sort_info)
{
	Dataset sort_info = split(encoded_sort_info, EQUAL_SIGN);

	sort_field = sort_info[SORT_FIELD];
	sort_direction = sort_info[SORT_DIRECTION];
}

void Presenter::decode_ids(string encoded_ids)
{
	Dataset ids = split(encoded_ids, ID_SEPARATOR);
	worker_ids.resize(ids.size());

	for (int i = ZERO; i < worker_ids.size(); ++i)
	{
		worker_ids[i] = stoi(ids[i]);
	}
}

Datasets Presenter::decode_results(Dataset encoded_result)
{
	Datasets results;

	table_header = split(encoded_result[TABLE_HEADER], FIELD_SEPARATOR);

	for (int i = ONE; i < encoded_result.size(); ++i)
	{
		Dataset result = split(encoded_result[i], FIELD_SEPARATOR);
		if (table_header == result)
			continue;
		results.push_back(result);
	}

	return results;
}

void Presenter::get_info_from_command_line_arg(string command)
{
	Dataset info = split(command, PART_SEPARATOR);
	if (info.size() == TWO)
	{
		decode_sort_info(info[SORT_INFO]);
		decode_ids(info[IDS]);
	}
	else
	{
		sort_field = NONE;
		decode_ids(info[ONLY_IDS]);
	}
}

void Presenter::get_command_from_load_balancer()
{
	char command[MAX_LINE];
	int fd = open(PATH_NAME, O_RDONLY);
    read(fd, command, MAX_LINE);
    close(fd);
    handle_command(command);
}

bool sortcol(const Dataset& v1, const Dataset& v2) 
{ 
	if (sort_direction == ASCENDING)
 		return v1[sort_index] < v2[sort_index]; 

 	return v1[sort_index] > v2[sort_index]; 
} 

void Presenter::sort_results(Datasets& results)
{
	if (sort_field == NONE)
		return;

	for (int i = ZERO; i < table_header.size(); ++i)
	{
		if (table_header[i] == sort_field)
		{
			sort_index = i;
			break;
		}
	}

	 sort(results.begin(), results.end(),sortcol);
}

void Presenter::merge_results(Datasets results) 
{ 
    int i = 0, j = 0; 
    Datasets temp;
  
    while (i < results.size() && j < datasets.size()) 
    { 

        if (sortcol(results[i], datasets[j]))
            temp.push_back(results[i++]); 
        else
            temp.push_back(datasets[j++]); 
    } 
  
    while (i < results.size()) 
        temp.push_back(results[i++]);
  
    while (j < datasets.size()) 
        temp.push_back(datasets[j++]); 

    datasets = temp;
} 

void Presenter::get_results()
{
	for (int i = ZERO; i < worker_ids.size(); ++i)
	{
		Dataset encoded_result = get_result_from_worker(worker_ids[i]);
		Datasets results = decode_results(encoded_result);
		sort_results(results);

		if (i == ZERO)
			datasets = results;
		else
			merge_results(results);
	}
}

void Presenter::print_results()
{
	for (int i = ZERO; i < datasets.size(); ++i)
	{
		for (int j = ZERO; j < datasets[ZERO].size(); ++j)
		{
			cout << datasets[i][j];

			if (j != (datasets[ZERO].size()-ONE))
				cout << SPACE << FIELD_SEPARATOR << SPACE;
			else
				cout << NEW_LINE;
		}
	}
}

Dataset Presenter::get_result_from_worker(Pid id)
{
	Dataset encoded_result;
	string path_name = to_string(id);
	usleep(TIME_DELAY);
	get_encoded_info_from_file(path_name, encoded_result);

    return encoded_result;
}

void Presenter::handle_command(string command)
{
	get_info_from_command_line_arg(command);
	get_results();
	print_results();
}

int main(int argc, char const *argv[])
{
	Presenter presenter;
	presenter.get_command_from_load_balancer();
	return ZERO;
}