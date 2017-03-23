function(build_gen_stream NAME)
    string(TOUPPER ${NAME} UPPERCASE_NAME)

    option(BUILD_GEN_${NAME} "Build stream for generator ${NAME}" ON)

    if(BUILD_${NAME})
        target_link_libraries(generator ${NAME})
        target_compile_definitions(generator PRIVATE -DBUILD_${NAME})
    endif()
endfunction()
