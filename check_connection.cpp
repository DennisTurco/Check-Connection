
/*
    Author	-> Dennis Turco 2022
	Email 	-> dennisturco@gmail.com
	WebSite	-> https://dennisturco.github.io/
	GitHub	-> https://github.com/DennisTurco

    Link: https://stackoverflow.com/questions/15778404/programmatically-check-whether-my-machine-has-internet-access-or-not
*/

#include <iostream>
#include <windows.h> 
#include <wininet.h>
#include <unistd.h>

#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>

void diagnostic_tool(){
    system("netsh interface set interface Ethernet disabled");
    std::cout.flush();
    sleep(5);
    system("netsh interface set interface Ethernet enabled");
}

void check_connection(){
    if (system("ping www.google.com")){
        std::cout<<"\nNot connnected to the internet\n\n";

        system("netsh interface show interface");
        // run the network card troubleshooting diagnostics
        while(system("ping www.google.com")){
            diagnostic_tool();
            std::cout.flush();
            sleep(10);  
            system("netsh interface show interface");
        }
        
    } else{
        std::cout<<"\nConnected to the internet\n\n";
        return;
    }
}


int main (void) {
    //check_connection();
    //system("runas /user:Administrator \"taskkill /im netsh interface set interface Ethernet disabled\"");
    ShellExecute(NULL, "runas", "netsh interface set interface Ethernet disabled", NULL, NULL, 0);
    return EXIT_SUCCESS;
}