FIND_LIBRARY(GD_LIB
  NAMES
    gd bgd
  PATHS
    $(GD_DIR)
)

IF(NOT EXISTS ${GD_LIB})
  MESSAGE(STATUS "** wt-homepage example needs gd library... Skipping.")
ELSE(NOT EXISTS ${GD_LIB})

  ADD_EXECUTABLE(Home.fcg Home.C TreeListExample.C ../style/CornerImage.C ../style/RoundedWidget.C ../treelist/TreeNode.C ../treelist/IconPair.C)
  TARGET_LINK_LIBRARIES(Home.fcg ${EXAMPLES_CONNECTOR} ${GD_LIB})

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/web
    ${WT_SOURCE_DIR}/src/wt
    ${WT_SOURCE_DIR}/xlobject/src
    ${WT_SOURCE_DIR}/examples/style
    ${WT_SOURCE_DIR}/examples/treelist
  )

  #
  # Create a deploy script
  #
  SET(APPNAME wt-homepage)
  SET(APPBIN Home.fcg)
  SET(APPRESOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/icons ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_SOURCE_DIR}/*.xml ${CMAKE_CURRENT_SOURCE_DIR}/*.css")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.C ${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../style/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../style/*.C")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../treelist/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../treelist/*.C")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../form/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../form/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../form/*.xml")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../hello/*.C")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../hangman/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../hangman/*.C")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../composer/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../composer/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../composer/*.xml ${CMAKE_CURRENT_SOURCE_DIR}/../composer/*.css")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../dragdrop/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../dragdrop/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../dragdrop/*.css")
  SET(APPRESOURCES
      "${APPRESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../filetreetable/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../filetreetable/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../filetreetable/*.css")

  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )

ENDIF(NOT EXISTS ${GD_LIB})

