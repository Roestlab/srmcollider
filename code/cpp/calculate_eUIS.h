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

#ifndef SRMCOLLIDE_EUIS_H
#define SRMCOLLIDE_EUIS_H

#include <vector>
#include <algorithm>
#include <numeric>

// Boost.Python headers
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

/*
 * Function to calculate whether there exists any combination of values from M
 * arrays (one value from each array) such that the M values are within a
 * certain window. 
 *
 * The first argument is a list that contains the length of the lists to be
 * checked. The second argument a list of lists which contain the values to be
 * checked, the third argument the window size.
 *
 * The second list is the main input and contains, for each transition, a
 * sorted list of the retention time of the interfering peptides. For example,
 * for three transitions this could look like:
 *
 * tr1 = [ 24.0    25.0    26.0   27.0 ]
 * tr2 = [ 24.7            26.0   27.9 ]
 * tr3 = [ 24.2    25.1    26.6   28.9 ]
 *
 * note that there is one peptide at 26.0 RT which interfers with
 * both transitions tr1 and tr2. Once you have this list, you can "scan" the
 * lists in c*k steps where c is the number of transitions and k is the
 * number of peptides (the length of the lists). You basically have c indices
 * of which you always increase the one with the lowest RT.  With this you can
 * answer the question whether there is a "slice" in RT in which n out of t
 * transitions fall 
 *
 * The function returns a all the "forbidden" tuples, e.g. tuples of 
 * transitions that are interfering and can thus not be used for an eUIS.
 *
 */
void calculate_eUIS(std::vector<int>& N, std::vector<std::vector<double> >& c_ssrcalcvalues,
    double ssrwindow, std::vector<std::vector<int> >& all_nonuis);

// Python wrapper for calculate_eUIS
boost::python::list py_calculate_eUIS(boost::python::list myN, boost::python::list py_ssrcalcvalues, double ssrwindow);

#endif
