/*Bacon_Number*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <fstream>

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

    vector<string> lines;
    vector<string> movieList;
    vector<string> actorList;
    int flag = 0;
    ifstream fin;
    string line;
    size_t it1;
    size_t it2;
    string del = "/";
    string s;

    // open movie database file
    fin.open( fileName, ios::in );

    // read in each line in file, pushing each onto a list
    while( getline( fin, line ) )
    {
        lines.push_back( line );
    }

    // iterate through the list of lines
    for( unsigned int i = 0; i < lines.size(); i++ )
    {
        line = lines[i];
        flag = 0;

        // in the line, grab each movie and actor              
        for( it1 = 0; it1 < line.size(); it1 = it2 + 1 )
        {
            // find the "/" in the line which seperates the actors and movies
            it2 = line.find( del, it1 );

            // if found
            if( it2 != string::npos )
            {
                s = line.substr( it1, it2-it1 );
                if( flag == 0 )
                {
                    movieList.push_back( s );
                    flag = 1;
                }
                else
                {
                    actorList.push_back( s );
                }
            }
            else
            {
                // add the last actor
                s = line.substr( it1, line.size() );
                actorList.push_back( s );
                break;
            }
        }
    }

    fin.close();

    cout << endl << "movies" << endl;
    for( vector<string>::const_iterator i = movieList.begin(); i != movieList.end(); i++ )
        cout << *i << endl;

    cout << endl << "actors" << endl;
    for( vector<string>::const_iterator i = actorList.begin(); i != actorList.end(); i++ )
        cout << *i << endl;


    return;
}
