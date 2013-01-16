PROJECT(WT)
SET(CMAKE_MODULE_PATH ${WT_SOURCE_DIR})

#
# Various things that must be configured by the user or packager ...
#

SET(BUILD_SHARED_LIBS ON)

IF(WIN32)

  SET(RUNDIR c:/witty CACHE PATH "Default path for wt session management")
  SET(CONFIGURATION c:/witty/wt_config.xml CACHE PATH "Path for the wt configuration file")
  SET(WEBUSER apache CACHE STRING "Webserver username (e.g. apache or www)")
  SET(WEBGROUP apache CACHE STRING "Webserver groupname (e.g. apache or www or users)")
  SET(DEPLOYROOT c:/witty/wt-examples CACHE PATH "Path to deploy examples into")
  OPTION(CONNECTOR_FCGI "Compile in FCGI connector (libwtfcgi) ?" OFF)
  OPTION(CONNECTOR_HTTP "Compile in HTTP connector (libwthttp) ?" ON)
  SET(BOOST_DIR c:/boost CACHE PATH "Use boost in directory")
  SET(BOOST_SUFFIX gcc CACHE STRING "boost compiler signature")
  SET(BOOST_VERSION boost-1_35 CACHE STRING "Use boost version in BOOST_DIR")
  SET(USERLIB_ROOT "" CACHE PATH "directory containing the dependencies")
  SET(GD_DIR c:/libraries/gdwin32 CACHE PATH "Search path for libgd")

ELSE(WIN32)

  SET(RUNDIR /usr/wt/run CACHE PATH "Default path for wt session management")
  SET(CONFIGURATION /etc/wt/wt_config.xml CACHE PATH "Path for the wt configuration file")
  SET(WEBUSER apache CACHE STRING "Webserver username (e.g. apache or www)")
  SET(WEBGROUP apache CACHE STRING "Webserver groupname (e.g. apache or www or users)")
  SET(DEPLOYROOT /var/www/localhost/htdocs/wt-examples CACHE PATH "Path to deploy examples into")
  OPTION(CONNECTOR_FCGI "Compile in FCGI connector (libwtfcgi) ?" ON)
  OPTION(CONNECTOR_HTTP "Compile in HTTP connector (libwthttp) ?" OFF)
  SET(BOOST_DIR /usr CACHE PATH "Use boost in directory")
  SET(BOOST_SUFFIX gcc CACHE STRING "boost compiler signature")
  SET(BOOST_VERSION "" CACHE STRING "Use boost version in BOOST_DIR")
  SET(USERLIB_ROOT /usr CACHE PATH "directory containing the dependencies")
  SET(GD_DIR "/usr/lib" CACHE PATH "Search path for libgd")

ENDIF(WIN32)

#SET(BOOST_DIR ${BOOST_DIR})
#SET(BOOST_SUFFIX ${BOOST_SUFFIX})
#SET(USERLIB_ROOT ${USERLIB_ROOT})
SET(BOOST_LIB_DIR ${BOOST_DIR}/lib)

FIND_PATH(BOOST_INCLUDE_DIR
    boost/lexical_cast.hpp
  PATHS
    ${BOOST_DIR}/include
    ${BOOST_DIR}/include/${BOOST_VERSION}
  NO_DEFAULT_PATH
)

LINK_DIRECTORIES(
  ${BOOST_LIB_DIR}
)

FIND_PATH(GD_INCLUDE_DIR
    gd.h
  PATHS
    ${GD_DIR}
    /usr/include
    /usr/local/include
)

IF (GD_INCLUDE_DIR)
  INCLUDE_DIRECTORIES(
    ${GD_INCLUDE_DIR}
  )
ENDIF (GD_INCLUDE_DIR)

INCLUDE_DIRECTORIES(
  ${BOOST_INCLUDE_DIR}
)

FIND_PATH(FCGI_INCLUDE_DIR
    fcgio.h
  PATHS
    /usr/include
    /usr/local/include
    /usr/include/fastcgi
)

IF (FCGI_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(
     ${FCGI_INCLUDE_DIR}
     )
ENDIF (FCGI_INCLUDE_DIR)
    
SET(EXAMPLES_CONNECTOR wtfcgi CACHE STRING "Connector used for examples")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#
# Begin of build definitions
#
IF(WIN32)
  ADD_DEFINITIONS(-DRUNDIR="${RUNDIR}" -DWT_CONFIG_XML="${CONFIGURATION}")
ELSE(WIN32)
  ADD_DEFINITIONS(-DRUNDIR=\\"${RUNDIR}\\" -DWT_CONFIG_XML=\\"${CONFIGURATION}\\")
ENDIF(WIN32)

ADD_DEFINITIONS(-DWT_API_NARROW_CHAR)

SUBDIRS(src EXCLUDE_FROM_ALL examples)
