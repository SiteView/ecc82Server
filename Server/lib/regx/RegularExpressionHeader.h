//---------------------------------------------------------------------------
#ifndef RegularExpressionHeaderH
#define RegularExpressionHeaderH
//---------------------------------------------------------------------------

#if defined(__BORLANDC__)
  #if defined(BUILD_REGULAR_EXPRESSION)
    #if defined(_RTLDLL)
      #define REGULAR_EXPRESSION_IMPORT_EXPORT __declspec(dllexport)
    #else
      #define REGULAR_EXPRESSION_IMPORT_EXPORT
    #endif
  #else 
    #if defined(_RTLDLL)
      #define REGULAR_EXPRESSION_IMPORT_EXPORT __declspec(dllimport)
      #if __BORLANDC__ >= 0x530 && __BORLANDC__ < 0x540
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionDllBcb3.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllSingleBcb3.lib")
        #endif
      #elif __BORLANDC__ >= 0x540 && __BORLANDC__ < 0x550
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionDllBcb4.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllSingleBcb4.lib")
        #endif
      #elif __BORLANDC__ >= 0x550 && __BORLANDC__ < 0x560
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionDllBcb5.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllSingleBcb5.lib")
        #endif
      #elif __BORLANDC__ >= 0x560 && __BORLANDC__ < 0x570
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionDllBcb6.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllSingleBcb6.lib")
        #endif
      #endif
    #else
      #define REGULAR_EXPRESSION_IMPORT_EXPORT
      #if __BORLANDC__ >= 0x530 && __BORLANDC__ < 0x540
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionLibBcb3.lib")
        #else
          #pragma comment(lib, "RegularExpressionLibSingleBcb3.lib")
        #endif
      #elif __BORLANDC__ >= 0x540 && __BORLANDC__ < 0x550
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionLibBcb4.lib")
        #else
          #pragma comment(lib, "RegularExpressionLibSingleBcb4.lib")
        #endif
      #elif __BORLANDC__ >= 0x550 && __BORLANDC__ < 0x560
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionLibBcb5.lib")
        #else
          #pragma comment(lib, "RegularExpressionLibSingleBcb5.lib")
        #endif
      #elif __BORLANDC__ >= 0x560 && __BORLANDC__ < 0x570
        #if defined(__MT__)
          #pragma comment(lib, "RegularExpressionLibBcb6.lib")
        #else
          #pragma comment(lib, "RegularExpressionLibSingleBcb6.lib")
        #endif
      #endif
    #endif
  #endif
#elif defined(_MSC_VER)
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
          #pragma comment(lib, "RegularExpressionDllVc6d.lib")
        #elif _MSC_VER < 1310
          #pragma comment(lib, "RegularExpressionDllVc7d.lib")
        #elif _MSC_VER < 1400
          #pragma comment(lib, "RegularExpressionDllVc71d.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllVc8d.lib")
        #endif
      #else
        #if _MSC_VER < 1300
          #pragma comment(lib, "RegularExpressionDllVc6.lib")
        #elif _MSC_VER < 1310
          #pragma comment(lib, "RegularExpressionDllVc7.lib")
        #elif _MSC_VER < 1400
          #pragma comment(lib, "RegularExpressionDllVc71.lib")
        #else
          #pragma comment(lib, "RegularExpressionDllVc8.lib")
        #endif
      #endif
    #else
      #define REGULAR_EXPRESSION_IMPORT_EXPORT
      #if defined(_DEBUG)
        #if defined(_MT)
          #if _MSC_VER < 1300
            #pragma comment(lib, "RegularExpressionLibVc6d.lib")
          #elif _MSC_VER < 1310
            #pragma comment(lib, "RegularExpressionLibVc7d.lib")
          #elif _MSC_VER < 1400
            #pragma comment(lib, "RegularExpressionLibVc71d.lib")
          #else
            #pragma comment(lib, "RegularExpressionLibVc8d.lib")
          #endif
        #else
          #if _MSC_VER < 1300
            #pragma comment(lib, "RegularExpressionLibSingleVc6d.lib")
          #elif _MSC_VER < 1310
            #pragma comment(lib, "RegularExpressionLibSingleVc7d.lib")
          #elif _MSC_VER < 1400
            #pragma comment(lib, "RegularExpressionLibSingleVc71d.lib")
          #endif
        #endif
      #else
        #if defined(_MT)
          #if _MSC_VER < 1300
            #pragma comment(lib, "RegularExpressionLibVc6.lib")
          #elif _MSC_VER < 1310
            #pragma comment(lib, "RegularExpressionLibVc7.lib")
          #elif _MSC_VER < 1400
            #pragma comment(lib, "RegularExpressionLibVc71.lib")
          #else
            #pragma comment(lib, "RegularExpressionLibVc8.lib")
          #endif
        #else
          #if _MSC_VER < 1300
            #pragma comment(lib, "RegularExpressionLibSingleVc6.lib")
          #elif _MSC_VER < 1310
            #pragma comment(lib, "RegularExpressionLibSingleVc7.lib")
          #elif _MSC_VER < 1400
            #pragma comment(lib, "RegularExpressionLibSingleVc71.lib")
          #endif
        #endif
      #endif
    #endif
  #endif
#endif

#endif