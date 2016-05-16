EEBO TCP similarity search
==========================

Setup
-----

    git submodule update --init --recursive

For Linux:

    make -j

For OS X & Homebrew:

    make -j CXX=g++-5

CSC Taito:

    module load gcc/5.3.0
    make -j


Directories
-----------

`tcp-xml` should point to a directory with TCP XML files.
For example, `tcp-xml/A00002.xml` should exist.

`output` will contain the results.


Running
-------

CSC Taito:

    sbatch batch-taito
