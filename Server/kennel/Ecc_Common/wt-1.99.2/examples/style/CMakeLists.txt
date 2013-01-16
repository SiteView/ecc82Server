#
# The style example needs the additional gd library,
# therefore we check first that it exists before trying to build
# the example.
#

FIND_LIBRARY(GD_LIB
  NAMES
    gd bgd
  PATHS
    ${GD_DIR}
)

IF(NOT EXISTS ${GD_LIB})
  MESSAGE(STATUS "** Style example needs gd library... Skipping.")
ELSE(NOT EXISTS ${GD_LIB})

  ADD_EXECUTABLE(styleexample.fcg CornerImage.C RoundedWidget.C StyleExample.C)
  TARGET_LINK_LIBRARIES(styleexample.fcg ${EXAMPLES_CONNECTOR} ${GD_LIB})

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/web
    ${WT_SOURCE_DIR}/src/wt
    ${WT_SOURCE_DIR}/xlobject/src
  )
  #
  # Create a deploy script
  #
  SET(APPNAME style)
  SET(APPBIN styleexample.fcg)
  SET(APPRESOURCES "")
  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )

ENDIF(NOT EXISTS ${GD_LIB})

