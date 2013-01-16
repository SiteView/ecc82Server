//---------------------------------------------------------------------------
#ifndef RegularExpressionClassH
#define RegularExpressionClassH
//---------------------------------------------------------------------------
#include <windows.h>
#include <locale>
#include <vector>
#include <string>
#include <iostream>
#include "RegularExpressionHeader.h"

namespace RegexClass
{

#pragma pack(push,8)

struct REMatch { std::string string; unsigned position; };
typedef std::vector<REMatch> REMatchInfo;

class REGULAR_EXPRESSION_IMPORT_EXPORT RegularExpression
  {
  
  public:

  typedef void (CALLBACK *TypeResults)(RegexClass::RegularExpression * Sender,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine);
  typedef void (CALLBACK *TypeGrepResults)(RegexClass::RegularExpression * Sender,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine,bool & bcontinue);
  typedef void (CALLBACK *TypeMergeResults)(RegexClass::RegularExpression * Sender,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine,bool & bcontinue);
#if _MSC_VER > 1310
  typedef void (CALLBACK *TypeGrepFileResults)(RegexClass::RegularExpression * Sender,const std::string & file,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
  typedef void (CALLBACK *TypeFindFileResults)(RegexClass::RegularExpression * Sender,const std::string & file,bool & bcontinue);
  typedef void (CALLBACK *TypeChangeFileResults)(RegexClass::RegularExpression * Sender,const std::string & file,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
#else
  typedef void (CALLBACK *TypeGrepFileResults)(RegexClass::RegularExpression * Sender,std::string file,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
  typedef void (CALLBACK *TypeFindFileResults)(RegexClass::RegularExpression * Sender,std::string file,bool & bcontinue);
  typedef void (CALLBACK *TypeChangeFileResults)(RegexClass::RegularExpression * Sender,std::string file,const RegexClass::REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
#endif
  
  __event void OnMatching(RegexClass::RegularExpression *,const RegexClass::REMatchInfo &,unsigned,unsigned);
  __event void OnSearching(RegexClass::RegularExpression *,const RegexClass::REMatchInfo &,unsigned,unsigned);
  __event void OnGrepping(RegexClass::RegularExpression *,const RegexClass::REMatchInfo &,unsigned,unsigned,bool &);
  __event void OnMerging(RegexClass::RegularExpression *,const RegexClass::REMatchInfo &,unsigned,unsigned,bool &);
#if _MSC_VER > 1310
  __event void OnChangingFiles(RegexClass::RegularExpression *,const std::string &,const RegexClass::REMatchInfo &,unsigned,bool &);
  __event void OnGreppingFiles(RegexClass::RegularExpression *,const std::string &,const RegexClass::REMatchInfo &,unsigned,bool &);
  __event void OnFindFiles(RegexClass::RegularExpression *,const std::string &,bool &);
#else
  __event void OnChangingFiles(RegexClass::RegularExpression *,std::string,const RegexClass::REMatchInfo &,unsigned,bool &);
  __event void OnGreppingFiles(RegexClass::RegularExpression *,std::string,const RegexClass::REMatchInfo &,unsigned,bool &);
  __event void OnFindFiles(RegexClass::RegularExpression *,std::string,bool &);
#endif
  
  enum Options
    {
    EscapeInCharacterSets,
    CharacterClasses,
    Intervals,
    LimitedOperations,
    NewlineAsAlternationOperator,
    QuestionmarkPlusNotOperators,
    BracesNotOperators,
    ParenthesesNotOperators,
    BackReferences,
    VerticalbarNotOperator,
    Literals,
    CaseInsensitive,
    NoLocaleSpecificCollation,
    Basic,
    Extended,
    Normal,
    Emacs,
    Awk,
    UGrep,
    Egrep,
    Sed,
    Perl
    };
    
  enum Matches
    {
    MatchDefault,
    MatchNotBeginningOfLine,
    MatchNotEndOfLine,
    MatchNotBeginningOfBuffer,
    MatchNotEndOfBuffer,
    MatchNotBeginningOfWord,
    MatchNotEndOfWord,
    MatchNotDotNewline,
    MatchNotDotNullCharacter,
    MatchShortest,
    MatchNotNullString,
    MatchContinuous,
    MatchPartial
    };
    
  enum ExpressionErrorCode
  {
    REG_NOERROR = 0,	/* Success.  */
    REG_NOEXPRESSION = 1,	/* No expression specified */

    /* POSIX regcomp return error codes.  (In the order listed in the
       standard.)  */
       
    REG_BADPAT = 2,		  /* Invalid pattern.  */
    REG_ECOLLATE = 3,	  /* Undefined collating element.  */
    REG_ECTYPE = 4,		  /* Invalid character class name.  */
    REG_EESCAPE = 5,	  /* Trailing backslash.  */
    REG_ESUBREG = 6,	  /* Invalid back reference.  */
    REG_EBRACK = 7,		  /* Unmatched left bracket.  */
    REG_EPAREN = 8,		  /* Parenthesis imbalance.  */
    REG_EBRACE = 9,		  /* Unmatched \{.  */
    REG_BADBR = 10,		  /* Invalid contents of \{\}.  */
    REG_ERANGE = 11,	  /* Invalid range end.  */
    REG_ESPACE = 12,	  /* Ran out of memory.  */
    REG_BADRPT = 13,	  /* No preceding re for repetition op.  */
    REG_EEND = 14,      /* unexpected end of expression */
    REG_ESIZE = 15,     /* expression too big */
    REG_ERPAREN = 16,   /* unmatched right parenthesis */
    REG_EMPTY = 17,     /* empty expression */
    REG_E_MEMORY = 18,  /* out of memory */
    REG_E_UNKNOWN = 19, /* unknown error */
    
    // Other errors
    
    REG_E_DLL = 20      /* Locale DLL could not be opened */
  };

  enum FileChangeError
    {
    FileChangeOpenOriginal,
    FileChangeCreateTemporary,
    FileChangeReadOriginal,
    FileChangeWriteTemporary,
    FileChangeDataMismatch,
    FileChangeCopyToBackup,
    FileChangeCopyToNew,
    FileChangeSetNewAttributes
    };

  enum FileSearchError
    {
    FileSearchOpen,
    FileSearchMap,
    FileSearchView
    };

  enum FormatTypes
    {
    FormatTypeAll,
    FormatTypeSed,
    FormatTypePerl
    };
    
  enum LocaleModels
    {
    LocaleModelCpp,
    LocaleModelWin32,
    LocaleModelC
    };

  struct FileChangeProblem
    {
    
    FileChangeProblem();
    ~FileChangeProblem();
    
    std::string file;
    FileChangeError error;
    unsigned position;
    std::string data;
    DWORD WinLastError;
    };

  struct FileSearchProblem
    {
    
    FileSearchProblem();
    ~FileSearchProblem();
    
    std::string file;
    FileSearchError error;
    DWORD WinLastError;
    };

  RegularExpression();
  virtual ~RegularExpression();
  RegularExpression(const RegularExpression &);
  bool operator == (const RegularExpression &) const;
  RegularExpression & operator = (const RegularExpression &);
  
  void SetContinueFiles(bool);
  void SetDirectoryRecursion(bool);
  void SetExpression(const std::string &);
  void SetFilesToMatch(const std::string &);
  void SetFormatString(const std::string &);
  void SetFormatType(FormatTypes);
  void SetLocale(const std::locale &);
  void SetLocaleDll(const std::string &);
  void SetLocaleModel(LocaleModels);
  void SetMaximumSplits(unsigned long);
  void SetMergeCopy(bool);
  void SetMergeOnlyFirst(bool);
  void SetSaveExtension(const std::string &);
  void SetSplitInputDelete(bool);
  void SetStringToMatch(const std::string &);
  
  bool          GetContinueFiles() const;
  bool          GetDirectoryRecursion() const;
  std::string   GetExpression() const;
  std::string   GetFilesToMatch() const;
  std::string   GetFormatString() const;
  FormatTypes   GetFormatType() const;
  std::locale   GetLocale() const;
  std::string   GetLocaleDll() const;
  LocaleModels  GetLocaleModel() const;
  unsigned long GetMaximumSplits() const;
  bool          GetMergeCopy() const;
  bool          GetMergeOnlyFirst() const;
  std::string   GetSaveExtension() const;
  bool          GetSplitInputDelete() const;
  std::string   GetStringToMatch() const;
  
  void SetOnChangingFiles(TypeChangeFileResults);
  void SetOnFindFiles(TypeFindFileResults);
  void SetOnGrepping(TypeGrepResults);
  void SetOnGreppingFiles(TypeGrepFileResults);
  void SetOnMatching(TypeResults);
  void SetOnMerging(TypeMergeResults);
  void SetOnSearching(TypeResults);
  
  TypeChangeFileResults GetOnChangingFiles() const;
  TypeFindFileResults   GetOnFindFiles() const;
  TypeGrepResults       GetOnGrepping() const;
  TypeGrepFileResults   GetOnGreppingFiles() const;
  TypeResults           GetOnMatching() const;
  TypeMergeResults      GetOnMerging() const;
  TypeResults           GetOnSearching() const;
  
  void AddExpressionOption(Options);
  void DeleteExpressionOption(Options);
  bool IsExpressionOption(Options) const;
  void ClearExpressionOptions();
  
  void AddMatchOption(Matches);
  void DeleteMatchOption(Matches);
  bool IsMatchOption(Matches) const;
  void ClearMatchOptions();
  
  ExpressionErrorCode ParseExpression();
  std::string GetExpressionError() const;
  const std::vector<FileChangeProblem> * GetChangeFilesErrors() const;
  const std::vector<FileSearchProblem> * GetFindFilesErrors() const;
  const std::vector<FileSearchProblem> * GetGrepFilesErrors() const;
  unsigned NumberOfSubexpressions();
  unsigned NumberOfSubexpressions() const;
  
  bool MatchAll();
  bool Match();
  bool Search();
  unsigned Grep();
  unsigned GrepFiles();
  unsigned FindFiles();
  unsigned ChangeFiles();
  void Format(std::string &) const;
  void Merge(std::string &);
  unsigned Split(std::vector<std::string> &);
  
  __declspec(property(get=GetContinueFiles,put=SetContinueFiles)) bool ContinueFiles;
  __declspec(property(get=GetDirectoryRecursion,put=SetDirectoryRecursion)) bool DirectoryRecursion;
  __declspec(property(get=GetExpression,put=SetExpression)) std::string Expression;
  __declspec(property(get=GetFilesToMatch,put=SetFilesToMatch)) std::string FilesToMatch;
  __declspec(property(get=GetFormatString,put=SetFormatString)) std::string FormatString;
  __declspec(property(get=GetFormatType,put=SetFormatType)) FormatTypes FormatType;
  __declspec(property(get=GetLocale,put=SetLocale)) std::locale Locale;
  __declspec(property(get=GetLocaleDll,put=SetLocaleDll)) std::string LocaleDll;
  __declspec(property(get=GetLocaleModel,put=SetLocaleModel)) LocaleModels LocaleModel;
  __declspec(property(get=GetMaximumSplits,put=SetMaximumSplits)) unsigned long MaximumSplits;
  __declspec(property(get=GetMergeCopy,put=SetMergeCopy)) bool MergeCopy;
  __declspec(property(get=GetMergeOnlyFirst,put=SetMergeOnlyFirst)) bool MergeOnlyFirst;
  __declspec(property(get=GetSaveExtension,put=SetSaveExtension)) std::string SaveExtension;
  __declspec(property(get=GetSplitInputDelete,put=SetSplitInputDelete)) bool SplitInputDelete;
  __declspec(property(get=GetStringToMatch,put=SetStringToMatch)) std::string StringToMatch;
  
  __declspec(property(get=GetOnChangingFiles,put=SetOnChangingFiles)) TypeChangeFileResults OnChangingFilesGlobal;
  __declspec(property(get=GetOnFindFiles,put=SetOnFindFiles)) TypeFindFileResults OnFindFilesGlobal;
  __declspec(property(get=GetOnGrepping,put=SetOnGrepping)) TypeGrepResults OnGreppingGlobal;
  __declspec(property(get=GetOnGreppingFiles,put=SetOnGreppingFiles)) TypeGrepFileResults OnGreppingFilesGlobal;
  __declspec(property(get=GetOnMatching,put=SetOnMatching)) TypeResults OnMatchingGlobal;
  __declspec(property(get=GetOnMerging,put=SetOnMerging)) TypeMergeResults OnMergingGlobal;
  __declspec(property(get=GetOnSearching,put=SetOnSearching)) TypeResults OnSearchingGlobal;
  
  REGULAR_EXPRESSION_IMPORT_EXPORT friend std::istream & __cdecl operator >> (std::istream &,RegularExpression &);
  REGULAR_EXPRESSION_IMPORT_EXPORT friend std::ostream & __cdecl operator << (std::ostream &,RegularExpression &);
  
private:

  friend class RegularExpressionPimpl;
  
  RegularExpressionPimpl * pimpl;

  };

#pragma pack(pop)

} /* namespace RegexClass */

#if !defined(NO_IMPLICIT_REGEXCLASS_NAMESPACE_USE)
using namespace RegexClass;
#endif

#endif
