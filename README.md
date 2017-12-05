srmcollider
========

Computes interferences in SRM (Selected Reaction Monitoring) experiments.

Homepage
------------
You can find our website (with an interactive tool) at [SRMCollider.org](http://www.srmcollider.org).

Publication
------------
This tool was published in April 2012 and can be cited as follows: 

Röst H, Malmström L, Aebersold R. A computational tool to detect and avoid redundancy in selected reaction monitoring. Mol Cell Proteomics. 2012 Aug;11(8):540-9. PMID 22535207Röst H, Malmström L, Aebersold R. A computational tool to detect and avoid redundancy in selected reaction monitoring. Mol Cell Proteomics. 2012 Aug;11(8):540-9. [PMID 22535207](http://www.ncbi.nlm.nih.gov/pubmed/22535207)

Installation
------------
For installation and further instructions, see [here](code/README.md).

Running
-------
There are several scripts in code/scripts/runscripts/ that will be installed and are useful for different types of analyses:

- `runcollider.py` is the most generic way to run the SRMCollider for
  individual peptides and generate information about the uniqueness of their
  transitions. 
- `run_uis.py` is used for high-throughput analyses of a whole proteome and
  computes summary statistics on the number of unique ion signatures (UIS) for
  each peptide in the query set.
- `run_integrated.py` is used for high-throughput analyses of a whole proteome
  and uses optimized code in C++ to speed up the analysis.
- `run_eUIS.py` is used for computing extended UIS (eUIS) as described in Röst et al.


Webserver
---------

The SRMCollider comes with a webserver which can be accessed  online at
[SRMCollider.org](http://www.srmcollider.org). For more information on how to
install the webserver locally, see [here](code/README.md).

Development
-----------

See the [README for developers](code/README.md).

