cmake_minimum_required(VERSION 3.9)
project(http_request)

set(CMAKE_CXX_STANDARD 11)

function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()

include_sub_directories_recursively(${HEADER_DIR})

aux_source_directory(http HTTP)
aux_source_directory(web WEB)

add_library(http_request SHARED ${HTTP} ${WEB})
