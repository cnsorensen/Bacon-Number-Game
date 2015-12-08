/*Bacon_Number*/

// includes
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <queue>
#include <ctime>

using namespace std;

// defines
typedef std::unordered_map<std::string,std::string> stringmap;

// function prototypes
void Read_File( string );
void Bacon_Number( string, string );
void BFS( string );
void buildHistogram();

// globals
struct graphVertex
{
    int DOS = 0;
    string parent = "";
    string parentMovie = "";
    bool visited = false;
};

// variables
int numMovies = 0;
int numActors = 0;
int sizeMovieHash = 285000;
int sizeActorHash = 285000;

// hash tables
unordered_map<string, vector<string>> movieHash( sizeMovieHash );
unordered_map<string, vector<string>> actorHash( sizeActorHash ); 
unordered_map<string, graphVertex> graphHash( sizeActorHash );

/*
 *
 */
int main( int argc, char** argv )
{
    string bacon = "Bacon, Kevin";

    if( argc < 2 || argc > 3 )
    {
        //cout << "Usage: ./Bacon_Number [-h|l] movie_file.txt [alt start]" << endl;
        cout << "Usage: ./Bacon_Number movie_file.txt [alt-start actor]" << endl;
        exit(0);
    }

    string fileName = argv[1];

    if( argc == 3 )
    {
        bacon = argv[2];   
        cout << "Alternative " << bacon << endl;
    }

    // start timer
    auto c1 = clock();
    
    // read in movie file
    Read_File( fileName );

    cout << "Reading movies: " << numMovies << " and " << numActors << " actors in database" << endl;
 
    // end timer
    auto c2 = clock();
    auto diff1 = c2 - c1;

    // print timer stats
    cout << "Time to read data and build graph: " << diff1 * 1000.0 / CLOCKS_PER_SEC << " sec" << endl;    

    // check to see if alternative actor is included
    if( argc == 3 )
    {
        if( actorHash.find( bacon ) == actorHash.end() )
        {
            cout << "Could not find performer named [" << bacon << "]" << endl;
            exit( 0 );
        }
    }


    // list movies with Kevin Bacon

    // create graph
    cout << endl << "Building MST ... ";
    auto c3 = clock();
    BFS( bacon );
    auto c4 = clock();
    cout << "done!" << endl;
    auto diff2 = c4 - c3;
    cout << endl << "Time to build MST: " << diff2 * 1000.0 / CLOCKS_PER_SEC << " sec" << endl;

    // build histogram here
    auto c5 = clock();
    buildHistogram();
    auto c6 = clock();
    auto diff3 = c6 - c5;
    cout << endl << "Time to calculate longest-shortest paths: " << diff3 * 1000.0 / CLOCKS_PER_SEC << " sec" << endl;


    // continue finding bacon numbers as long as actors are being accepted
    string actor;
    while( true )
    {
        // recieve the user input
        cout << "Enter the name of an actor <Last, First> \"\" to exit: ";
        getline( cin, actor );
        
        // exit program when user presses enter with no text
        if( actor.size() == 0 )
        {
            exit( 0 );
        }

        // retrieve the bacon number
        Bacon_Number( bacon, actor );
    }

    return 1;
}

/*
 *
 */
void Bacon_Number( string bacon, string actor )
{
    auto a_i = actorHash.find( actor );

    // check for actor
    if( a_i  == actorHash.end() )
    {
        cout << "Could not find performer named [" << actor << "]" << endl;
        return;
    }

    auto g_i = graphHash.find( actor );

    // print bacon number
    cout << g_i -> second.DOS << ": ";
    
    while( a_i -> first != bacon )
    {
        cout << a_i -> first << " => ";
        cout << g_i -> second.parentMovie << " => ";
    
        a_i = actorHash.find( g_i -> second.parent );
        g_i = graphHash.find( g_i -> second.parent );
    }
    
    // print bacon
    cout << a_i -> first << endl;

    return;
}

/*
 *
 */
void Read_File( string fileName )
{
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
                    numMovies++;

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
                        numActors++;
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
                    
                    numActors++;
                }
                
                // add current movie to the actor
                actorHash[currentActor].push_back( currentMovie );

                break;
            }
        }
    }

    fin.close();

