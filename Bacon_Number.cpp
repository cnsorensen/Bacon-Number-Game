/*Bacon_Number*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

//Debugging includes
#include <typeinfo>

using namespace std;

// defines
typedef std::unordered_map<std::string,std::string> stringmap;

// function prototypes
void Read_File( string, stringmap, stringmap, stringmap );
void Bacon_Number( string );

// globals
struct vertex
{
    int DOS = 0;
    string parent = "";
    bool visited = false;
};

int main( int argc, char** argv )
{
    string actor = "Bacon, Kevin";

    if( argc < 2 || argc > 4 )
    {
        cout << "Usage: ./Bacon_Number [-h|l] movie_file.txt [alt start]" << endl;
        exit(0);
    }

    string fileName = argv[1];

    if( argc > 2 )
    {
        actor = argv[argc-1];   
        cout << actor << endl;
    
    }

    stringmap movies;
    stringmap actors;
    stringmap verticies;

    Read_File( fileName, movies, actors, verticies );
 
    Bacon_Number( actor );

    return 1;
}

void Bacon_Number( string actor )
{
    cout << actor << endl;

    return;
}

void Read_File( string fileName, stringmap movies, stringmap actors, stringmap verticies )
{
    cout << fileName << endl;

    vector<string> line;
    int flag = 0;

    ifstream fin;
    fin.open( filename, ios::in );

    string line;

    while( getline( fin, line, '/' ) )
    {
        if(        
    }

    return;
}
