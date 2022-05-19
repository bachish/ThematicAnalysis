#ifndef SOLARIX_GRAMMAR_ENGINE__H
#define SOLARIX_GRAMMAR_ENGINE__H
#pragma once

// -----------------------------------------------------------------------------
// File SOLARIX_GRAMMAR_ENGINE.H
//
// (c) by Elijah Koziev
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://www.wordysoft.com 
//
// Manual page: http://www.solarix.ru/for_developers/grammar-engine-api.shtml
//
// You must not eliminate, delete or supress these copyright strings
// from the file!
//
// Content:
// SOLARIX Grammar engine API functions.
// API ��� ������� � ��������������� ������, Windows & Linux & Mac OS X, 32/64
//
// ���� �������������� ��������� (������� �������, ���������, ��������� � �.�.)
// ��������� � ���� ������������� �������� � ����� _sg_api.h, �������
// ������������ ������������ YGRES.
//
// 09.10.2006 - �����������, � ������ �������� API ������ ������������� �����
//              �������� "sol_"
//
// 09.10.2006 - ������ ���������� ����������� ���������� ��������� (RegexEx)
//
// 12.10.2006 - ������� ��������� sol_TranslateToBase - ����� ����������� �
//              ������� �����. 
//
// 15.10.2006 - ���������� ��������������� � ���������� ������ ����������.
//
// 21.11.2006 - � ��������� sol_TranslateToBase �������� ����� ��������.
//
// 05.05.2007 - changing API to implement a new multithreading features of
//              the search engine.
//
// 08.09.2007 - API ������ PRO � LITE ������� ��������� �����������,
//              ���������������� � LITE ��������� �������� ����������.
//
// 27.09.2007 - ��������� sol_GetResultW ��� ��������� ������ ����������� � wchar_t
// 13.11.2008 - ��������� sol_CreatePortableSearcher � sol_AttachPortableIndex
// 15.11.2008 - ��������� ������ ��������� �������� ����
// 03.02.2009 - �������� ���� FG_OMIT_FREQ1 ��� ������������� � ��������������
//              N-����� � ��������� ��������.
// 13.02.2009 - �������� API ������ SentenceBroker
// 24.02.2009 - ������� sol_EnumInClass ���������� 3 ��� �����
// 25.02.2009 - ��������� sol_PreloadNGramsCaches
// 02.03.2009 - ��� sol_Tokenize � sol_CreateSentenceBroker ��������� ��������
//              �����.
// 08.04.2009 - ������ sol_PreloadNGramsCaches
// 10.04.2009 - ��������� API �������������� (sol_Paraphrase)
// 20.07.2009 - ����������� API ������� � ���� N-�����: ������ sol_SeekNGrams,
//              ��������� sol_Seek2Grams, sol_Seek3Grams, sol_Seek4Grams,
//              sol_Seek5Grams
// 03.11.2009 - ��������� ��������� sol_MorphologyAnalysis ��� ����������
//              ���������������� ������� � ������ ���������.
// 22.07.2010 - ��������� ��������� �������� ��� ��������� ������ �����������,
//              � ��� ����� sol_ListLinksTxt, sol_FindPhrase, sol_ListEntries,
//              � ��������� ������.
// 15.09.2010 - ������������ ��������� sol_TranslatePhrase, ��������� 
//              sol_DeleteTranslation � ��.
// 04.04.2011 - ��������� sol_MorphologyAnalysisA � sol_SyntaxAnalysisA
// 30.07.2011 - ��������� ������� sol_GenerateWordforms ��� ������ ����
//              ���� �����, ��������������� ��������� �������������� ���������
// 28.08.2012 - �������� ���� ��������� ������������� ������� SOL_GREN_SPARSE
// 12.02.2013 - � sol_MorphologyAnalysis � sol_SyntaxAnalysis �������� ���������
//              ��������� �������� - ������ ������� 10 ����� ������ ������������
//              ����� ����������� ��� ���������� ������� �������������� �����, �
//              ������� 22 ���� - ����. ������� � �������������.
// -----------------------------------------------------------------------------
//
// CD->20.06.2005
// LC->28.03.2016
// --------------

#if defined DLL_EXPORTS
 #define GREN_EXPORTS
#else
 #define GREN_IMPORTS
#endif

#undef DLL_ENTRY

#if defined GREN_IMPORTS
 #if defined LEM_WINDOWS || defined WIN32
  #define GREN_API(RetType) extern RetType __stdcall
  #define SX_STDCALL __stdcall
 #else

  #if defined __cplusplus
  #define GREN_API(RetType) extern "C" RetType
  #else
  #define GREN_API(RetType) extern RetType
  #endif

  #define SX_STDCALL
 #endif
#elif defined GREN_EXPORTS
 #if defined LEM_WINDOWS || defined WIN32
  #define GREN_API(RetType) __declspec(dllexport) RetType __stdcall
  #define SX_STDCALL __stdcall
 #else
  #if defined __cplusplus
  #define GREN_API(RetType) extern "C" RetType __attribute__((visibility("default")))
  #else
  #define GREN_API(RetType) extern RetType __attribute__((visibility("default")))
  #endif
  #define SX_STDCALL
 #endif
#else
 #error
#endif

#if !defined GREN_INTERNAL_TYPES

 typedef void* HGREN;

 typedef void* HGREN_STR;
 typedef void* HGREN_INTARRAY;
 typedef void* HGREN_WCOORD;
 typedef void* HGREN_RESPACK;
 typedef void* HGREN_LINKAGE;
 typedef const void* HGREN_TREENODE;
 typedef void* HGREN_SBROKER;

 typedef void* HGREN_SPOKEN;
 typedef void* HGREN_LONGSTRING;

#endif // !defined DLL_EXPORTS

typedef void* HFLEXIONS;
typedef void* HFLEXIONTABLE;

typedef void* HLINKSINFO;
typedef void* HTRANINFO;


// Instance creation flags for sol_CreateGrammarEngineEx and sol_LoadDictionaryEx
#define SOL_GREN_LAZY_LEXICON 0x00000001


// Create the instance of grammatical dictionary and connecti it to the database
// http://www.solarix.ru/api/en/sol_CreateGrammarEngine.shtml

GREN_API(HGREN) sol_CreateGrammarEngineW( const wchar_t *DictionaryXml );
GREN_API(HGREN) sol_CreateGrammarEngineA( const char *DictionaryXml );
GREN_API(HGREN) sol_CreateGrammarEngine8( const char *DictionaryXmlUtf8 );
#if defined UNICODE || defined _UNICODE 
#define sol_CreateGrammarEngine sol_CreateGrammarEngineW
#else
#define sol_CreateGrammarEngine sol_CreateGrammarEngineA
#endif

GREN_API(HGREN) sol_CreateGrammarEngineExW( const wchar_t *DictionaryXml, int Flag );
GREN_API(HGREN) sol_CreateGrammarEngineExA( const char *DictionaryXml, int Flag );
GREN_API(HGREN) sol_CreateGrammarEngineEx8( const char *DictionaryXmlUtf8, int Flag );
#if defined UNICODE || defined _UNICODE
#define sol_CreateGrammarEngineEx sol_CreateGrammarEngineExW
#else
#define sol_CreateGrammarEngineEx sol_CreateGrammarEngineExA
#endif


