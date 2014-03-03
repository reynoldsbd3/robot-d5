#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>

#include "logging.h"
#include "robot.h"

// Prototypes
void update(struct log_data *data);

/* Displays log data sequentially and allows navigation
 *
 * PARAM bot : struct robot*
 *		Location of robot struct with all robot's peripherals
 *
 * RETURN void
 *		This method returns nothing
 */
void dump(struct robot *bot) {

	// Variable declarations
	struct log_data *current = bot->journal;
	bool dumping = true;

	// Present a menu to view logs
	while (dumping) {

		if ((*bot->btns).LeftPressed() &&
			!current->prev->begin) {

			current = current->prev;
			update(current);

		} else if ((*bot->btns).RightPressed() &&
			!current->next->end) {

			current = current->next;
			update(current);
		} else if ((*bot->btns).MiddlePressed()) {

			dumping = false;
		}

		// Keep the display from jumping
		Sleep(250);
	}
}

/* Returns address of a new empty linked list for storing log data.
 *
 * RETURN struct log_data*
 *		Returns a pointer to a log_data item in a linked list
 */
struct log_data *init_log() {

	// Variable declarations
	struct log_data new_begin;
	struct log_data new_end;

	// Set up initial structs
	new_begin.begin = true;
	new_begin.end = false;
	new_end.begin = false;
	new_end.end = true;
	new_begin.id = 0;

	// Link initial structs
	new_begin.next = &new_end;
	new_end.prev = &new_begin;

	// Return address of new empty linked list
	return &new_begin;
}

/* Logs a data entry in the linked list of entries.
 *
 * PARAM current : struct log_data*
 *		Location of a node in the current list of data
 *
 * PARAM new_data : struct log_data*
 *		Location of new node to insert
 *
 * RETURN struct log_data*
 *		Pointer to the last entry in the linked list
 */
struct log_data *log(struct log_data *current, struct log_data *new_data) {

	// Variable declarations
	struct log_data *p;

	// Move to the end of the linked list
	for (p = current; !p->next->end; p = p->next);

	// Add new node at the end of list
	new_data->next = p->next;
	new_data->prev = p;
	new_data->id = p->id + 1;
	p->next = new_data;

	return new_data;
}

/* Displays information stored in a log data entry to the LCD screen.
 *
 * PARAM data : struct log_data*
 *		Pointer to a log_data to be displayed
 *
 * RETURN void
 *		This method returns nothing
 */
void update(struct log_data *data) {

	LCD.Clear();
	LCD.Write("Instruction ");
	LCD.WriteLine(data->id);
	LCD.Write("Function:   ");
	LCD.WriteLine(data->fname);
	LCD.WriteLine("----------------");
	LCD.WriteLine("Message:");
	LCD.WriteLine(data->msg);
	LCD.Write("Value: ");
	LCD.WriteLine(data->value);
}