ExternalProject_Add (
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND "" #${GIT_EXECUTABLE} pull v.1.3.5 is broken
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    GIT_TAG "v1.3.4" # master is broken
    LOG_DOWNLOAD ON)

ExternalProject_Get_Property (catch source_dir)
include_directories (${source_dir}/single_include)

file (GLOB_RECURSE srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach (fileSrc ${srcs})
    get_filename_component (fileName ${fileSrc} NAME_WE)
    get_filename_component (filePath ${fileSrc} DIRECTORY)

    add_executable (test_${fileName} ${fileSrc})

    set_target_properties (test_${fileName} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN}/${filePath})

    add_test (NAME test_${fileName} 
        WORKING_DIRECTORY ${TEST_BIN}
        COMMAND ${TEST_BIN}/${filePath}/test_${fileName})
endforeach (fileSrc)
