#
# The hangman example needs the additional mysql++ library,
# therefore we check first that it exists before trying to build
# the example.
#

FIND_LIBRARY(MYSQLPP_LIB mysqlpp
  /usr/lib
  /usr/local/lib
  c:/mysql++/static/release
)

FIND_PATH(MYSQLPP_INCLUDE mysql++/lockable.h
  /usr/include
  /usr/local/include
  c:/mysql++/include
)

FIND_PATH(MYSQL_INCLUDE mysql.h
  /usr/include
  /usr/local/include
  /usr/include/mysql
  /usr/local/include/mysql
  c:/mysql/include
)

IF(NOT EXISTS ${MYSQLPP_INCLUDE} OR NOT EXISTS ${MYSQLPP_LIB})
  MESSAGE(STATUS "** hangman example needs mysql++-2.x library... Skipping.")
ELSE(NOT EXISTS ${MYSQLPP_INCLUDE} OR NOT EXISTS ${MYSQLPP_LIB})

  ADD_EXECUTABLE(hangman.fcg hangman.C HangmanGame.C HangmanWidget.C HighScoresWidget.C LoginWidget.C Dictionary.C HangmanDb.C)
  TARGET_LINK_LIBRARIES(hangman.fcg ${EXAMPLES_CONNECTOR} ${MYSQLPP_LIB})

  INCLUDE_DIRECTORIES(
    ${WT_SOURCE_DIR}/src/web
    ${WT_SOURCE_DIR}/src/wt
    ${WT_SOURCE_DIR}/xlobject/src
    ${MYSQLPP_INCLUDE}
    ${MYSQL_INCLUDE}
  )

  # Static linkage of mysqlpp, hence we don't want __declspecs
  ADD_DEFINITIONS(-DMYSQLPP_NO_DLL)

  #
  # Create a deploy script
  #
  SET(APPNAME hangman)
  SET(APPBIN hangman.fcg)
  SET(APPRESOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/icons ${CMAKE_CURRENT_SOURCE_DIR}/dict*.txt")
  CONFIGURE_FILE(
    ${WT_SOURCE_DIR}/deploy.sh
    ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh
  )
  
ENDIF(NOT EXISTS ${MYSQLPP_INCLUDE} OR NOT EXISTS ${MYSQLPP_LIB})

