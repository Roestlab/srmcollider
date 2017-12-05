/*
 *
 * Program       : SRMCollider
 * Author        : Hannes Roest <roest@imsb.biol.ethz.ch>
 * Date          : 05.02.2011 
 *
 *
 * Copyright (C) 2011 - 2012 Hannes Roest
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA
 *
 */

/*
 * The functions in this file calculate all combinations of M elements
 * drawn without replacement from a set of N elements. Order of elements
 * does NOT matter.
 *
 * The function Display is used to write to a ofstream which was defined
 * before. Instead of writing the indices, a string mapping can be
 * provided.
 *
 * The function _combinations is used to calculate the indices and call
 * Display
 *
 * The function _combinations_wrapper can be called from Python. It
 * opens the file and converts the Python arguments:
 * # M: number of elements to be picked
 * # N: pool of elements
 * # filename: output filename
 * # mapping: (list of strings) needs to be supplied to map the output
 *   indices to meaningful strings.
 * # exclude: (list of lists of integers) a list with 'exceptions'. Each
 *   list consists of integers, has length M and defines a combination
 *   to be skipped. The indices start at 0 and end at M-1 and need to be
 *   ordered.
 *
 * Example: M = 2, N = 4, mapping = ['Spam', 'Eggs', 'Bacon', 'Sausage' ] 
 * Call: combinations(2, 4,'outfile', ['Spam', 'Eggs', 'Bacon', 'Sausage'], [])
 * Result:
 * Spam, Eggs
 * Spam, Bacon
 * Spam, Sausage
 * Eggs, Bacon
 * Eggs, Sausage
 * Bacon, Sausage
 *
 * Example: M = 2, N = 4, mapping = ['Spam', 'Eggs', 'Bacon', 'Sausage' ] 
 *          exclude = [ [0,1], [1,2] ]
 * Call: combinations(2, 4,'outfile', ['Spam', 'Eggs', 'Bacon', 'Sausage'],  
 *                    [ [0,1], [1,2] ] )
 * Result:
 * Spam, Bacon
 * Spam, Sausage
 * Eggs, Sausage
 * Bacon, Sausage
 *
 *
 * Signature
 *void combinations(int,int,string,list<string>,list<list<int> >)
*/

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Display(int vi[], int size, ofstream &myfile, const vector<string>&
        string_mapping) {
    for(int i=0; i<size-1; ++i)
        myfile << string_mapping[vi[i]] << ",";
    myfile << string_mapping[vi[size-1]];
    myfile << endl;
}

void _combinations(int M, int N, ofstream &myfile, const vector<string>&
        string_mapping, const vector<vector<int> >& exclude) {
    // The basic idea is to create an index array of length M that contains
    // numbers between 0 and N. The indices denote the combination produced and
    // we always increment the rightmost index. If it goes above N, we try to
    // increase the one left to it until we find one that still can be
    // increased. We stop when the rightmost index hits N-M, we thus go from
    // (0,1,2,3...M-1) to (N-M,N-M+1,N-M+2...N-1)
    int j, k;
    bool found = false;
    int* index = new int[M];
    //initialize with numbers from 0 to M = range( M )
    for(int k=0;k<M;k++) index[k] = k;
    while (index[0] < N-M) {
        /* Do we need to skip this one, e.g. is it in the exclude?  */
        for(std::vector<int>::size_type i = 0; i != exclude.size(); i++) {
            found = true;
            for(std::vector<int>::size_type j = 0; j != exclude[i].size(); j++) {
                /* std::cout << someVector[i]; ... */
                if( exclude[i][j] != index[j] ) found = false;
            }
            if(found) break;
        }
        if(!found) Display( index, M, myfile, string_mapping);

        index[ M-1 ] += 1;
        if (index[ M-1 ] >= N) {
            //#now we hit the end, need to increment other positions than last
            //#the last position may reach N-1, the second last only N-2 etc.
            j = M-1;
            while (j >= 0 and index[j] >= N-M+j) j -= 1;
            //#j contains the value of the index that needs to be incremented
            index[j] += 1;
            k = j + 1;
            while (k < M) { index[k] = index[k-1] + 1; k += 1; } 
        }
    }
    //TODO also check whether the last one is here!
    Display( index, M, myfile, string_mapping);
    delete [] index;
}



/* int main()
{
    cout << "starting \n";

    clock_t start, finish;
    start = clock();

    vector<string> mystrings(4);
    mystrings[0] =  "Spam";
    mystrings[1] = "Eggs";
    mystrings[2] =  "Bacon";
    mystrings[3] =  "Sausage";

    vector<vector<int> > exclude(0);

    ofstream myfile;
    myfile.open ("test.out");
    _combinations( 2, 4, myfile, mystrings, exclude);
    myfile.close();

    cout << "done \n";
    finish = clock();
    cout <<  finish - start << endl;
    cout << CLOCKS_PER_SEC  << endl;
    cout << ( (finish - start) * 1.0 /CLOCKS_PER_SEC ) << endl;

return 0;
}  */


