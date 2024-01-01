#
# Try to find CEC library and include path.
# Once done this will define
#
# CEC_FOUND
# CEC_INCLUDE_PATH
# CEC_LIBRARY
#

FIND_PATH(CEC_INCLUDE_DIR
        NAMES
        cec.h
        PATHS
        ${CMAKE_INCLUDE_PATH}/include/libcec/
        ~/include/libcec/
        /usr/include/libcec/
        /usr/local/include/libcec/
        /sw/include/libcec/
        /opt/local/include/libcec/
        DOC "The directory where cec.h resides")

FIND_LIBRARY(CEC_LIBRARY
        NAMES CEC cec
        PATHS
        ${CMAKE_LIBRARY_PATH}/lib/
        ~/lib/
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        DOC "The CEC library")


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEC REQUIRED_VARS CEC_LIBRARY CEC_INCLUDE_DIR)
