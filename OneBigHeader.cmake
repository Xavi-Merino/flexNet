FILE(GLOB sources "${PROJECT_SOURCE_DIR}/src/*")
list(REVERSE sources)
list(LENGTH sources len_sources)
set(HEADER_NAME "simulator.hpp")

file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_NAME})
STRING(LENGTH "${PROJECT_SOURCE_DIR}/src/" lenvar)
set(aproved_sources "")
foreach( aux RANGE ${len_sources} )
    foreach( src_file ${sources} )
        string(SUBSTRING "${src_file}" ${lenvar} -1 only_header)
        list(FIND aproved_sources "${only_header}" found)
        if(NOT (found EQUAL -1))
            continue()
        endif()
        
        file(STRINGS "${src_file}" header_str)
        set (is_valid TRUE)
        foreach(LINE IN LISTS header_str)
            if(LINE MATCHES "#include \"")
                set (is_valid FALSE)
                foreach(header ${aproved_sources})
                    if(LINE MATCHES "${header}")
                        set (is_valid TRUE)
                    endif()
                endforeach(header ${aproved_sources})
                if(NOT is_valid)
                    break()
                endif()
            endif()
        endforeach()
        if(is_valid)
            string(SUBSTRING "${src_file}" ${lenvar} -1 only_header)
            LIST(APPEND aproved_sources 
            "${only_header}"
            )
        endif()
    endforeach( src_file ${sources} )
endforeach()
foreach(header ${aproved_sources})
    file(READ "${PROJECT_SOURCE_DIR}/src/${header}" aux)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_NAME} "${aux}\n")

endforeach(header ${aproved_sources})

file(READ ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_NAME} filedata)
string(REGEX REPLACE "#include \"" "// #include \"" filedata "${filedata}")
file(WRITE  ${CMAKE_CURRENT_BINARY_DIR}/${HEADER_NAME} "${filedata}")