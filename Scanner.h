#ifndef COMPILER_SCANNER_H
#define COMPILER_SCANNER_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <iostream>

enum Token {
    START = 0, NUMBER, REAL_NUMBER, NUMBER_E, NUMBER_E_POW, IDENTIFICATOR,

    BIT_NOT, HASH, HEX_VALUE, PERCENT, BIT_AND, OPEN_BRACKET, CLOSE_BRACKET, MULT,
    ADD, COMMA, SUB, POINT, DIVISION, COLON, SEMI_COLON, LESS_THAN, EQUAL, GREATER_THAN, DOG, OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET, BIT_OR, CIRCUMFLEX, DASH, CLOSE_BRACE, COMMENT, SINGLE_COMMENT,

    NOT_EQUAL, GREATER_OR_EQUAL_THAN, LESS_OR_EQUAL_THAN,
    SHIFT_LEFT, SHIFT_RIGHT, DOUBLE_DIV, DOUBLE_MULT, ASSIGNMENT, ADD_EQUAL, SUB_EQUAL, MULT_EQUAL, DIV_EQUAL, DOUBLE_POINT,

    AND, ARRAY, ASM, BEGIN, BREAK, CASE, CONST, CONSTRUCTOR, CONTINUE, DESTRUCTOR, DIV, DO,
    DOWNTO, ELSE, END, FALSE, T_FILE, FOR, FUNCTION, GOTO, IF, IMPLEMENTATION, IN, INLINE, INTERFACE, LABEL, MOD, NIL,

    NOT, OBJECT, OF, ON, OPERATOR, OR, PACKED, PROCEDURE, PROGRAM, RECORD, REPEAT, SET, SHL, SHR, STRING, THEN, TO,
    TRUE, TYPE, UNIT, UNTIL, USES, VAR, WHILE, WITH, XOR,

    INTEGER, BYTE, WORD, SHORTINT, LONGINT, REAL, SINGLE, DOUBLE,
    EXTENDED, BOOLEAN, CHAR,

    TOKEN_STRING, T_EOF, ERROR, COUNT,
};

static std::vector<std::string> enum_to_str {
    "START", "NUMBER", "REAL_NUMBER", "NUMBER", "NUMBER", "IDENTIFICATOR",

    "BIT_NOT", "HASH", "HEX_VALUE", "PERCENT", "BIT_AND", "OPEN_BRACKET", "CLOSE_BRACKET", "MULT",
    "ADD", "COMMA", "SUB", "POINT", "DIVISION", "COLON", "SEMI_COLON", "LESS_THAN", "EQUAL", "GREATER_THAN", "DOG",
    "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", "BIT_OR", "CIRCUMFLEX", "DASH", "CLOSE_BRACE", "COMMENT", "SINGLE_COMMENT",


    "NOT_EQUAL", "GREATER_OR_EQUAL_THAN", "LESS_OR_EQUAL_THAN",
    "SHIFT_LEFT", "SHIFT_RIGHT", "DOUBLE_DIV", "DOUBLE_MULT", "ASSIGNMENT", "ADD_EQUAL", "SUB_EQUAL", "MULT_EQUAL", "DIV_EQUAL",
    "DOUBLE_POINT",

    "AND", "ARRAY", "ASM", "BEGIN", "BREAK", "CASE", "CONST", "CONSTRUCTOR", "CONTINUE", "DESTRUCTOR", "DIV",
    "DO", "DOWNTO", "ELSE", "END", "FALSE", "T_FILE", "FOR", "FUNCTION", "GOTO", "IF", "IMPLEMENTATION", "IN",
    "INLINE", "INTERFACE", "LABEL", "MOD", "NIL",

    "NOT", "OBJECT", "OF", "ON", "OPERATOR", "OR", "PACKED", "PROCEDURE", "PROGRAM", "RECORD", "REPEAT", "SET", "SHL",
    "SHR", "STRING", "THEN", "TO", "TRUE", "TYPE", "UNIT", "UNTIL", "USES", "VAR", "WHILE", "WITH", "XOR",

    "INTEGER", "BYTE", "WORD", "SHORTINT", "LONGINT", "REAL", "SINGLE", "DOUBLE",
    "EXTENDED", "BOOLEAN", "CHAR",

    "TOKEN_STRING", "T_EOF", "ERROR", "COUNT"
};

