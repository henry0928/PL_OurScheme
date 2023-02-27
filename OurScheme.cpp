# include <iostream> // cout, endl
# include <string> // string, find_last_of, substr
# include <string.h> // string, find_last_of, substr
# include <vector> // vector, push_back
# include <cstdlib> // system, atoi
# include <sstream>
# include <stdio.h>
# include <ctype.h> // isdigit() 

using namespace std; 

enum TokenType {
  LEFT_PAREN, RIGHT_PAREN, INT, STRING, DOT, 
  FLOAT, NIL, T, QUOTE, SYMBOL, JUMP, SKIP, ERROR,
  NOTHING, NOTSYMBOL
};

struct Node {
  char token_str[200] ;
  int token_type ;
  bool dot ;
  bool is_left ;
  bool is_right ; 
  Node * left ;
  Node * right ;
};

struct Token_struct {
  char token_str[200] ;
  int token_type ;
  int line ;
  int column ;
};

struct Define_struct {
  char token_str[200] ;
  int token_type ; 
  Node * ptr ;
};

typedef char Str2[1000] ;
typedef char Str[200] ;
typedef Node* Pointer ;
vector <Token_struct> g_print_done ;
vector <Token_struct> g_combine_done ;
vector <Token_struct> g_s_exp_done ;
vector <Token_struct> g_token_line ; 
vector <char> g_temptoken ;
vector <Define_struct> g_define_line ; 


bool glist = true ;
bool g_define = false ;
int g_level = 0 ;
bool g_correct = false ;
int g_column = 0 ;
bool g_fuck_EOF = false ;
int g_space_num = 0 ;
int g_line = 0 ;
bool g_is_dot = false ;
bool g_error_right_paren = false ;
void Scanner() ;
int Check_type( Token_struct token, bool is_string ) ;
bool Int_num( char str[200] ) ;
bool Float_num( char str[200] ) ;
void Parser() ;
bool ATOM( Token_struct token ) ;
Token_struct Get_token() ;
bool Peek_token( Token_struct &token ) ;
bool S_EXP() ;
bool Peek_combine( Token_struct &combine_token ) ;
void Combine() ;
void Pretty_print( Node * ptr, bool is_whitespace ) ;
bool Build_node( Pointer &agent ) ;
Token_struct Get_combine() ;
void Clean_up( Token_struct &temp ) ;
void Error_control() ;
void Control_nil() ;
void Evaul( Pointer &agent, Pointer travel ) ;
Pointer Cons( Pointer agent ) ;
Pointer Quote( Pointer agent ) ;
bool Define( Pointer agent ) ;
void Clean_up_str( Define_struct &temp ) ;
Pointer Car( Pointer agent ) ;
Pointer Cdr( Pointer agent ) ;
Pointer List( Pointer agent, bool is_while  ) ;
Pointer Is_atom( Pointer agent ) ;
bool Is_command( Pointer &temp ) ;
bool Command( Str temp ) ;
bool Check_define( Pointer &temp ) ;
Pointer Is_integer( Pointer agent ) ;
Pointer Is_string( Pointer agent ) ;
Pointer Is_boolean( Pointer agent ) ;
Pointer Is_symbol( Pointer agent ) ;
Pointer Is_number( Pointer agent ) ;
Pointer Is_null( Pointer agent ) ;
void Check_list( Pointer temp ) ;
Pointer Is_list( Pointer agent ) ;
Pointer Is_pair( Pointer agent ) ;
Pointer Plus( Pointer agent ) ;
Pointer Minus( Pointer agent ) ;
Pointer Multi( Pointer agent ) ;
Pointer Div( Pointer agent ) ;
Pointer Not( Pointer agent ) ;
Pointer And( Pointer agent ) ;
Pointer Or( Pointer agent ) ;
Pointer Bigger( Pointer agent ) ;
Pointer Bigger_equal( Pointer agent ) ;
Pointer Smaller( Pointer agent ) ;
Pointer Smaller_equal( Pointer agent ) ;
Pointer Equal( Pointer agent ) ;
Pointer String_append( Pointer agent ) ;
Pointer String_bigger( Pointer agent ) ;
Pointer String_smaller( Pointer agent ) ;
Pointer String_equal( Pointer agent ) ;
void Check_content( Str2 &str, Pointer agent ) ;
Pointer Node_equal( Pointer agent ) ;
Pointer Node_eqv( Pointer agent ) ;
void Add_paren( Pointer temp ) ;
Pointer Clean_env( Pointer agent ) ;
Pointer Condition_if( Pointer agent ) ;
Pointer Begin( Pointer agent ) ;
Pointer Condition( Pointer agent ) ;
void Exit( Pointer agent ) ;
void Not_list( Pointer temp ) ;
void Not_list_cond( Pointer temp ) ;
void Change_name( Pointer temp ) ;

int main() {
  int testnum = 0 ;
  cin >> testnum ;
  cout << "Welcome to OurScheme!" << "\n\n" ;
  Scanner() ;  
  Parser() ;
  //system("PAUSE");
  g_print_done.clear() ;
  g_combine_done.clear() ;
  g_s_exp_done.clear() ;
  g_token_line.clear() ;
  g_define_line.clear() ;
} // main()

void Clean_up( Token_struct &temp ) {
  int i = 0 ;
  while ( i < 200 ) {
    temp.token_str[i] = '\0' ;
    i = i + 1 ;
  } // while 
  
} // Clean_up()


bool Int_num( char str[200] ) {
    
  int i = 1 ;
  if ( strlen( str ) == 1 ) {
    if ( !isdigit( str[0] ) )
      return false ;
    else 
      return true ;
  } // if 
  
  else {
    while ( str[i] != '\0' ) {
      if ( !isdigit( str[i] ) )
        return false ;
      i = i + 1 ;
    } // while 
  } // else
   
  return true ;

} // Int_num()

bool Float_num( char str[200] ) {
  int i = 1 ;
  int count = 0 ;
  bool  correct = false ;
  if ( str[0] == '.' ) 
    count = 1 ;
  if ( strlen( str ) == 2 && isdigit( str[0] ) && str[1] == '.' )
    correct = true ;      
  while ( str[i] != '\0' ) {
    if ( str[i] == '.' )
      count = count + 1 ;
    else if ( isdigit( str[i] ) )
      correct = true ;
    else 
      return false ;    
    i = i + 1 ;
  } // while
  
  if ( count == 1 && correct ) 
    return true ;
  else 
    return false ;
} // Float_num()

int Check_type( Token_struct token, bool is_string ) {
  
  if ( is_string == true )
    return STRING ;
  else if ( strcmp( token.token_str, "(" ) == 0 ) 
    return LEFT_PAREN ;
  else if ( strcmp( token.token_str, ")" ) == 0 )
    return RIGHT_PAREN ;
  else if ( strcmp( token.token_str, "." ) == 0 )
    return DOT ;
  else if ( strcmp( token.token_str, "t" ) == 0 || strcmp( token.token_str, "#t" ) == 0 ) 
    return T ;
  else if ( strcmp( token.token_str, "nil" ) == 0 || strcmp( token.token_str, "#f" ) == 0 
            || strcmp( token.token_str, "()" ) == 0 )
    return NIL ;
  else if ( strcmp( token.token_str, "\'" ) == 0 )
    return QUOTE ;
  else if ( token.token_str[0] == '+' || token.token_str[0] == '-' ||
            isdigit( token.token_str[0] ) || token.token_str[0] == '.' ) {
    if ( Int_num( token.token_str ) == true && token.token_str[0] != '.' )
      return INT ;
    else if ( Float_num( token.token_str ) == true )  
      return FLOAT ;
    else
      return SYMBOL ;
  } // else if 
  else 
    return SYMBOL ;
} // Check_type()

void Scanner() {
  int i = 0, j = 0 ;
  int line = 1 ;
  int column = 0 ;
  bool is_enter = false ;
  bool is_escape = false ;
  bool end_while = false ;
  bool is_string = false ; 
  char for_string[200] ;
  char peek_char = '\0' ;
  Token_struct error_string  ;
  Token_struct henry ;
  Token_struct jump_line ;
  Clean_up( henry ) ;
  error_string.token_type = ERROR ;
  jump_line.token_type = JUMP ;
  char temp = '\0' ;
  char trash = '\0' ;
  temp = cin.get() ;
  column = column + 1 ;
  while ( temp != 'Z' ) {  // there is no EOF in cmd!
    if ( temp == ' ' || temp == '\n' || temp == '\t' ) {
      if ( g_temptoken.empty() == false ) {
        while ( i < g_temptoken.size() ) {
          henry.token_str[i] = g_temptoken[i] ;
          i = i + 1 ;
        } // while 

        henry.token_str[i] = '\0' ;
        i = 0 ;
        g_temptoken.clear() ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column - 1 ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        if ( temp == '\n' )
          column = 0 ;
        else 
          column = 1 ;
        Clean_up( henry ) ;
      } // if
 
      if ( temp == '\n' ) {
        column = 0 ;
        line = line + 1 ;
        g_token_line.push_back( jump_line ) ;
      } // if  
    
      g_temptoken.clear() ;
    } // if 
    else if ( temp == '(' || temp == ')' || temp == '\'' ) {
      peek_char = cin.peek() ;  
      if ( temp == '(' && peek_char == ')' ) {  // �B�z()�Ofalse������
        if ( g_temptoken.empty() == false ) {
          while ( i < g_temptoken.size() ) {
            henry.token_str[i] = g_temptoken[i] ;
            i = i + 1 ;
          } // while 

          henry.token_str[i] = '\0' ; 
          i = 0 ;
          g_temptoken.clear() ;
          henry.token_type = Check_type( henry, is_string ) ;
          henry.column = column - 1 ;
          henry.line = line ;
          g_token_line.push_back( henry ) ;
          column = 0 ;
          Clean_up( henry ) ;
        } // if 
        
        henry.token_str[0] = '(' ;
        henry.token_str[1] = ')' ;
        henry.token_str[2] = '\0' ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column + 1 ; // ()�S�����p 
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 0 ;
        Clean_up( henry ) ;
        temp = cin.get() ; // ��)��J�i�� 
         
      } // if 
      else if ( g_temptoken.empty() == false ) {  // (2��token) �@�Ӧۤv�@�ӫe����  
        while ( i < g_temptoken.size() ) {
          henry.token_str[i] = g_temptoken[i] ;
          i = i + 1 ;
        } // while 

        henry.token_str[i] = '\0' ;
        i = 0 ;
        g_temptoken.clear() ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column - 1 ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 1 ;
        Clean_up( henry ) ;

        henry.token_str[0] = temp ;
        henry.token_str[1] = '\0' ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = 1 ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 0 ;
        Clean_up( henry ) ;

      } // else if
      else {  // (1��token) �ۤv�� 
        henry.token_str[0] = temp ;
        henry.token_str[1] = '\0' ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 0 ;
        Clean_up( henry ) ;
      } // else 	

      g_temptoken.clear() ;
    } // else if 
    else if ( temp == '"' ) {
      if ( g_temptoken.empty() == false ) {
        while ( i < g_temptoken.size() ) {
          henry.token_str[i] = g_temptoken[i] ;
          i = i + 1 ;
        } // while 

        henry.token_str[i] = '\0' ;
        i = 0 ;
        g_temptoken.clear() ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column - 1 ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 1 ;
        Clean_up( henry ) ; 
      } // if 

      char escape = '\0' ;
      j = 0 ;
      end_while = false ;
      
      while ( cin.peek() != EOF && temp != '\n' && end_while == false  ) { 
        temp = cin.get() ;
        peek_char = cin.peek() ;
        if ( temp != '\n' )
          column = column + 1 ; 
        if ( escape == '\\' && temp == '\\' && peek_char == '\\' ) {
          temp = cin.get() ;
          j = j + 1 ;
        } // if 
        else if ( escape == '\\' && temp == '\\' && peek_char == 't' ) 
          is_escape = true ;
        else if ( escape == '\\' && temp == '\\' && peek_char == 'n' ) 
          is_escape = true ;
        else if ( escape == '\\' && temp == 'n' && is_escape == false )
          for_string[j-1] = '\n' ;
        else if ( escape == '\\' && temp == 't' && is_escape == false )
          for_string[j-1] = '\t' ;
        else if ( escape == '\\' && temp == '"' && is_escape == false ) 
          for_string[j-1] = '"' ;     
        else if ( escape == '\\' && temp == '\\' && is_escape == false ) 
          for_string[j-1] = '\\' ;
        else if ( temp == '"' ) {
          is_escape = false ;
          is_string = true ;
          for_string[j] = '\0' ;
          strcpy( henry.token_str, for_string ) ;
          henry.token_type = Check_type( henry, is_string ) ;
          henry.column = column ;
          henry.line = line ;
          g_token_line.push_back( henry ) ;
          peek_char = cin.peek() ;
          if ( peek_char == '"' )
            column = 0 ;
          else 
            column = 1 ;
          end_while = true ;
        } // else if 
        else {
          is_escape = false ;
          for_string[j] = temp ;
          j = j + 1 ;
        } // else  

        escape = temp ;
        
      } // while
      
      is_enter = false ;
      if ( end_while == false ) {
        int p = 0 ;
        while ( p < j ) {
          if ( for_string[p] == '\n' )  
            is_enter = true ;
          p = p + 1 ;
        } // while 
        
        for_string[j] = '\0' ;
        strcpy( error_string.token_str, for_string ) ;
        if ( is_enter == false )
          error_string.column = column - 1 ;
        else 
          error_string.column = column ; 
               
        g_token_line.push_back( error_string ) ;
      } // if 
      
      is_string = false ;
      j = 0 ;
      int a = 0 ;
      while ( a < 200 ) {
        for_string[a] = '\0' ;
        a = a + 1 ;
      } // while 
      
      Clean_up( error_string ) ;
      Clean_up( henry ) ;
      
      if ( temp == '\n' ) {
        column = 0 ; 
        g_token_line.push_back( jump_line ) ;
      } // if  
      
      g_temptoken.clear() ;
  
    } // else if 
    else if ( temp == ';' ) {
      if ( g_temptoken.empty() == false ) {
        while ( i < g_temptoken.size() ) {
          henry.token_str[i] = g_temptoken[i] ;
          i = i + 1 ;
        } // while 
        
        i = 0 ;
        g_temptoken.clear() ;
        henry.token_type = Check_type( henry, is_string ) ;
        henry.column = column - 1 ;
        henry.line = line ;
        g_token_line.push_back( henry ) ;
        column = 0 ;
        Clean_up( henry ) ;
      } // if 
      
      while ( cin.peek() != EOF && temp != '\n' ) {
        temp = cin.get() ;
        column ++ ;
      } // while
      
      if ( temp == '\n' ) {
        column = 0 ;
        line = line + 1 ;
        g_token_line.push_back( jump_line ) ;
      } // if 
      
      g_temptoken.clear() ;  
    } // else if 
    else 
      g_temptoken.push_back( temp ) ;

    temp = cin.get() ;  
    column = column + 1 ;
        
  } // while
  
  Control_nil() ;  
  
} // Scanner()

