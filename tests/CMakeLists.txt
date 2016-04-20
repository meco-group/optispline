enable_testing()

add_test(python python test_create_bspline.py)
add_test(matlab matlab -nojvm -nodesktop -r "pwd;test_matlab")

set_tests_properties(python PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../swig\;$ENV{PYTHONPATH}")
