
/*
    Author	-> Dennis Turco 2022
	Email 	-> dennisturco@gmail.com
	WebSite	-> https://dennisturco.github.io/
	GitHub	-> https://github.com/DennisTurco

    Link: https://stackoverflow.com/questions/15778404/programmatically-check-whether-my-machine-has-internet-access-or-not
          https://stackoverflow.com/questions/8046097/how-to-check-if-a-process-has-the-administrative-rights
*/

#include <iostream>
#include <windows.h> 
#include <wininet.h>
#include <unistd.h>

bool check_administrator_rights (void) {
    // https://stackoverflow.com/questions/8046097/how-to-check-if-a-process-has-the-administrative-rights
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess( ),TOKEN_QUERY,&hToken ) ) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if( hToken ) {
        CloseHandle( hToken );
    }
    return fRet;
}

void diagnostic_tool (void){
    system("netsh interface set interface Ethernet disabled");
    sleep(5);
    system("netsh interface set interface Ethernet enabled");
}

void check_connection (void){
    if (system("ping www.google.com")){
        std::cout<<"\nNot connnected to the internet\n\n";
        int count = 1;
        std::cout.flush();

        system("netsh interface show interface");
        // run the network card troubleshooting diagnostics
        while(system("ping www.google.com")){
            std::cout<<"\n\n -> TEST - "<<count<<": ";
            diagnostic_tool();
            sleep(15);  
            system("netsh interface show interface");
            ++count;
        }
        
    } else{
        std::cout<<"\nConnected to the internet\n\n";
        return;
    }
}


int main (void) {

    if (!check_administrator_rights()) {
        std::cout<<"You must run this program as administrator!!\n";
        system("pause");
        return EXIT_FAILURE;
    };

    check_connection();
    
    system("pause");

    return EXIT_SUCCESS;
}