void Control_nil() {
    
  int i = 0 ;
  int j = 0 ;
  while ( i < g_token_line.size() ) {
    if ( g_token_line[i].token_type == LEFT_PAREN ) {
      j = i + 1 ;
      while ( g_token_line[j].token_type == JUMP )  
        j = j + 1 ;
      if ( g_token_line[j].token_type == RIGHT_PAREN ) {
        g_token_line[j].token_type = JUMP ;
        g_token_line[i].token_type = NIL ; 
        g_token_line[i].column = g_token_line[i].column + 1 ;
        strcpy( g_token_line[i].token_str, "()" ) ;
      } // if      
    } // if  
        
    i = i + 1 ;
    
  } // while 
    
} // Control_nil()

Token_struct Get_token() {
  Token_struct token ;
  token = g_token_line.front() ;
  token.line = g_line ;
  if ( g_token_line.size() == 0 )
    return token;
  g_token_line.erase( g_token_line.begin() ) ;
  return token ; 
  
} // Get_token()

bool Peek_token( Token_struct &token ) {
  Token_struct temp_token ;
  while ( g_token_line.size() > 0 && g_token_line.front().token_type == JUMP ) { // ���L���� 
    if ( strcmp( g_token_line.front().token_str, ")" ) == 0 )
      g_token_line.erase( g_token_line.begin() ) ;
    else {
      g_token_line.erase( g_token_line.begin() ) ;
      g_line = g_line + 1 ;
      g_column = 0 ;
    } // else 
    
  } // while  	
  
  temp_token = g_token_line.front() ;
  if ( g_line == 0 )
    g_line = 1 ;  
  temp_token.line = g_line ;   
  if ( g_token_line.size() == 0 && temp_token.token_type == JUMP )
    return false ;
  if ( g_token_line.size() > 0 ) {
    token = temp_token ;
    return true ;
  } // if
  else
    return false ;

} // Peek_token()

bool ATOM( Token_struct token ) {
  if ( token.token_type == SYMBOL || token.token_type == INT || token.token_type == STRING 
       || token.token_type == FLOAT || token.token_type == NIL || token.token_type == T )
    return true ;
  else 
    return false ;  

} // ATOM()

bool S_EXP() {
  Token_struct token, peek ;
  if ( Peek_token( peek ) == true ) 
    ;
  else  
    return false ; // eof�����D 
      
  if ( ATOM( peek ) == true ) {
    g_error_right_paren = false ;  
    token = Get_token() ;
    g_s_exp_done.push_back( token ) ;
    g_column = g_column + token.column ;
    return true ;
  } // if  
  
  else if ( peek.token_type == LEFT_PAREN ) {
    g_error_right_paren = false ;  
    token = Get_token() ; 
    g_column = g_column + token.column ;  
    g_s_exp_done.push_back( token ) ;
    if ( S_EXP() == true ) {
      g_error_right_paren = true ;  
      while ( S_EXP() == true ) {
        g_error_right_paren = true ;
      } // while 

      if ( g_correct == true && g_is_dot == true ) { 
        g_correct = false ;
        g_is_dot = false ; // reset g_is_dot
        token = Get_token() ;
        g_s_exp_done.push_back( token ) ;
        g_error_right_paren = false ;
        if ( S_EXP() == true ) {  
          if ( Peek_token( peek ) == true ) {
            if ( peek.token_type == RIGHT_PAREN ) {
              token = Get_token() ;
              // g_column = g_column + token.column ;
              g_s_exp_done.push_back( token ) ;
              return true ;
            } // if 
            else {
              g_column = g_column + peek.column ;    
              Error_control() ;
              if ( peek.token_type == STRING ) {
                cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ;  
                cout << peek.line ;
                cout << " Column " << g_column - strlen( peek.token_str ) - 1 << " is >>" << "\"" ;
                cout << peek.token_str << "\"" << "<<" ; 
                cout << "\n\n" ;
              } // if
              else if ( peek.token_type == ERROR ) {
                cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ;  
                cout << peek.line ;
                cout << " Column " << g_column - strlen( peek.token_str ) + 1 << " is >>" << "\"" ;
                cout << peek.token_str << "<<" ; 
                cout << "\n\n" ;  
              } // else if 
              else if ( strlen( peek.token_str ) > 1 ) {
                cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ;  
                cout << peek.line ;
                cout << " Column " << g_column - strlen( peek.token_str ) + 1 << " is >>" ;
                if ( strcmp( peek.token_str, "()" ) == 0 )
                  cout << peek.token_str[0] << "<<" ;
                else    
                  cout << peek.token_str << "<<" ; 
                cout << "\n\n" ;  
              } // else if 
              else {
                cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ; 
                cout << peek.line ;
                cout << " Column " << g_column << " is >>" << peek.token_str << "<<" << "\n\n" ;
              } // else 
              
              g_column = 0 ;
              return false ;
            } // else 
          } // if 
          else 
            return false ; // eof�����D  
        } // if 
        else 
          return false ; 
      } // if 
      else if ( g_correct == true ) {
        g_correct = false ;  
        if ( Peek_token( peek ) == true ) {
          if ( peek.token_type == RIGHT_PAREN ) {
            token = Get_token() ;  
            g_s_exp_done.push_back( token ) ;
            return true ;
          } // if 
          else { 
            g_column = g_column + peek.column ;
            Error_control() ;
            if ( peek.token_type == STRING ) {
              cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ;
              cout << peek.line ;
              cout << " Column " << g_column - strlen( peek.token_str ) - 1 << " is >>" << "\"" ; 
              cout << peek.token_str << "\"" << "<<" ; 
              cout << "\n\n" ;
            } // if
            else if ( strlen( peek.token_str ) > 1 ) {
              cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ; 
              cout << peek.line ;
              cout << " Column " << g_column - strlen( peek.token_str ) + 1 << " is >>" ;
              if ( strcmp( peek.token_str, "()" ) == 0 )
                cout << peek.token_str[0] << "<<" ;
              else    
                cout << peek.token_str << "<<" ; 
              cout << "\n\n" ;  
            } // else if
            else {
              cout << "> " << "ERROR (unexpected token) : ')' expected when token at Line " ; 
              cout << peek.line ;
              cout << " Column " << g_column << " is >>" << peek.token_str << "<<" << "\n\n" ;
            } // else
             
            g_column = 0 ; 
            return false ;
          } // else 
        } // if 
        else 
          return false ; // eof�����D 
          
      } // else if
      else 
        return false ;

    } // if
    else 
      return false ; 
  } // else if 
  
  else if ( peek.token_type == QUOTE ) {
    g_error_right_paren = false ;  
    token = Get_token() ;
    g_column = g_column + token.column ;
    g_s_exp_done.push_back( token ) ;
    if ( S_EXP() == true ) 
      return true ;
    else 
      return false ;
  } // else if 	
  else if ( peek.token_type == ERROR ) {
    Error_control() ;
    cout << "> " << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << peek.line ;
    cout << " Column " << peek.column + g_column + 1 << "\n\n" ;
    g_column = 0 ;   
    g_error_right_paren = false ;
    g_correct = false ;
    return false ; 
  } // else if 
  else {
    if ( g_error_right_paren == true && peek.token_type == DOT ) {
      g_error_right_paren = false ;  
      g_column = g_column + peek.column ;  
      g_is_dot = true ;
      g_correct = true ;
      return false ;
    } // if
    else if ( g_error_right_paren == true && peek.token_type == RIGHT_PAREN ) {
      g_column = g_column + peek.column ;   
      g_error_right_paren = false ; 
      g_correct = true ; 
      return false ;
    } // else if 
    else {
      g_error_right_paren = false ;
      g_column = g_column + peek.column ; 
      Error_control() ;
      g_correct = false ;
      if ( peek.token_type == STRING ) {
        cout << "> " << "ERROR (unexpected token) : atom or '(' expected when token at Line " << peek.line ;
        cout << " Column " << g_column - strlen( peek.token_str ) - 1 << " is >>" << "\"" ;
        cout << peek.token_str << "\"" << "<<" << "\n\n" ;
      } // if
      else if ( strlen( peek.token_str ) > 1 ) {
        cout << "> " << "ERROR (unexpected token) : atom or '(' expected when token at Line " ; 
        cout << peek.line ;
        cout << " Column " << g_column - strlen( peek.token_str ) + 1 << " is >>" ;
        cout << peek.token_str << "<<" ; 
        cout << "\n\n" ;  
      } // else if 
      else {
        cout << "> " << "ERROR (unexpected token) : atom or '(' expected when token at Line " << peek.line ;
        cout << " Column " << g_column << " is >>" << peek.token_str << "<<" << "\n\n" ;
      } // else 
      
      g_column = 0 ;
      return false ;
    } // else 
      
  } // else 

} // S_EXP()

