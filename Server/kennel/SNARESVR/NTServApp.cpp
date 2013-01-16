// NTService.cpp

#include "NTServApp.h"
#include "SnareSvr.h"
#define _DEBUG
int main(int argc, char* argv[])
{
    // Create the service object
    CSnareSvrService CustomServiceObject;

    // Parse for standard arguments (install, uninstall, version etc.)
    if (!CustomServiceObject.ParseStandardArgs(argc, argv)) 
	{
		if(argc>1) {
			// Go into debug mode if it was a flag we don't recognise

			CustomServiceObject.DEBUGSET=1;
			
			// we're debugging, so fake a few calls & run as a console app
			if (CustomServiceObject.OnInit())
			{
				CustomServiceObject.m_bIsRunning = TRUE;
				CustomServiceObject.Run();
			}
			
		} else {
#ifndef _DEBUG
			// Didn't find any standard args so start the service

			CustomServiceObject.StartService();
			//if (CustomServiceObject.OnInit())
			//{
			//	CustomServiceObject.m_bIsRunning = TRUE;
			//	CustomServiceObject.Run();
			//}
#else
			// we're debugging, so fake a few calls & run as a console app
			if (CustomServiceObject.OnInit())
			{
				CustomServiceObject.m_bIsRunning = TRUE;
				CustomServiceObject.Run();
			}
#endif
		}
	}

    // When we get here, the service has been stopped
    return CustomServiceObject.m_Status.dwWin32ExitCode;
}
