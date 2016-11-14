enable_testing()


file(GLOB files "*.py")
foreach(file ${files})
    if(NOT "${file}" MATCHES "helpers.py")
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
      set_tests_properties("python_${BASE}" valgrind_${BASE} PROPERTIES ENVIRONMENT "MECO_BINARIES_OVERRIDE_CASADI=$ENV{CASADI_PREFIX};MECO_BINARIES_OVERRIDE_CPP_SPLINES=${CMAKE_CURRENT_BINARY_DIR}/../swig")
    endif()
endforeach()

add_test(matlab
    matlab -nojvm -nodesktop -r "cd ${CMAKE_CURRENT_SOURCE_DIR};runAll('matlab')"
    )

set_tests_properties(matlab PROPERTIES ENVIRONMENT "MECO_BINARIES_OVERRIDE_CASADI=$ENV{CASADI_PREFIX};MECO_BINARIES_OVERRIDE_CPP_SPLINES=${CMAKE_CURRENT_BINARY_DIR}/../swig")









