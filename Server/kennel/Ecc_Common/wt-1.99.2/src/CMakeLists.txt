SUBDIRS(web wt)

INCLUDE(FindThreads)

IF(WIN32)
  # When linking statically to dependency libraries, uncomment this line
  ADD_DEFINITIONS(-DXL_EXPORTS -DWITTY_DLL -DWITTY_DLL_EXPORTS -DXML_LIBRARY)
  # When linking to dll version of dependency libraries, uncomment this line
  #ADD_DEFINITIONS(-DXL_EXPORTS -DWITTY_DLL -DWITTY_DLL_EXPORTS -DBOOST_ALL_DYN_LINK)
  # For just static linking to boost, uncomment this
  #ADD_DEFINITIONS(-DXL_EXPORTS -DWITTY_DLL -DWITTY_DLL_EXPORTS)
ENDIF(WIN32)

FIND_LIBRARY(XERCES_CPP_LIB
NAMES
  xerces-c xerces-c_2 xerces-c_static_2
PATHS
  /usr/lib
  /usr/local/lib
  ${USERLIB_ROOT}/lib
  c:/libraries/lib
)

FIND_PATH(XERCES_CPP_INCLUDE xercesc/util/PlatformUtils.hpp
  /usr/include
  /usr/local/include
  ${USERLIB_ROOT}/include
  c:/tmp/witty/xerces-c-src_2_7_0/src
)

IF(NOT EXISTS ${XERCES_CPP_LIB} OR NOT EXISTS ${XERCES_CPP_INCLUDE})
  MESSAGE("** Error finding Wt required library: ")
  MESSAGE(FATAL_ERROR "** Wt requires the Xerces-C++ XML parser library.")
ENDIF(NOT EXISTS ${XERCES_CPP_LIB} OR NOT EXISTS ${XERCES_CPP_INCLUDE})

#IF(WIN32)
#  SET(BOOST_LIB_DIR "c:/boost/lib")
#ENDIF(WIN32)

IF(WIN32)
  SET(BOOST_REGEX_LIB "")
  SET(BOOST_REGEX_LIB_MT "")
  SET(BOOST_THREAD_LIB "")
  SET(BOOST_SIGNALS_LIB "")
  SET(BOOST_SIGNALS_LIB_MT "")
  SET(BOOST_PO_LIB "")
ELSE(WIN32)
  FIND_LIBRARY(BOOST_REGEX_LIB_MT
    NAMES
      boost_regex-${BOOST_SUFFIX}-mt boost_regex-mt
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_REGEX_LIB
    NAMES
      boost_regex-${BOOST_SUFFIX} boost_regex
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_PO_LIB_MT
    NAMES
      boost_program_options-${BOOST_SUFFIX}-mt boost_program_options-mt
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_PO_LIB
    NAMES
      boost_program_options-${BOOST_SUFFIX} boost_program_options
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_SIGNALS_LIB_MT
    NAMES
      boost_signals-${BOOST_SUFFIX}-mt boost_signals-mt
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_SIGNALS_LIB
    NAMES
      boost_signals-${BOOST_SUFFIX} boost_signals
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(BOOST_THREAD_LIB
    NAMES
      boost_thread-${BOOST_SUFFIX}-mt boost_thread-mt
    PATHS
      ${BOOST_LIB_DIR}
    NO_DEFAULT_PATH
  )
ENDIF(WIN32)

IF(NOT WIN32)
   IF(BOOST_THREAD_LIB
      AND BOOST_REGEX_LIB_MT
      AND BOOST_SIGNALS_LIB_MT
      AND BOOST_PO_LIB_MT
      AND BOOST_INCLUDE_DIR)

     ADD_DEFINITIONS(-DTHREADED -DBOOST_HAS_THREADS)
     SET(BOOST_LIBS ${BOOST_THREAD_LIB} ${BOOST_REGEX_LIB_MT}
                    ${BOOST_PO_LIB_MT} ${BOOST_SIGNALS_LIB_MT})

   ELSE(BOOST_THREAD_LIB
        AND BOOST_REGEX_LIB_MT
        AND BOOST_SIGNALS_LIB_MT
        AND BOOST_PO_LIB_MT
        AND BOOST_INCLUDE_DIR)

     IF(BOOST_REGEX_LIB
        AND BOOST_SIGNALS_LIB
        AND BOOST_PO_LIB
        AND BOOST_INCLUDE_DIR)

          MESSAGE("** Disabling multi threading.")

     SET(BOOST_LIBS ${BOOST_REGEX_LIB}
                    ${BOOST_PO_LIB}
		    ${BOOST_SIGNALS_LIB})

     ELSE(BOOST_REGEX_LIB
        AND BOOST_SIGNALS_LIB
        AND BOOST_PO_LIB
        AND BOOST_INCLUDE_DIR)

          MESSAGE("** Error finding Wt required library: ")
          MESSAGE(FATAL_ERROR "** Wt requires the C++ boost library")

     ENDIF(BOOST_REGEX_LIB
           AND BOOST_SIGNALS_LIB
           AND BOOST_PO_LIB
           AND BOOST_INCLUDE_DIR)

   ENDIF(BOOST_THREAD_LIB
      AND BOOST_REGEX_LIB_MT
      AND BOOST_SIGNALS_LIB_MT
      AND BOOST_PO_LIB_MT
      AND BOOST_INCLUDE_DIR)

ENDIF(NOT WIN32)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/web
  ${CMAKE_CURRENT_SOURCE_DIR}/wt
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XERCES_CPP_INCLUDE}
  ${BOOST_INCLUDE_DIR}
)

