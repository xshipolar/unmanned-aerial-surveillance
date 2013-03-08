/*
 * uisys.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
#include <uisys.hpp>

using namespace std;
static UAS_ui uiModule;

 void initUisys()
 {
 	uiModule.init_ui();
 }

void* runUisys(void*)
{
	if(uiModule.isInitialized())
	uiModule.load_ui();
	while(true){
    if(uiModule.poll_state("READ_DATA")==ON || read_flag==true)
    {
        cout<<"Reading State\n";
        if(cvWaitKey(400)=='q')                       //Resetting the ui on pressing Esc
        {
           uiModule.change_state_val("READ_DATA",OFF);
           uiModule.change_state_val("GPS",OFF);
           uiModule.change_state_val("IMU",OFF);
           uiModule.change_state_val("CAMERA ANGLE",OFF);
           read_flag=false;
        }
        if(uiModule.poll_state("GPS")==ON)
        //Print GPS Coordinates
        if(uiModule.poll_state("IMU")==ON)
        //Print IMU Values
        if(uiModule.poll_state("CAMERA ANGLE")==ON);
        //Print Camera Angle Values
    }
    else if(uiModule.poll_state("CHECK STATE")==ON || check_state_flag==true)
    {
    	//print system state
        cout<<"Checking State\n";
    	uiModule.change_state_val("CHECK STATE",OFF);
        check_state_flag=false;
    }
    else if(uiModule.poll_state("SET_STATE")==ON || set_state_flag==true)
    {
        cout<<"Setting State\n";
        bool flag=true;
        while(true){
        cout<<"Current Status of SYSTEM : ";//Print out current system state
        cout<<"Select from the following options >>>\n";
        cout<<"Idle					  1\n";
        cout<<"Auto 				  2\n";
        cout<<"Semi-Auto			  3\n";
        cout<<"End					  4\n";
        char ch=getchar();
        switch(ch)
        {
        	case '1':
        	//Switch to Manual;
        	break;
        	case '2':
        	//Switch to Auto
        	break;
        	case '3':
        	//Switch to Semi-Auto
        	break;
        	case '4':
        	//Switch to end state
        	break;
        	default : 
        	cout<<"Error in selection\n";
        	flag=false;
        }
        if(flag==true)
        break;
    		}
        uiModule.change_state_val("SET STATE",OFF);
        set_state_flag=false;

    }
    else if(uiModule.poll_state("EXIT")==ON || exit_flag==true)
    {
    	cout<<"Hello! Thanks for using this UI!\n";
    	exit(0);
    }
    else
    {
        bool flag=true;
        while(true){
        cout<<"Welcome to the UI\n";
        cout<<"Select from the options below\n";
        cout<<"Read Data             1\n";
        cout<<"Check State           2\n";
        cout<<"Change State          3\n";
        cout<<"Exit                  4\n";
        //char ch=getchar();
        char ch;
        cin>>ch;
        //cout <<ch<<"\n";
        flag=true;
        switch(ch)
        {
        	case '1':
        	uiModule.change_state_val("READ STATE",ON);
            read_flag=true;
        	break;
        	case '2':
        	uiModule.change_state_val("CHECK STATE",ON);
            check_state_flag=true;
        	break;
        	case '3':
        	uiModule.change_state_val("CHANGE STATE",ON);
            set_state_flag=true;
        	break;
        	case '4':
        	uiModule.change_state_val("Exit",ON);
            exit_flag=true;
        	break;
        	default:
        	cout<<"Error in selection!!\n";
        	flag=false;
        }
        if(flag==true)
       	break;
   			}
	}
    	}
}
 