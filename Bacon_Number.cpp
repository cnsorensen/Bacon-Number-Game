/*Bacon_Number*/

/*
 * CSC 300 Data Structures - Fall 2015
 * Prog #4 - Graph Problems
 * Six Degrees of Kevin Bacon
 * Dr. John Weiss
 * December 7, 2015
 *
 * Author: Chrissy Sorensen
 *
 * Compile: g++ Bacon_Numper.cpp -std=c++11 -o Bacon_Number
 *
 * Usage: ./Bacon_Number <movie_file.txt> ["Alt Actor"]
 *
 * This program calculates the Kevin Bacon number for any actor. It reads in a text
 * file containing a formatted set of records containing movies followed by a list
 * of actors in those movies. The interface prompts the user to type an actor's
 * name and the program returns the Bacon Number followed by the chain of movies
 * and actors connecting the actor to Kevin Bacon.
 *
 * This program also accepts another "Kevin Bacon". It creates a graph centered
 * around this actor and will determine its number for the other actors. 
 *
 */

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
void listMovies( string );
void lsEndpoints();

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
int maxDist = 0;

// hash tables
unordered_map<string, vector<string>> movieHash( sizeMovieHash );
unordered_map<string, vector<string>> actorHash( sizeActorHash ); 
unordered_map<string, graphVertex> graphHash( sizeActorHash );

/*
 * main: Takes the name of the movie text file and optional alternative actor. It 
 *      makes calls to read in the file, perform bfs, and create histogram and times
 *      them all. It continously prompts the user for an actor and calls the
 *      Bacon_Number function to print the bacon number and path. It returns when 
 *      the user enters without typing in an actor's name.
 */
int main( int argc, char** argv )
{
    string bacon = "Bacon, Kevin";

    if( argc < 2 || argc > 3 )
    {
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
    listMovies( bacon );

    // create graph
    cout << endl << "Building MST ... ";
    auto c3 = clock();
    BFS( bacon );
    auto c4 = clock();
    cout << "done!" << endl;
    auto diff2 = c4 - c3;
    cout << endl << "Time to build MST: " << diff2 * 1000.0 / CLOCKS_PER_SEC << " sec" << endl;

    // build and print histogram
    auto c5 = clock();
    buildHistogram();
    
    // print longest-shortest endpoints
    lsEndpoints();

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
 * Bacon number: Takes in the actor and bacon. It iterates through the graph
 *      printing the the path between the actor and bacon.
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
 * Read_File: Takes in a file name, opens it, and reads in the movies followed by the
 *      actors of that movie. It pushes the movies into a hash table and lists the
 *      actors in that hash table. It also pushes the actors into its own hash table
 *      and keeps a list of the movies that actor is in.
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

    return;
}

/*
 * BFS: Performs a breadth first search on the graph finding the shortest path 
 *      between the Bacon actor and every other actor
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

    return;
}

/*
 * buildHistogram: This creates a histogram that gets printed to the console. It
 *      goes through the graph and counts the number of actors that have the same
 *      path length. It prints the values to the screen.
 */
void buildHistogram()
{
    cout << "Histogram:" << endl;

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
        {
            cout << j << "  " << histogram[j] << endl;
            maxDist = j;
        }   
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

/*
 * listMovies: Takes in an actors name and prints out all the movies the actor is in
 */
void listMovies( string actor )
{
    // access the actor
    auto it = actorHash.find( actor );

    cout << endl << it -> second.size() << " " << actor << " movies:" << endl;
    
    // print out each movie
    for( unsigned int i = 0; i < it -> second.size(); i++ )
    {
        cout << it -> second[i] << endl;
    }

    return;
}

/*
 * lsEndpoints: Prints the longest-shortest endpoints of Bacon
 */
void lsEndpoints()
{
    cout << endl << "Longest-shortest endpoints: " << endl;

    for( auto it = graphHash.begin(); it != graphHash.end(); it++ )
    {
        if( it -> second.DOS == maxDist )
            cout << it -> first << endl;
    }

    return;
}
