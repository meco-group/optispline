enable_testing()

add_test(python echo 123)

set_tests_properties(python PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../swig\;$ENV{PYTHONPATH}")
