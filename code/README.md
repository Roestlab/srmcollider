# SRMCollider

The SRMCollider code is organized into a Python module, a C++ module (see the
[C++ README.md](cpp/README.md) and a testing module (see the `./test/` folder).

For installation instructions, see the [INSTALL](INSTALL) file. When all
dependencies are present, the software can be installed with 

```
$ python setup.py install
```

# Usage

## Setup 

You can set up the SRMCollider on a protein fasta file using these commands:

```
python scripts/misc/trypsinize.py mygenome.fasta peptides.txt
perl SSRCalc3.pl --alg 3.0 --source_file peptides.txt \
   --output tsv  --B 1 --A 0 > ssrcalc.out
python scripts/misc/create_db.py --sqlite_database=mygenome.sqlite \
    --tsv_file=ssrcalc.out --peptide_table=mygenome
```

note that you need SSRCalc installed to compute the second step. This script is
for instance included in a TPP install and can be obtained at
[from the TPP repository](https://sashimi.svn.sourceforge.net/svnroot/sashimi/tags/release_4-3-1/trans_proteomic_pipeline/perl/)
(`SSRCalc3.par` and `SSRCalc3.pl` are the files you need). The last steps
creates a sqlite file `mygenome.sqlite` with a table called "mygenome". In a
real example, replace "mygenome" with your genome of interest, e.g. "yeast".

Alternatively, if you are using MySQL (see MySQL setup below) you can create
the database as follows:

```
python scripts/misc/create_db.py --mysql_config=~/.my.cnf.srmcollider \
    --peptide_table=mygenome --tsv_file=ssrcalc.out 
```

## Query individual peptides

To query single peptides that have relative transition intensity information
associated with them, there is the `runcollider.py` script.

It allows to input a list of peptides with relative transition
intensity information and will output the minimal number of transitions needed
to create a unique assay. 

Accepted inputs are srmAtlas tsv files and mProphet csv files.
When using mProphet files, it is also possible to use experimental intensities
to check whether the measured transitions are still sufficient to form an UIS.

Example Workflow: 

- open https://db.systemsbiology.net/sbeams/cgi/PeptideAtlas/GetTransitions
- search for some protein, e.g. YLR304C (in yeast)
-- make sure that you select as many fragments per peptide as possible, e.g. use the option "Num of highest Inten Frag Ions to Keep:"
- select TSV Download
- run the following command

```
    runcollider.py SRMAtlasAssays.tsv --srmatlas_tsv \
      --sqlite_database=mygenome.sqlite \
      --peptide_table=mygenome --max_uis 5 --q3_low 100 \
      --q1_window=0.7 --q3_window=0.7
```

Where you will need to adjust the Q1/Q3 windows. You will use the same database
that you generated earlier (mygenome).  You should see output statistics
describing the number of transitions minimally required per peptide, how many
peptides cannot be observed and the minimal number of transition required to
observe the provided peptides.

This will produce two files, outfile_peptides.csv and outfile_transitions.csv
where the outfile_peptides.csv file contains only 2 columns: the name /
sequence of the peptide and secondly the number of minimally necessary
transitions. NOTE that a value of -1 means that there were not enough
transitions to be specific in the given background.

Further parameters that might be of interest here are

- `--help` : Prints the available parameters
- `--safety` : Number of transitions to add above the absolute
           minimum. Defaults to 3.
- `--ssrcalc_window` : Only consider peptides close in retention time (as predicted by SSRCalc)

## Query whole proteomes

To run queries against whole proteomes, using the `runcollider.py` script is
not advisable since it is not fast enough. This is mostly because it has to
perform an MySQL query for each peptide in the input and this will slow down
the execution.

Instead there are several sample scripts in the `code/scripts/runscripts`
directory which will query a whole proteome. One of these is `run_uis.py` which
will create a rangetree and query it instead of the MySQL database.

To run the SRMCollider on all peptides between 400 and 1400 Da, execute the
following command:

```
  python run_uis.py 1 400 1400 --peptide_table=mygenome \
    --max_uis 5 --use_db --mysql_config=~/.my.cnf.srmcollider"
```

However that for large genomes, this might cause all memory of
the system to be used since it tries to build a rangetree with all peptides in
the genome.  There are two solutions: 

* either partition the calls to `run_uis.py` using the convenient
  `scripts/misc/prepare_rangetree.py` script which will generate a .sh file that has
  many calls to `run_uis.py` using different ranges such that each range contains
  a specified number of peptides. 
* The other solution is to use the `--use_db` parameter which will query the
  database to get the interfering transitions instead of using a rangetree.
  This will also work if you did not compile the C++ extensions.

The drawback with the first method is increased overhead when setting up the
individual rangetrees, the drawback with the second method is the slowdown
created by using MySQL queries instead of querying the rangetree.

Changing the constraints for Q1/Q3/RT can be easily done using the `--q1_window`,
`--q3_window` and `--ssrcalc_window` parameters.

## MySQL setup

Instead of sqlite, you can also use mysql as a backend for the SRMCollider.
This tends to be faster, but slightly more complicated to set up. First, log in
to your mysql server as root and issue the following commands

```
  CREATE USER 'srmcollider'@'localhost' IDENTIFIED BY 'srmcollider';
  CREATE DATABASE srmcollider;
  GRANT ALL ON srmcollider.* TO 'srmcollider'@'localhost';
  FLUSH PRIVILEGES;
```

This will create a user with the name "srmcollider" and the password
"srmcollider". Do _not_ do this on a server that is accessible to the public,
chose some other username/password combination! Then create a
.my.cnf.srmcollider file that looks like this

[client]
host=localhost
user=srmcollider
port=3306
password=srmcollider
database=srmcollider

Now you have a MySQL server that is configured for the SRMCollider.  The script
sqltest_tables_setup.py in the test folder is able to fill test data in (see
action below) and the scripts/create_db.py script can add real data.


# Code organization

## Python library

There `srcmollider` contains multiple library files:

- `collider.py` contains the main functions including the SRMCollider class which controls the main program
- `SRM_parameters.py` contains a parameter object that defines runtime parameters for the SRMCollider
- `precursor.py` contains the `PrecursorAccess` class, allowing clients to select precursors from a local or in-memory database
- `DDB.py` contains data structures for peptides and proteins
- `Residues.py` contains accurate masses for AA residues
- `uis_functions.py` some helper functions, UIS code, combinatorics

- `Fileparser.py` some helper functions for file parsing
- `srmcollider_website_helper.py` important functions for the webserver
- `progress.py` implements a progress bar

## Webserver 

The scripts for the webserver are in `./cgi-scripts`, specifically the main
executable can be found at `cgi-scripts/srmcollider.py`. It relies on some
helper and config files:

- `cgi-scripts/collider_config.py` config file
- `srmcollider/srmcollider_website_helper.py`  helper script
- `cgi-scripts/plot_graph_dynamic.py` script to dynamically plot a graph to show interferences

The helper contains the class SRMColliderController which takes care of data
parsing etc. The cgi script has the "main" function as entry point and which
will hand over to `do_analysis` for processing.

## Executables

The executables can be found under `./scripts/`, where `./scripts/runscripts`
contains executables used for doing single peptide or whole proteome analysis
whereas `./scripts/misc` contains various auxiliary scripts (used for
populating the database, generating a tryptic digest etc).

## C++ module

To speed up computation, some functions have a core C++ component.  The C++
code can be found under `./cpp` and `./cpp/py`, see [the associated README
file](cpp/README.md) for more information on the C++ module itself.

