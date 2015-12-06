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
struct graphVertex
{
    int DOS = 0;
    string parent = "orphan";
    bool visited = false;
};

// hash tables
unordered_map<string, vector<string>> movieHash;
unordered_map<string, vector<string>> actorHash; 
unordered_map<string, graphVertex> graphHash;

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
    string currentActor;
    string currentMovie;

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
        
        // toggle flag to count first item as a movie
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
                    // set the current movie being looked at
                    currentMovie = s;

                    // add to the list of movies
                    movieList.push_back( currentMovie );

                    // create an element in the movie hash table for current movie
                    movieHash.emplace( currentMovie, vector<string>() );
                    
                    // toggle the flag to count item as actors now
                    flag = 1;
                }
                else
                {   
                    // set the current actor being looked at
                    currentActor = s;

                    // push into the list of actors
                    actorList.push_back( s );

                    // add actor to the corresponding movie
                    movieHash[currentMovie].push_back( s );

                    // check to see if the actor is in the actor hash table
                    if( actorHash.find( currentActor ) == actorHash.end() )
                    {
                        // if it isn't, add actor to hash table
                        actorHash.emplace( currentActor, vector<string>() );
                        
                        // insert actor into graph
                        graphHash.emplace( currentActor, graphVertex() );
                    }

                    // add movie to the corresponding actor
                    actorHash[currentActor].push_back( currentMovie );
                }
            }
            else
            {
                // add the last actor
                s = line.substr( it1, line.size() );
                
                // set the current actor being looked at
                currentActor = s;

                // push into the list of actors
                actorList.push_back( s );

                // add actor to the current movie
                movieHash[currentMovie].push_back( s );
                
                // check to see if actor is in the hash table
                if( actorHash.find( currentActor ) == actorHash.end() )
                {
                    // if it isn't, add actor to hash table
                    actorHash.emplace( currentActor, vector<string>() );
            
                    // insert into graph
                    graphHash.emplace( currentActor, graphVertex() );
                }
                
                // add current movie to the actor
                actorHash[currentActor].push_back( currentMovie );
                break;
            }
        }
    }

    fin.close();

/////////////////////
    cout << endl << "movies" << endl;
    for( vector<string>::const_iterator i = movieList.begin(); i != movieList.end(); i++ )
        cout << *i << endl;

    cout << endl << "actors" << endl;
    for( vector<string>::const_iterator i = actorList.begin(); i != actorList.end(); i++ )
        cout << *i << endl;

    cout << endl << "movieHash" << endl;
    for( unsigned int i = 0; i < movieHash.bucket_count(); i++ )
    {
        for( auto local_it = movieHash.begin(i); local_it != movieHash.end(i); local_it++ )
        {
            cout << endl << "movie: " << local_it -> first << endl;
            for( unsigned int j = 0; j < local_it -> second.size(); j++ )
            {
                cout << local_it -> second[j] << endl;
            }
        }
    }

    cout << endl << "actorHash" << endl;
    for( unsigned int i = 0; i < actorHash.bucket_count(); i++ )
    {
        for( auto local_it2 = actorHash.begin(i); local_it2 != actorHash.end(i); local_it2++ )
        {
            cout << endl << "actor: " << local_it2 -> first << endl;
            for( unsigned int j = 0; j < local_it2 -> second.size(); j++ )
            {
                cout << local_it2 -> second[j] << endl;
            }
        }
    }

    cout << endl << "graph hash" << endl;
    for( unsigned int i = 0; i < graphHash.bucket_count(); i++ )
    {
        for( auto local_it3 = graphHash.begin(i); local_it3 != graphHash.end(i); local_it3++ )
        {
            cout << endl << "actor: " << local_it3 -> first << endl;
            cout << local_it3 -> second.DOS << " " << local_it3 -> second.parent << " " << local_it3 -> second.visited; 
        }
    }


    return;
}
