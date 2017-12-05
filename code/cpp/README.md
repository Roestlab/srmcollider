
# libSRMCollider

This is a C++ library which is used by the SRMCollider software which also
contains Python bindings as the main way to interact with the package. 

To configure, and build run 

    cmake .
    make 

All tests are in ./test and in order to run them use

    make && make test

All Python extension modules are in ./py and will not be built with cmake.
Please use the Python setup procedure to achieve this (should be in ..).

# Functionality

The C++ is made out of 5 components, a combinatorics module, a range tree
module, a common library, a module for computation of eUIS (extended UIS) and a
file for integrated runs which pushes most computation down to C++ to gain
speed.

## SRMCollider Library

General purpose library functions and data structures are in `src/srmcolliderLib.h`
using namespace SRMCollider::Common. These include for example fragment ion mass calculations. 
The implementations are in the corresponding .cpp file.

## Combinatorics

The combinatorics library is found in the file `src/combinatorics.h` using the
namespace SRMCollider::Combinatorics which computes combinations of drawing M
elements from a set of N elements (e.g. M transitions out of a total of N).
This is for example useful when computing all combinations of non UIS
transitions, see function `get_non_uis`.

## Rangetree

The rangetree library is found in `src/rangetree.h`. It is able to build a tree
and query a tree. There are two trees:

- SRMCollider::SimpleRangetree which is a 2D tree on RT, Q1 and for each entry a struct with parent_id and q1_charge is stored
- SRMCollider::ExtendedRangetree which is a 2D tree on RT, Q1 and for each entry a struct with sequence, peptide_key, transition_group_id, q1_charge and isotopic modification is stored. The additional information is needed for example in `integratedrun.cpp`

The implementations are in the corresponding .cpp file.

## Extended UIS

Extended UIS can be calculated using `src/calculate_eUIS.h`, see function
`calculate_eUIS` for example. The input for the module is a list of RT values
for each transition to be tested.
The implementations are in the corresponding .cpp file.

## Integrated run 

The functions necessary for an integrated, mostly C++ run are described in 
`src/integratedrun.h` and use the namespae SRMCollider::IntegratedRun to combine
the rangetree query and the computation of the non UIS transitions into a single function, 
`wrap_all_bitwise` or `min_needed`. The implementations are in the corresponding .cpp file.


# Python bindings

In the ./py folder, you can find bindings for four of the above modules plus a
shared python integration library at `py/py_srmcolliderLib.h`. The common library
contains some SRMCollider::Common functions as well as SRMCollider::pyToC which
can convert from C++ to Python data structures and back. The individual bindings are 

- `py/py_combinations.cpp`
- `py/py_getNonUis.cpp`
- `py/py_integratedrun.cpp`
- `py/py_rangetree.cpp`

Each of these can be compile to an independent Python module using the
`setup.py` installation script provided with SRMCollider.