void Error_control() {

  while ( g_token_line.size() > 0 && g_token_line.front().token_type != JUMP )
    g_token_line.erase( g_token_line.begin() ) ;
  
  if ( g_token_line.size() > 0 && g_token_line.front().token_type == JUMP )  
    g_token_line.erase( g_token_line.begin() ) ; 
  
  g_line = 1 ; 
    
} // Error_control()

void Parser() {
  Token_struct peek ;
  int k = 0 ; 
  while ( g_token_line.empty() == false && g_fuck_EOF == false ) {
    if ( S_EXP() == true ) { 
      g_column = 0 ;
      g_line = 0 ;  
      int size = g_s_exp_done.size() ;
 
      if ( g_s_exp_done.size() == 3 && 
           ( strcmp( g_s_exp_done[size-1].token_str, ")" ) == 0 
             && strcmp( g_s_exp_done[size-2].token_str, "exit" ) == 0
             && strcmp( g_s_exp_done[size-3].token_str, "(" ) == 0 ) )                       
        g_fuck_EOF = true ;
      else if ( g_s_exp_done.size() == 5 && 
                ( strcmp( g_s_exp_done[size-5].token_str, "(" ) == 0 
                  && strcmp( g_s_exp_done[size-4].token_str, "exit" )  == 0
                  && strcmp( g_s_exp_done[size-3].token_str, "." ) == 0
                  && g_s_exp_done[size-2].token_type == NIL 
                  && strcmp( g_s_exp_done[size-1].token_str, ")" ) == 0 ) )
        g_fuck_EOF = true ;   
      else 
        g_fuck_EOF = false ;    
        
      if ( g_fuck_EOF == true )
        cout << "> " << "\n" << "Thanks for using OurScheme!" ;
      else { 
        Combine() ;
        Node * agent  ;
        g_level = 0 ;
        if ( Build_node( agent ) == true ) {
          cout << "> " ;
          Node * ans ;
          try {
            Evaul( ans, agent ) ;
            if ( g_define == false )
              Pretty_print( ans, false ) ;
            else
              ; 
          } // try
          catch ( const char * str ) {
            if ( strcmp( str, "NO" ) != 0 && strcmp( str, "go home" ) != 0  ) 
              cout << "\n" ;        
          } // catch
          if ( g_fuck_EOF == false )  
            cout << "\n" ;
          g_level = 0 ;
          g_space_num = 0 ;
          g_s_exp_done.clear() ;
          g_combine_done.clear() ;
          g_print_done.clear() ;
        } // if 
        else 
          cout << "wrong" ;
      } // else 
    } // if  
    else {    
      g_s_exp_done.clear() ; 
    } // else 
  } // while 
  
  if ( g_fuck_EOF == false )
    cout << "> ERROR (no more input) : END-OF-FILE encountered\nThanks for using OurScheme!" ;

} // Parser()

void Combine() {
  int i = 0 ;
  int j = 0 ;
  int p = 0 ;
  int k = 0 ;
  int size = g_s_exp_done.size() ;
  int count = 0 ;
  while ( i < size - 1  ) {
    if ( g_s_exp_done[i].token_type == DOT && g_s_exp_done[i+1].token_type == LEFT_PAREN ) {
      p = i ;
      while ( p < size  ) {
        if ( g_s_exp_done[p].token_type == LEFT_PAREN )
          count = count + 1 ;

        p = p + 1 ;  
      } // while 
      
      g_s_exp_done[i].token_type = SKIP ; 
      i = i + 1 ;
      g_s_exp_done[i].token_type = SKIP ;    
      j = i + 1 ;
      while ( j < size ) {
        if ( g_s_exp_done[j].token_type == RIGHT_PAREN  )
          count = count - 1 ;
        if ( count == 0 ) {
          g_s_exp_done[j].token_type = SKIP ; 
          j = size ;
        } // if 
        
        j = j + 1 ;
      } // while 
    } // if  
    else if ( g_s_exp_done[i].token_type == DOT && g_s_exp_done[i+1].token_type == NIL ) { 
      g_s_exp_done[i].token_type = SKIP ; 
      i = i + 1 ;
      g_s_exp_done[i].token_type = SKIP ; 
    } // else if 
    else 
      ;
    i = i + 1 ;   
      
  } // while     
    
  k = 0 ;  
  while ( k < size ) {
    if ( g_s_exp_done[k].token_type != SKIP )
      g_combine_done.push_back( g_s_exp_done[k] ) ;    
    k = k + 1 ;
  } // while 
  
  k = 0 ; 
  while ( k < g_combine_done.size() ) {
    g_print_done.push_back( g_combine_done[k] ) ;
    k = k + 1 ;  
  } // while 
  
  
  k = 0 ; 
  while ( k < g_combine_done.size() ) {
    // cout <<g_combine_done[k].token_str ;
    k = k + 1 ;  
  } // while   

} // Combine()


Token_struct Get_combine() {
  Token_struct combine_token ;
  combine_token = g_combine_done.front() ;
  g_combine_done.erase( g_combine_done.begin() ) ;
  return combine_token ;
  
} // Get_combine()

bool Peek_combine( Token_struct &combine_token ) {

  if ( g_combine_done.size() > 0 ) {
    combine_token = g_combine_done.front() ;
    return true ;
  } // if 
  else 
    return false ;

} // Peek_combine()


bool Build_node( Pointer &agent ) {
    
  agent = new Node ;
  agent->token_type = SKIP ;
  agent->left = NULL ;
  agent->right = NULL ;
  Node * run = agent ;
  Node * temp = new Node ;
  temp->token_type = SKIP ;
  temp->left = NULL ;
  temp->right = NULL ;
  Token_struct peek, combine_token ;
  if ( Peek_combine( peek ) == true ) 
    ;
  else
    return false ;
  
  if ( ATOM( peek ) == true ) {
    combine_token = Get_combine() ;  
    agent->is_left = false ;
    agent->token_type = combine_token.token_type ;
    strcpy( agent->token_str, combine_token.token_str ) ;
    return true ;
  } // if  
  
  else if ( peek.token_type == LEFT_PAREN ) {
    combine_token = Get_combine() ;
    run->is_left = true ;     
    if ( Build_node( temp ) == true ) {
      run->left = temp ;
      while ( Build_node( temp ) == true ) {
        run->right = new Node ;
        run = run-> right ;
        run->token_type = SKIP ;
        run->is_left = false ; 
        run->left = temp ;  
      } // while
 
      if ( g_is_dot == true ) {
        g_is_dot = false ; // reset g_is_dot
        combine_token = Get_combine() ;
        run->token_type = combine_token.token_type ;
        strcpy( run->token_str, combine_token.token_str ) ; 
        if ( Build_node( temp ) == true ) {
          run->right = temp ;  
          if ( Peek_combine( peek ) == true ) {
            if ( peek.token_type == RIGHT_PAREN ) {
              combine_token = Get_combine() ;
              return true ;
            } // if 
            else 
              return false ;
          } // if 
          else 
            return false ; // eof�����D  
        } // if 
        else 
          return false ; 
      } // if 
      else {
        if ( Peek_combine( peek ) == true ) {
          if ( peek.token_type == RIGHT_PAREN ) {
            combine_token = Get_combine() ;
            run->right = NULL ;    
            return true ;
          } // if 
          else 
            return false ;
        } // if 
        else 
          return false ; // eof�����D      
      } // else 
          
    } // if 
    else 
      return false ;
        
  } // else if 
  else if ( peek.token_type == QUOTE ) {
    combine_token = Get_combine() ;
    run->token_type = combine_token.token_type ;
    strcpy( run->token_str, combine_token.token_str ) ;
    run->is_left = true ; 
    run->right = NULL ;    
    if ( Build_node( temp ) == true ) {
      run->left = temp ;
      return true ;    
    } // if 
    else 
      return false ;
      
  } // else if 
  else {
    if ( peek.token_type == DOT ) 
      g_is_dot = true ;
    return false ;
  } // else

} // Build_node()

void Pretty_print( Node * ptr, bool is_whitespace ) {
  Token_struct temp ;
  bool henry = false ;
  int i = 0 ;
  if ( ptr == NULL ) 
    return ;
  if ( 1 ) { 
    if ( ptr->is_left == true ) {
      if ( is_whitespace == true )  
        cout << "( " ;
      else {
        while ( i < g_space_num ) {
          cout << "  " ;
          i = i + 1 ;    
        } // while   
        
        i = 0 ;
        cout << "( " ;
      } // else   
      
      henry = true ;  
      g_space_num = g_space_num + 1 ; 
    } // if
     
    if ( ptr->token_type == QUOTE ) {
      cout << "quote" << "\n" ;
      henry = false ;
    } // if
      
    Pretty_print( ptr->left, henry ) ;
    
    temp.token_type = ptr->token_type ;
    if ( ATOM( temp ) || temp.token_type == DOT ) {
      if ( is_whitespace == false || temp.token_type == DOT ) {
        while ( i < g_space_num ) {
          cout << "  " ;
          i = i + 1 ;    
        } // while
        
        i = 0 ;
        if ( temp.token_type == DOT || is_whitespace == true ) 
          henry = false ;
      } // if 
      else 
        henry = false ;
        
      if ( ptr->token_type == T )  
        cout << "#t" << "\n" ;  
      else if ( ptr->token_type == NIL )
        cout << "nil" << "\n" ;  
      else if ( ptr->token_type == STRING )
        cout << "\"" << ptr->token_str << "\"" << "\n" ;  
      else if ( ptr->token_type == INT ) 
        cout << atoi( ptr->token_str ) << "\n" ;
      else if ( ptr->token_type == FLOAT ) {  
        printf( "%.3f", atof( ptr->token_str ) ) ;
        cout << "\n" ;
      } // if   
      else 
        cout << ptr->token_str << "\n" ;
    } // if

    Pretty_print( ptr->right, henry ) ;
    
    if ( ptr->is_left == true ) {
      g_space_num = g_space_num - 1 ;
      i = 0 ;
      while ( i < g_space_num ) {
        cout << "  " ;
        i = i + 1 ;    
      } // while
 
      i = 0 ;  
      cout << ")" << "\n" ;  
    } // if 
    
  } // if 
  
} // Pretty_print()

void Add_paren( Pointer temp ) {
    
  if ( temp->left == NULL && temp->right == NULL ) 
    ;
  else {
    temp->is_left = true ;
  } // else 
    
} // Add_paren()

