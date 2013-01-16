ADD_EXECUTABLE(demotreelist.fcg DemoTreeList.C IconPair.C TreeNode.C)
TARGET_LINK_LIBRARIES(demotreelist.fcg ${EXAMPLES_CONNECTOR})

INCLUDE_DIRECTORIES(
  ${WT_SOURCE_DIR}/src/web
  ${WT_SOURCE_DIR}/src/wt
  ${WT_SOURCE_DIR}/xlobject/src
)

#
# Create a deploy script
#
SET(APPNAME treelist)
SET(APPBIN demotreelist.fcg)
SET(APPRESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icons ${CMAKE_CURRENT_SOURCE_DIR}/*.C ${CMAKE_CURRENT_SOURCE_DIR}/*.h")
CONFIGURE_FILE(
  ${WT_SOURCE_DIR}/deploy.sh
  ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
)
