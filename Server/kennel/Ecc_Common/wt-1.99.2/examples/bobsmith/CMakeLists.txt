ADD_EXECUTABLE(bobsmith.fcg bobsmith.C)
TARGET_LINK_LIBRARIES(bobsmith.fcg ${EXAMPLES_CONNECTOR})

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
)

#
# Create a deploy script
#
SET(APPNAME bobsmith)
SET(APPBIN bobsmith.fcg)
SET(APPRESOURCES "")
CONFIGURE_FILE(
  ${WT_SOURCE_DIR}/deploy.sh
  ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
)