LINK_DIRECTORIES(
  ${BOOST_LIB_DIR}
)

ADD_EXECUTABLE(filetostring web/skeleton/FileToString.C)
GET_TARGET_PROPERTY(FILE_TO_STRING_EXE filetostring LOCATION)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Plain_html.C
	           COMMAND ${FILE_TO_STRING_EXE}
                   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Plain.html
                        ${CMAKE_CURRENT_BINARY_DIR}/Plain_html.C
                        Plain_html
		   MAIN_DEPENDENCY
		        ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Plain.html
                   DEPENDS
		        filetostring
                   COMMENT
		        "Making skeleton Plain_html")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Ajax_html.C
	           COMMAND ${FILE_TO_STRING_EXE}
                   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Ajax.html
                        ${CMAKE_CURRENT_BINARY_DIR}/Ajax_html.C
                        Ajax_html
		   MAIN_DEPENDENCY
                        ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Ajax.html
		   DEPENDS
		        filetostring
                   COMMENT
		        "Making skeleton Ajax")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/JsNoAjax_html.C
	           COMMAND ${FILE_TO_STRING_EXE}
                   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/JsNoAjax.html
                        ${CMAKE_CURRENT_BINARY_DIR}/JsNoAjax_html.C
                        JsNoAjax_html
		   MAIN_DEPENDENCY
		        ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/JsNoAjax.html
                   DEPENDS
		        filetostring
                   COMMENT
		        "Making skeleton JsNoAjax")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Boot_html.C
	           COMMAND ${FILE_TO_STRING_EXE}
                   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Boot.html
                        ${CMAKE_CURRENT_BINARY_DIR}/Boot_html.C
                        Boot_html
		   MAIN_DEPENDENCY
		        ${CMAKE_CURRENT_SOURCE_DIR}/web/skeleton/Boot.html
                   DEPENDS
		        filetostring
                   COMMENT
		        "Making skeleton Boot")

