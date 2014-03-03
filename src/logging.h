#ifndef LOGGING_H_
#define LOGGING_H_

// Struct containing the log data from a function call
// Forms a linked list with other entries
struct log_data {

	int id;
	char *fname;
	float value;
	char *msg;

	bool begin;
	bool end;

	struct log_data *prev;
	struct log_data *next;
};

// Logging functions
void dump(struct log_data *first, struct robot *bot);
struct log_data *init_log();
struct log_data *log(struct log_data *data, struct log_data *last);

#endif