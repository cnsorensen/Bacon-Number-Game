#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
//#include <iterator>
#include <unordered_map>

using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        cout << "you fucked up" << endl;
        exit(0);
    }

    ifstream fin;    

    fin.open( argv[1], ios::in );

    vector<string> movies;
    vector<string> actors;
    vector<string> lines;
    vector<string> currentMovieActors;
    vector<string> currentActorMovies;

    int flag = 0;

/***************/
    size_t it1;
    size_t it2;
    string del = "/";
    string s2;
    string x;
    string currentActor;
    string currentMovie;
/*****************/

    unordered_map<string, vector<string>> movieHash;
    unordered_map<string, vector<string>> actorHash;

    // push all lines in file into vector
    while( !fin.eof()  )
    {
        getline( fin, x );
        lines.push_back( x );        
    }
    
    for( int i = 0; i < lines.size(); i++ )
    {
        string line = lines[i];
        //string del = "/";
        int flag = 0;
        for( it1 = 0; it1 < line.size(); it1 = it2 + 1 )
        {
            it2 = line.find( del, it1 );    
            if( it2 != string::npos )
            {
                s2 = line.substr( it1, it2-it1 );
                if( flag == 0 )
                {
                    //movies.push_back( line.substr( it1, it2-1 ) );
                    movies.push_back( s2 );
                    flag = 1;
                    currentMovie = s2;
                    movieHash.emplace( currentMovie,vector<string>() ); 
                }
                else
                {
                    //actors.push_back( line.substr( it1, it2 ) );
                    actors.push_back( s2 );
                    currentActor = s2;
                    actorHash[currentMovie].push_back(s2); 
                }
            }
            else
            {
                s2 = line.substr( it1, line.size() );
                actors.push_back(s2);
                actorHash[currentMovie].push_back(s2);
                break;
            }
        }
    }

    auto z = actorHash["Stone, Emma"];
    //cout << z -> first << endl;
    //cout << actorHash["Stone, Emma"] << endl;
    //cout << z->first << z->second << endl;
    //cout << actorHash["Stone, Emma"][0] << endl;
    cout << z[0] << endl;
    cout << endl << "movies" << endl;
    for( vector<string>::const_iterator i = movies.begin(); i != movies.end(); i++ )
        cout << *i << endl;

    cout << endl << "actors" << endl;
    for( vector<string>::const_iterator i = actors.begin(); i != actors.end(); i++ )
        cout << *i << endl;

    cout << endl << "movieHash" << endl;
    for( unsigned int i = 0; i < movieHash.bucket_count(); i++ )
    {
        for( auto local_it = movieHash.begin(i); local_it != movieHash.end(i); local_it++ )
        {
            cout << local_it -> first << endl;
            for( int j = 0; j < local_it -> second.size(); j++ )
            {
                cout << local_it -> second[j] << endl;
            }
        }
    }

    fin.close();
/////////////////////////////////////////////////////////////



    return 1;
}