void Evaul( Pointer &agent, Pointer travel ) {

  bool is_def = false ;
  bool goin = false ;
  g_define = false ;    
  g_level = g_level + 1 ;
  Token_struct temp ;
  Node temp_ptr ;
  Node * run = travel ;
  temp.token_type = run->token_type ;
  
  if ( ATOM( temp ) == true ) {
    if ( temp.token_type == SYMBOL ) {
      if ( Check_define( run ) == false && Is_command( run ) == false ) {
        cout << "ERROR (unbound symbol) : " << run->token_str ;
        throw "" ;
      } // if
    } // if 
    
    Check_define( run ) ;
    Is_command( run ) ;
    Add_paren( run ) ;
    agent = run ;   
  } // if
  else {
    if ( run->token_type == QUOTE ) {
      goin = true ;
      agent = Quote( run ) ;
    } // if
    else if ( run->left->token_type == SKIP ) {
      Evaul( run->left, run->left ) ;    
    } // else if
    else 
      ; 
    
    Not_list( run ) ;
    if ( goin == false ) {
      Check_define( run->left ) ;
      if ( run->left->token_type == QUOTE ) {
        run->left = Quote( run->left ) ;
        Evaul( agent, run ) ;
      } // if
      else if ( strcmp( run->left->token_str, "quote" ) == 0 ) {  
        if ( goin == true )
          goin = false ;
        else if ( strcmp( run->left->token_str, "quote" ) == 0 ) {
          if ( run->right == NULL ) {
            cout << "ERROR (incorrect number of arguments) : quote" ;
            throw "" ; 
          } // if
 
          agent = Quote( run->right ) ; 
        } // else if 
      } // else if  
      else if ( strcmp( run->left->token_str, "cons" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure cons>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : cons" ;
          throw "" ; 
        } // if
      
        agent = Cons( run->right ) ; 
      } // else if 
      else if ( strcmp( run->left->token_str, "define" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure define>" ) == 0 ) {
        g_define = Define( run ) ;    
      } // else if 
      else if ( strcmp( run->left->token_str, "car" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure car>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : car" ;
          throw "" ; 
        } // if
      
        agent = Car( run->right ) ;
      } // else if
      else if ( strcmp( run->left->token_str, "cdr" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure cdr>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : cdr" ;
          throw "" ; 
        } // if
      
        agent = Cdr( run->right ) ;
      } // else if 
      else if ( strcmp( run->left->token_str, "list" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure list>" ) == 0 ) {
        if ( run->right == NULL ) 
          agent = List( run, true ) ;
        else   
          agent = List( run->right, false ) ;
      } // else if
      else if ( strcmp( run->left->token_str, "atom?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure atom?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : atom?" ;
          throw "" ; 
        } // if
      
        agent = Is_atom( run->right ) ;
      } // else if 
      else if ( strcmp( run->left->token_str, "integer?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure integer?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : integer?" ;
          throw "" ; 
        } // if
      
        agent = Is_integer( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "string?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure string?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : string?" ;
          throw "" ; 
        } // if
      
        agent = Is_string( run->right ) ;   
      } // else if 
      else if ( strcmp( run->left->token_str, "boolean?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure boolean?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : boolean?" ;
          throw "" ; 
        } // if
            
        agent = Is_boolean( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "symbol?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure symbol?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : symbol?" ;
          throw "" ; 
        } // if

        agent = Is_symbol( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "number?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure number?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : number?" ;
          throw "" ; 
        } // if

        agent = Is_number( run->right ) ;   
      } // else if 
      else if ( strcmp( run->left->token_str, "real?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure real?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : real?" ;
          throw "" ; 
        } // if

        agent = Is_number( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "null?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure null?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : null?" ;
          throw "" ; 
        } // if

        agent = Is_null( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "list?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure list?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : list?" ;
          throw "" ; 
        } // if

        agent = Is_list( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "pair?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure pair?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : pair?" ;
          throw "" ; 
        } // if

        agent = Is_pair( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "+" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure +>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : +" ;
          throw "" ; 
        } // if

        agent = Plus( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "-" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure ->" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : -" ;
          throw "" ; 
        } // if

        agent = Minus( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "*" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure *>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : *" ;
          throw "" ; 
        } // if

        agent = Multi( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "/" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure />" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : /" ;
          throw "" ; 
        } // if

        agent = Div( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "not" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure not>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : not" ;
          throw "" ; 
        } // if

        agent = Not( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "and" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure and>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : and" ;
          throw "" ; 
        } // if

        agent = And( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "or" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure or>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : or" ;
          throw "" ; 
        } // if

        agent = Or( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, ">" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure >>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : >" ;
          throw "" ; 
        } // if

        agent = Bigger( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, ">=" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure >=>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : >=" ;
          throw "" ; 
        } // if

        agent = Bigger_equal( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "<" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure <>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : <" ;
          throw "" ; 
        } // if

        agent = Smaller( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "<=" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure <=>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : <=" ;
          throw "" ; 
        } // if

        agent = Smaller_equal( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "=" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure =>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : =" ;
          throw "" ; 
        } // if

        agent = Equal( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "string-append" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure string-append>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : string-append" ;
          throw "" ; 
        } // if

        agent = String_append( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "string>?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure string>?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : string>?" ;
          throw "" ; 
        } // if
      
        agent = String_bigger( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "string<?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure string<?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : string<?" ;
          throw "" ; 
        } // if

        agent = String_smaller( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "string=?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure string=?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : string=?" ;
          throw "" ; 
        } // if

        agent = String_equal( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "equal?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure equal?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : equal?" ;
          throw "" ; 
        } // if

        agent = Node_equal( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "eqv?" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure eqv?>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : eqv?" ;
          throw "" ; 
        } // if

        agent = Node_eqv( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "clean-environment" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure clean-environment>" ) == 0 ) {
        agent = Clean_env( run ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "if" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure if>" ) == 0 ) {
        agent = Condition_if( run ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "begin" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure begin>" ) == 0 ) {
        if ( run->right == NULL ) {
          cout << "ERROR (incorrect number of arguments) : begin" ;
          throw "" ; 
        } // if
      
        agent = Begin( run->right ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "cond" ) == 0 || 
                strcmp( run->left->token_str, "#<procedure cond>" ) == 0 ) {
        agent = Condition( run ) ;   
      } // else if
      else if ( strcmp( run->left->token_str, "exit" ) == 0 ||
                strcmp( run->left->token_str, "#<procedure exit>" ) == 0 ) {
        Exit( run ) ;   
      } // else if
      else if ( goin != true ) {
        if ( run->left->token_type == SYMBOL ) {
          cout << "ERROR (unbound symbol) : " << run->left->token_str ;
          throw "" ;
        } // if
        else {
          cout << "ERROR (attempt to apply non-function) : " ;
          Pretty_print( run->left, false ) ;
          throw "NO" ; 
        } // else 
      
      } // else if
  
    } // if
  } // else 
   
} // Evaul()

void Not_list( Pointer temp ) {

  Pointer error_node = temp ;
  while ( temp != NULL ) {
    if ( temp->token_type == DOT ) {
      cout << "ERROR (non-list) : " ;
      Pretty_print( error_node, false ) ;
      throw "NO" ;
    } // if 
    
    temp = temp->right ;
  } // while 

} // Not_list()

Pointer Clean_env( Pointer agent ) {
    
  if ( g_level == 1 ) 
    ;
  else {
    cout << "ERROR (level of CLEAN-ENVIRONMENT)" ;
    throw "" ;  
  } // else  
  
  if ( agent->right == NULL ) {
    Node * node = new Node ;
    node->left = NULL ;
    node->right = NULL ;
    g_define_line.clear() ;
    node->is_left = false ;
    node->token_type = SYMBOL ;
    strcpy( node->token_str, "environment cleaned" ) ;
    return node ;
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : clean-environment" ;
    throw "" ;
  } // else

} // Clean_env()

bool Check_define( Pointer &temp ) {
 
  bool bound = false ;
  int i = 0 ;
  while ( i < g_define_line.size() ) {
    if ( strcmp( g_define_line[i].token_str, temp->token_str ) == 0 ) {
      temp = g_define_line[i].ptr ;
      i = g_define_line.size() ;
      bound = true ;
    } // if 
    
    i = i + 1 ;
    
  } // while 
    
  return bound ;  
} // Check_define()

void Check_list( Pointer temp ) {

  if ( temp == NULL )
    return ;
  if ( 1 ) {
    if ( temp->token_type == DOT )
      glist = false ;  
    Check_list( temp->left ) ;
    Check_list( temp->right ) ;
  } // if     

} // Check_list()

Pointer Cons( Pointer agent ) {

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Token_struct temp ;
  Node * temp_node ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 2 ) {
    string str = "ERROR (incorrect number of arguments) : cons" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;  
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            temp_node = g_define_line[i].ptr ;
            ptr_vec.push_back( temp_node ) ; 
            has_define = true ;
          } // if 
       
          i = i + 1 ;   
        } // while
        
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if   
        else 
          ;
      } // else 
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ; 
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while 
  
  if ( ptr_vec.size() == 2 ) {
    node->left = ptr_vec[0] ;
    node->right = ptr_vec[1] ;
    if ( node->right->token_type == NIL ) 
      node->right->token_type = NOTHING ;
    node->is_left = true ;
    if ( node->right->is_left == false && node->right->token_type != NOTHING
         && strcmp( node->right->token_str, "()" ) != 0 ) {
      node->token_type = DOT ;
      strcpy( node->token_str, "." ) ;
    } // if 
    else {
      Node * henry = new Node ;
      henry->is_left = false ;
      henry->token_type = node->right->token_type ;
      henry->left = node->right->left ;
      henry->right = node->right->right ;
      node->token_type = NOTHING ;
      strcpy( node->token_str, "" ) ;
      node->right = henry ;
      // node->right->is_left = false ; 
    } // else 

    ptr_vec.clear() ;
    return node ;
  } // if 
    
  ptr_vec.clear() ;
  node->token_type = NOTHING ;
  return node ;
    
} // Cons()

Pointer Quote( Pointer agent ) {
  
  if ( agent->right != NULL ) {
    string str = "ERROR (incorrect number of arguments) : quote" ;
    cout << str ;
    throw "" ; 
  } // if
  
  return agent->left ;    
       
} // Quote() 

void Clean_up_str( Define_struct &temp ) {
    
  int i = 0 ;
  while ( i < 200 ) {
    temp.token_str[i] = '\0' ;
    i = i + 1 ;  
  } // while    
    
} // Clean_up_str()


bool Define( Pointer agent ) {  
  Node * error_node = agent ;
  // Pretty_print( error_node, false ) ;
  if ( g_level == 1 ) {
    if ( agent->right != NULL )
      agent = agent->right ;
    else {
      cout << "ERROR (DEFINE format) : " ;
      Pretty_print( error_node, false ) ;
      throw "NO" ; 
    } // else
    
  } // if 
  else {
    cout << "ERROR (level of DEFINE)" ;
    throw "" ;
  } // else
  
  bool repeat = false ;
  int i = 0 ;
  int count = 0 ;
  int same = -1 ;
  int same2 = -1 ; 
  Pointer ans_evaul ;
  Token_struct temp ;
  Define_struct temp_ans ;
  Clean_up_str( temp_ans ) ;
  Define_struct ans ;
  vector <Define_struct> check ;
  Node * var2 = agent ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
  
  if ( var_num != 2 ) {
    cout << "ERROR (DEFINE format) : " ;
    Pretty_print( error_node, false ) ;
    throw "NO" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    count = count + 1 ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      temp_ans.ptr = agent->left ;
      temp_ans.token_type = agent->left->token_type ;
      strcpy( temp_ans.token_str, agent->left->token_str ) ;
      check.push_back( temp_ans ) ;  
      Clean_up_str( temp_ans ) ;
    } // if  
    else if ( temp.token_type == SYMBOL ) {
      i = 0 ;
      repeat = false ;  
      while ( i < g_define_line.size() ) {
        if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
          if ( count == 1 )
            same = i ;
          else
            same2 = i ;
          repeat = true ;
        } // if
        
        i = i + 1 ;
      } // while

      if ( count == 2 && ! Command( agent->left->token_str )
           && repeat == false ) {
        cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
        throw "" ; 
      } // if 

      // �@���ˬd�O�_����O�ۦP��function()
      temp_ans.ptr = agent->left ;
      temp_ans.token_type = agent->left->token_type ;
      strcpy( temp_ans.token_str, agent->left->token_str ) ;
      check.push_back( temp_ans ) ;  
      Clean_up_str( temp_ans ) ;
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans_evaul, agent->left ) ;
      temp_ans.ptr = ans_evaul ;
      temp_ans.token_type = NOTSYMBOL ;
      strcpy( temp_ans.token_str, "" ) ;
      Clean_up_str( temp_ans ) ;
      check.push_back( temp_ans ) ;
    } // else if
    
    agent = agent->right ; 
    
  } // while  
    
  if ( check.size() == 2 ) {
    char henry[200] ;
    strcpy( henry, check[0].token_str ) ;
    if ( check[0].token_type == SYMBOL && Command( henry ) == false ) { 
      if ( repeat == true && same != -1 
           && strcmp( check[0].token_str, g_define_line[same].token_str ) == 0 ) {
        if ( check[1].ptr != NULL ) {
          if ( same2 != -1 && strcmp( check[1].token_str, g_define_line[same2].token_str ) == 0 ) {
            g_define_line[same].ptr = g_define_line[same2].ptr ;
            cout << check[0].token_str << " defined\n" ;
            return true ;  
          } // if
          else {
            g_define_line[same].ptr = check[1].ptr ;
            cout << check[0].token_str << " defined\n" ; 
            return true ;
          } // else 
        } // if
        else 
          cout << "gg" ; 
      } // if       
      else if ( repeat == true && same2 != -1 
                && strcmp( check[1].token_str, g_define_line[same2].token_str ) == 0 ) {
        if ( check[1].ptr != NULL ) { 
          strcpy( ans.token_str, check[0].token_str ) ;
          ans.ptr = g_define_line[same2].ptr ;
          g_define_line.push_back( ans ) ;
          cout << ans.token_str << " defined\n" ; 
          return true ;
        } // if  
        else 
          cout << "gg" ; 
      } // else if  
      else {
        if ( check[1].ptr != NULL ) { 
          strcpy( ans.token_str, check[0].token_str ) ;
          ans.ptr = check[1].ptr ;
          g_define_line.push_back( ans ) ;
          cout << ans.token_str << " defined\n" ; 
          return true ;
        } // if 
        else 
          cout << "gg" ; 
      } // else 
    } // if
    else {
      cout << "ERROR (DEFINE format) : " ;
      Pretty_print( error_node, false ) ;
      throw "NO" ;
    } // else  
  } // if   
  else {
    cout << "ERROR (DEFINE format) : " ;
    Pretty_print( error_node, false ) ;
    throw "NO" ; 
  } // else 
  
  return false ;
       
} // Define()

