ADD_EXECUTABLE(composer.fcg 
  AddresseeEdit.C
  AttachmentEdit.C
  ComposeExample.C
  Composer.C
  ContactSuggestions.C
  Label.C
  Option.C
  OptionList.C
)
TARGET_LINK_LIBRARIES(composer.fcg ${EXAMPLES_CONNECTOR})

#
# If you have Wt installed somehwere, you should use the
# installed Wt header files for your own Wt projects.
# e.g. INCLUDE_DIRECTORIES(/usr/local/wt/include)
# instead of the following:
#
INCLUDE_DIRECTORIES(
  ${WT_SOURCE_DIR}/src/wt
  ${WT_SOURCE_DIR}/examples/treelist
)

#
# Create a deploy script
#
SET(APPNAME composer)
SET(APPBIN composer.fcg)
SET(APPRESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/composer.xml ${CMAKE_CURRENT_SOURCE_DIR}/composer.css ${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png ${CMAKE_CURRENT_SOURCE_DIR}/../treelist/icons")

CONFIGURE_FILE(
  ${WT_SOURCE_DIR}/deploy.sh
  ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
)
