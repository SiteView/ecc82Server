// RegularExpression.h: interface for the RegularExpression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGULAREXPRESSION_H__D4133B40_BB9D_11D3_BD0C_000000000000__INCLUDED_)
#define AFX_REGULAREXPRESSION_H__D4133B40_BB9D_11D3_BD0C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include <locale>
#include <vector>
#include "RegularExpressionExport.h"

#pragma pack(push,8)

struct REMatch { CString string; unsigned position; };
typedef std::vector<REMatch> REMatchInfo;

class REGULAR_EXPRESSION_IMPORT_EXPORT RegularExpression : public CObject
{
public:

  typedef void (CALLBACK *TypeResults)(RegularExpression * cre,const REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine);
  typedef void (CALLBACK *TypeGrepResults)(RegularExpression * cre,const REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine,bool & bcontinue);
  typedef void (CALLBACK *TypeMergeResults)(RegularExpression* cre,const REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine,bool & bcontinue);
#if _MSC_VER > 1310
  typedef void (CALLBACK *TypeGrepFileResults)(RegularExpression * cre,const CString & file,const REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
  typedef void (CALLBACK *TypeFindFileResults)(RegularExpression * cre,const CString & file,bool & bcontinue);
  typedef void (CALLBACK *TypeChangeFileResults)(RegularExpression* cre,const CString & file,const REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
#else
  typedef void (CALLBACK *TypeGrepFileResults)(RegularExpression * cre,CString file,const REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
  typedef void (CALLBACK *TypeFindFileResults)(RegularExpression * cre,CString file,bool & bcontinue);
  typedef void (CALLBACK *TypeChangeFileResults)(RegularExpression* cre,CString file,const REMatchInfo & Matches,unsigned LineIndex,bool & bcontinue);
#endif
  
  __event void OnMatching(RegularExpression *,const REMatchInfo &,unsigned,unsigned);
  __event void OnSearching(RegularExpression *,const REMatchInfo &,unsigned,unsigned);
  __event void OnGrepping(RegularExpression *,const REMatchInfo &,unsigned,unsigned,bool &);
  __event void OnMerging(RegularExpression *,const REMatchInfo &,unsigned,unsigned,bool &);
#if _MSC_VER > 1310
  __event void OnChangingFiles(RegularExpression *,const CString &,const REMatchInfo &,unsigned,bool &);
  __event void OnGreppingFiles(RegularExpression *,const CString &,const REMatchInfo &,unsigned,bool &);
  __event void OnFindFiles(RegularExpression *,const CString &,bool &);
#else
  __event void OnChangingFiles(RegularExpression *,CString,const REMatchInfo &,unsigned,bool &);
  __event void OnGreppingFiles(RegularExpression *,CString,const REMatchInfo &,unsigned,bool &);
  __event void OnFindFiles(RegularExpression *,CString,bool &);
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
    
    CString file;
    FileChangeError error;
    unsigned position;
    CString data;
    DWORD WinLastError;
    };

  struct FileSearchProblem
    {
    
    FileSearchProblem();
    ~FileSearchProblem();
    
    CString file;
    FileSearchError error;
    DWORD WinLastError;
    };

  RegularExpression();
  RegularExpression(const RegularExpression &);
  virtual ~RegularExpression();
  bool operator == (const RegularExpression &) const;
  RegularExpression & operator = (const RegularExpression &);

// Overridables
	virtual void Serialize(CArchive &);

#if defined(_DEBUG)
	// Diagnostic Support
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &) const;
#endif

  void SetContinueFiles(bool);
  void SetDirectoryRecursion(bool);
  void SetExpression(const CString &);
  void SetFilesToMatch(const CString &);
  void SetFormatString(const CString &);
  void SetFormatType(FormatTypes);
  void SetLocale(const std::locale &);
  void SetLocaleDll(const CString &);
  void SetLocaleModel(LocaleModels);
  void SetMaximumSplits(unsigned long);
  void SetMergeCopy(bool);
  void SetMergeOnlyFirst(bool);
  void SetSaveExtension(const CString &);
  void SetSplitInputDelete(bool);
  void SetStringToMatch(const CString &);
  
  bool          GetContinueFiles() const;
  bool          GetDirectoryRecursion() const;
  CString       GetExpression() const;
  CString       GetFilesToMatch() const;
  CString       GetFormatString() const;
  FormatTypes   GetFormatType() const;
  std::locale   GetLocale() const;
  CString       GetLocaleDll() const;
  LocaleModels  GetLocaleModel() const;
  unsigned long GetMaximumSplits() const;
  bool          GetMergeCopy() const;
  bool          GetMergeOnlyFirst() const;
  CString       GetSaveExtension() const;
  bool          GetSplitInputDelete() const;
  CString       GetStringToMatch() const;
  
  void SetOnChangingFiles(TypeChangeFileResults);
  void SetOnFindingFiles(TypeFindFileResults);
  void SetOnGrepping(TypeGrepResults);
  void SetOnGreppingFiles(TypeGrepFileResults);
  void SetOnMatching(TypeResults);
  void SetOnMerging(TypeMergeResults);
  void SetOnSearching(TypeResults);
  
  TypeChangeFileResults GetOnChangingFiles() const;
  TypeFindFileResults   GetOnFindingFiles() const;
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
  CString GetExpressionError() const;
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
  void Format(CString &) const;
  void Merge(CString &);
  unsigned Split(CStringArray &);
  
  __declspec(property(get=GetContinueFiles,put=SetContinueFiles)) bool ContinueFiles;
  __declspec(property(get=GetDirectoryRecursion,put=SetDirectoryRecursion)) bool DirectoryRecursion;
  __declspec(property(get=GetExpression,put=SetExpression)) CString Expression;
  __declspec(property(get=GetFilesToMatch,put=SetFilesToMatch)) CString FilesToMatch;
  __declspec(property(get=GetFormatString,put=SetFormatString)) CString FormatString;
  __declspec(property(get=GetFormatType,put=SetFormatType)) FormatTypes FormatType;
  __declspec(property(get=GetLocale,put=SetLocale)) std::locale Locale;
  __declspec(property(get=GetLocaleDll,put=SetLocaleDll)) CString LocaleDll;
  __declspec(property(get=GetLocaleModel,put=SetLocaleModel)) LocaleModels LocaleModel;
  __declspec(property(get=GetMaximumSplits,put=SetMaximumSplits)) unsigned long MaximumSplits;
  __declspec(property(get=GetMergeCopy,put=SetMergeCopy)) bool MergeCopy;
  __declspec(property(get=GetMergeOnlyFirst,put=SetMergeOnlyFirst)) bool MergeOnlyFirst;
  __declspec(property(get=GetSaveExtension,put=SetSaveExtension)) CString SaveExtension;
  __declspec(property(get=GetSplitInputDelete,put=SetSplitInputDelete)) bool SplitInputDelete;
  __declspec(property(get=GetStringToMatch,put=SetStringToMatch)) CString StringToMatch;
  
  __declspec(property(get=GetOnChangingFiles,put=SetOnChangingFiles)) TypeChangeFileResults OnChangingFilesGlobal;
  __declspec(property(get=GetOnFindingFiles,put=SetOnFindingFiles)) TypeFindFileResults OnFindFilesGlobal;
  __declspec(property(get=GetOnGrepping,put=SetOnGrepping)) TypeGrepResults OnGreppingGlobal;
  __declspec(property(get=GetOnGreppingFiles,put=SetOnGreppingFiles)) TypeGrepFileResults OnGreppingFilesGlobal;
  __declspec(property(get=GetOnMatching,put=SetOnMatching)) TypeResults OnMatchingGlobal;
  __declspec(property(get=GetOnMerging,put=SetOnMerging)) TypeMergeResults OnMergingGlobal;
  __declspec(property(get=GetOnSearching,put=SetOnSearching)) TypeResults OnSearchingGlobal;
  
private:

  friend class RegularExpressionPimpl;

  RegularExpressionPimpl * pimpl;
  
DECLARE_SERIAL(RegularExpression)
};

#pragma pack(pop)

#endif // !defined(AFX_REGULAREXPRESSION_H__D4133B40_BB9D_11D3_BD0C_000000000000__INCLUDED_)