/////////////////////
/*
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
*/
/*
    cout << endl << "actorHash" << endl;
    // each item in actorHash
    for( unsigned int i = 0; i < actorHash.bucket_count(); i++ )
    {
        // each actor
        for( auto local_it2 = actorHash.begin(i); local_it2 != actorHash.end(i); local_it2++ )
        {
            cout << endl << "actor: " << local_it2 -> first << endl;
            // each movie actor did
            for( unsigned int j = 0; j < local_it2 -> second.size(); j++ )
            {
                cout << local_it2 -> second[j] << endl;
            }
        }
    }
*/
/*
    cout << endl << "graph hash" << endl;
    for( unsigned int i = 0; i < graphHash.bucket_count(); i++ )
    {
        for( auto local_it3 = graphHash.begin(i); local_it3 != graphHash.end(i); local_it3++ )
        {
            cout << endl << "actor: " << local_it3 -> first << endl;
            cout << local_it3 -> second.DOS << " " << local_it3 -> second.parent << " " << local_it3 -> second.visited; 
        }
    }
*/

    return;
}

/*
 *
 */
void BFS( string v )
{
    string x_s;   //movie
    string w_s;   //actor
    string y_s;   //actor

    queue<string> q;
    q.push( v );

    auto v_g = graphHash.find( v );
    v_g -> second.DOS = 0;
    v_g -> second.parent = "";
    v_g -> second.visited = true;

    while( !q.empty() )
    {
        w_s = q.front();
        q.pop();
        // set all the stuff to their stuff in the graph hash       
        auto w_i = actorHash.find( w_s );

        // for each movie x that actor w is in
        for( unsigned int i = 0; i < w_i -> second.size(); i++ )
        {
            x_s = w_i -> second[i];
            auto x_i = movieHash.find( x_s );

            // for each actor y in movie x
            for( unsigned int j = 0; j < x_i -> second.size(); j++ )
            {
                y_s = x_i -> second[j];
                //auto y_i = actorHash.find( y_s );
                auto currentVertex = graphHash.find( y_s );
                
                if( currentVertex -> second.visited != true )
                {
                    auto parentVertex = graphHash.find( w_s );
                    currentVertex -> second.visited = true;
                    currentVertex -> second.parent = w_s;
                    currentVertex -> second.DOS = parentVertex -> second.DOS + 1;
                    currentVertex -> second.parentMovie = x_s;
                    q.push( y_s );
                }
            }
        }
    }
/*
        // each actor
        for( auto local_it2 = actorHash.begin(i); local_it2 != actorHash.end(i); local_it2++ )
        {
            cout << endl << "actor: " << local_it2 -> first << endl;
            // each movie actor did
            for( unsigned int j = 0; j < local_it2 -> second.size(); j++ )
            {
                cout << local_it2 -> second[j] << endl;
            }
        }
*/
/////////////////////SAMPLE////////////
   /* auto iter = actorHash.find("Stone, Emma");
    if( iter != actorHash.end() )
    {
        cout << "purple" << endl;    
        cout << iter -> second[0] << endl;
    }
    else
        cout << "not here!!" << endl;
 */   
/*
    cout << endl << "graph hash" << endl;
    for( unsigned int i = 0; i < graphHash.bucket_count(); i++ )
    {
        for( auto local_it3 = graphHash.begin(i); local_it3 != graphHash.end(i); local_it3++ )
        {
            cout << endl << "actor: " << local_it3 -> first << endl;
            cout << local_it3 -> second.DOS << " \"" << local_it3 -> second.parent << "\" " << local_it3 -> second.visited; 
        }
    }
*/

    return;
}

/*
 *
 */
void buildHistogram()
{
    // holds the dos and the number of actors with that dos
    // initalized to 20, almost never greater than 10
    int histogram[20] = {0};

    // find values for each distance
    for( auto it = graphHash.begin(); it != graphHash.end(); it++ )
    {
        histogram[ it -> second.DOS ]++;        
    }


    // print histogram
    // print the value for bacon
    cout << endl << 0 << "  " << 1 << endl;

    // go through the rest of the values
    for( int j = 1; j < 20; j++ )
    {
        // only print if there is an occurance of that dos
        if( histogram[j] != 0 )
            cout << j << "  " << histogram[j] << endl;
    }

    // print the ones that have no bacon number
    cout << "Inf" << "  " << histogram[0] - 1 << endl;

    // compute the average bacon number
    float div = 0.0;
    float num = 0.0;
    for( int i = 1; i < 20; i++ )
    {
        div += histogram[i];
        num += i * histogram[i];
    }

    cout << endl << "Avg path length : " << num / div  << endl;

    return;
}
