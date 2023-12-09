
SET(Restclient_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        restclient-cpp
)

FIND_PATH(Restclient_INCLUDE_DIR restclient.h
        HINTS
        PATH_SUFFIXES include/SDL2 include
        PATHS ${Restclient_SEARCH_PATHS}
)

FIND_LIBRARY(Restclient_LIBRARY
        NAMES restclient-cpp
        HINTS
        PATH_SUFFIXES lib64 lib
        PATHS ${Restclient_SEARCH_PATHS}
)

FIND_PACKAGE(CURL)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Restclient REQUIRED_VARS Restclient_LIBRARY Restclient_INCLUDE_DIR)