FIND_LIBRARY(GD_LIB
  NAMES
    gd bgd
  PATHS
    ${GD_DIR}
)

IF(NOT EXISTS ${GD_LIB})
  MESSAGE(STATUS "** Mandelbrot example needs gd library... Skipping.")
ELSE(NOT EXISTS ${GD_LIB})

  ADD_EXECUTABLE(mandelbrot.fcg MandelbrotImage.C MandelbrotExample.C)
  TARGET_LINK_LIBRARIES(mandelbrot.fcg ${EXAMPLES_CONNECTOR} ${GD_LIB})

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/web
    ${WT_SOURCE_DIR}/src/wt
    ${WT_SOURCE_DIR}/xlobject/src
  )

  #
  # Create a deploy script
  #
  SET(APPNAME mandelbrot)
  SET(APPBIN mandelbrot.fcg)
  SET(APPRESOURCES "")

  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )

ENDIF(NOT EXISTS ${GD_LIB})
