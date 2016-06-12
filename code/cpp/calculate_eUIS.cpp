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
namespace python = boost::python;

bool SortIntDoublePairSecond(const std::pair<int,double>& left, const std::pair<int,double>& right)
{
  return left.second < right.second;
}

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
    double ssrwindow, std::vector<std::vector<int> >& all_nonuis) 
{

    int M = (int)N.size(); // M is the numer of transitions

    int k, i;
    unsigned int m, n;
    int sumlen = 0;
    std::vector<int> index; index.resize(M); // current index
    std::vector<int> sort_idx; sort_idx.resize(M);
    std::vector<bool> discarded_indices; discarded_indices.resize(M);
    std::vector<double> myssr; myssr.resize(M); // current slice
    std::vector< std::pair<int,double> > with_index;

    for(int k=0;k<M;k++) index[k] = 0;
    for(int k=0;k<M;k++) sumlen += N[k];
    for(int k=0;k<M;k++) discarded_indices[k] = false;

    double max_elem = 0;
    for(k = 0; k < M; k++) {
      for(i = 0; i < N[k]; i++) {
        if (c_ssrcalcvalues[k][i] > max_elem) {max_elem = c_ssrcalcvalues[k][i];}
      }
    }

    // check whether there are any empty lists
    int cnt =0;
    for(k=0; k < M; k++) {
      if(N[k] == 0)
      {
        discarded_indices[k] = true;
        cnt++;
      }
    }

    // all lists are empty
    if(cnt==M) {return;}

    // We do this c * k times (c = nr transitions, k = list lengths) by
    // advancing one pivot element at a time and thus having each element as
    // pivot at least once
    while(true) 
    {

        for(k=0; k < M; k++) 
        {
          if(!discarded_indices[k])
          {
            myssr[k] = c_ssrcalcvalues[k][ index[k] ];
          }
        }

        // Find the pivot element (the one with the lowest RT)
        double smin = max_elem;
        int piv_i = -1;
        for(k=0; k < M; k++) 
        {
          if(!discarded_indices[k])
            if(myssr[k] <= smin) 
            {
              smin = myssr[k];
              piv_i = k;
            }
        }

        // We need the current slice to be sorted but also keep the original
        // indices to map back to the original.
        //
        // Store a list of pairs (index, RT), sort the list by RT and then
        // store the sorted values (as well as the indices)
        with_index.resize(0);
        for(k=0; k < M; k++) 
        {
          with_index.push_back(std::make_pair(k,myssr[k]));
        }
        std::stable_sort(with_index.begin(), with_index.end(), SortIntDoublePairSecond);
        for(k=0; k < M; k++) 
        {
          sort_idx[k] = with_index[k].first;
          myssr[k] = with_index[k].second;
        }

        // Find all N different combinations that are not UIS. Since they are
        // sorted, for each element we only need to consider elements that have
        // a higher index.
        for(k=0; k < M; k++) 
        {
          if(discarded_indices[sort_idx[k]]) continue;

          std::vector<int> nonuis;
          nonuis.push_back(sort_idx[k]);

          // Look at all elements with a higher index and add them to the
          // current non UIS list if they are within the RT window
          for(i=k+1; i < M; i++) 
          {
            if(discarded_indices[sort_idx[i]]) continue;
            if(!(std::fabs(myssr[k] - myssr[i]) > ssrwindow))
            {
              nonuis.push_back(sort_idx[i]);
            }
          }

          // Append the new set of transitions as non UIS if they are not yet
          // present in the result. 
          std::sort(nonuis.begin(), nonuis.end());
          bool is_present = false;
          bool this_not_present = true;
          for(m=0; m<all_nonuis.size(); m++)
          {
            this_not_present = false;
            for(n=0; n<all_nonuis[m].size() and n < nonuis.size(); n++)
            {
              if(nonuis[n] != all_nonuis[m][n])
              {
                this_not_present = true; break;
              }
            }
            if(all_nonuis[m].size() != nonuis.size()) {this_not_present = true;}

            //cout << " compared " << m << endl;
            if(!this_not_present) 
            {
              is_present = true; 
              break;
            }
          }

          if(!is_present)
          {
            all_nonuis.push_back(nonuis);

          }
        }

        // Advance the pivot element
        index[piv_i] += 1;

        // Catch case if we advanced it beyond the end of the list and check
        // whether we have any transition lists left (or all pivots have been
        // advanced beyond the end) -> if so, we terminate
        if(index[piv_i] >= N[piv_i])
        {
          discarded_indices[ piv_i ] = true;
          int dcount = 0;
          while(dcount < M && discarded_indices[dcount]) dcount++;
          if(dcount >= M)
              break;
        }
    } // end of while(true)
    return;
}

// Python wrapper for calculate_eUIS
python::list py_calculate_eUIS(python::list myN, python::list py_ssrcalcvalues, double ssrwindow) 
{
    python::list result;
    std::vector<std::vector<int> > all_nonuis;

    int M = python::extract<int>(myN.attr("__len__")());
    int k, i;
    unsigned int m, n;

    // fill up N
    std::vector<int> N; N.resize(M);
    for(int k=0;k<M;k++) N[k] = python::extract<int>(myN[k]);

    // fil up c_ssrcalc values
    std::vector<std::vector<double> > c_ssrcalcvalues; c_ssrcalcvalues.resize(M);
    for(k = 0; k < M; k++) { c_ssrcalcvalues[k].resize(N[k]); }
    python::list tmplist;
    for(k = 0; k < M; k++) {
        tmplist = python::extract<python::list>(py_ssrcalcvalues[k]);
        for(i = 0; i < N[k]; i++) {
            c_ssrcalcvalues[k][i] = python::extract<double>(tmplist[i]); 
        }
    }

    calculate_eUIS(N, c_ssrcalcvalues, ssrwindow, all_nonuis);

    // convert to python datastructure
    for(m=0; m<all_nonuis.size(); m++)
    {
      python::list tmplist;
      for(n=0; n<all_nonuis[m].size(); n++)
      {
        tmplist.append(all_nonuis[m][n]);
      }
      result.append(tmplist);
    }

    return result;
}

#endif
