/*
 * ui.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
#include <UAS_ui.hpp>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
/* init_ui()
 *	Description : Initialize ui
 *	Inputs : none
 *	Outputs : none
 *  Special Mention : none
 */	
using namespace std;
UAS_ui::UAS_ui() {
}
void UAS_ui::init_ui()
{
	UAS_ui::head=new_state("","MAIN",ON);		//Initializes head to MAIN and turns mode ON
	cout<<"Head initialized to :"<<UAS_ui::head->name<<"\n";
}

bool UAS_ui::isInitialized()
{
	if(UAS_ui::head==NULL)					//Checking to see if the head is initialized
	return false;
	return true;
}
/* load_ui()
 *	Description : Load ui with a sample state diagram
 *	Inputs : none
 *	Outputs : none
 *  Special Mention : none
 */	
void UAS_ui::load_ui()
{
	int n;
	n=add_state("MAIN","READ DATA",OFF);				//MAIN->NAVIGATION(OM)
	if(n==-1)
	cout<<"Adding read failed\n";
	n=add_state("MAIN","CHECK STATE",OFF);					//MAIN->VISION(ON)
	if(n==-1)
	cout<<"Adding check state failed\n";
	n=add_state("MAIN","CHANGE STATE",OFF);			//MAIN->COMMUNICATION(ON)
	if(n==-1)
	cout<<"Adding change state failed\n";
	n=add_state("MAIN","EXIT",OFF);
	if(n==-1)
	cout<<"Adding Exit state failed\n";
	n=add_state("READ DATA","GPS",OFF);				//MAIN->NAVIGATION->AUTO(OFF)
	if(n==-1)
	cout<<"Adding GPS failed\n";
	n=add_state("READ DATA","IMU",OFF);			//MAIN->NAVIGATION->MANUAL(ON)
	if(n==-1)
	cout<<"Adding IMU failed\n";
	n=add_state("READ DATA","CAMERA ANGLE",OFF);
	if(n==-1)
	cout<<"Adding CAMERA_ANGLE failed\n";
	//add_state("CHANGE STATE","AUTO",OFF);
	//add_state("CHANGE STATE","SEMI-AUTO",OFF);
	//add_state("CHANGE STATE","MANUAL",OFF);
	cout<<"Printing tree->>>\n";
	print_tree();
}
/* new_state()
 *	Description : Load ui with a sample state diagram
 *	Inputs : name : name of new state
 * 			 value : mode of new state
 *	Outputs : pointer to the new structure declared
 *  Special Mention : Returns NULL if state not found
 */
ui_struct* UAS_ui::new_state(string state_head,string state_name,mode value)
{
	int i;
	ui_struct* ptr=new ui_struct();		//Declaring new ui_struct
	ptr->head=state_head;
	ptr->name=state_name;
	ptr->val=value;
	//ui_struct* temp=new ui_struct[5];
	for(i=0;i<5;i++)
	ptr->nodes[i]=new ui_struct;

	ptr->no_of_nodes=0;
	return ptr;

}
/*  find_state()
 *	Description : Find state with a particular name in the state diagram
 *	Inputs : current : pointer to state/sub_state to search in
 * 			 key 	 : state name to search for
 *	Outputs : pointer to found state. 
 *  Special Mention : none
 */
ui_struct* UAS_ui::find_state(ui_struct* current,string key)
{
	if(current==NULL)					//End of state diagram reached!
	return NULL;					//Search unsuccessful in sub-tree
	ui_struct* ret;
	if(current->name==key)				//Search successful!
	return current;
	for(int i=0;i<current->no_of_nodes;i++)
	if((ret=find_state((current->nodes[i]),key))!=NULL)
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
int UAS_ui::add_state(string add,string state_name,mode val)
{
	ui_struct* ptr;
	ptr=find_state(head,add);			//Finding state
	if(ptr==NULL)
	return -1;		//Error in finding name
	ui_struct* new_ptr=new_state(add,state_name,val);
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
int UAS_ui::remove_state(string rem)
{
	ui_struct* ptr;
	ptr=find_state(head,rem);
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
void UAS_ui::remove_state(ui_struct* node)
{
	if(node->no_of_nodes!=0)
	{
		for(int i=0;i<node->no_of_nodes;i++)
			remove_state(node->nodes[i]);			//Recursive removal of sub-states(if any)
	}
	free (node->nodes);
	free (node);

}
/*  change_state_val()
 *	Description : Change the mode of a particular state
 *	Inputs : name : name of state to be modified
 			 new_val : new mode
 *	Outputs : #status of mode change
 *  Special Mention : Returns -1 on error and 0 on success
 */
int UAS_ui::change_state_val(string name,mode new_val)
{
	ui_struct* ptr;
	ptr=find_state(head,name);					//Searching for state in state diagram
	if(ptr==NULL)
	{
		cout<<"State : "<<name<<" not found!\n";
		return -1;
	}
	ptr->val=new_val;							//Mode changed
	return 0;
}
/*  poll_state()
 *	Description : Find out the mode associated with a state
 *	Inputs : poll : name of state to be polled
 *	Outputs : mode of existing state
 *  Special Mention : Returns NOT_DEFINED on error and mode on success
 */
mode UAS_ui::poll_state(string poll)
{
	ui_struct* ptr;
	ptr=find_state(head,poll);
	if(ptr==NULL)
	return NOT_DEFINED;					//Error
	return ptr->val;
}

void UAS_ui::print_tree(ui_struct* ptr)
{
	int i;
	if(ptr==NULL)
	return;
	cout<<ptr->name;
	printf("->(");
	for(i=0;i<ptr->no_of_nodes;i++)
	print_tree(ptr->nodes[i]);
	printf(")");
	
}

void UAS_ui::print_tree()
{
	print_tree(UAS_ui::head);
	printf("\n");
}