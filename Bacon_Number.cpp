/*Bacon_Number*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

//Debugging includes
#include <typeinfo>

using namespace std;

int main( int argc, char** argv )
{
    string actor;

    if( argc < 2 || argc > 4 )
    {
        cout << "Usage: ./Bacon_Number [-h|l] movie_file.txt [alt start]" << endl;
        exit(0);
    }

    if( argc == 3 || argc == 4 )
    {
        actor = argv[3];

        cout << actor << endl;   
        cout << typeid(actor).name() << endl;
    } 

    return 1;
}
