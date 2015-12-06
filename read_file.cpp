#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
//#include <regex>
#include <vector>

using namespace std;

int main( int argc, char** argv )
{
    ifstream fin;    

    fin.open( argv[1], ios::in );

    vector<string> seglist;
    vector<string> movies;
    vector<string> actors;

    int flag = 0;

    string x;
    while( !fin.eof()  )
    {
        getline( fin, x, '/' );
        seglist.push_back(x);
        if( flag == 0 )
        {
            movies.push_back(x);
            flag = 1;
        }
        if( flag == 1 )
        {
            actors.push_back(x);
            flag = 0;
        }
    }

    cout << "seglist" << endl;
    for( vector<string>::const_iterator i = seglist.begin(); i != seglist.end(); i++ )
        cout << *i << endl;


    /*for( vector<string>::const_iterator i = seglist.begin(); i != seglist.end(); i++ )
    {
        cout << "i: " << *i << endl;
        if( *i == "1" )
        {
            i++;
            movies.push_back( *i );
        }
        else
            actors.push_back( *i );
    }*/

//    cout << "seglist" << endl;
//    for( vector<string>::const_iterator i = seglist.begin(); i != seglist.end(); i++ )
//        cout << *i << endl;

    cout << "movies" << endl;
    for( vector<string>::const_iterator i = movies.begin(); i != movies.end(); i++ )
        cout << *i << endl;

    cout << "actors" << endl;
    for( vector<string>::const_iterator i = actors.begin(); i != actors.end(); i++ )
        cout << *i << endl;

    return 1;
}
