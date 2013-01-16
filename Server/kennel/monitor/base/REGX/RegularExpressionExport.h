#if !defined(REGULAR_EXPRESSION_EXPORT_H)
#define REGULAR_EXPRESSION_EXPORT_H

#if defined(BUILD_REGULAR_EXPRESSION)
  #if defined(_DLL)
    #define REGULAR_EXPRESSION_IMPORT_EXPORT __declspec(dllexport)
  #else
    #define REGULAR_EXPRESSION_IMPORT_EXPORT
  #endif
#else 
  #if defined(_DLL)
    #define REGULAR_EXPRESSION_IMPORT_EXPORT __declspec(dllimport)
    #if defined(_DEBUG)
      #if _MSC_VER < 1300
        #pragma comment(lib, "RegularExpressionComponentDllVc6d.lib")
      #elif _MSC_VER < 1310
        #pragma comment(lib, "RegularExpressionComponentDllVc7d.lib")
      #elif _MSC_VER < 1400
        #pragma comment(lib, "RegularExpressionComponentDllVc71d.lib")
      #else
        #pragma comment(lib, "RegularExpressionComponentDllVc8d.lib")
      #endif
    #else
      #if _MSC_VER < 1300
        #pragma comment(lib, "RegularExpressionComponentDllVc6.lib")
      #elif _MSC_VER < 1310
        #pragma comment(lib, "RegularExpressionComponentDllVc7.lib")
      #elif _MSC_VER < 1400
        #pragma comment(lib, "RegularExpressionComponentDllVc71.lib")
      #else
        #pragma comment(lib, "RegularExpressionComponentDllVc8.lib")
      #endif
    #endif
  #else
    #define REGULAR_EXPRESSION_IMPORT_EXPORT
    #if defined(_DEBUG)
      #if _MSC_VER < 1300
        #pragma comment(lib, "RegularExpressionComponentLibVc6d.lib")
      #elif _MSC_VER < 1310
        #pragma comment(lib, "RegularExpressionComponentLibVc7d.lib")
      #elif _MSC_VER < 1400
        #pragma comment(lib, "RegularExpressionComponentLibVc71d.lib")
      #else
        #pragma comment(lib, "RegularExpressionComponentLibVc8d.lib")
      #endif
    #else
      #if _MSC_VER < 1300
        #pragma comment(lib, "RegularExpressionComponentLibVc6.lib")
      #elif _MSC_VER < 1310
        #pragma comment(lib, "RegularExpressionComponentLibVc7.lib")
      #elif _MSC_VER < 1400
        #pragma comment(lib, "RegularExpressionComponentLibVc71.lib")
      #else
        #pragma comment(lib, "RegularExpressionComponentLibVc8.lib")
      #endif
    #endif
  #endif
#endif

#endif
