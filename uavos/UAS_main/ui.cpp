/*
 * ui.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
#include <ui.hpp>

/* init_ui()
 *	Description : Initialize ui
 *	Inputs : none
 *	Outputs : none
 *  Special Mention : none
 */	
void UAS_UI::init_ui()
{
	head=new_state(MAIN,ON);		//Initializes head to MAIN and turns mode ON

}
/* load_ui()
 *	Description : Load ui with a sample state diagram
 *	Inputs : none
 *	Outputs : none
 *  Special Mention : none
 */	
void UAS_UI::load_ui()
{
	add_state(MAIN,NAVIGATION,ON);				//MAIN->NAVIGATION(OM)
	add_state(MAIN,VISION,ON);					//MAIN->VISION(ON)
	add_state(MAIN,COMMUNICATION,ON);			//MAIN->COMMUNICATION(ON)
	add_state(NAVIGATION,AUTO,OFF);				//MAIN->NAVIGATION->AUTO(OFF)
	add_state(NAVIGATION,MANUAL,ON);			//MAIN->NAVIGATION->MANUAL(ON)
}
/* new_state()
 *	Description : Load ui with a sample state diagram
 *	Inputs : name : name of new state
 * 			 value : mode of new state
 *	Outputs : pointer to the new structure declared
 *  Special Mention : Returns NULL if state not found
 */
ui_struct* UAS_UI::new_state(state name,mode value)
{
	ui_struct* ptr=new ui_struct();		//Declaring new ui_struct
	ptr->name=state_name;
	ptr->val=value;
	ptr->nodes=new ui_struct*[5];
	ptr->no_of_nodes=0;

}
/*  find_state()
 *	Description : Find state with a particular name in the state diagram
 *	Inputs : current : pointer to state/sub_state to search in
 * 			 key 	 : state name to search for
 *	Outputs : pointer to found state. 
 *  Special Mention : none
 */
ui_struct* UAS_UI::find_state(ui_struct* current,state key)
{
	if(current==NULL)					//End of state diagram reached!
		return NULL;					//Search unsuccessful in sub-tree
	ui_struct* ret;
	if(current->name==key)				//Search successful!
	return current;
	for(int i=0;i<current->no_of_nodes;i++)
	if((ret=find_state((current->nodes[i],key))!=NULL)
	break;
	return ret;
}
/*  add_state()
 *	Description : Add a sub-state to one of the states the state diagram
 *	Inputs : add 		: state name to which sub-state is supposed to be added
 *           state_name : name of sub-state
 * 			 val 		: mode of state
 *	Outputs : #status of addition
 *  Special Mention : Returns -1 on error. Returns 0 on success
 */
int UAS_UI::add_state(state add,state state_name,mode val)
{
	ui_struct* ptr;
	ptr=find_state(head,name);			//Finding state
	if(ptr==NULL)
	return -1;		//Error in finding name
	ui_struct* new_ptr=new_state(state_name,val);
	if(ptr->no_of_nodes==0)
	ptr->nodes[ptr->no_of_nodes]=new_ptr;
	ptr->no_of_nodes++;
	return 0;	
}
/*  remove_state()
 *	Description : Remove a state from the state diagram
 *	Inputs : rem : state name to which sub-state is supposed to be added
 *	Outputs : #status of removal 
 *  Special Mention : Returns -1 on error and 0 on success
 */
int UAS_UI::remove_state(state rem)
{
	ui_struct* ptr;
	ptr=find_state(head,name);
	if(ptr==NULL)
	return -1;		//Error in finding name
	remove_state(ptr);
	
	
}
/*  remove_state()
 *	Description : Remove a state from the state diagram
 *	Inputs : node : pointer to ui_struct to be deleted
 *	Outputs : #status of removal 
 *  Special Mention : Returns -1 on error and 0 on success
 */
void UAS_UI::remove_state(ui_struct* node)
{
	if(node->no_of_nodes!=0)
	{
		for(int i=0;i<node->no_of_nodes;i++)
			remove_state(node->nodes[i]);			//Recursive removal of sub-states(if any)
	}
	free(node->nodes);
	free(node);

}
/*  change_state_val()
 *	Description : Change the mode of a particular state
 *	Inputs : name : name of state to be modified
 			 new_val : new mode
 *	Outputs : #status of mode change
 *  Special Mention : Returns -1 on error and 0 on success
 */
int UAS_UI::change_state_val(state name,mode new_val)
{
	ui_struct* ptr;
	ptr=find_state(head,name);					//Searching for state in state diagram
	if(ptr==NULL)
	return -1;
	ptr->val=new_val;							//Mode changed
	return 0;
}
/*  poll_state()
 *	Description : Find out the mode associated with a state
 *	Inputs : poll : name of state to be polled
 *	Outputs : mode of existing state
 *  Special Mention : Returns NOT_DEFINED on error and mode on success
 */
mode UAS_UI::poll_state(state poll)
{
	ui_struct* ptr;
	ptr=find_state(head,name);
	if(ptr==NULL)
	return NOT_DEFINED;					//Error
	return ptr->mode;
}
