
// ����������� �� ������ �����.
class LexerTextPos
{
 private:
  int32_t start_position; // ������ ������� �������.
  int32_t token_length; // ����� ������ � ������� ��������.
  const Word_Form * wordform;
  int32_t negative_score; // ����������� ������ �� ������ �����������.
  const LexerTextPos * previous;
  
  const LexerTextPos * next_chained; // ���� �� ���� ������� �� 1 �����, � ����� �������
                                     // ����������������, �� ��� ������� �������� ����� ����
                                     // next_chained.
 
 public:
  LexerTextPos(
               const LexerTextPos * _previous,
               const LexerTextPos * _next_chained,
               const Word_Form * _wordform,
               int _negative_score,
               lem::int32_t _start,
               lem::int32_t _length
              )
   : start_position(_start), token_length(_length), wordform(_wordform),
     negative_score(_negative_score), previous(_previous), next_chained(_next_chained)
  {
   LEM_CHECKIT_Z( wordform!=NULL );
   LEM_CHECKIT_Z( start_position>=0 );
   LEM_CHECKIT_Z( token_length>0 );
   LEM_CHECKIT_Z( negative_score<=0 );
  }

  const LexerTextPos * GetNextChained() const { return next_chained; }
  
  // ���������� ��������� �� ����� ������ �� ������.
  const wchar_t* NextText( const wchar_t *text ) const
  {
   if( next_chained!=NULL )
    return text + next_chained->start_position;
   else
    return text + start_position + token_length;
  }
  
  lem::int32_t GetNextCharPos() const
  {
   if( next_chained!=NULL )
    return next_chained->start_position;
   else
    return start_position+token_length;
  }
  
  const LexerTextPos& GetPrev() const { return *previous; }
  
  lem::int32_t CalcTotalScore() const
  { return negative_score + previous==NULL ? 0 : previous->CalcTotalScore(); }
}



struct TextTokenizationRules : lem::NonCopyable
{
 lem::MCollect<wchar_t> spaces; // ���������� �������.
 lem::MCollect<wchar_t> delimeters; // �������-�����������.
 
 
};



class WrittenTextLexer : public BasicLexer
{
 private:
  Solarix::Dictionary * dict;
  lem::MCollect<int> language_ids; // �����, �� ������� ������������ ������� �����������.
  const TextTokenizationRules * rules; // ��� ����������� ������� ����������� ��� ��������� ������ ������.
  
  const wchar_t * text; // ����������� �����.
  lem::PtrCollect<Word_Form> wordforms; // ��� ��������������� ���������� ��� ���� �����.
  lem::PtrCollect<LexerTextPos> positions; // ��� �������, �� ������� ������ ����� �������������
  
  // ����� �� ��������� ������������� ����, ����� ������� ���������� �������������.
  // ����� � �������� ��������.
  std::map< lem::UCString, const Word_Form* > recognitions;

  // ��������� �����, ������������ ���������� ����������. ����� �������
  // ������ �� ���������� � ��� ����������.
  private const Word_Form * Recognize( const lem::UCString & word )
  {
   std::map<lem::UCString,Word_Form*>::iterator it = recognitions.find(word);
   if( it==recognitions.end() )
    {
     // ����� ��� �� �����������, ��������� ��������������� ������.
     const Word_Form * new_recognition = ...
     recognitions.insert( std::make_pair( word, new_recognition ) );
     return new_recognition;
    }
   else
    return it->second;
  }
  
  private void Extract(
                       const wchar_t * current_text,
                       lem::int32_t current_idx,
                       LexerTextPos *previous_pos,
                       lem::MCollect<LexerTextPos*> & tokens
                      )
  {
   next.clear();

   if( 0 != *current_text )
    {
     // ������ ���� �����, ��������� ������� �� ���� ������.

     // ���������� �������. �������, ��� ��� ��� ��������������� - ��������������
     // ��������� ���, �� ���� �� ���� ������ ������� �������� ��� �� ��������
     // ����������� ������������.
     current_text = rules->SkipSpace( current_text );
     
     // ... ���� ��� �������� �����������, ��� ������� � ������� ���� �������, �� ����
     // ������� �������������� ����� � ����������� ��������� � ��. �������������.
     // ��������� ������� �����������.
     while( ... )
     {
      lem::int32_t token_len = ...
      
      lem::UCString token = ...      
      
      const Word_Form * new_wordform = Recognize( token );
     
      LexerTextPos * new_token = new LexerTextPos(
                                                  previous_pos,
                                                  new_wordform,
                                                  current_idx,
                                                  token_len
                                                 );
                                                 
      // ���� ���� ���������� ������, �� �������� �����������
      // ��������� �������.
      if( previous_pos!=NULL )
       {
        // ...
        
        // ���� ���� ����������, �� ������� new_token � ������ ���������� � NULL.
       }

      if( new_token!=NULL )
       {
        positions.push_back( new_token );      
        tokens.push_back(new_token);
       }
     }
     
     // ��� ����� ������������� �������� � ������� �������� �����. ����� �������,
     // ������� ����� ���� ����� ������� ������, ��� �������� ���������.
    }

   return;
  }
 
 public:
  WrittenTextLexer(
                   const wchar_t *Text,
                   const lem::MCollect<int> & _language_ids,
                   Solarix::Dictionary * _dict
                  )
   : dict(_dict), language_ids(_language_ids), text(Text)
  {
   LEM_CHECKIT_Z( text!=NULL );

   rules = dict->GetTokenizationRules( language_id );
  }
  
  // �������� ����������� ������ - ��������� ������ ������, � ������� ����������
  // �������������� ����.
  virtual void Start( lem::MCollect<const LexerTextPos*> & tokens )
  {
   Extract( text, 0, NULL, tokens );
   return;
  }
  
  // �� ������ current ���������� �����������. ��������, �������� �������������� ����,
  // ������ �� ������� ����� ����������� ����� �������� � ������ next.
  // ���������� true, ���� ����������� ����������; false ���� ������ ������ ��� ����.
  virtual bool Next( const LexerTextPos * current, lem::MCollect<const LexerTextPos*> & next )
  {
   if( current->GetNextChained()!=NULL )
    {
     next.push_back( current->GetNextChained() );
     return true;
    }
   else
    {
     const wchar_t * text2 = current->NextText( text );
     lem::int32_t pos = current->GetNextCharPos();
     ExtractTokens( text2, pos, current, next );
     return !next.empty();
    }
  }
};
