[![Build Status](https://travis-ci.org/meco-group/cpp_splines.svg?branch=master)](https://travis-ci.org/meco-group/cpp_splines)
[![Coverage Status](https://coveralls.io/repos/github/meco-group/cpp_splines/badge.svg?branch=master)](https://coveralls.io/github/meco-group/cpp_splines?branch=master)
# Run the binariess (inside MECO group):

 1. 
    - Linux: `sudo mount -t davfs https://drives.kuleuven.be/hcwebdav /mnt`
    - Windows: should be mounted already ('J:').
 2. Locate and run the example scripts on the mounted drive, inside the MECO folder, `software/cpp_splines/master/test.*`

# cpp_splines


To run unittests:

All tests, with `-V` for verbose:

`ctest -V`

Only python tests :

`ctest -V -R python`

Only matlab tests (with `-V` for verbose):

`ctest -V -R matlab`




