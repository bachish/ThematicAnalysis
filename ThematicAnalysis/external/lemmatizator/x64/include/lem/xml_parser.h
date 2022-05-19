/// -----------------------------------------------------------------------------
/// \file XML_PARSER.H
///
/// (c) Koziev Elijah
///
/// ������� ������ ��� XML-������.
/// -----------------------------------------------------------------------------
///
/// CD->20.11.2004
/// LC->14.08.2009
/// --------------

#ifndef LEM_XML_PARSER__H
#define LEM_XML_PARSER__H
#pragma once

 #include <lem/noncopyable.h>
 #include <lem/runtime_checks.h>
 #include <lem/containers.h>
 #include <lem/fstring.h>
 #include <lem/stl.h>
 #include <lem/ustreams.h>

 namespace lem
 {
  class Stream;

  namespace Xml
  {
   class Out_Of_Range : public E_BaseException
   {
    public:
     Out_Of_Range(void)
      : E_BaseException( L"Out of range error in XML parser" ) {}
   };


   class Invalid_Format : public E_BaseException
   {
    public:
     Invalid_Format(void)
      : E_BaseException( L"Incorrect XML file format" ) {}
   };

   class Attribute_Not_Found : public E_BaseException
   {
    public:
     Attribute_Not_Found(void) : E_BaseException( L"Attribute not found" ) {}
     Attribute_Not_Found( const lem::UFString &msg ) : E_BaseException(msg) {}
   };
   

   class Node;
   struct NodeVisitor
   {
    virtual void operator()( const Node *n )=0;
    virtual void operator()( Node *n ) { operator()( (const Node*)n ); }
    virtual ~NodeVisitor(void) {}
   };

   

   // *********************************************************************
   // ��������� ���� XML-������, ������ � �������������� � ���� ����������
   // ������.
   // *********************************************************************
   class Node : NonCopyable
   {
    private:
     friend class Parser;

     typedef enum { CommentNode, CDataNode, InvalidNode, ScriptNode, OtherNode } NodeType;
     NodeType node_type;

     UFString name;
     UFString body;
     UFString visible_text; // ��� HTML ��������� - ������� � �������� �����.
     MCollect<Node*> nodes;
     Collect< std::pair< UFString/*name*/,UFString/*value*/> > attrs;

     bool closing; // ��� ���� </abc>
     bool closed; // ���������� ���, ��� �������� ������� �����������.

     Node( const Node &x );
     const Node& operator=( const Node &x );

    public:
     Node(void);
     ~Node(void);

     bool IsComment(void) const { return node_type==CommentNode; }
     bool IsCData(void) const { return node_type==CDataNode; }
     bool IsInvalid(void) const { return node_type==InvalidNode; }

     inline bool GetClosing(void) const { return closing; }
     inline void SetClosing( bool f ) { closing=f; }

     inline bool GetClosed(void) const { return closed; }
     inline void SetClosed( bool f ) { closed=f; }

     inline const UFString& GetName(void) const { return name; }
     inline const UFString& GetBody(void) const { return body; }
     inline const UFString& GetVisibleText(void) const { return visible_text; }

     int GetInt(void) const;

     inline       MCollect<Node*>& GetNodes(void)       { return nodes; }
     inline const MCollect<Node*>& GetNodes(void) const { return nodes; }

           Node* GetNode( const UFString &Name );
     const Node* GetNode( const UFString &Name ) const;

     const Collect< std::pair<UFString,UFString> >& GetAttrs(void) const { return attrs; }
           Collect< std::pair<UFString,UFString> >& GetAttrs(void)       { return attrs; }

     int FindAttr( const lem::UFString &attr ) const;

     /// ���������� �������� ��������
     const UFString& GetAttr( const lem::UFString &attr, bool exception_if_missing=true ) const;

     // *************************************************************
     // ������ ����� �� ���� ������ - ������� ������ ���� �
     // �������� ������. ���� ����� ����������, �� ���������� NULL.
     // *************************************************************
     Node* Find_Everywhere( const UFString &Name );
     const Node* Find_Everywhere( const UFString &Name ) const;

     // Return the pointer to node identified by sequence of child tags
     const Node* Find_By_Path( const UFString &path ) const;
     const Node* Find_By_Path( const Collect<UFString> &Steps ) const;

     lem::UFString Get_By_Path( const UFString &Path ) const;

     /// Return the list of all nodes with given name
     lem::MCollect<const Node*> GetNodes( const UFString &Name ) const;

     /// ��������� ���������� ����� ���� ��� ����� ��������
     bool MatchNameNoCase( const lem::UFString &x ) const;

     /// ���������� ������ ����������� �����, ��� ����� ��������� � �������� ��� ����� ��������.
     void FindMatchNameNoCase( const lem::UFString &name, lem::MCollect<const lem::Xml::Node*> &nodes ) const;

     /// ���� �� ����� ��������� ���� �������� ��������.
     bool MatchAttrNoCase( const lem::UFString &attr, const lem::UFString &value ) const;

     /// ����� ����� �������� ��������� ������, ��������� � ��� ���������.
     const lem::Xml::Node* FindTagWithAttr( 
                                           const lem::UFString &tag,
                                           const lem::UFString &attr_name,
                                           const lem::UFString &attr_value
                                          );

     /// Recursively walk over tree, processing each node with v->operator()
     void Walk( NodeVisitor *v ) const;
    };

   // *************************************************************
   // ������������������ �������� ��� �������� ����������� �����
   // � ������������ ������
   // *************************************************************
   class Iterator : NonCopyable
   {
    private:
     Node *root;    // ������� ���� - ������������� � ���� ����������
     UFString name; // ��� ������������ �����

     int icur;

    public:
     Iterator( Node *Root, const UFString &Name );

     inline Node& operator*(void) const
     {
      #if LEM_DEBUGGING==1
       if( icur==UNKNOWN )
        throw Out_Of_Range();
      #endif

      return *(root->GetNodes()[icur]);
     }

     inline Node* operator->(void) const
     {
      #if LEM_DEBUGGING==1
       if( icur==UNKNOWN )
        throw Out_Of_Range();
      #endif

      return root->GetNodes()[icur];
     }

     // ������������� �� ��������� ������� � �������� ������.
     Iterator& operator++(void);
     Iterator& operator++(int);

     // Returns TRUE if no more nodes available
     inline bool eof(void) const { return icur==UNKNOWN; }
   };


   class ConstIterator : NonCopyable
   {
    private:
     const Node *root;    // ������� ���� - ������������� � ���� ����������
     UFString name; // ��� ������������ �����

     int icur;

    public:
     ConstIterator( const Node *Root, const UFString &Name );

     inline const Node& operator*(void) const
     {
      #if LEM_DEBUGGING==1
       if( icur==UNKNOWN )
        throw Out_Of_Range();
      #endif

      return *(root->GetNodes()[icur]);
     }

     inline const Node* operator->(void) const
     {
      #if LEM_DEBUGGING==1
       if( icur==UNKNOWN )
        throw Out_Of_Range();
      #endif

      return root->GetNodes()[icur];
     }

     // ������������� �� ��������� ������� � �������� ������.
     ConstIterator& operator++(void);
     ConstIterator& operator++(int);

     // Returns TRUE if no more nodes available
     inline bool eof(void) const { return icur==UNKNOWN; }
   };





   class Parser : NonCopyable
   {
    public:  
     typedef enum { UnknownDoc, XmlDoc, HtmlDoc } DocType;
     
     DocType doctype;

    private:
     lem::Ptr<Node> root;
     UFString header; // ������ ����������� ��� ���� <?xml version="x.x" ... ?> ��� <!DOCTYPE html ...
     UFString charset;

     Parser( const Parser& );
     void operator=( const Parser& );
     

     void Load_Content( lem::Char_Stream::WideStream *reader );

     void Break_Tag(
                    const UFString &tag,
                    UFString &name,
                    lem::Collect< std::pair<UFString,UFString> > &attrs,
                    lem::UFString &body
                   ) const;
                   
     void Read_Tag( lem::Char_Stream::WideStream *reader, UFString &tag ) const;

     Node* Load_Node( lem::Char_Stream::WideStream *reader ) const;

    public:
     Parser(void);
     virtual ~Parser(void) {} 

     void LoadString( const wchar_t *str );

     virtual void Load( const lem::Path &path );
     virtual void Load( lem::Ptr<Stream> bin );
     virtual void Load( lem::Char_Stream::WideStream *reader );

//        lem::Ptr<Node> GetRoot(void)       { return root; }

     const Node & GetRoot(void) const { return *root; }
     
     static void Split_Path( const UFString &path, lem::Collect<UFString> &steps );

     const Node* Find_By_Path( const UFString &Path ) const;
     lem::UFString Get_By_Path( const UFString &Path ) const;

     const Node* Find_By_Path( const lem::Collect<UFString> &Steps ) const;
   };

   
  } // namespace Xml
 } // namespace lem

#endif
