ADD_EXECUTABLE(dragdrop.fcg 
  Character.C
  DragExample.C
)
TARGET_LINK_LIBRARIES(dragdrop.fcg ${EXAMPLES_CONNECTOR})

#
# If you have Wt installed somehwere, you should use the
# installed Wt header files for your own Wt projects.
# e.g. INCLUDE_DIRECTORIES(/usr/local/wt/include)
# instead of the following:
#
INCLUDE_DIRECTORIES(
  ${WT_SOURCE_DIR}/src/web
  ${WT_SOURCE_DIR}/src/wt
  ${WT_SOURCE_DIR}/xlobject/src
  ${WT_SOURCE_DIR}/examples/treelist
)

#
# Create a deploy script
#
SET(APPNAME dragdrop)
SET(APPBIN dragdrop.fcg)
SET(APPRESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dragdrop.css ${CMAKE_CURRENT_SOURCE_DIR}/icons/*.jpg ${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png")

CONFIGURE_FILE(
  ${WT_SOURCE_DIR}/deploy.sh
  ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
)
