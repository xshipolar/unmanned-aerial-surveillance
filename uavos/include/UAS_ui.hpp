/*
 * UAS_ui.hpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */

#ifndef UAS_UI_H_
#define UAS_UI_H_
#include <string> 

using namespace std;

enum mode {ON,OFF,NOT_DEFINED};			//Mode of status

struct ui_struct{
		string head;
		string name;
		mode val;
		ui_struct* nodes[5];
		int no_of_nodes;};


//enum state {MAIN,READ_DATA,CHECK_STATE,CHANGE_STATE,GPS,IMU,ANGLE};	//definingg new states




class UAS_ui
{
	
	public:
	UAS_ui();

	
	void init_ui();
	bool isInitialized();
	void load_ui();
	ui_struct* new_state(string state_head,string name,mode val);
	ui_struct* find_state(ui_struct* current,string key);
	int add_state(string add,string state_name,mode val);
	int remove_state(string rem);
	int change_state_val(string name,mode new_val);
	mode poll_state(string poll);
	ui_struct* head;

	private:
	void remove_state(ui_struct* node);

};

#endif /* UAS_UI_H_ */