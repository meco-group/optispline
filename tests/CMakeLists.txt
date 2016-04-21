enable_testing()

add_test(python 
  python test_create_bspline.py
)

add_test(matlab
  matlab -nojvm -nodesktop -r "addpath('$ENV{CASADI_PREFIX}');addpath('${CMAKE_CURRENT_SOURCE_DIR}/../swig');test_matlab"
)

# Test with valgrind (catches memory errors; takes a long while to run)
add_test(valgrind
  valgrind --leak-check=full
  --suppressions=$ENV{CASADI_PREFIX}/include/valgrind-python.supp --suppressions=$ENV{CASADI_PREFIX}/include/valgrind-python.supp 
  --suppressions=cpp_splines.supp
  --show-possibly-lost=no --error-limit=no --gen-suppressions=all
  python test_create_bspline.py)

# Allow the receiving script to check if it is running inside valgrind
# This makes it possible to short-circuit expensive tests
set_tests_properties(valgrind PROPERTIES ENVIRONMENT "VALGRIND=1")


set_tests_properties(python valgrind PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../swig:$ENV{CASADI_PREFIX}:$ENV{PYTHONPATH}")


