#include "worker.h"

string Worker::encode_result(Dataset result)
{
	string encoded_result = EMPTY;

	for (int i = ZERO; i < result.size(); ++i)
	{
		encoded_result += result[i];
		if (i != (result.size() - ONE))
			encoded_result += FIELD_SEPARATOR;
	}

	return encoded_result;
}

void Worker::fill_presenter_message()
{
	string message = EMPTY;

	for (int i = ZERO; i < results.size(); ++i)
	{
		message += encode_result(results[i]);
		if (i != (results.size() - ONE))
			message += RESULT_SEPARATOR;
	}

	presenter_message = message; 
}

void Worker::read_files()
{
	for (int i = ZERO; i < file_paths.size(); ++i)
	{
		Datasets dataset;
		get_info_from_file(file_paths[i], dataset);
		datasets.insert(datasets.end(), dataset.begin(), dataset.end());
	}
}

bool Worker::has_condition(Dataset dataset, string field_name, string query_value)
{
	int filter_index;

	for (int i = ZERO; i < datasets[TABLE_HEADER].size(); ++i)
	{
		if (datasets[TABLE_HEADER][i] == field_name)
		{
			filter_index = i;
			break;
		}
	}

	if (dataset[filter_index] == query_value)
		return TRUE;
	return FALSE;
}

void Worker::apply_filters()
{
	results.push_back(datasets[TABLE_HEADER]);

	for (int i = ONE; i < datasets.size(); ++i)
	{
		if (datasets[i] == datasets[TABLE_HEADER])
			continue;

		bool selected = TRUE;

		for (int j = ZERO; j < field_names.size(); ++j)
		{
			if (!has_condition(datasets[i], field_names[j], query_values[j]))
				selected = FALSE;
		}

		if (selected)
			results.push_back(datasets[i]);
	}
}

void Worker::decode_filters(string encoded_filters)
{
	Dataset filters = split(encoded_filters, FILTER_SEPARATOR);

	for (int i = ZERO; i < filters.size(); ++i)
	{
		Dataset filter = split(filters[i], EQUAL_SIGN);
		field_names.push_back(filter[FIELD_NAME]);
		query_values.push_back(filter[QUERY_VALUE]);
	}
}

void Worker::decode_file_path(string encoded_files)
{
	file_paths = split(encoded_files, FILE_SEPARATOR);
}

void Worker::get_info_from_command_line_arg(string command)
{
	Dataset info = split(command, PART_SEPARATOR);

	if (info.size() == TWO)
	{
		decode_filters(info[FILTERS]);
		decode_file_path(info[FILE_PATHS]);
	}
	else
	{
		decode_file_path(info[ONLY_FILES]);
	}
}

void Worker::handle_command(string command)
{
	get_info_from_command_line_arg(command);
	read_files();
	apply_filters();
	fill_presenter_message();
	send_results_to_presenter();
}

void Worker::send_results_to_presenter()
{
	string path_name = to_string(getpid());
	unlink(path_name.c_str());
	mkfifo(path_name.c_str(), READ_WRITE);

	int fd = open(path_name.c_str(), O_WRONLY);
	//cout << presenter_message << endl;
    write(fd, (Message)presenter_message.c_str() , strlen((Message)presenter_message.c_str()) + ONE);
    close(fd);
}

int main(int argc, char const *argv[])
{
	string command = argv[COMMAND];
    Worker worker;
    worker.handle_command(command);
}