enable_testing()


file(GLOB files "*.py")
foreach(file ${files})
    get_filename_component(BASE ${file} NAME_WE)
    # Test with valgrind (catches memory errors; takes a long while to run)
    # Allow the receiving script to check if it is running inside valgrind
    # This makes it possible to short-circuit expensive tests
    add_test(valgrind_${BASE}
        valgrind --leak-check=full
        --suppressions=$ENV{CASADI_PREFIX}/include/valgrind-python.supp --suppressions=$ENV{CASADI_PREFIX}/include/valgrind-python.supp 
        --suppressions=cpp_splines.supp
        --show-possibly-lost=no --error-limit=no --gen-suppressions=all
        python ${file}) 
    set_tests_properties(valgrind_${BASE} PROPERTIES ENVIRONMENT "VALGRIND=1")
    add_test("python_${BASE}" python ${file})
    set_tests_properties("python_${BASE}" valgrind_${BASE} PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../swig:$ENV{CASADI_PREFIX}:$ENV{PYTHONPATH}")
endforeach()

add_test(matlab
    matlab -nojvm -nodesktop -r "addpath('$ENV{CASADI_PREFIX}');addpath('${CMAKE_CURRENT_SOURCE_DIR}/../swig');test_matlab"
    )








