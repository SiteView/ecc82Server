IF(WIN32)
  SET(BOOST_FS_LIB_MT "")
ELSE(WIN32)
  FIND_LIBRARY(BOOST_FS_LIB_MT
    NAMES
      boost_filesystem-${BOOST_SUFFIX}-mt boost_filesystem-mt boost_filesystem
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
ENDIF(WIN32)

IF(NOT WIN32 AND NOT EXISTS ${BOOST_FS_LIB_MT})
  MESSAGE(STATUS "** Form example needs boost_filesystem library... Skipping.")
ELSE(NOT WIN32 AND NOT EXISTS ${BOOST_FS_LIB_MT})

  ADD_EXECUTABLE(filetreetable.fcg
    FileTreeTable.C
    FileTreeTableNode.C
    FileTreeExample.C
  )

  TARGET_LINK_LIBRARIES(filetreetable.fcg
    ${EXAMPLES_CONNECTOR}
    ${BOOST_FS_LIB_MT}
  )

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/wt
  )

  #
  # Create a deploy script
  #
  SET(APPNAME filetreetable)
  SET(APPBIN filetreetable.fcg)
  SET(APPRESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.css")
  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )

ENDIF(NOT WIN32 AND NOT EXISTS ${BOOST_FS_LIB_MT})