typedef HGREN (SX_STDCALL * sol_CreateGrammarEngine8_Fun)( const char *DictionaryXmlUtf8 );
typedef HGREN (SX_STDCALL * sol_CreateGrammarEngineEx8_Fun)( const char *DictionaryXmlUtf8, int Flag );


// Destroy the instance of grammatical dicrionary and free all allocated resources
// http://www.solarix.ru/api/en/sol_DeleteGrammarEngine.shtml
GREN_API(int) sol_DeleteGrammarEngine( HGREN hEngine );

typedef int (SX_STDCALL * sol_DeleteGrammarEngine_Fun)( HGREN hEngine );


// Load the dictionary (morphology and syntax analyzer + thesaurus)
// http://www.solarix.ru/api/en/sol_LoadDictionary.shtml
GREN_API(int) sol_LoadDictionaryA( HGREN hEngine, const char *Filename );
GREN_API(int) sol_LoadDictionaryW( HGREN hEngine, const wchar_t *Filename );
GREN_API(int) sol_LoadDictionary8( HGREN hEngine, const char *FilenameUtf8 );
#if defined UNICODE || defined _UNICODE 
#define sol_LoadDictionary sol_LoadDictionaryW
#else
#define sol_LoadDictionary sol_LoadDictionaryA
#endif


GREN_API(int) sol_LoadDictionaryExA( HGREN hEngine, const char *Filename, int Flags );
GREN_API(int) sol_LoadDictionaryExW( HGREN hEngine, const wchar_t *Filename, int Flags );
GREN_API(int) sol_LoadDictionaryEx8( HGREN hEngine, const char *FilenameUtf8, int Flags );
#if defined UNICODE || defined _UNICODE 
#define sol_LoadDictionaryEx sol_LoadDictionaryExW
#else
#define sol_LoadDictionaryEx sol_LoadDictionaryExA
#endif

typedef int (SX_STDCALL * sol_LoadDictionaryEx8_Fun)( HGREN hEngine, const char *FilenameUtf8, int Flags );


// http://www.solarix.ru/api/en/sol_UnloadDictionary.shtml
GREN_API(void) sol_UnloadDictionary( HGREN hEngine );
typedef int (SX_STDCALL * sol_UnloadDictionary_Fun)( HGREN hEngine );


// Return the Solarix project version info.
// ���� ��������� Major, Minor, Build �� ����� NULL, �� ����� ���
// ������������ ��������������� ����� ������ ������ ����������.
//
// ���������� 0 ��� DEMO, 1 ��� PRO, 2 ��� PREMIUM
GREN_API(int) sol_GetVersion( HGREN hEngine, int *Major, int *Minor, int *Build );
typedef int (SX_STDCALL * sol_GetVersion_Fun)( HGREN hEngine, int *Major, int *Minor, int *Build );

// ���������� ����� ������ ������������ �������.
// http://www.solarix.ru/api/en/sol_DictionaryVersion.shtml
GREN_API(int) sol_DictionaryVersion( HGREN hEngine );
typedef int (SX_STDCALL * sol_DictionaryVersion_Fun)( HGREN hEngine );


// Returns the value indicating whether the dictionary has been initialized and loaded
// http://www.solarix.ru/api/en/sol_IsDictionaryLoaded.shtml
GREN_API(int) sol_IsDictionaryLoaded( HGREN hEngine );


// Count the total number of entries in dictionary.
// ������� ��������� ������ � �������.
// http://www.solarix.ru/api/ru/sol_CountEntries.shtml
GREN_API(int) sol_CountEntries( HGREN hEngine );
typedef int (SX_STDCALL * sol_CountEntries_Fun)( HGREN hEngine );

// Count the total number of wordforms in dictionary.
// ������� �������������� ���� �� ���� ��������� ������� � ���������
GREN_API(int) sol_CountForms( HGREN hEngine );

// ���������� ���������: ������� ������� �����.
// http://www.solarix.ru/api/ru/sol_CountLinks.shtml
GREN_API(int) sol_CountLinks( HGREN hEngine );

// Returns the max lexem length
// ���������� ������������ ����� �������
// http://www.solarix.ru/api/en/sol_MaxLexemLen.shtml
GREN_API(int) sol_MaxLexemLen( HGREN hEngine );
GREN_API(int) sol_MaxLexemLen8( HGREN hEngine );
typedef int (SX_STDCALL * sol_MaxLexemLen8_Fun)( HGREN hEngine );

// *******************************************
// Set the default language ID.
// ������������� ���� �� ���������. ������������� ��� �����
// ����� ������� � ����� _sg_api.h
// *******************************************
GREN_API(int) sol_SetLanguage( HGREN hEngine, int Language );



// ����� ������ � ���������
GREN_API(HGREN_INTARRAY) sol_SeekThesaurus(
                                            HGREN hEngine,
                                            int iEntry,
                                            int Synonyms,
                                            int Grammar_Links,
                                            int Translations,
                                            int Semantics,
                                            int nJumps
                                           );

// http://www.solarix.ru/api/en/sol_Thesaurus.shtml
GREN_API(HGREN_INTARRAY) sol_Thesaurus( HGREN hEngine, int EntryID, int LinkType );

// http://www.solarix.ru/api/en/sol_ListLinksTxt.shtml
GREN_API(HLINKSINFO) sol_ListLinksTxt( HGREN hEngine, int EntryID1, int LinkType, int Flags );

// http://www.solarix.ru/api/en/sol_DeleteLinksInfo.shtml
GREN_API(int) sol_DeleteLinksInfo( HGREN hEng, HLINKSINFO hList );

// http://www.solarix.ru/api/en/sol_LinksInfoCount.shtml
GREN_API(int) sol_LinksInfoCount( HGREN hEng, HLINKSINFO hList );

