#ifndef COMPILER_SCANNER_H
#define COMPILER_SCANNER_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <iostream>

enum TokenType {
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

static std::vector<std::string> enumToStr {
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

static std::map <std::string, TokenType> reserveWords = {
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

static std::map <std::string, TokenType> doubleOperators = {
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

static std::map <int, TokenType> tokenToTokenType = {
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

struct ScannerState {
    TokenType token;
    bool add;
    bool save;
    bool find;
    bool toSave;
    std::string errorMsg;
    ScannerState(TokenType token = START, bool add = false, bool save = false, bool find = false, bool toSave = true,
              std::string errorMsg = ""):
            token(token), add(add), save(save), find(find), toSave(toSave), errorMsg(errorMsg) {};
    ~ScannerState() {};
};

struct Lexem {
    std::string val;
    TokenType token;
    std::pair <int, int> pos;
    Lexem(std::string val = "START", TokenType token = START, std::pair <int, int> pos = std::make_pair(1, 1)):
            val(val), token(token), pos(pos) {};
};


class Scanner {
public:
    Scanner(char*);
    void NextToken();
    Lexem GetLexem();
private:
    void CheckSymbol(int);
    void CheckError();
    Lexem SaveLexem(ScannerState, Lexem);
    ScannerState ChangeState(int);
    void IncPos(char);
    void ReplaceEscapeSymbols(int, std::string);
    Lexem TokenStrChgPos(Lexem);

    FILE* source;
    Lexem last;
    ScannerState currentState;
    std::string toSave = "";
    std::string backBuffer = "";
    bool eof = false;
    std::pair <int, int> pos = std::make_pair(1, 0);
    int toSaveLenStr = 0;
};

#endif //COMPILER_SCANNER_H
