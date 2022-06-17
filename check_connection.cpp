
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
#include <fstream>
#include <cstring>

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

void diagnostic_tool (const char *command_disabled, const char *command_enabled){
    system(command_disabled);
    sleep(5);
    system(command_enabled);
}

void check_connection (const char *network_name){
    if (system("ping www.google.com")){
        std::cout<<"\nNot connnected to the internet\n\n";
        int count = 1;
        std::cout.flush();

        // build command "command_disabled"
        char *command_disabled = new char[40 + std::string(network_name).length()];
        command_disabled[0] = '\n';
        strcat(command_disabled, "netsh interface set interface ");
        strcat(command_disabled, network_name);
        strcat(command_disabled, " disabled");

        // build command "command_enabled"
        char *command_enabled = new char[40 + std::string(network_name).length()];
        command_enabled[0] = '\n';
        strcat(command_enabled, "netsh interface set interface ");
        strcat(command_enabled, network_name);
        strcat(command_enabled, " enabled");


        system("netsh interface show interface");
        // run the network card troubleshooting diagnostics
        while(system("ping www.google.com")){
            std::cout<<"\n\n -> TEST - "<<count<<": ";
            diagnostic_tool(command_disabled, command_enabled);
            sleep(15);  
            system("netsh interface show interface");
            ++count;
        }
        
    }
    
    std::cout<<"\nConnected to the internet\n\n";
}


int main (void) {

    // check administrator rights
    if (!check_administrator_rights()) {
        std::cout<<"You must run this program as administrator!!\n";
        system("pause");
        return EXIT_FAILURE;
    };

    
    // get the network name from file "data.txt"
    std::ifstream file;
    file.open("data.txt");
    if (file.fail()) {
        std::cout<<"ERROR!! File 'data.txt' is missing...\n"; 
        system("pause");
        return EXIT_FAILURE;
    }
    char *network_name = new char[100];
    network_name[0] = '\0';
    char c;
    for (int i=0; i<100; ++i){
        file.get(c);
        if (c == '"'){ //find "
            file.get(c);
            int counter = 0;
            while(c != '"'){
                network_name[counter] = c;
                ++counter;
                file.get(c);
            }
        }
    }
    if (std::string(network_name).length() == 0) { // string is empty
        std::cout<<"ERROR!! network name is missing in file 'data.txt', insert it between the \" \" \n";
        system("pause");
        return EXIT_FAILURE;
    }
    file.close();

    // check connection
    check_connection(network_name);
    
    system("pause");

    return EXIT_SUCCESS;
}