// http://www.solarix.ru/api/en/sol_LinksInfoType.shtml
GREN_API(int) sol_LinksInfoType( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_LinksInfoID.shtml
GREN_API(int) sol_LinksInfoID( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/ru/sol_LinksInfoCode.shtml
GREN_API(int) sol_LinksInfoCode( HGREN hEngine, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_LinksInfoEKey1.shtml
GREN_API(int) sol_LinksInfoEKey1( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_LinksInfoEKey2.shtml
GREN_API(int) sol_LinksInfoEKey2( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_LinksInfoTagsTxt.shtml
GREN_API(const wchar_t*) sol_LinksInfoTagsTxt( HGREN hEng, HLINKSINFO hList, int Index );
GREN_API(const char*) sol_LinksInfoTagsTxt8( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_LinksInfoFlagsTxt.shtml
GREN_API(const wchar_t*) sol_LinksInfoFlagsTxt( HGREN hEng, HLINKSINFO hList, int Index );
GREN_API(const char*) sol_LinksInfoFlagsTxt8( HGREN hEng, HLINKSINFO hList, int Index );

// http://www.solarix.ru/api/en/sol_DeleteLink.shtml
GREN_API(int) sol_DeleteLink( HGREN hEng, int LinkID, int LinkType );

// http://www.solarix.ru/api/en/sol_AddLink.shtml
GREN_API(int) sol_AddLink( HGREN hEng, int LinkType, int EntryID1, int LinkCode, int EntryID2, const wchar_t *Tags );
GREN_API(int) sol_AddLink8( HGREN hEng, int LinkType, int EntryID1, int LinkCode, int EntryID2, const char *TagsUtf8 );

// http://www.solarix.ru/api/en/sol_SetLinkTags.shtml
GREN_API(int) sol_SetLinkTags( HGREN hEngine, int LinkType, int LinkID, const wchar_t *Tags );
GREN_API(int) sol_SetLinkTags8( HGREN hEngine, int LinkType, int LinkID, const char *TagsUtf8 );

// http://www.solarix.ru/api/en/sol_SetLinkFlags.shtml
GREN_API(int) sol_SetLinkFlags( HGREN hEngine, int LinkID, const wchar_t *Flags );
GREN_API(int) sol_SetLinkFlags8( HGREN hEngine, int LinkID, const char *FlagsUtf8 );


// ***************************************************************************
// Find the word entry index.
// ����� ������� ��������� ������ � ���������.
// http://www.solarix.ru/api/en/sol_FindEntry.shtml
// ***************************************************************************
GREN_API(int) sol_FindEntry(
                             HGREN hEngine,
                             const wchar_t *Word, // Entry name
                             int ClassID,         // Required grammatical class (part of speech) ID
                             int LanguageID       // Language ID (if ambiguous)
                            );
GREN_API(int) sol_FindEntry8( HGREN hEngine, const char *Word, int ClassID, int LanguageID );
typedef int (SX_STDCALL * sol_FindEntry8_Fun)( HGREN hEngine, const char *Word, int ClassID, int LanguageID );


// http://www.solarix.ru/api/en/sol_FindClass.shtml
GREN_API(int) sol_FindClass( HGREN hEngine, const wchar_t *ClassName );
GREN_API(int) sol_FindClass8( HGREN hEngine, const char *ClassNameUtf8 );

// http://www.solarix.ru/api/en/sol_FindEnum.shtml
GREN_API(int) sol_FindEnum( HGREN hEngine, const wchar_t *EnumName );
GREN_API(int) sol_FindEnum8( HGREN hEngine, const char *EnumNameUtf8 );

// http://www.solarix.ru/api/en/sol_FindEnumState.shtml
GREN_API(int) sol_FindEnumState( HGREN hEngine, int EnumID, const wchar_t *StateName );
GREN_API(int) sol_FindEnumState8( HGREN hEngine, int EnumID, const char *StateNameUtf8 );


#define sol_FindStrings( hEngine, Word ) sol_FindStringsEx(hEngine,Word,false,false,false,false,false,0)


// ����� ���� ��������� ��� ���������� �����, � ��������� ������ ���������
// �� ���������. ������������ ������ �� ������� �����.
GREN_API(HGREN_STR) sol_FindStringsEx(
                                       HGREN hEngine,
                                       const wchar_t *Word,
                                       int Allow_Dynforms,
                                       int Synonyms,
                                       int Grammar_Links,
                                       int Translations,
                                       int Semantics,
                                       int nJumps
                                      );


GREN_API(int) sol_CountStrings( HGREN_STR hStrings );
GREN_API(int) sol_GetStrings( HGREN_STR hStrings, wchar_t **Res );
GREN_API(int) sol_GetStringLen( HGREN_STR hStrings, int i );
GREN_API(int) sol_GetStringW( HGREN_STR hStrings, int i, wchar_t *Res );
GREN_API(int) sol_GetStringA( HGREN_STR hStrings, int i, char *Res );
GREN_API(int) sol_GetString8( HGREN_STR hStrings, int i, char *ResUtf8 );
GREN_API(int) sol_DeleteStrings( HGREN_STR hStrings );

typedef int (SX_STDCALL * sol_CountStrings_Fun)( HGREN_STR hStrings );
typedef int (SX_STDCALL * sol_GetStringLen_Fun)( HGREN_STR hStrings, int i );
typedef int (SX_STDCALL * sol_GetString8_Fun)( HGREN_STR hStrings, int i, char *ResUtf8 );
typedef int (SX_STDCALL * sol_DeleteStrings_Fun)( HGREN_STR hStrings );


// ������� - ���������� ����� �������� � �����, ������� ���������� 
// "������", ���������� ��� ���� ���� ����� �����. � ������ �������������
// �������� ������, ������ 0. 
// ������ � ������ PRO 
GREN_API(int) sol_Stemmer( HGREN hEngine, const wchar_t *Word );


// ���������� "���" ��������� ������ - ������ ��� ������� �����,
// �� ����� ���� ������ ������ (��������, ������ ���� - ������
// � ���� - �������). ������ 0 � ������ ������, -1 ������.
// http://www.solarix.ru/api/en/sol_GetEntryName.shtml
GREN_API(int) sol_GetEntryName( HGREN hEngine, int EntryID, wchar_t *Result );
GREN_API(int) sol_GetEntryName8( HGREN hEngine, int EntryID, char *Result );
typedef int (SX_STDCALL * sol_GetEntryName8_Fun)( HGREN hEngine, int EntryID, char *Result );


GREN_API(int) sol_GetEntryFreq( HGREN hEngine, int EntryID );


// http://www.solarix.ru/api/en/sol_GetEntryCoordState.shtml
GREN_API(int) sol_GetEntryCoordState( HGREN hEngine, int EntryID, int CategoryID );

GREN_API(int) sol_FindEntryCoordPair( HGREN hEngine, int EntryID, int CategoryID, int StateID );


// ���������� ������ ��������������� ������, � �������� ��������� ��������� ������.
// Returns the part of speech ID for the word entry
// http://www.solarix.ru/api/en/sol_GetEntryClass.shtml
GREN_API(int) sol_GetEntryClass( HGREN hEngine, int EntryID );
typedef int (SX_STDCALL * sol_GetEntryClass_Fun)( HGREN hEngine, int EntryID );


// ���������� ��� ��������������� ������ �� ��� �������
// Returns the part of speech name
// http://www.solarix.ru/api/en/sol_GetClassName.shtml
GREN_API(int) sol_GetClassName( HGREN hEngine, int ClassID, wchar_t *Result );
GREN_API(int) sol_GetClassName8( HGREN hEngine, int ClassID, char *ResultUtf8 );
typedef int ( SX_STDCALL * sol_GetClassName8_Fun)( HGREN hEngine, int ClassID, char *ResultUtf8 );

// ���������� ��� ���������� �� �������
// Returns the grammatical category name
// http://www.solarix.ru/api/en/sol_GetCoordName.shtml
GREN_API(int) sol_GetCoordName( HGREN hEngine, int CoordID, wchar_t *Result );
GREN_API(int) sol_GetCoordName8( HGREN hEngine, int CoordID, char *ResultUtf8 );
typedef int (SX_STDCALL * sol_GetCoordName8_Fun)( HGREN hEngine, int CoordID, char *ResultUtf8 );



// ���������, ����� ���� ������ ������� CoordID ��� ����� ���� ClassID:
// 0 - �� ����� ���������,
// 1 - ������� (����� ��� ���� ��������� ������),
// 2 - ��������� (�������� ����������),
// 3 - ���
// http://www.solarix.ru/api/en/sol_GetCoordType.shtml
GREN_API(int) sol_GetCoordType( HGREN hEngine, int CoordID, int ClassID );
typedef int (SX_STDCALL * sol_GetCoordType_Fun)( HGREN hEngine, int CoordID, int ClassID );


// ���������� ��� ��������� ���������� �� �� ID
// http://www.solarix.ru/api/en/sol_GetCoordStateName.shtml
GREN_API(int) sol_GetCoordStateName( HGREN hEngine, int CoordID, int StateID, wchar_t *Result );
GREN_API(int) sol_GetCoordStateName8( HGREN hEngine, int CoordID, int StateID, char *ResultUtf8 );
typedef int (SX_STDCALL * sol_GetCoordStateName8_Fun)( HGREN hEngine, int CoordID, int StateID, char *ResultUtf8 );

// ���������� ���������� ��������� � ����������. ��� ������������ ��������� ������ 0
// http://www.solarix.ru/api/en/sol_CountCoordStates.shtml
GREN_API(int) sol_CountCoordStates( HGREN hEngine, int CoordID );
typedef int (SX_STDCALL * sol_CountCoordStates_Fun)( HGREN hEngine, int CoordID );


// Returns the gender of noun (unapplayable for English) 
//
// ���������� ��� ���������������� (����������� ��� �����������)
// http://www.solarix.ru/grammar/gender.shtml
GREN_API(int) sol_GetNounGender( HGREN hEngine, int EntryIndex );

// ��������� ���������������� - ���������� � ������ ����� ��������� ����� 
// EntryIndex ��������� ������ � �������� ������ Number � � ������ ������ Case.
// http://www.solarix.ru/for_developers/russian-noun-declension.shtml
GREN_API(int) sol_GetNounForm(
                               HGREN hEngine,   
                               int EntryIndex,
                               int Number,
                               int Case,
                               wchar_t *Result
                              );


// **************************************************************************
// ��������� ������� - ������������ ����� � ������ ����� Number, ���� Gender,
// ������� Tense � ���� Person.
// http://www.solarix.ru/for_developers/russian-verb-conjugation.shtml
// **************************************************************************
GREN_API(int) sol_GetVerbForm(
                               HGREN hEngine,  
                               int EntryIndex,
                               int Number,
                               int Gender,
                               int Tense,
                               int Person, 
                               wchar_t *Result
                               );


// *************************************************************************
// ��������� ��������������� � ������ ����� Number, ��� Gender, ����� Case,
// ����� �������������� Anim, �������-������ ����� Shortness, �����
// ��������� Compar_Form.
// http://www.solarix.ru/for_developers/russian-adjective-declension.shtml
// *************************************************************************
GREN_API(int) sol_GetAdjectiveForm(
                                    HGREN hEngine,
                                    int EntryIndex,
                                    int Number,
                                    int Gender,
                                    int Case,
                                    int Anim,
                                    int Shortness,
                                    int Compar_Form, 
                                    wchar_t *Result
                                   );

// ***********************************************************************
// ������������ ���������������� � �������������. �������� �������� �����
// � (�����������) - ��������������/����������������.
// ***********************************************************************
GREN_API(int) sol_CorrNounNumber(
                                 HGREN hEngine, 
                                 int EntryIndex,   // id_entry
                                 int Value,        // � ����� ������ �����������
                                 int Case,         // NOMINATIVE_CASE_ru
                                 int Anim,         // INANIMATIVE_FORM_ru
                                 wchar_t *Result      // ����� ��� ����������
                                 );

GREN_API(int) sol_CorrVerbNumber(
                                 HGREN hEngine, 
                                 int EntryIndex,
                                 int Value,
                                 int Gender, 
                                 int Tense,
                                 wchar_t *Result
                                 );

GREN_API(int) sol_CorrAdjNumber(
                                HGREN hEngine, 
                                int EntryIndex,
                                int Value,
                                int Case, 
                                int Gender,
                                int Anim,  
                                wchar_t *Result
                               );

// ����� ��������� ������ � ���������� � ���, ��� ������� ����������� ����������
// ����� ������������� �������� ������ Word. ���������� ������ ������ EntryIndex,
// ������ ����� Form, � ������ ��������������� ������ � ��������� ������.
GREN_API(int) sol_FindWord(
                            HGREN hEngine, 
                            const wchar_t *Word,
                            int *EntryIndex,
                            int *Form,
                            int *Class 
                           );

GREN_API(int) sol_GenerateWordform(
                                    HGREN hEngine, 
                                    int EntryID,
                                    int npairs,
                                    const int *pairs,
                                    wchar_t *Result
                                   );


GREN_API(HGREN_STR) sol_GenerateWordforms(
                                           HGREN hEngine, 
                                           int EntryID,
                                           int npairs,
                                           const int *pairs
                                          );



// ****************************************************************
// �����, �������� ������������� ���������� value, ����������� �
// ��������� ������������� (������������ �����)
// ****************************************************************
GREN_API(int)sol_Value2Text(
                             HGREN hEngine, 
                             wchar_t *Result,
                             int Value,
                             int Gender
                            );

// ������� ����� �����. ������������ ����� �������� ���, ���������� ��� ����
// ��������� � ������ ����� ��������� ������, ���� -1 ���� ����� �� ������.
GREN_API(int) sol_SeekWord( HGREN hEngine, const wchar_t *word, int Allow_Dynforms );


// ��������� ������ ���������� � ������������ ��������������
// ����������������, �������� "������->�����". ��� ������ ���������
// �������� (���������� � ���������� ������� �������)!
// ������� �������� EntryIndex - ��������� ����������� ������ sol_FindEntry.
// ������������ ������ ��������� ������ (���������������) ��� �������������,
// ��������, � sol_GetNounForm, ���� -1 ���� �������� � ����������������
// �� �������������� ���������.
GREN_API(int) sol_TranslateToNoun( HGREN hEngine, int EntryIndex );


// ���������� ��������� ������ � ������������ ��� ������������� ���������
// �������������� ����� ������� (����������).
// http://www.solarix.ru/for_developers/api/lemmatization.shtml
GREN_API(int) sol_TranslateToInfinitive( HGREN hEngine, int EntryIndex );


// ���������� � ������� �����.
// ������ 1, ���� �������������� ����� �����, 0 ���� ����� �� ������� � �������.
// ��������� ������������ � Word, ������� �������� �����.
// ���� ����� ��������� ��������� �������������� ������� ����, �� �����
// ����������� �����-�� (�������������) ����.
//
// http://www.solarix.ru/for_developers/api/lemmatization.shtml
GREN_API(int) sol_TranslateToBase( HGREN hEngine, wchar_t *Word, int AllowDynforms ); 


// ���������� ������ ������� ���� ��� �����. 
// http://www.solarix.ru/for_developers/api/lemmatization.shtml
GREN_API(HGREN_STR) sol_TranslateToBases( HGREN hEngine, const wchar_t *Word, int AllowDynforms );


// http://www.solarix.ru/api/ru/sol_ProjectWord.shtml
// ���������� ���� �������� ���������� ����� � ������ � ���������� �������.
// Find all possible mappings of the word to the lexicon and return tha handle of the list. 
GREN_API(HGREN_WCOORD) sol_ProjectWord( HGREN hEngine, const wchar_t *Word, int Allow_Dynforms );
GREN_API(HGREN_WCOORD) sol_ProjectWord8( HGREN hEngine, const char *WordUtf8, int Allow_Dynforms );
typedef HGREN_WCOORD (SX_STDCALL * sol_ProjectWord8_Fun)( HGREN hEngine, const char *WordUtf8, int Allow_Dynforms );

GREN_API(HGREN_WCOORD) sol_ProjectMisspelledWord( HGREN hEngine, const wchar_t *Word, int Allow_Dynforms, int nmaxmiss );

// http://www.solarix.ru/api/ru/sol_CountProjections.shtml
// Count the number of mappings in the list. 
GREN_API(int) sol_CountProjections( HGREN_WCOORD hList );
typedef int (SX_STDCALL * sol_CountProjections_Fun)( HGREN_WCOORD hList );

// http://www.solarix.ru/api/ru/sol_DeleteProjections.shtml
// Delete the results of sol_ProjectWord
GREN_API(void) sol_DeleteProjections( HGREN_WCOORD hList );
typedef int (SX_STDCALL * sol_DeleteProjections_Fun)( HGREN_WCOORD hList );

// http://www.solarix.ru/api/ru/sol_GetIEntry.shtml
// Return the entry id (primary key). The entry properties can be accessed 
// via id, see sol_GetEntryName for example.
// ������������ id (primary key) ��������� ������ ����� ��������
GREN_API(int) sol_GetIEntry( HGREN_WCOORD hList, int Index );
typedef int (SX_STDCALL * sol_GetIEntry_Fun)( HGREN_WCOORD hList, int Index );

// http://www.solarix.ru/api/ru/sol_GetProjCoordState.shtml
// ������������ ��������� ��������������� �������� CoordID � �������� Index
GREN_API(int) sol_GetProjCoordState( HGREN hEngine, HGREN_WCOORD hList, int Index, int CoordID );
typedef int (SX_STDCALL * sol_GetProjCoordState_Fun)( HGREN hEngine, HGREN_WCOORD hList, int Index, int CoordID );

GREN_API(int) sol_GetProjCoordCount( HGREN hEngine, HGREN_WCOORD hList, int Index );
typedef int (SX_STDCALL * sol_GetProjCoordCount_Fun)( HGREN hEngine, HGREN_WCOORD hList, int Index );

GREN_API(int) sol_GetProjCoordId( HGREN hEngine, HGREN_WCOORD hList, int Index, int TagIndex );
typedef int (SX_STDCALL * sol_GetProjCoordId_Fun)( HGREN hEngine, HGREN_WCOORD hList, int Index, int TagIndex );

GREN_API(int) sol_GetProjStateId( HGREN hEngine, HGREN_WCOORD hList, int Index, int TagIndex );
typedef int (SX_STDCALL * sol_GetProjStateId_Fun)( HGREN hEngine, HGREN_WCOORD hList, int Index, int TagIndex );


#define SOL_GREN_ALLOW_FUZZY           0x00000002
#define SOL_GREN_COMPLETE_ONLY         0x00000004
#define SOL_GREN_PRETOKENIZED          0x00000008
#define SOL_GREN_TOKENIZE_ONLY         0x00000010
#define SOL_GREN_DISABLE_FILTERS       0x00000020
#define SOL_GREN_REORDER_TREE          0x00000400
#define SOL_GREN_MODEL                 0x00000800
#define SOL_GREN_MODEL_ONLY            0x00002000


// http://www.solarix.ru/api/ru/sol_MorphologyAnalysis.shtml
// Morphology analysis
GREN_API(HGREN_RESPACK) sol_MorphologyAnalysis(
                                                HGREN hEngine,
                                                const wchar_t *Sentence,
                                                int Flags,
                                                int UnusedFlags,
                                                int Constraints,
                                                int Language
                                               );
// Morphology analysis
GREN_API(HGREN_RESPACK) sol_MorphologyAnalysis8(
                                                 HGREN hEngine,
                                                 const char *SentenceUtf8,
                                                 int Flags,
                                                 int UnusedFlags,
                                                 int Constraints,
                                                 int Language
                                                );

GREN_API(HGREN_RESPACK) sol_MorphologyAnalysisA(
                                                 HGREN hEngine,
                                                 const char *Sentence,
                                                 int Flags,
                                                 int UnusedFlags,
                                                 int Constraints,
                                                 int Language
                                                );

typedef HGREN_RESPACK (SX_STDCALL * sol_MorphologyAnalysis8_Fun)(
                                                                 HGREN hEngine,
                                                                 const char *SentenceUtf8,
                                                                 int Flags,
                                                                 int UnusedFlags,
                                                                 int Constraints,
                                                                 int Language
                                                                );


// http://www.solarix.ru/api/ru/sol_SyntaxAnalysis.shtml
// **************************************************************************
// ���������� ��������������� ������� - �� ����� �����, �� ������ ����������
// ������, �������� �������������� �������� ���������� ��������������� �����.
// ��� ������� �������� � ���� ��������, �������� ������������� ����������.
// **************************************************************************
GREN_API(HGREN_RESPACK) sol_SyntaxAnalysis(
                                            HGREN hEngine,
                                            const wchar_t *Sentence,
                                            int MorphologicalFlags,
                                            int SyntacticFlags,
                                            int Constraints,
                                            int Language
                                           );

GREN_API(HGREN_RESPACK) sol_SyntaxAnalysis8(
                                             HGREN hEngine,
                                             const char *SentenceUtf8,
                                             int MorphologicalFlags,
                                             int SyntacticFlags,
                                             int Constraints,
                                             int Language
                                            );

GREN_API(HGREN_RESPACK) sol_SyntaxAnalysisA(
                                             HGREN hEngine,
                                             const char *Sentence,
                                             int MorphologicalFlags,
                                             int SyntacticFlags,
                                             int Constraints,
                                             int Language
                                            );

typedef HGREN_RESPACK (SX_STDCALL * sol_SyntaxAnalysis8_Fun)(
                                                             HGREN hEngine,
                                                             const char *SentenceUtf8,
                                                             int MorphologicalFlags,
                                                             int SyntacticFlags,
                                                             int Constraints,
                                                             int Language
                                                            );



GREN_API(wchar_t*) sol_NormalizePhraseW( HGREN hEngine, HGREN_RESPACK hLinkages );
GREN_API(char*) sol_NormalizePhrase8( HGREN hEngine, HGREN_RESPACK hLinkages );



// http://www.solarix.ru/api/ru/sol_DeleteResPack.shtml
// �������� ����������� ���������������� ��� ��������������� ������� 
GREN_API(void) sol_DeleteResPack( HGREN_RESPACK hPack );
typedef void (SX_STDCALL * sol_DeleteResPack_Fun)( HGREN_RESPACK hPack );


// http://www.solarix.ru/api/ru/sol_CountGrafs.shtml
// ������� �������������� ��������� ������� �����?  
GREN_API(int) sol_CountGrafs( HGREN_RESPACK hPack ); 
typedef int (SX_STDCALL * sol_CountGrafs_Fun)( HGREN_RESPACK hPack );

// http://www.solarix.ru/api/ru/sol_CountRoots.shtml
// ������� �������� (����� �������� ������) � �������� �����?
GREN_API(int) sol_CountRoots( HGREN_RESPACK hPack, int iGraf );
typedef int (SX_STDCALL * sol_CountRoots_Fun)( HGREN_RESPACK hPack, int iGraf );

// http://www.solarix.ru/api/ru/sol_GetRoot.shtml
// ��������� ��������� �� �������� ���� �������� ������ 
GREN_API(HGREN_TREENODE) sol_GetRoot( HGREN_RESPACK hPack, int iGraf, int iRoot );
typedef HGREN_TREENODE (SX_STDCALL * sol_GetRoot_Fun)( HGREN_RESPACK hPack, int iGraf, int iRoot );

// http://www.solarix.ru/api/ru/sol_CountLeafs.shtml
// ���������� ������������� � ������� ���� �����
GREN_API(int) sol_CountLeafs( HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_CountLeafs_Fun)( HGREN_TREENODE hNode );

// http://www.solarix.ru/api/ru/sol_GetLeafLinkType.shtml
// ��� �������������� ����� ����� �������� ����� � ��� ��������, �������� ���������� ������� (�� 0)
GREN_API(int) sol_GetLeafLinkType( HGREN_TREENODE hNode, int iLeaf );
typedef int (SX_STDCALL * sol_GetLeafLinkType_Fun)( HGREN_TREENODE hNode, int iLeaf );

// http://www.solarix.ru/api/ru/sol_GetLeaf.shtml
// ���������� ��������� �� �������� �����
GREN_API(HGREN_TREENODE) sol_GetLeaf( HGREN_TREENODE hNode, int iLeaf );
typedef HGREN_TREENODE (SX_STDCALL * sol_GetLeaf_Fun)( HGREN_TREENODE hNode, int iLeaf );

// http://www.solarix.ru/api/ru/sol_GetNodeIEntry.shtml
// ���������� ������ ��������� ������ � ����
GREN_API(int) sol_GetNodeIEntry( HGREN hEngine, HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_GetNodeIEntry_Fun)( HGREN hEngine, HGREN_TREENODE hNode );

// http://www.solarix.ru/api/ru/sol_GetNodeVerIEntry.shtml
GREN_API(int) sol_GetNodeVerIEntry( HGREN hEngine, HGREN_TREENODE hNode, int iver );
typedef int (SX_STDCALL * sol_GetNodeVerIEntry_Fun)( HGREN hEngine, HGREN_TREENODE hNode, int iver );

// http://www.solarix.ru/api/ru/sol_GetNodePosition.shtml
// Position of the first word in the node
GREN_API(int) sol_GetNodePosition( HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_GetNodePosition_Fun)( HGREN_TREENODE hNode );

GREN_API(int) sol_GetNodeContentsLen( HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_GetNodeContentsLen_Fun)( HGREN_TREENODE hNode );

// http://www.solarix.ru/api/ru/sol_GetNodeContents.shtml
// ��������� ���������� ����
GREN_API(void) sol_GetNodeContents( HGREN_TREENODE hNode, wchar_t *Buffer );
GREN_API(void) sol_GetNodeContents8( HGREN_TREENODE hNode, char *BufferUtf8 );
typedef void (SX_STDCALL * sol_GetNodeContents_Fun)( HGREN_TREENODE hNode, wchar_t *Buffer );
typedef void (SX_STDCALL * sol_GetNodeContents8_Fun)( HGREN_TREENODE hNode, char *BufferUtf8 );

// http://www.solarix.ru/api/ru/sol_GetNodeCoordState.shtml
// ��������� ���������� ��� ����
GREN_API(int) sol_GetNodeCoordState( HGREN_TREENODE hNode, int CoordID );
GREN_API(int) sol_GetNodeVerCoordState( HGREN_TREENODE hNode, int iver, int CoordID );
typedef int (SX_STDCALL * sol_GetNodeCoordState_Fun)( HGREN_TREENODE hNode, int CoordID );
typedef int (SX_STDCALL * sol_GetNodeVerCoordState_Fun)( HGREN_TREENODE hNode, int iver, int CoordID );

// http://www.solarix.ru/api/ru/sol_GetNodeVersionCount.shtml
// ���������� �������������� ������ ����������, ������� ��������.
GREN_API(int) sol_GetNodeVersionCount( HGREN hEngine, HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_GetNodeVersionCount_Fun)( HGREN hEngine, HGREN_TREENODE hNode );

// http://www.solarix.ru/api/ru/sol_GetNodeCoordPair.shtml
// ��������� ������� ������������ ���� � ����
GREN_API(int) sol_GetNodeCoordPair( HGREN_TREENODE hNode, int CoordID, int StateID );
GREN_API(int) sol_GetNodeVerCoordPair( HGREN_TREENODE hNode, int iver, int CoordID, int StateID );
typedef int (SX_STDCALL * sol_GetNodeCoordPair_Fun)( HGREN_TREENODE hNode, int CoordID, int StateID );
typedef int (SX_STDCALL * sol_GetNodeVerCoordPair_Fun)( HGREN_TREENODE hNode, int iver, int CoordID, int StateID );

// http://www.solarix.ru/api/ru/sol_GetNodePairsCount.shtml
// ����� ��������� ������������ ��� � ����
GREN_API(int) sol_GetNodePairsCount( HGREN_TREENODE hNode );
GREN_API(int) sol_GetNodeVerPairsCount( HGREN_TREENODE hNode, int iver );
typedef int (SX_STDCALL * sol_GetNodePairsCount_Fun)( HGREN_TREENODE hNode );
typedef int (SX_STDCALL * sol_GetNodeVerPairsCount_Fun)( HGREN_TREENODE hNode, int iver );

// http://www.solarix.ru/api/ru/sol_GetNodePairCoord.shtml
GREN_API(int) sol_GetNodePairCoord( HGREN_TREENODE hNode, int ipair );
GREN_API(int) sol_GetNodeVerPairCoord( HGREN_TREENODE hNode, int iver, int ipair );
typedef int (SX_STDCALL * sol_GetNodePairCoord_Fun)( HGREN_TREENODE hNode, int ipair );
typedef int (SX_STDCALL * sol_GetNodeVerPairCoord_Fun)( HGREN_TREENODE hNode, int iver, int ipair );

// http://www.solarix.ru/api/ru/sol_GetNodePairState.shtml
GREN_API(int) sol_GetNodePairState( HGREN_TREENODE hNode, int ipair );
GREN_API(int) sol_GetNodeVerPairState( HGREN_TREENODE hNode, int iver, int ipair );
typedef int (SX_STDCALL * sol_GetNodePairState_Fun)( HGREN_TREENODE hNode, int ipair );
typedef int (SX_STDCALL * sol_GetNodeVerPairState_Fun)( HGREN_TREENODE hNode, int iver, int ipair );




GREN_API(int) sol_RestoreCasing( HGREN hEngine, wchar_t *Word, int EntryIndex );
GREN_API(int) sol_RestoreCasing8( HGREN hEngine, char *WordUtf8, int EntryIndex );
typedef int (SX_STDCALL * sol_RestoreCasing8_Fun)( HGREN hEngine, char *WordUtf8, int EntryIndex );


GREN_API(HGREN_SBROKER) sol_CreateSentenceBroker( HGREN hEngine, const wchar_t *Filename, const wchar_t *DefaultCodepage, int LanguageID );
GREN_API(HGREN_SBROKER) sol_CreateSentenceBroker8( HGREN hEngine, const char *Filename8, const char *DefaultCodepage8, int LanguageID );

GREN_API(HGREN_SBROKER) sol_CreateSentenceBrokerMemW( HGREN hEngine, const wchar_t *Text, int LanguageID );
GREN_API(HGREN_SBROKER) sol_CreateSentenceBrokerMemA( HGREN hEngine, const char *Text, int LanguageID );
GREN_API(HGREN_SBROKER) sol_CreateSentenceBrokerMem8( HGREN hEngine, const char *TextUtf8, int LanguageID );
GREN_API(int) sol_FetchSentence( HGREN_SBROKER hBroker );
GREN_API(int) sol_GetFetchedSentenceW( HGREN_SBROKER hBroker, wchar_t *Buffer );
GREN_API(int) sol_GetFetchedSentenceA( HGREN_SBROKER hBroker, char *Buffer );
GREN_API(int) sol_GetFetchedSentence8( HGREN_SBROKER hBroker, char *BufferUtf8 );
GREN_API(int) sol_GetFetchedSentenceLen( HGREN_SBROKER hBroker );
GREN_API(void) sol_DeleteSentenceBroker( HGREN_SBROKER hBroker );

typedef HGREN_SBROKER (SX_STDCALL * sol_CreateSentenceBrokerMem8_Fun)( HGREN hEngine, char *WordUtf8, int EntryIndex );
typedef int (SX_STDCALL * sol_FetchSentence_Fun)( HGREN_SBROKER hBroker );
typedef int (SX_STDCALL * sol_GetFetchedSentenceLen_Fun)( HGREN_SBROKER hBroker );
typedef int (SX_STDCALL * sol_GetFetchedSentence8_Fun)( HGREN_SBROKER hBroker, char *BufferUtf8 );
typedef int (SX_STDCALL * sol_DeleteSentenceBroker_Fun)( HGREN_SBROKER hBroker );



// Split the string into words, using the rules for language with given ID
// http://www.solarix.ru/api/ru/sol_Tokenize.shtml
GREN_API(HGREN_STR) sol_TokenizeW( HGREN hEngine, const wchar_t *Sentence, int LanguageID );
GREN_API(HGREN_STR) sol_TokenizeA( HGREN hEngine, const char *Sentence, int LanguageID );
GREN_API(HGREN_STR) sol_Tokenize8( HGREN hEngine, const char *SentenceUtf8, int LanguageID );
typedef HGREN_STR (SX_STDCALL * sol_Tokenize8_Fun)( HGREN hEngine, const char *SentenceUtf8, int LanguageID );

#if defined UNICODE || defined _UNICODE 
 #define sol_Tokenize sol_TokenizeW
#else
 #define sol_Tokenize sol_TokenizeA
#endif


// N-grams database lookup
// http://www.solarix.ru/for_developers/api/ngrams-api.shtml
GREN_API(int) sol_Seek1Grams( HGREN hEngine, int type, const wchar_t *word1 );
GREN_API(int) sol_Seek2Grams( HGREN hEngine, int type, const wchar_t *word1, const wchar_t *word2 );
GREN_API(int) sol_Seek3Grams( HGREN hEngine, int type, const wchar_t *word1, const wchar_t *word2, const wchar_t *word3 );
GREN_API(int) sol_Seek4Grams( HGREN hEngine, int type, const wchar_t *word1, const wchar_t *word2, const wchar_t *word3, const wchar_t *word4 );
GREN_API(int) sol_Seek5Grams( HGREN hEngine, int type, const wchar_t *word1, const wchar_t *word2, const wchar_t *word3, const wchar_t *word4, const wchar_t *word5 );
GREN_API(int) sol_CountNGrams( HGREN hEngine, int type, int Order, unsigned int *Hi, unsigned int *Lo );

GREN_API(int) sol_Seek1Grams8( HGREN hEngine, int type, const char *word1 );
GREN_API(int) sol_Seek2Grams8( HGREN hEngine, int type, const char *word1, const char *word2 );
GREN_API(int) sol_Seek3Grams8( HGREN hEngine, int type, const char *word1, const char *word2, const char *word3 );
GREN_API(int) sol_Seek4Grams8( HGREN hEngine, int type, const char *word1, const char *word2, const char *word3, const char *word4 );
GREN_API(int) sol_Seek5Grams8( HGREN hEngine, int type, const char *word1, const char *word2, const char *word3, const char *word4, const char *word5 );



GREN_API(int) sol_CountInts( HGREN_INTARRAY h );
GREN_API(int) sol_GetInt( HGREN_INTARRAY h, int i );
GREN_API(void) sol_DeleteInts( HGREN_INTARRAY h );

GREN_API(int) sol_IsLanguagePhrase( HGREN hEngine, const wchar_t *Phrase, int Language );
GREN_API(int) sol_GuessPhraseLanguage( HGREN hEngine, const wchar_t *Phrase );

GREN_API(int) sol_Free( HGREN hEngine, void *Ptr );

GREN_API(int) sol_MatchNGrams( HGREN hEngine, const wchar_t *Text, int * unmatched_2_ngrams, int *n2, int *n3 );

GREN_API(int) sol_Syllabs(
                           HGREN hEngine,
                           const wchar_t *OrgWord,
                           wchar_t SyllabDelimiter,
                           wchar_t *Result,
                           int LanguageID
                          );

GREN_API(int) sol_Syllabs8(
                            HGREN hEngine,
                            const char *OrgWord,
                            char SyllabDelimiter, 
                            char *Result,
                            int LanguageID
                           );

// Find the internal language index by its name (usually English name)
// Returns -1 if no language is found.
GREN_API(int) sol_FindLanguage( HGREN hEngine, const wchar_t *LanguageName );
GREN_API(int) sol_FindLanguage8( HGREN hEngine, const char *LanguageNameUtf8 );

// Probe the lexicon for language entries.
// Returns 1 if language is present in lexicon, 0 if not,
GREN_API(int) sol_HasLanguage( HGREN hEngine, int LanguageID );

// Returns the total number of distinct lexems in lexicon
GREN_API(int) sol_CountLexems( HGREN hEng );

// Flexion Engine: guess suitable flexion handlers for the word
GREN_API(HFLEXIONS) sol_FindFlexionHandlers( HGREN hEngine, const wchar_t *WordBasicForm, int SearchEntries );

// Flexion engine: return the number of matching entries
GREN_API(int) sol_CountEntriesInFlexionHandlers( HGREN hEngine, HFLEXIONS hFlexs );

// Flexion engine: return the number of matching paradigmas
GREN_API(int) sol_CountParadigmasInFlexionHandlers( HGREN hEngine, HFLEXIONS hFlexs );

// Flexion engine: return the entry ID
GREN_API(int) sol_GetEntryInFlexionHandlers( HGREN hEngine, HFLEXIONS hFlexs, int Index );

// Flexion engine: return the paradigma internal index and human-friendly name
GREN_API(int) sol_GetParadigmaInFlexionHandlers( HGREN hEngine, HFLEXIONS hFlexs, int Index, wchar_t *ParadigmaName );

GREN_API(HFLEXIONTABLE) sol_BuildFlexionHandler(
                                                 HGREN hEng,
                                                 HFLEXIONS hFlexs,
                                                 const wchar_t *ParadigmaName,
                                                 int EntryIndex
                                                );

GREN_API(const wchar_t*) sol_GetFlexionHandlerWordform( HGREN hEngine, HFLEXIONTABLE hFlex, const wchar_t *dims );

GREN_API(int) sol_CountFlexionHandlerWordform( HGREN hEngine, HFLEXIONTABLE hFlex );
GREN_API(const wchar_t*) sol_GetFlexionHandlerWordformText( HGREN hEngine, HFLEXIONTABLE hFlex, int FormIndex );

GREN_API(int) sol_DeleteFlexionHandlers( HGREN hEng, HFLEXIONS hFlexs );
GREN_API(int) sol_DeleteFlexionHandler( HGREN hEng, HFLEXIONTABLE hFlex );



// http://www.solarix.ru/api/en/sol_FindPhrase.shtml
GREN_API(int) sol_FindPhrase( HGREN hEng, const wchar_t *Phrase, int Flags );
GREN_API(int) sol_FindPhrase8( HGREN hEng, const char *Phrase, int Flags );

GREN_API(wchar_t*) sol_GetPhraseText( HGREN hEngine, int PhraseID );
GREN_API(char*) sol_GetPhraseText8( HGREN hEngine, int PhraseID );

GREN_API(int) sol_GetPhraseLanguage( HGREN hEngine, int PhraseID );
GREN_API(int) sol_GetPhraseClass( HGREN hEngine, int PhraseID );
GREN_API(int) sol_AddPhrase( HGREN hEngine, const wchar_t *Phrase, int LanguageID, int PartOfSpeechID, int ProcessingFlags );
GREN_API(int) sol_AddPhrase8( HGREN hEngine, const char *Phrase, int LanguageID, int PartOfSpeechID, int ProcessingFlags );
GREN_API(int) sol_SetPhraseNote( HGREN hEngine, int PhraseID, const wchar_t *Name, const wchar_t *Value );

GREN_API(int) sol_ProcessPhraseEntry(
                                      HGREN hEngine,
                                      int te_id,
                                      const wchar_t *Scenario,
                                      int LanguageID,
                                      wchar_t DelimiterChar
                                     );
GREN_API(int) sol_DeletePhrase( HGREN hEngine, int te_id );



GREN_API(HGREN_INTARRAY) sol_ListEntries( HGREN hEngine, int Flags, int EntryType, const wchar_t *Mask, int LanguageID, int PartOfSpeechID );
GREN_API(HGREN_INTARRAY) sol_ListEntries8( HGREN hEngine, int Flags, int EntryType, const char *MaskUtf8, int LanguageID, int PartOfSpeechID );
GREN_API(HGREN_STR) sol_ListEntryForms( HGREN hEngine, int EntryKey );


GREN_API(int) sol_SaveDictionary( HGREN hEngine, int Flags, const wchar_t *Folder );

GREN_API(int) sol_AddWord( HGREN hEng, const wchar_t *Txt );
GREN_API(int) sol_AddWord8( HGREN hEng, const char *Txt );

GREN_API(int) sol_ReserveLexiconSpace( HGREN hEngine, int n );

// Error reporting functions.
GREN_API(int) sol_GetErrorLen( HGREN hEngine );

GREN_API(int) sol_GetErrorLen8( HGREN hEngine );
typedef int (SX_STDCALL * sol_GetErrorLen8_Fun)( HGREN hEngine );

GREN_API(int) sol_GetError( HGREN hEngine, wchar_t *Buffer, int buffer_len );
GREN_API(int) sol_GetError8( HGREN hEngine, char *BufferUtf8, int buffer_len );
typedef int (SX_STDCALL * sol_GetError8_Fun)( HGREN hEngine, char *BufferUtf8, int buffer_len );

GREN_API(void) sol_ClearError( HGREN hEngine );

GREN_API(int) sol_LemmatizeWord( HGREN hEngine, wchar_t *word, int Allow_Dynforms );

GREN_API(HGREN_INTARRAY) sol_ListPartsOfSpeech( HGREN hEngine, int Language );

GREN_API(int) sol_FindTagW( HGREN hEngine, const wchar_t *TagName );
GREN_API(int) sol_FindTagValueW( HGREN hEngine, int TagID, const wchar_t *ValueName );


GREN_API(HGREN_SPOKEN) sol_PronounceW( HGREN hEngine, const wchar_t * Sentence, int LanguageID, int AnalysisFlags, int SynthesisFlags, void * VoiceOptions );
GREN_API(int) sol_DeleteSpoken( HGREN_SPOKEN hData );


GREN_API(int) sol_CountNodeMarks( HGREN_TREENODE hNode );
GREN_API(int) sol_GetNodeMarkNameW( HGREN_TREENODE hNode, int mark_index, wchar_t * name_buffer );
GREN_API(HGREN_LONGSTRING) sol_SerializeNodeMark( HGREN hEngine, HGREN_TREENODE hNode, int mark_index, int format );
GREN_API(int) sol_GetLongStringLenW( HGREN_LONGSTRING hString );
GREN_API(int) sol_GetLongStringW( HGREN_LONGSTRING hString, wchar_t * buffer );
GREN_API(int) sol_DeleteLongString( HGREN_LONGSTRING hString );

GREN_API(const char*) sol_RenderSyntaxTree8( HGREN hEngine, HGREN_RESPACK hSyntaxTree, int RenderOptions );

GREN_API(void*) sol_OpenCorpusStorage8( HGREN hEngine, const char * filename, int for_writing );
GREN_API(int) sol_CloseCorpusStorage( HGREN hEngine, void * file_handle );
GREN_API(int) sol_WriteSyntaxTree( HGREN hEngine, void * file_handle, const wchar_t * sentence_utf8,  HGREN_RESPACK respack );

GREN_API(void*) sol_LoadSyntaxTree( HGREN hEngine, void * file_handle );
GREN_API(HGREN_RESPACK) sol_GetTreeHandle( void * ptr );
GREN_API(const wchar_t*) sol_GetSentenceW( void * ptr );
GREN_API(int) sol_FreeSyntaxTree( void * tree );
GREN_API(HGREN_RESPACK) sol_CreateLinkages(HGREN hEngine);
GREN_API(HGREN_LINKAGE) sol_CreateLinkage(HGREN hEngine, HGREN_RESPACK hLinkages);
GREN_API(int) sol_AddBeginMarker(HGREN hEngine, HGREN_LINKAGE hLinkage);
GREN_API(int) sol_AddEndMarker(HGREN hEngine, HGREN_LINKAGE hLinkage);
GREN_API(int) sol_AddNodeToLinkage(HGREN hEngine, HGREN_LINKAGE hLinkage, HGREN_TREENODE hNode);
GREN_API(HGREN_TREENODE) sol_CreateTreeNodeW(HGREN hEngine, int id_entry, const wchar_t *word, int n_pair, const int * pairs);



#endif