bool Command( Str temp ) {

  if ( strcmp( temp, "cons" ) == 0 )
    return true ;
  else if ( strcmp( temp, "define" ) == 0 ) 
    return true ;
  else if ( strcmp( temp, "quote" ) == 0 ) 
    return true ;
  else if ( strcmp( temp, "'" ) == 0 ) 
    return true ;      
  else if ( strcmp( temp, "car" ) == 0 ) 
    return true ;
  else if ( strcmp( temp, "cdr" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "list" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "atom?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "string?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "integer?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "null?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "number?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "real?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "pair?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "list?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "boolean?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "symbol?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "+" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "-" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "*" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "/" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "not" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "and" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "or" ) == 0 )  
    return true ;
  else if ( strcmp( temp, ">" ) == 0 )  
    return true ;
  else if ( strcmp( temp, ">=" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "<" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "<=" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "=" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "string-append" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "string>?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "string<?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "string=?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "eqv?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "equal?" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "begin" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "cond" ) == 0 ) 
    return true ;
  else if ( strcmp( temp, "if" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "clean-environment" ) == 0 )  
    return true ;
  else if ( strcmp( temp, "exit" ) == 0 )  
    return true ;                                                              
  else 
    return false ;

} // Command()

Pointer Car( Pointer agent ) {

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node * ans ;
  Node *node = new Node ;
  node->left = NULL ;
  node->right = NULL ;
  Node * car_ans ;
  Token_struct temp ;
  Node * temp_node ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : car" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if
      else {
        i = 0 ;
        has_define = false ;  
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            temp_node = g_define_line[i].ptr ;
            ptr_vec.push_back( temp_node ) ;
            has_define = true ;
          } // if 
           
          i = i + 1 ;   
        } // while
        
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while 
  
  if ( ptr_vec.size() == 1 ) {
    car_ans = ptr_vec[0] ;
    if ( car_ans->left != NULL )  
      return car_ans->left ;
    else {
      cout << "ERROR (car with incorrect argument type) : " ;
      if ( car_ans->token_type == STRING ) 
        cout << "\"" << car_ans->token_str << "\"" ;
      else {
        Pretty_print( car_ans, false ) ;
        throw "NO" ;  
      } // else
      
      throw "" ;

    } // else     
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : car" ;
    throw "" ;  
  } // else 
  
  node->token_type = NOTHING ;
  return node ;
  
} // Car()

Pointer Cdr( Pointer agent ) {

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node * ans ;
  Node *node = new Node ;
  node->left = NULL ;
  node->right = NULL ;
  Node * cdr_ans ;
  Token_struct temp ;
  Node * temp_node ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : cdr" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if
      else {
        i = 0 ;
        has_define = false ;  
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            temp_node = g_define_line[i].ptr ;
            ptr_vec.push_back( temp_node ) ;
            has_define = true ;
          } // if 
 
          i = i + 1 ;   
        } // while
  
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while 
  
  if ( ptr_vec.size() == 1 ) {
    cdr_ans = ptr_vec[0] ;
    if ( cdr_ans->left != NULL ) {
      if ( cdr_ans->right != NULL ) {
        Node * answer = new Node ;
        // answer = cdr_ans->right ;
        answer->left = cdr_ans->right->left ;
        answer->right = cdr_ans->right->right ;
        answer->token_type = cdr_ans->right->token_type ;
        answer->is_left = false ;
        strcpy( answer->token_str, cdr_ans->right->token_str ) ; 
        Node * henry = new Node ;
        henry->token_type = answer->token_type ;
        henry->is_left = true ; 
        henry->left = answer->left ;
        henry->right = answer->right ;
        strcpy( henry->token_str, answer->token_str ) ;
        if ( cdr_ans->token_type == DOT ) 
          return answer ;
        else {
          return henry ;  
        } // else
      } // if
      else {
        node->is_left = false ;
        node->token_type = NIL ;
        return node ;
      } // else   
    } // if 
    else {
      cout << "ERROR (cdr with incorrect argument type) : " ;
      if ( cdr_ans->token_type == STRING )
        cout << "\"" << cdr_ans->token_str << "\"" ;
      else {
        Pretty_print( cdr_ans, false ) ;
        throw "NO" ;  
      } // else 

      throw "" ;
 
    } // else 

  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : cdr" ;
    throw "" ; 
  } // else 
  
  node->token_type = NOTHING ;
  return node ;
      
} // Cdr()

Pointer List( Pointer agent, bool is_while  ) {
  
  int num = 0 ;
  int i = 0 ;
  bool has_define = false ; 
  Node * ans ;
  Node * head = new Node ;
  head->is_left = true ;
  head->right = NULL ;
  head->left = NULL ;
  head->token_type = agent->token_type ;
  strcpy( head->token_str, agent->token_str ) ;
  Node * run = head ;
  Token_struct temp ;
  Node * temp_node ;
  while ( agent != NULL && is_while == false ) {
    num = num + 1 ;  
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) { 
      run->left = agent->left ;
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        run->left = agent->left ;
      } // if 
      else {
        i = 0 ;
        has_define = false ;  
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            temp_node = g_define_line[i].ptr ;
            run->left = temp_node ; 
            has_define = true ;
          } // if 
       
          i = i + 1 ;   
        } // while

        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) { 
      Evaul( ans, agent->left ) ;
      run->left = ans ;
    } // else if

    agent = agent->right ;
    if ( agent != NULL ) {
      run->right = new Node ;
      run->right = agent ;
      run = run->right ;
    } // if 
    else 
      run->right = NULL ;
  } // while 
  
  if ( num > 0 )
    return head ;
  else {
    head->is_left = false ;
    head->token_type = NIL ;
    strcpy( head->token_str, "" ) ;
    return head ; 
  } // else 
} // List()

