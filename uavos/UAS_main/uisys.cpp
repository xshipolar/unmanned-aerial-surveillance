/*
 * uisys.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
#include <uisys.hpp>

/*
 * uisys.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */
#include <uisys.hpp>

 static UAS_UI uiModule;

 void initUisys();
 {
 	uiModule.init_ui();
 }

void* runUiSys(void*)
{
	if(uiModule.isInitialized())
	uiModule.LoadModule();
}
 