SET(libsources
wt/WSignal.C
wt/WAbstractToggleButton.C
wt/WAnchor.C
wt/WApplication.C
wt/WBorder.C
wt/WBreak.C
wt/WButtonGroup.C
wt/WCheckBox.C
wt/WColor.C
wt/WComboBox.C
wt/WCompositeWidget.C
wt/WContainerWidget.C
wt/WCssDecorationStyle.C
wt/WCssStyleSheet.C
wt/WDoubleValidator.C
wt/WEnvironment.C
wt/WEvent.C
wt/WFileResource.C
wt/WFileUpload.C
wt/WFont.C
wt/WFormWidget.C
wt/WGroupBox.C
wt/WIconPair.C
wt/WImage.C
wt/WInPlaceEdit.C
wt/WIntValidator.C
wt/WInteractWidget.C
wt/WJavascriptSlot.C
wt/WLabel.C
wt/WLength.C
wt/WLineEdit.C
wt/WMenu.C
wt/WMenuItem.C
wt/WMessage.C
wt/WMessageResourceBundle.C
wt/WMessageResources.C
wt/WObject.C
wt/WPushButton.C
wt/WRadioButton.C
wt/WRegExpValidator.C
wt/WResource.C
wt/WScrollArea.C
wt/WScrollBar.C
wt/WSelectionBox.C
wt/WStackedWidget.C
wt/WStatelessSlot.C
wt/WStringUtil.C
wt/WSuggestionPopup.C
wt/WTable.C
wt/WTableCell.C
wt/WText.C
wt/WTextArea.C
wt/WTimer.C
wt/WTimerWidget.C
wt/WTreeNode.C
wt/WTreeTable.C
wt/WTreeTableNode.C
wt/WValidationStatus.C
wt/WValidator.C
wt/WVirtualImage.C
wt/WWebWidget.C
wt/WWidget.C
web/CgiParser.C
web/Configuration.C
web/FileServe.C
web/WebRequest.C
web/WebStream.C
web/WebSession.C
web/WebRenderer.C
web/WebController.C
web/WtException.C
web/WtRandom.C
web/DomElement.C
web/EscapeOStream.C
web/TimeUtil.C
Plain_html.C
Ajax_html.C
JsNoAjax_html.C
Boot_html.C
)

ADD_LIBRARY(wt ${libsources})

TARGET_LINK_LIBRARIES(wt ${XERCES_CPP_LIB} ${BOOST_LIBS})

INSTALL_TARGETS(/lib wt)

CONFIGURE_FILE(
  ${WT_SOURCE_DIR}/WtInstall.cmake
  ${WT_BINARY_DIR}/WtInstall.cmake
)
SET_TARGET_PROPERTIES(wt PROPERTIES
  POST_INSTALL_SCRIPT ${WT_BINARY_DIR}/WtInstall.cmake
)

IF(WIN32)
  #SET_TARGET_PROPERTIES(wt PROPERTIES LINK_FLAGS "/force:unresolved")
  TARGET_LINK_LIBRARIES(wt winmm wsock32)
ENDIF(WIN32)

IF(CONNECTOR_FCGI)
  FIND_LIBRARY(FCGI_LIB fcgi
    /usr/lib
    /usr/local/lib
    ${USERLIB_ROOT}/lib
  )
  IF(NOT EXISTS ${FCGI_LIB})
    MESSAGE("** Error finding Wt required library: ")
    MESSAGE(FATAL_ERROR "** Wt FCGI connector requires libfcgi, the FastCGI client library")
  ENDIF(NOT EXISTS ${FCGI_LIB})

  FIND_LIBRARY(FCGIPP_LIB fcgi++
    /usr/lib
    /usr/local/lib
    ${USERLIB_ROOT}/lib
  )
  IF(NOT EXISTS ${FCGIPP_LIB})
    MESSAGE("** Error finding Wt required library: ")
    MESSAGE(FATAL_ERROR "** Wt FCGI connector requires libfcgi++, the FastCGI C++ client library")
  ENDIF(NOT EXISTS ${FCGIPP_LIB})

  SET(libfcgisources
    fcgi/FCGIRecord.C
    fcgi/FCGIStream.C
    fcgi/Server.C
    fcgi/SessionInfo.C
  )

  ADD_LIBRARY(wtfcgi ${libfcgisources})
  TARGET_LINK_LIBRARIES(wtfcgi wt ${FCGI_LIB} ${FCGIPP_LIB})

  INSTALL_TARGETS(/lib wtfcgi)
ENDIF(CONNECTOR_FCGI)

IF(CONNECTOR_HTTP)
  SUBDIRS(http) 
ENDIF(CONNECTOR_HTTP)