bool Is_command( Pointer &temp ) {

  if ( strcmp( temp->token_str, "cons" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure cons>" ) ; 
    return true ;
  } // if
  else if ( strcmp( temp->token_str, "quote" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure quote>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "car" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure car>" ) ;
    return true ; 
  } // else if 
  else if ( strcmp( temp->token_str, "cdr" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure cdr>" ) ;
    return true ; 
  } // else if 
  else if ( strcmp( temp->token_str, "list" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure list>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "define" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure define>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "+" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure +>" ) ;
    return true ;  
  } // else if 
  else if ( strcmp( temp->token_str, "-" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure ->" ) ;
    return true ;
  } // else if
  else if ( strcmp( temp->token_str, "*" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure *>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "/" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure />" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "atom?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure atom?>" ) ;
    return true ; 
  } // else if 
  else if ( strcmp( temp->token_str, "integer?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure integer?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "string?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure string?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "boolean?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure boolean?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "symbol?" ) == 0 ) { 
    strcpy( temp->token_str, "#<procedure symbol?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "number?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure number?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "real?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure real?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "null?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure null?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "list?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure list?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "pair?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure pair?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "not" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure not>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "and" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure and>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "or" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure or>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, ">" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure >>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, ">=" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure >=>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "<" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure <>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "<=" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure <=>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "=" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure =>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "string-append" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure string-append>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "string>?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure string>?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "string<?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure string<?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "string=?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure string=?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "equal?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure equal?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "eqv?" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure eqv?>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "if" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure if>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "clean-environment" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure clean-environment>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "cond" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure cond>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "begin" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure begin>" ) ;
    return true ; 
  } // else if
  else if ( strcmp( temp->token_str, "exit" ) == 0 ) {
    strcpy( temp->token_str, "#<procedure exit>" ) ;
    return true ; 
  } // else if
  else 
    return false ;
} // Is_command()

Pointer Is_atom( Pointer agent ) {

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_atom_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : atom?" ;
    cout << str ;
    throw "" ; 
  } // if
 
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_atom_ans = ptr_vec[0] ;
    temp.token_type = is_atom_ans->token_type ;
    if ( is_atom_ans->left == NULL && is_atom_ans->right == NULL && ATOM( temp ) == true ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : atom?" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Is_atom()


Pointer Is_integer( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_integer_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : integer?" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_integer_ans = ptr_vec[0] ;
    if ( is_integer_ans->left == NULL && is_integer_ans->right == NULL 
         && is_integer_ans->token_type == INT ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : integer?" ;
    throw "" ;
  } // else
    
  node->token_type = NOTHING ;
  return node ;

} // Is_integer()

Pointer Is_string( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_string_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : string?" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_string_ans = ptr_vec[0] ;
    if ( is_string_ans->left == NULL && is_string_ans->right == NULL 
         && is_string_ans->token_type == STRING ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : string?" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Is_string()

Pointer Is_boolean( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_boolean_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : boolean?" ;
    cout << str ;
    throw "" ; 
  } // if
 
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;    
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_boolean_ans = ptr_vec[0] ;
    if ( is_boolean_ans->left == NULL && is_boolean_ans->right == NULL
         && ( is_boolean_ans->token_type == T || is_boolean_ans->token_type == NIL ) ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : boolean?" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // Is_boolean()

Pointer Is_symbol( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_symbol_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : symbol?" ;
    cout << str ;
    throw "" ; 
  } // if

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_symbol_ans = ptr_vec[0] ;
    if ( is_symbol_ans->left == NULL && is_symbol_ans->right == NULL 
         && is_symbol_ans->token_type == SYMBOL ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : symbol?" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;
    
} // Is_symbol()

Pointer Is_number( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_number_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : number?" ;
    cout << str ;
    throw "" ; 
  } // if
 
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_number_ans = ptr_vec[0] ;
    if ( is_number_ans->left == NULL && is_number_ans->right == NULL 
         && ( is_number_ans->token_type == INT || is_number_ans->token_type == FLOAT ) ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : number?" ;
    throw "" ;
  } // else
    
  node->token_type = NOTHING ;
  return node ;

} // Is_number()

Pointer Is_null( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_null_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : null?" ;
    cout << str ;
    throw "" ; 
  } // if

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_null_ans = ptr_vec[0] ;
    if ( is_null_ans->left == NULL && is_null_ans->right == NULL && is_null_ans->token_type == NIL  ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : null?" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Is_null()

Pointer Is_list( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_list_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : list?" ;
    cout << str ;
    throw "" ; 
  } // if

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_list_ans = ptr_vec[0] ;
    if ( is_list_ans->left == NULL && is_list_ans->right == NULL ) {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;      
    } // if
    else {
      glist = true ;  
      Check_list( is_list_ans ) ;
      if ( glist == true ) {
        node->token_type = T ;
        node->is_left = false ;
        return node ; 
      } // if 
      else {
        node->token_type = NIL ;
        node->is_left = false ;
        return node ;    
      } // else 
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : list?" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Is_list()

Pointer Is_pair( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * is_pair_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : pair?" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    is_pair_ans = ptr_vec[0] ;
    if ( is_pair_ans->is_left == true ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;
    } // if
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ; 
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : pair?" ;
    throw "" ;
  } // else
    
  node->token_type = NOTHING ;
  return node ;

} // Is_pair()

Pointer Plus( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int total_int = 0 ;
  float total_float = 0 ;
  int count = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * plus_ans ;
  Node * temp_ans ; 
  Token_struct temp ;
  bool change = false ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : +" ;
    cout << str ;
    throw "" ; 
  } // if
   
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT ) {
        if ( temp_ans->token_type == INT ) 
          count = count + 1 ;      
      } // if 
      else {
        cout << "ERROR (+ with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )  
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;

        } // else
         
        throw "" ;
        
      } // else
      
      j = j + 1 ;   
    } // while
    
    j = 0 ;
    if ( count == ptr_vec.size() ) {
      while ( j < ptr_vec.size() ) {
        plus_ans = ptr_vec[j] ;
        total_int = total_int + atoi( plus_ans->token_str ) ;
        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%d", total_int ) ;
      node->is_left = false ;
      node->token_type = INT ;
      return node ;
    } // if 
    else {     
      while ( j < ptr_vec.size() ) {
        plus_ans = ptr_vec[j] ;
        if ( j == 0 ) {
          if ( plus_ans->token_type == FLOAT ) {
            total_float = atof( plus_ans->token_str ) ;
            change = true ;
          } // if
          else {
            total_int = atoi( plus_ans->token_str ) ;
            change = false ;
          } // else  
        } // if  
        else {
          if ( plus_ans->token_type == FLOAT ) {
            if ( change == false ) 
              total_float = total_int + atof( plus_ans->token_str ) ; 
            else 
              total_float = total_float + atof( plus_ans->token_str ) ;
            change = true ;
          } // if 
          else {
            if ( change == false ) 
              total_int = total_int + atoi( plus_ans->token_str ) ; 
            else 
              total_float = total_float + atoi( plus_ans->token_str ) ;  
          } // else
        } // else

        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%f", total_float ) ;
      node->is_left = false ;
      node->token_type = FLOAT ;
      return node ;      
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : +" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;
  
} // Plus()

Pointer Minus( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int total_int = 0 ;
  float total_float = 0 ;
  int count = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * minus_ans ;
  Node * temp_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  bool change = false ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : -" ;
    cout << str ;
    throw "" ; 
  } // if
   
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT ) {
        if ( temp_ans->token_type == INT ) 
          count = count + 1 ;      
      } // if 
      else {
        cout << "ERROR (- with incorrect argument type) : " ; 
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;

        } // else 
        
        throw "" ;
        
      } // else
      
      j = j + 1 ;   
    } // while
    
    j = 0 ;
    if ( count == ptr_vec.size() ) {
      while ( j < ptr_vec.size() ) {
        minus_ans = ptr_vec[j] ;
        if ( j == 0 )
          total_int = atoi( minus_ans->token_str ) ;
        else 
          total_int = total_int - atoi( minus_ans->token_str ) ;
        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%d", total_int ) ;
      node->is_left = false ;
      node->token_type = INT ;
      return node ;
    } // if 
    else {     
      while ( j < ptr_vec.size() ) {
        minus_ans = ptr_vec[j] ;
        if ( j == 0 ) {
          if ( minus_ans->token_type == FLOAT ) {
            total_float = atof( minus_ans->token_str ) ;
            change = true ;
          } // if
          else {
            total_int = atoi( minus_ans->token_str ) ;
            change = false ;
          } // else  
        } // if  
        else {
          if ( minus_ans->token_type == FLOAT ) {
            if ( change == false ) 
              total_float = total_int - atof( minus_ans->token_str ) ; 
            else 
              total_float = total_float - atof( minus_ans->token_str ) ;
            change = true ;
          } // if 
          else {
            if ( change == false ) 
              total_int = total_int - atoi( minus_ans->token_str ) ; 
            else 
              total_float = total_float - atoi( minus_ans->token_str ) ;  
          } // else
        } // else

        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%f", total_float ) ;
      node->is_left = false ;
      node->token_type = FLOAT ;
      return node ;      
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : -" ;
    throw "" ;
  } // else
  
  node->token_type = NOTHING ;
  return node ;
    
} // Minus()

Pointer Multi( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int total_int = 0 ;
  float total_float = 0 ;
  int count = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * multi_ans ;
  Node * temp_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  bool change = false ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : *" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT ) {
        if ( temp_ans->token_type == INT ) 
          count = count + 1 ;      
      } // if 
      else {
        cout << "ERROR (* with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;

        } // else

        throw "" ;

      } // else
      
      j = j + 1 ;   
    } // while
    
    j = 0 ;
    if ( count == ptr_vec.size() ) {
      while ( j < ptr_vec.size() ) {
        multi_ans = ptr_vec[j] ;
        if ( j == 0 )
          total_int = atoi( multi_ans->token_str ) ;
        else 
          total_int = total_int * atoi( multi_ans->token_str ) ;
        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%d", total_int ) ;
      node->is_left = false ;
      node->token_type = INT ;
      return node ;
    } // if 
    else {     
      while ( j < ptr_vec.size() ) {
        multi_ans = ptr_vec[j] ;
        if ( j == 0 ) {
          if ( multi_ans->token_type == FLOAT ) {
            total_float = atof( multi_ans->token_str ) ;
            change = true ;
          } // if
          else {
            total_int = atoi( multi_ans->token_str ) ;
            change = false ;    
          } // else 
        } // if    
        else {
          if ( multi_ans->token_type == FLOAT ) {
            if ( change == false ) 
              total_float = total_int * atof( multi_ans->token_str ) ; 
            else 
              total_float = total_float * atof( multi_ans->token_str ) ;
            change = true ;
          } // if 
          else {
            if ( change == false ) 
              total_int = total_int * atoi( multi_ans->token_str ) ; 
            else 
              total_float = total_float * atoi( multi_ans->token_str ) ;  
          } // else
 
        } // else

        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%f", total_float ) ;
      node->is_left = false ;
      node->token_type = FLOAT ;
      return node ;      
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : *" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;
 
} // Multi()

Pointer Div( Pointer agent ) {
    
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int total_int = 0 ;
  float total_float = 0 ;
  int count = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * div_ans ;
  Node * temp_ans ; 
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  bool change = false ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : /" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT ) {
        if ( temp_ans->token_type == INT ) 
          count = count + 1 ;      
      } // if 
      else {
        cout << "ERROR (/ with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;

        } // else

        throw "" ;

      } // else
      
      j = j + 1 ;   
    } // while
    
    j = 0 ;
    if ( count == ptr_vec.size() ) {
      while ( j < ptr_vec.size() ) {
        div_ans = ptr_vec[j] ;
        if ( j == 0 )
          total_int = atoi( div_ans->token_str ) ;
        else {
          if ( atoi( div_ans->token_str ) == 0 ) {
            cout << "ERROR (division by zero) : /" ;
            throw "" ; 
          } // if 
          else    
            total_int = total_int / atoi( div_ans->token_str ) ;
        } // else 
        
        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%d", total_int ) ;
      node->is_left = false ;
      node->token_type = INT ;
      return node ;
    } // if 
    else {     
      while ( j < ptr_vec.size() ) {
        div_ans = ptr_vec[j] ;
        if ( j == 0 ) {
          if ( div_ans->token_type == FLOAT ) {
            total_float = atof( div_ans->token_str ) ;
            change = true ;
          } // if 
          else {
            total_int = atoi( div_ans->token_str ) ;
            change = false ; 
          } // else
        } // if  
        else {
          if ( atof( div_ans->token_str ) == 0 ) {
            cout << "ERROR (division by zero) : /" ;
            throw "" ; 
          } // if  
          else { 
            if ( div_ans->token_type == FLOAT ) {
              if ( change == false ) 
                total_float = total_int / atof( div_ans->token_str ) ; 
              else 
                total_float = total_float / atof( div_ans->token_str ) ;
              change = true ;
            } // if 
            else {
              if ( change == false ) 
                total_int = total_int / atoi( div_ans->token_str ) ; 
              else 
                total_float = total_float / atoi( div_ans->token_str ) ;  
            } // else
            
          } // else 
        } // else 
        
        j = j + 1 ;     
      } // while 
      
      sprintf( node->token_str, "%f", total_float ) ;
      node->is_left = false ;
      node->token_type = FLOAT ;
      return node ;      
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : /" ;
    throw "" ; 
  } // else

  node->token_type = NOTHING ;
  return node ;
    
} // Div()

Pointer Not( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * not_ans ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 1 ) {
    string str = "ERROR (incorrect number of arguments) : not" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 1 ) {
    not_ans = ptr_vec[0] ;  
    if ( not_ans->left == NULL && not_ans->right == NULL && not_ans->token_type == NIL  ) {
      node->token_type = T ;
      node->is_left = false ;
      return node ;      
    } // if 
    else {
      node->token_type = NIL ;
      node->is_left = false ;
      return node ;
    } // else 
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : not" ;
    throw "" ; 
  } // else

  node->token_type = NOTHING ;
  return node ;
  
} // Not()

Pointer And( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *ans ;
  Node * answer ;
  Node * define_ans ;
  Token_struct temp ;
  bool evaul = true ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : and" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      if ( agent->left->token_type == NIL )
        return agent->left ;
      else    
        ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            define_ans = g_define_line[i].ptr ;
            if ( define_ans->token_type == NIL )
              return define_ans ;
            else {    
              Is_command( define_ans ) ;  
              ptr_vec.push_back( define_ans ) ;
            } // else
   
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      if ( ans->token_type == NIL )
        return ans ;
      else    
        ptr_vec.push_back( ans ) ; 
    } // else if 
    
    agent = agent->right ;
  } // while
  
  answer = ptr_vec[ptr_vec.size() - 1] ; 
  return answer ;


} // And()

Pointer Or( Pointer agent ) {

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *ans ;
  Node * answer ;
  Node * define_ans ;
  Token_struct temp ;
  bool evaul = true ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : or" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      if ( agent->left->token_type != NIL )
        return agent->left ;
      else    
        ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        return agent->left ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            define_ans = g_define_line[i].ptr ;
            Is_command( define_ans ) ;
            if ( define_ans->token_type != NIL )
              return define_ans ;
            else {
              Is_command( define_ans ) ;   
              ptr_vec.push_back( define_ans ) ;
            } // else

            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ; 
      if ( ans->token_type != NIL )
        return ans ;
      else    
        ptr_vec.push_back( ans ) ; 
    } // else if 
    
    agent = agent->right ;
  } // while
  
  answer = ptr_vec[ptr_vec.size() - 1] ; 
  return answer ; 

} // Or()

Pointer Bigger( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  float player1 = 0 ;
  float player2 = 0 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : >" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  bool correct = true ;
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT )
        ;    
      else {
        cout << "ERROR (> with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;
        } // else

        throw "" ;

      } // else 

      j = j + 1 ;   
    } // while
    
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      player1 = atof( ans1->token_str ) ;
      player2 = atof( ans2->token_str ) ;
      if ( player1 <= player2 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      else 
        ;
        
      j = j + 1 ;  
    } // while 
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : >" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Bigger()

