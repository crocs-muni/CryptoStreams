find_package(Git)

function(detect_version FILE)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --dirty
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
    else(${VERSION_TAG})
        message(WARNING "Git not found")
        set(GIT_COMMIT "n/a")
        set(GIT_COMMIT_SHORT "n/a")
    endif()
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}.in" "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}" @ONLY)
endfunction(detect_version)
