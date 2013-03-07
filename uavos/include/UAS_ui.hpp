/*
 * ui.hpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
typedef struct ui_struct{
		state name;
		mode val;
		ui_struct* nodes[5];
		int no_of_nodes};


enum state {MAIN,NAVIGATION,VISION,COMMUNICATION,AUTO,MANUAL};	//definingg new states

enum mode {ON,OFF,NOT_DEFINED};			//Mode of status

ui_struct* head;

class UAS_ui
{
	private:

	void remove_state(ui_struct* node);

	public:

	extern void init_ui();
	extern bool is_Initialized():
	extern void load_ui();
	extern ui_struct* new_state(state name,mode val);
	extern ui_struct* find_state(state name);

	extern int add_state(state add,state state_name,mode val);
	extern int remove_state(state rem);
	extern int change_state_val(state level,mode new_val);
	extern mode poll_state(state poll);
}