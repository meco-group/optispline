enable_testing()

if(WITH_PYTHON)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/tests/*.py" )

foreach(file ${files})
    file(RELATIVE_PATH relfile ${CMAKE_SOURCE_DIR} ${file})
    string(REPLACE "/" "__" BASE ${relfile})
    string(REPLACE ".py" "" BASE ${BASE})
    # Test with valgrind (catches memory errors; takes a long while to run)
    # Allow the receiving script to check if it is running inside valgrind
    # This makes it possible to short-circuit expensive tests
    add_test(valgrind_${BASE}
        valgrind --leak-check=full
        --suppressions=$ENV{CASADI_PREFIX}/casadi/include/casadi/valgrind-python.supp --suppressions=$ENV{CASADI_PREFIX}/casadi/include/casadi/valgrind-python.supp 
        --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/cpp_splines.supp
        --show-possibly-lost=no --error-limit=no --gen-suppressions=all
        python${PYTHON_VERSION} ${file})
    set_tests_properties(valgrind_${BASE} PROPERTIES ENVIRONMENT "VALGRIND=1")
    add_test("python_${BASE}" python${PYTHON_VERSION} ${file})
    set_tests_properties("python_${BASE}" valgrind_${BASE} PROPERTIES ENVIRONMENT "MECO_BINARIES_OVERRIDE_CASADI=$ENV{CASADI_PREFIX};MECO_BINARIES_OVERRIDE_CPP_SPLINES=${CMAKE_BINARY_DIR}/swig;")
endforeach()

file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/examples/*.py")

foreach(file ${files})
    file(RELATIVE_PATH relfile ${CMAKE_SOURCE_DIR} ${file})
    string(REPLACE "/" "__" BASE ${relfile})
    string(REPLACE ".py" "" BASE ${BASE})
    # Test with valgrind (catches memory errors; takes a long while to run)
    # Allow the receiving script to check if it is running inside valgrind
    # This makes it possible to short-circuit expensive tests
    add_test("python_${BASE}" python${PYTHON_VERSION} ${file} -dAgg)
    set_tests_properties("python_${BASE}" PROPERTIES ENVIRONMENT "MECO_BINARIES_OVERRIDE_CASADI=$ENV{CASADI_PREFIX};MECO_BINARIES_OVERRIDE_CPP_SPLINES=${CMAKE_BINARY_DIR}/swig")
endforeach()
endif()


if(WITH_MATLAB)
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/tests/*.m" "${CMAKE_SOURCE_DIR}/examples/*.m")

foreach(file ${files})
    file(RELATIVE_PATH relfile ${CMAKE_SOURCE_DIR} ${file})
    string(REPLACE "/" "__" BASE ${relfile})
    string(REPLACE ".m" "" BASE ${BASE})

    get_filename_component(folder ${file} DIRECTORY)
    get_filename_component(fileshort ${file} NAME_WE)

    add_test(matlab_${BASE}
    matlab -softwareopengl -nosplash -nodesktop -r "try,cd '${folder}',${fileshort},catch e,e.getReport,exit(1),end,exit force")

    set_tests_properties(matlab_${BASE} PROPERTIES ENVIRONMENT "MECO_BINARIES_OVERRIDE_CASADI=$ENV{CASADI_PREFIX};MECO_BINARIES_OVERRIDE_CPP_SPLINES=${CMAKE_BINARY_DIR}/swig")

endforeach()
endif()

add_definitions(-DCASADI_WITH_THREAD)

add_executable(main main.cpp)
target_link_libraries(main splines)








