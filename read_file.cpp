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
    ifstream fin;    

    fin.open( argv[1], ios::in );

    vector<string> movies;
    vector<string> actors;
    vector<string> lines;

    int flag = 0;

/***************/
    size_t it1;
    std::size_t it2;
    string del = "/";
    std::size_t pos;
    string s2;
/*****************/

    string x;
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
                }
                else
                {
                    //actors.push_back( line.substr( it1, it2 ) );
                    actors.push_back( s2 );
                }
            }
            else
            {
                s2 = line.substr( it1, line.size() );
                actors.push_back(s2);
                break;
            }
        }
    }

    cout << endl << "movies" << endl;
    for( vector<string>::const_iterator i = movies.begin(); i != movies.end(); i++ )
        cout << *i << endl;

    cout << endl << "actors" << endl;
    for( vector<string>::const_iterator i = actors.begin(); i != actors.end(); i++ )
        cout << *i << endl;

    fin.close();
/////////////////////////////////////////////////////////////



    return 1;
}
