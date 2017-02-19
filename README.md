[![build status](https://gitlab.mech.kuleuven.be/meco-software/cpp_splines/badges/develop/build.svg)](https://gitlab.mech.kuleuven.be/meco-software/cpp_splines/commits/develop)


[![coverage report](https://gitlab.mech.kuleuven.be/meco-software/cpp_splines/badges/develop/coverage.svg)](https://gitlab.mech.kuleuven.be/meco-software/cpp_splines/commits/develop)


# Run the binaries (inside MECO group):

See instructions https://gitlab.mech.kuleuven.be/meco-software/meco_binaries/tree/master

# cpp_splines


To run unittests:

All tests, with `-V` for verbose:

`ctest -V`

Only python tests :

`ctest -V -R python`

Only matlab tests (with `-V` for verbose):

`ctest -V -R matlab`

Cpplint filters:

-whitespace/semicolon,-whitespace/blank_line,-whitespace/operators,-whitespace/indent,-whitespace/comments,-build/include_order,-build/include_order,-build/namespaces,-build/include_what_you_use,-readability/streams,-runtime/references,-runtime/int,-runtime/explicit,-runtime/printf,-build/c++11,-readability/braces,-runtime/indentation_namespace


linesize 100