Pointer Bigger_equal( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  float player1 = 0 ;
  float player2 = 0 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : >=" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  bool correct = true ;
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT )
        ;    
      else {
        cout << "ERROR (>= with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;
        } // else
        
        throw "" ;

      } // else

      j = j + 1 ;   
    } // while
    
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      player1 = atof( ans1->token_str ) ;
      player2 = atof( ans2->token_str ) ;
      if ( player1 < player2 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      else 
        ;
        
      j = j + 1 ;  
    } // while 
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : >=" ;
    throw "" ;
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // Bigger_equal()

Pointer Smaller( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  float player1 = 0 ;
  float player2 = 0 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : <" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  bool correct = true ;
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT )
        ;    
      else {
        cout << "ERROR (< with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;
        } // else
        
        throw "" ;
        
      } // else

      j = j + 1 ;   
    } // while
    
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      player1 = atof( ans1->token_str ) ;
      player2 = atof( ans2->token_str ) ;
      if ( player1 >= player2 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      else 
        ;
        
      j = j + 1 ;  
    } // while 
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : <" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // Smaller()

Pointer Smaller_equal( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  float player1 = 0 ;
  float player2 = 0 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : <=" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  bool correct = true ;
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT )
        ;    
      else {
        cout << "ERROR (<= with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;
        } // else
        
        throw "" ;

      } // else

      j = j + 1 ;   
    } // while
    
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      player1 = atof( ans1->token_str ) ;
      player2 = atof( ans2->token_str ) ;
      if ( player1 > player2 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      else 
        ;
        
      j = j + 1 ;  
    } // while 
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : <=" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;
  
} // Smaller_equal()

Pointer Equal( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  float player1 = 0 ;
  float player2 = 0 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : =" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  bool correct = true ;
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;
      if ( temp_ans->token_type == INT || temp_ans->token_type == FLOAT )
        ;    
      else {
        cout << "ERROR (= with incorrect argument type) : " ;
        if ( temp_ans->token_type == STRING )
          cout << "\"" << temp_ans->token_str << "\"" ;
        else {
          Pretty_print( temp_ans, false ) ;
          throw "NO" ;
        } // else
        
        throw "" ;
        
      } // else

      j = j + 1 ;   
    } // while
    
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      player1 = atof( ans1->token_str ) ;
      player2 = atof( ans2->token_str ) ;
      if ( player1 != player2 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      else 
        ;
        
      j = j + 1 ;  
    } // while 
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : =" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // Equal()

Pointer String_append( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : string-append" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;  
      if ( temp_ans->token_type != STRING ) {
        cout << "ERROR (string-append with incorrect argument type) : " ;
        Pretty_print( temp_ans, false ) ;
        throw "NO" ;
      } // if 
      else {
        if ( j == 0 ) 
          strcpy( node->token_str, temp_ans->token_str ) ;
        else 
          strcat( node->token_str, temp_ans->token_str ) ;
      } // else
        
      j = j + 1 ; 
    } // while 
    
    node->is_left = false ;
    node->token_type = STRING ;
    return node ;

  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : string-append" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;
  
} // String_append()

Pointer String_bigger( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : string>?" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;  
      if ( temp_ans->token_type != STRING ) {
        cout << "ERROR (string>? with incorrect argument type) : " ;
        Pretty_print( temp_ans, false ) ;
        throw "NO" ;
      } // if 

      j = j + 1 ; 
    } // while 
    
    bool correct = true ;
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      if ( strcmp( ans1->token_str, ans2->token_str ) <= 0 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      
      j = j + 1 ;
      
    } // while
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ; 
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;     
    } // else 

  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : string>?" ;
    throw "" ;  
  } // else

  node->token_type = NOTHING ;
  return node ;

} // String_bigger()

Pointer String_smaller( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : string<?" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;  
      if ( temp_ans->token_type != STRING ) {
        cout << "ERROR (string<? with incorrect argument type) : " ;
        Pretty_print( temp_ans, false ) ;
        throw "NO" ;
      } // if

      j = j + 1 ; 
    } // while 
    
    bool correct = true ;
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      if ( strcmp( ans1->token_str, ans2->token_str ) >= 0 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      
      j = j + 1 ;
      
    } // while
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ; 
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;     
    } // else 

  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : string<?" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // String_smaller()

Pointer String_equal( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ;
  Node * ans1 ;
  Node * ans2 ;
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : string=?" ;
    cout << str ;
    throw "" ; 
  } // if
 
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 2 ) {
    while ( j < ptr_vec.size() ) {
      temp_ans = ptr_vec[j] ;  
      if ( temp_ans->token_type != STRING ) {
        cout << "ERROR (string=? with incorrect argument type) : " ;
        Pretty_print( temp_ans, false ) ;
        throw "NO" ;
      } // if
 
      j = j + 1 ; 
    } // while 
    
    bool correct = true ;
    j = 0 ;
    while ( j < ptr_vec.size() - 1 ) {
      ans1 = ptr_vec[j] ;
      ans2 = ptr_vec[j+1] ;
      if ( strcmp( ans1->token_str, ans2->token_str ) != 0 ) {
        j = ptr_vec.size() ;
        correct = false ;
      } // if 
      
      j = j + 1 ;
      
    } // while
    
    if ( correct == true ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ; 
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;     
    } // else 

  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : string=?" ;
    throw "" ;  
  } // else 

  node->token_type = NOTHING ;
  return node ;

} // String_equal()

Pointer Node_equal( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int a = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * temp_ans ; 
  char ans1[1000] ;
  while ( a < 1000 ) {
    ans1[a] = '\0' ;
    a = a + 1 ;
  } // while
  
  a = 0 ; 
  char ans2[1000] ;
  while ( a < 1000 ) {
    ans2[a] = '\0' ;
    a = a + 1 ;
  } // while
  
  Token_struct temp ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num != 2 ) {
    string str = "ERROR (incorrect number of arguments) : equal?" ;
    cout << str ;
    throw "" ; 
  } // if 

  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            ptr_vec.push_back( g_define_line[i].ptr ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 2 ) {
    temp_ans = ptr_vec[0] ;
    Check_content( ans1, temp_ans ) ;
    temp_ans = ptr_vec[1] ;
    Check_content( ans2, temp_ans ) ;
    if ( strcmp( ans1, ans2 ) == 0 ) {
      node->is_left = false ;
      node->token_type = T ;
      return node ;    
    } // if 
    else {
      node->is_left = false ;
      node->token_type = NIL ;
      return node ;
    } // else 
    
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : equal?" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;
  
} // Node_equal()

void Check_content( Str2 &str, Pointer agent ) {
  Token_struct temp_token ;
  if ( agent == NULL )
    return ;
  if ( 1 ) {
    if ( agent->is_left == true ) 
      strcat( str, "(" ) ;
    if ( agent->token_type == QUOTE )
      strcat( str, "quote" ) ;    
    Check_content( str, agent->left ) ;
    temp_token.token_type = agent->token_type ;
    if ( ATOM( temp_token ) == true || agent->token_type == DOT ) { 
      if ( temp_token.token_type == T ) {    
        strcat( str, "#t" ) ;
      } // if  
      else if ( temp_token.token_type == NIL ) {    
        strcat( str, "nil" ) ;
      } // else if
      else if ( temp_token.token_type == STRING ) {
        strcat( str, "\"" ) ;
        strcat( str, agent->token_str ) ; 
        strcat( str, "\"" ) ;
      } // else if 
      else    
        strcat( str, agent->token_str ) ;      
    } // if 
    
    Check_content( str, agent->right ) ;
    if ( agent->is_left == true ) 
      strcat( str, ")" ) ;  
  } // if     

} // Check_content()

