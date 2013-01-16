#
# The form example needs the additional boost_date_time library,
# therefore we check first that it exists before trying to build
# the example.
#

IF(WIN32)
  SET(BOOST_DT_LIB_MT "")
ELSE(WIN32)
  FIND_LIBRARY(BOOST_DT_LIB_MT
    NAMES
      boost_date_time-${BOOST_SUFFIX}-mt boost_date_time-mt boost_date_time
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
ENDIF(WIN32)

IF(NOT WIN32 AND NOT EXISTS ${BOOST_DT_LIB_MT})
  MESSAGE(STATUS "** Form example needs boost_date_time library... Skipping.")
ELSE(NOT WIN32 AND NOT EXISTS ${BOOST_DT_LIB_MT})

  ADD_EXECUTABLE(formexample.fcg DateValidator.C Form.C FormExample.C)
  TARGET_LINK_LIBRARIES(formexample.fcg ${EXAMPLES_CONNECTOR} ${BOOST_DT_LIB_MT})

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/web
    ${WT_SOURCE_DIR}/src/wt
    ${WT_SOURCE_DIR}/xlobject/src
  )

  #
  # Create a deploy script
  #
  SET(APPNAME form)
  SET(APPBIN formexample.fcg)
  SET(APPRESOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/icons ${CMAKE_CURRENT_SOURCE_DIR}/*.xml")
  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )

ENDIF(NOT WIN32 AND NOT EXISTS ${BOOST_DT_LIB_MT})

