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
 *
 *
 * PYTHON interface
*/

// SRMCollider Lib
#include <combinations.cpp>

#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

void _combinations_wrapper(int M, int N, const char* filename,
        boost::python::list mapping,
        boost::python::list exclude) {
    /*
     * M and and N are the combinations parameter and will produce M choose N results
     * filename is the name of the output file
     * mapping is a list of strings of size N which maps the indices to output
     * exclude is a list of lists (a list of indices to exclude)
     * 
    */
    ofstream myfile;
    boost::python::list inner_list;
    myfile.open (filename);

    int mapping_length = boost::python::extract<int>(mapping.attr("__len__")());
    //Do Error checking, the mapping needs to be at least as long as N 
    if (mapping_length < N) {
        PyErr_SetString(PyExc_ValueError, 
            "The string mapping must be at least of length N");
        boost::python::throw_error_already_set();
        return;
    }
    //Also M should not be bigger than N (not much point) and not smaller than 0
    if(M>N) {
        PyErr_SetString(PyExc_ValueError, 
            "M cannot be larger than N");
        boost::python::throw_error_already_set();
        return;
    }
    if(N < 0 || M < 0) {
        PyErr_SetString(PyExc_ValueError, 
            "M and N need to be larger than 0");
        boost::python::throw_error_already_set();
        return;
    }

    /* Convert the mapping */
    vector<string> mystrings(mapping_length);
    for (int i=0; i<mapping_length; i++) {
        mystrings[i] = boost::python::extract<char const *>(mapping[i]);
    }

    /* Convert the exclude list */
    int exclude_length = boost::python::extract<int>(exclude.attr("__len__")());
    vector< vector<int> > exclude_indices(exclude_length, vector<int>(M,0));    
    for (int i=0; i<exclude_length; i++) {
        inner_list = boost::python::extract< boost::python::list >(exclude[i]);
        int inner_length = boost::python::extract<int>(inner_list.attr("__len__")());

        /* exclude list needs to be the lenght of the indices we produce */
        if(inner_length != M) {
            PyErr_SetString(PyExc_ValueError, 
                "In c_combinations.combinations module... \n"
                "The exclude list needs to consist of a list of indices you \n"
                "want to exclude. It is thus a list of lists where each inner\n"
                "element has length M.");
            boost::python::throw_error_already_set();
            return;
        }

        for (int j=0; j<inner_length; j++) {
            exclude_indices[i][j] = boost::python::extract<int>(inner_list[j]);
        }
    }

    /*

    import c_combinations, string, profile
    #c_combinations.combinations(7, 30, "c.out", [l for l in string.letters[:30]], [ range(7),range(7) ])
    c_combinations.combinations(7, 30, "c.out", [l for l in string.letters[:30]], range(7))
    c_combinations.combinations(7, 30, "c.out", [l for l in string.letters[:30]], [ [ [1,2,4] ] ])
    c_combinations.combinations(  2,  4, 'outfile', ['Spam', 'Eggs', 'Bacon', 'Sausage' ],  [  [0,1], [1,2] ] )
    print c_combinations.combinations.__doc__

    profile.run('c_combinations.combinations(7, 30, "c.out", [l for l in string.letters[:30]], [ range(7) for i in range(100) ])')

    c_combinations.combinations(2, 5, "c.out", [l for l in string.letters[:30]],
        [ [0,1],  [1,2], [2,3 ], [1,3] ] )

    */

    _combinations( M, N, myfile, mystrings, exclude_indices);
    myfile.close();
}

void initcombinations() {;}

using namespace boost::python;
BOOST_PYTHON_MODULE(c_combinations)
{
    def("combinations", _combinations_wrapper, 

 " The functions in this file calculate all combinations of M elements\n"
 " drawn without replacement from a set of N elements. Order of elements\n"
 " does NOT matter.\n"
 "\n"
 " The function combinations can be called from Python. It\n"
 " opens the file and converts the Python arguments:\n"
 " # M: number of elements to be picked\n"
 " # N: pool of elements\n"
 " # filename: output filename\n"
 " # mapping: (list of strings) needs to be supplied to map the\n"
 "   output to meaningful strings. \n"
 " # exclude: (list of lists of integers) a list with 'exceptions'. Each\n"
 "   list consists of integers, has length M and defines a combination\n"
 "   to be skipped. The indices start at 0 and end at M-1 and need to be\n"
 "   ordered.\n"
 "\n"
 " Example: M = 2, N = 4, mapping = ['Spam', 'Eggs', 'Bacon', 'Sausage' ] \n"
 " Call: combinations(2, 4,'outfile', ['Spam', 'Eggs', 'Bacon', 'Sausage'], [])\n"
 " Result:\n"
 " Spam, Eggs\n"
 " Spam, Bacon\n"
 " Spam, Sausage\n"
 " Eggs, Bacon\n"
 " Eggs, Sausage\n"
 " Bacon, Sausage\n"
 "\n"
 " Example: M = 2, N = 4, mapping = ['Spam', 'Eggs', 'Bacon', 'Sausage' ] \n"
 "          exclude = [ [0,1], [1,2] ]\n"
 " Call: combinations(2, 4,'outfile', ['Spam', 'Eggs', 'Bacon', 'Sausage'],  \n"
 "                    [ [0,1], [1,2] ] )\n"
 " Result:\n"
 " Spam, Bacon\n"
 " Spam, Sausage\n"
 " Eggs, Sausage\n"
 " Bacon, Sausage\n"
 "\n"
 "\n"
 " Signature\n"
 "void combinations(int,int,string,list<string>,list<list<int> >)\n"

           );
}

