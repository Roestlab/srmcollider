
# SRMCollider

The SRMCollider code is organized into a Python module, a C++ module (see
[cpp/README.md](cpp/README.md) and a testing module (see test/).

For installation instructions, see the [INSTALL](INSTALL) file.

# Python module

This folder contains the main Python module

## Executables

The following files are executables

- `run_eUIS.py`
- `run_integrated.py`
- `run_swath.py`
- `run_uis.py`

## Python library

There are several helper files


- `DDB.py` contains data structures for peptides and proteins
- `Residues.py` contains accurate masses for AA residues
- `SRM_parameters.py` contains a parameter object that defines runtime parameters for the SRMCollider
- `precursor.py` contains the Precursor class, the main object stored in the SQL data tables
- `uis_functions.py` some helper functions, UIS code, combinatorics
- `Fileparser.py` some helper functions for file parsing
- `srmcollider_website_helper.py` important functions for the webserver

The main functions are contained in the `collider.py` file with the SRMCollider class which ties them all together.

Additional scripts are found in the `./scripts` folder for trypsination of fasta files for example.

## Webserver 

The scripts for the webserver are in `./cgi-scripts`, specifically the
executable can be found at `cgi-scripts/srmcollider.py`. It relies on some
helper and config files:

- `cgi-scripts/collider_config.py` config file
- `srmcollider_website_helper.py`  helper script
- `cgi-scripts/plot_graph_dynamic.py` script to dynamically plot a graph to show interferences

The helper contains the class SRMColliderController which takes care of parsing
etc. The cgi script has the "main" function as entry point and which will hand
over to `do_analysis` for processing.

# C++ module

The C++ code can be found under `./cpp` and `./cpp/py`, see
[here](cpp/README.md) for more information on the C++ module itself.