static std::map <std::string, Token> reserve_words = {
        {"and", AND},
        {"array", ARRAY},
        {"asm", ASM},
        {"begin", BEGIN},
        {"break", BREAK},
        {"case", CASE},
        {"const", CONST},
        {"constructor", CONSTRUCTOR},
        {"continue", CONTINUE},
        {"destructor", DESTRUCTOR},
        {"div", DIV},
        {"do", DO},
        {"downto", DOWNTO},
        {"else", ELSE},
        {"end", END},
        {"false", FALSE},
        {"file", T_FILE},
        {"for", FOR},
        {"function", FUNCTION},
        {"goto", GOTO},
        {"if", IF},
        {"implementation", IMPLEMENTATION},
        {"in", IN},
        {"inline", INLINE},
        {"interface", INTERFACE},
        {"label", LABEL},
        {"mod", MOD},
        {"nil", NIL},
        {"not", NOT},
        {"object", OBJECT},
        {"of", OF},
        {"on", ON},
        {"operator", OPERATOR},
        {"or", OR},
        {"packed", PACKED},
        {"procedure", PROCEDURE},
        {"program", PROGRAM},
        {"record", RECORD},
        {"repeat", REPEAT},
        {"set", SET},
        {"shl", SHL},
        {"shr", SHR},
        {"string", STRING},
        {"then", THEN},
        {"to", TO},
        {"true", TRUE},
        {"type", TYPE},
        {"unit", UNIT},
        {"until", UNTIL},
        {"uses", USES},
        {"var", VAR},
        {"while", WHILE},
        {"with", WITH},
        {"xor", XOR},
        {"integer", INTEGER},
        {"byte", BYTE},
        {"word", WORD},
        {"shortint", SHORTINT},
        {"longint", LONGINT},
        {"real", REAL},
        {"single", SINGLE},
        {"double", DOUBLE},
        {"extended", EXTENDED},
        {"boolean", BOOLEAN},
        {"char", CHAR}
};

static std::map <std::string, Token> doubleOperators = {
        {"<>", NOT_EQUAL},
        {">=", GREATER_OR_EQUAL_THAN},
        {"<=", LESS_OR_EQUAL_THAN},
        {"<<", SHIFT_LEFT},
        {">>", SHIFT_RIGHT},
        {"//", DOUBLE_DIV},
        {"**", DOUBLE_MULT},
        {":=", ASSIGNMENT},
        {"+=", ADD_EQUAL},
        {"-=", SUB_EQUAL},
        {"*=", MULT_EQUAL},
        {"/=", DIV_EQUAL},
        {"..", DOUBLE_POINT},
};

static std::map <int, Token> tokenToTokenType = {
        {'+', ADD},
        {'-', SUB},
        {'*', MULT},
        {'/', DIVISION},
        {'=', EQUAL},
        {'>', GREATER_THAN},
        {'<', LESS_THAN},
        {'&', BIT_AND},
        {'|', BIT_OR},
        {'!', BIT_NOT},
        {'[', OPEN_SQUARE_BRACKET},
        {']', CLOSE_SQUARE_BRACKET},
        {'.', POINT},
        {',', COMMA},
        {'(', OPEN_BRACKET},
        {')', CLOSE_BRACKET},
        {':' , COLON},
        {'^', CIRCUMFLEX},
        {'@', DOG},
        {'{', COMMENT},
        {'}', CLOSE_BRACE},
        {'$', HEX_VALUE},
        {'#', HASH},
        {'%', PERCENT},
        {';', SEMI_COLON},
        {' ', START},
        {'\n', START},
        {'\0', START},
        {'\t', START},
        {'_', DASH},
        {'\'', TOKEN_STRING},
        {'\\', TOKEN_STRING},
        {EOF, START},
};

static std::vector <int> tokensStr = {
        '!', '"', '\'', '#', '$', '%', '&', '\\', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@',
        '[', ']', '^', '_', '{', '}', ' ', '\n', '\t', '\0', EOF
};

struct TokenType {
    Token tokenType;
    bool add;
    bool save;
    bool find;
    bool to_save;
    std::string errorMsg;
    TokenType(Token tokenType = START, bool add = false, bool save = false, bool find = false, bool to_save = true,
              std::string errorMsg = ""):
            tokenType(tokenType), add(add), save(save), find(find), to_save(to_save), errorMsg(errorMsg) {};
    ~TokenType() {};
};

static std::vector <std::map <char, TokenType>> states_global(COUNT);

struct Lexem {
    std::string val;
    Token tokenType;
    std::pair <int, int> pos;
    Lexem(std::string val = "START", Token tokenType = START, std::pair <int, int> pos = std::make_pair(1, 1)):
            val(val), tokenType(tokenType), pos(pos) {};
};

class Scanner {
public:
    Scanner(char*);
    Lexem NextToken();
private:
    void CheckSymbol(int);
    void CheckError();
    Lexem SaveLexem(TokenType, Lexem);
    TokenType ChangeState(int);
    void IncPos(char);

    FILE* source_;
    TokenType current_state;
    std::string to_save = "";
    std::string backbuffer = "";
    bool eof = false;
    std::pair <int, int> cur_pos = std::make_pair(1, 0);
};

void InitStates();
void fillWith(Token state, TokenType bl, TokenType sl, TokenType nums);
void fillWith(Token state, bool add, bool save);

#endif //COMPILER_SCANNER_H