Pointer Node_eqv( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int a = 0 ;
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * define_ans ;
  Node * ans1 ;
  Node * ans2 ;    
  Token_struct temp ;
  Token_struct token1 ;
  Token_struct token2 ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num < 2 ) {
    string str = "ERROR (incorrect number of arguments) : eqv?" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Command( agent->left->token_str ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            define_ans = g_define_line[i].ptr ;
            ptr_vec.push_back( define_ans ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() == 2 ) {
    ans1 = ptr_vec[0] ;
    ans2 = ptr_vec[1] ;
    token1.token_type = ans1->token_type ;
    token2.token_type = ans2->token_type ;
    if ( ptr_vec[0] == ptr_vec[1] ) { 
      node->is_left = false ;
      node->token_type = T ;
      return node ;
    } // if 
    else {
      if ( ATOM( token1 ) == true && ATOM( token2 ) == true
           && token1.token_type != STRING && token2.token_type != STRING
           && token1.token_type != SYMBOL && token2.token_type != SYMBOL ) {
        if ( strcmp( ans1->token_str, ans2->token_str ) == 0 ) {
          node->is_left = false ;
          node->token_type = T ;
          return node ; 
        } // if
        else if ( token1.token_type == T && token2.token_type == T ) {
          node->is_left = false ;
          node->token_type = T ;
          return node ;   
        } // else if 
        else if ( token1.token_type == NIL && token2.token_type == NIL ) {
          node->is_left = false ;
          node->token_type = T ;
          return node ;   
        } // else if 
        else {
          node->is_left = false ;
          node->token_type = NIL ;
          return node ;
        } // else 
      } // if 
      else {
        node->is_left = false ;
        node->token_type = NIL ;
        return node ;
      } // else   
    } // else 
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : eqv?" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // Node_eqv()

Pointer Condition_if( Pointer agent ) {
  
  Node * if_node = agent ;  
  if ( agent->right != NULL ) 
    agent = agent->right ;
  else {
    cout << "ERROR (incorrect number of arguments) : if" ;
    throw "" ;  
  } // else

  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int a = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * ans1 ;
  Node * ans2 ;
  Node * ans3 ;
  Node * define_ans ;
  Token_struct temp ;
  Token_struct token1 ;
  Token_struct token2 ;
  bool evaul = true ;
  Node * var = agent ;
  int var_num = 0 ;
  while ( var != NULL ) { 
    var = var->right ;
    var_num = var_num + 1 ;
  } // while
   
  if ( var_num > 3 ) {
    string str = "ERROR (incorrect number of arguments) : if" ;
    cout << str ;
    throw "" ; 
  } // if 
  
  if ( agent->left == NULL )
    ; // throw()
  temp.token_type = agent->left->token_type ;
  if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
    if ( temp.token_type == NIL )
      evaul = false ;  
  } // if 
  else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
    if ( Is_command( agent->left ) == true ) {
      ;  
    } // if 
    else {
      i = 0 ;
      has_define = false ;
      while ( i < g_define_line.size() ) {
        if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
          define_ans = g_define_line[i].ptr ;
          if ( define_ans->token_type == NIL )
            evaul = false ;
          has_define = true ;
        } // if 
        
        i = i + 1 ;   
      } // while
          
      if ( has_define == false ) {
        cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
        throw "" ;
      } // if 
      else 
        ;
    } // else 
      
  } // else if 
  else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
            || strcmp( agent->left->token_str, "quote" ) == 0 ) {
    Evaul( ans, agent->left ) ;
    if ( ans->token_type == NIL ) 
      evaul = false ;

  } // else if 

  if ( evaul == true ) {
    if ( agent->right != NULL ) {
      agent = agent->right ;
      temp.token_type = agent->left->token_type ;
      if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
        return agent->left ;
      } // if 
      else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
        if ( Is_command( agent->left ) == true ) {
          return agent->left ;  
        } // if 
        else {
          i = 0 ;
          has_define = false ;
          while ( i < g_define_line.size() ) {
            if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
              define_ans = g_define_line[i].ptr ;  
              if ( Is_command( define_ans ) == true ) 
                ;
              has_define = true ;
            } // if 
        
            i = i + 1 ;   
          } // while
          
          if ( has_define == false ) {
            cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
            throw "" ;
          } // if 
          else {
            return define_ans ;
          } // else 
          
        } // else 
      } // else if 
      else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
                || strcmp( agent->left->token_str, "quote" ) == 0 ) {
        Evaul( ans, agent->left ) ;
        return ans ;
      } // else if
       
    } // if 
    else {
      cout << "ERROR (incorrect number of arguments) : if" ;
      throw "" ;
    } // else  
    
  } // if
  else {
    if ( agent->right != NULL ) 
      agent = agent->right ;
    else {
      cout << "ERROR (incorrect number of arguments) : if" ;
      throw "" ;
    } // else 

    if ( agent->right != NULL ) {
      agent = agent->right ;
      temp.token_type = agent->left->token_type ;
      if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
        return agent->left ;
      } // if 
      else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
        if ( Is_command( agent->left ) == true ) {
          return agent->left ;  
        } // if 
        else {
          i = 0 ;
          has_define = false ;
          while ( i < g_define_line.size() ) {
            if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
              define_ans = g_define_line[i].ptr ;
              if ( Is_command( define_ans ) == true ) 
                ;
              has_define = true ;
            } // if 
        
            i = i + 1 ;   
          } // while
          
          if ( has_define == false ) {
            cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
            throw "" ;
          } // if 
          else {
            return define_ans ;
          } // else 
          
        } // else 
      } // else if 
      else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
                || strcmp( agent->left->token_str, "quote" ) == 0 ) {
        Evaul( ans, agent->left ) ;
        return ans ;
      } // else if
       
    } // if
    else {
      cout << "ERROR (no return value) : " ;
      Pretty_print( if_node, false )  ;
      throw "NO" ;
    } // else 
    
  } // else 
  
  node->token_type = NOTHING ;
  return node ;

} // Condition_if()

Pointer Begin( Pointer agent ) {
 
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int a = 0 ; 
  bool has_define = false ; 
  Node *node = new Node ;
  Node *ans ;
  Node * define_ans ;
  node->left = NULL ;
  node->right = NULL ;
  Node * answer ; 
  Token_struct temp ;
  while ( agent != NULL ) {
    temp.token_type = agent->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      ptr_vec.push_back( agent->left ) ; 
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( agent->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( agent->left ) == true ) {
        ptr_vec.push_back( agent->left ) ;    
      } // if 
      else {
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, agent->left->token_str ) == 0 ) {
            define_ans = g_define_line[i].ptr ;
            Is_command( define_ans ) ; 
            ptr_vec.push_back( define_ans ) ;
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << agent->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
      
    } // else if 
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( agent->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, agent->left ) ;
      ptr_vec.push_back( ans ) ;
    } // else if 
    
    agent = agent->right ;
  } // while
  
  if ( ptr_vec.size() >= 1 ) {
    answer = ptr_vec[ptr_vec.size()-1] ;
    return answer ;   
  } // if 
  else {
    cout << "ERROR (incorrect number of arguments) : begin" ;
    throw "" ;
  } // else

  node->token_type = NOTHING ;
  return node ;

} // Begin()

Pointer Condition( Pointer agent ) {
 
  Node * cond_node = agent ;  
  if ( agent->right != NULL ) 
    agent = agent->right ;
  else {
    cout << "ERROR (COND format) : " ;
    Pretty_print( cond_node, false ) ;
    throw "NO" ;  
  } // else
  
  vector <Pointer> ptr_vec ; 
  int i = 0 ;
  int j = 0 ;
  int a = 0 ;
  Node * answer ;  
  bool has_define = false ; 
  Node *node = new Node ;
  Node * run ;
  Node * define_ans ;
  node->left = NULL ;
  node->right = NULL ;
  int num = 0 ;
  Node * ans ; 
  bool evaul = true ;
  bool no_return = false ; 
  Token_struct temp ;
  Node * check_cond = agent ;
  while ( check_cond != NULL ) {
    if ( check_cond->left->left == NULL || check_cond->left->right == NULL  ) {
      cout << "ERROR (COND format) : " ;
      Pretty_print( cond_node, false ) ;
      throw "NO" ; 
    } // if 
    
    check_cond = check_cond->right ;
    
  } // while 
  
  while ( agent != NULL ) {
    evaul = true ;
    num = num + 1 ;
    Not_list_cond( agent->left ) ;
    run = agent->left ;
    temp.token_type = run->left->token_type ;
    if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
      if ( temp.token_type == NIL )
        evaul = false ;  
    } // if 
    else if ( temp.token_type == SYMBOL && strcmp( run->left->token_str, "quote" ) != 0  ) {
      if ( Is_command( run->left ) == true )
        ;
      else if ( agent->right == NULL && strcmp( run->left->token_str, "else" ) == 0 ) {
        ;
      } // else if
      else {  
        i = 0 ;
        has_define = false ;
        while ( i < g_define_line.size() ) {
          if ( strcmp( g_define_line[i].token_str, run->left->token_str ) == 0 ) {
            define_ans = g_define_line[i].ptr ;
            if ( define_ans->token_type == NIL )
              evaul = false ;  
            has_define = true ;
          } // if 
          
          i = i + 1 ;   
        } // while
          
        if ( has_define == false ) {
          cout << "ERROR (unbound symbol) : " << run->left->token_str ;
          throw "" ;
        } // if 
        else 
          ;
      } // else 
    } // else if
    else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
              || strcmp( run->left->token_str, "quote" ) == 0 ) {
      Evaul( ans, run->left ) ;
      if ( ans->token_type == NIL ) 
        evaul = false ;  
    } // else if
    
    // ��2��s-exp
     
    if ( evaul == true ) {
      no_return = true ;
      if ( run->right != NULL ) 
        run = run->right ;
      else {
        cout << "ERROR (COND format) : " ;
        Pretty_print( cond_node, false ) ;
        throw "NO" ;
      } // else

      while ( run != NULL ) {
      
        temp.token_type = run->left->token_type ;
        if ( ATOM( temp ) == true && temp.token_type != SYMBOL ) {
          ptr_vec.push_back( run->left ) ;  
        } // if 
        else if ( temp.token_type == SYMBOL && strcmp( run->left->token_str, "quote" ) != 0  ) {
          if ( Is_command( run->left ) == true )
            ptr_vec.push_back( run->left ) ;
          else {
            i = 0 ;
            has_define = false ;
            while ( i < g_define_line.size() ) {
              if ( strcmp( g_define_line[i].token_str, run->left->token_str ) == 0 ) {
                define_ans = g_define_line[i].ptr ;
                if ( Is_command( define_ans ) == true )
                  ;
                has_define = true ;
              } // if 

              i = i + 1 ;   
            } // while

            if ( has_define == false ) {
              cout << "ERROR (unbound symbol) : " << run->left->token_str ;
              throw "" ;
            } // if 
            else {
              ptr_vec.push_back( define_ans ) ;
            } // else
          
          } // else 
        } // else if
        else if ( temp.token_type == SKIP || temp.token_type == QUOTE 
                  || strcmp( run->left->token_str, "quote" ) == 0 ) {
          Evaul( ans, run->left ) ;
          ptr_vec.push_back( ans ) ;
        } // else if
        
        run = run->right ;
      } // while
      
      answer = ptr_vec[ptr_vec.size() - 1] ;
      return answer ;   
    } // if 

    agent = agent->right ;
  } // while
  
  if ( no_return == false ) {
    cout << "ERROR (no return value) : " ;
    Pretty_print( cond_node, false ) ;
    throw "NO" ; 
  } // if 

  node->token_type = NOTHING ;
  return node ;

} // Condition()

void Exit( Pointer agent ) {

  if ( g_level == 1 ) {
    if ( agent->right != NULL ) { 
      if ( agent->token_type == DOT && agent->right->token_type == NIL 
           && agent->right->left == NULL && agent->right->right == NULL ) {
        cout << "\n" << "Thanks for using OurScheme!" ; 
        g_fuck_EOF = true ;
        throw "go home" ;  
      } // if 
      else {
        cout << "ERROR (incorrect number of arguments) : exit" ;
        throw "" ;
      } // else 
    } // if 
    else {
      cout << "\n" << "Thanks for using OurScheme!" ; 
      g_fuck_EOF = true ;
      throw "go home" ;
    } // else 
  } // if
  else {
    cout << "ERROR (level of EXIT)" ;
    throw "" ;
  } // else
  
} // Exit()

void Not_list_cond( Pointer temp ) {

  Pointer error_node = temp ;
  while ( temp != NULL ) {
    if ( temp->token_type == DOT ) {
      cout << "ERROR (COND format) : " ;
      Pretty_print( error_node, false ) ;
      throw "NO" ;
    } // if 
    
    temp = temp->right ;
  } // while 

} // Not_list_cond()


