#include "Scanner.h"

static std::vector <std::map <char, ScannerState>> statesGlobal(COUNT);

void fillWith(TokenType state, ScannerState bl, ScannerState sl, ScannerState nums) {
    for (char c = 'A'; c <= 'Z'; c++) {
        statesGlobal[state][c] = bl;
    }
    for (char c = 'a'; c <= 'z'; c++) {
        statesGlobal[state][c] = sl;
    }
    for (char c = '0'; c <= '9'; c++) {
        statesGlobal[state][c] = nums;
    }
}

void fillWith(TokenType state, bool add, bool save) {
    for (auto c: tokensStr) {
        statesGlobal[state][c] = ScannerState(tokenToTokenType[c], add, save);
    }
    statesGlobal[state]['\n'] = ScannerState(START, add, save, false, false);
    statesGlobal[state]['\t'] = ScannerState(START, add, save, false, false);
    statesGlobal[state]['\0'] = ScannerState(START, add, save, false, false);
    statesGlobal[state][' '] = ScannerState(START, add, save, false, false);
    statesGlobal[state]['{'] = ScannerState(COMMENT, add, save, false, false);
    statesGlobal[state]['\''] = ScannerState(TOKEN_STRING, add, save, false, false);
};

void InitStates() {
    for (int i = START; i < statesGlobal.size(); i++) {
        if (i == START) {
            fillWith(START, ScannerState(IDENTIFICATOR), ScannerState(IDENTIFICATOR), ScannerState(NUMBER));
            fillWith(START, false, false);
        }
        if (i == NUMBER || i == REAL_NUMBER) {
            ScannerState tmp = ScannerState(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((TokenType)i, tmp, tmp, ScannerState((TokenType)i));
            fillWith((TokenType)i, false, true);
        }
        if (i == NUMBER_E) {
            ScannerState tmp = ScannerState(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((TokenType)i, tmp, tmp, ScannerState(NUMBER_E));
            fillWith((TokenType)i, false, true);
        }
        if (i == NUMBER_E_POW) {
            ScannerState tmp = ScannerState(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((TokenType)i, tmp, tmp, ScannerState(NUMBER_E_POW));
            fillWith((TokenType)i, false, true);
        }
        if (i >= BIT_NOT && i < COMMENT) {
            fillWith((TokenType)i, ScannerState(IDENTIFICATOR, false, true), ScannerState(IDENTIFICATOR, false, true), ScannerState(NUMBER, false, true));
            fillWith((TokenType)i, false, true);
        }
        if (i == IDENTIFICATOR) {
            fillWith(IDENTIFICATOR, ScannerState(IDENTIFICATOR), ScannerState(IDENTIFICATOR), ScannerState(IDENTIFICATOR));
            fillWith(IDENTIFICATOR, false, true);
        }
        if (i == COMMENT || i == SINGLE_COMMENT) {
            fillWith((TokenType)i, ScannerState((TokenType)i, false, false, false, false), ScannerState((TokenType)i, false, false, false, false),
                     ScannerState((TokenType)i, false, false, false, false));
            for (auto c: tokensStr) {
                statesGlobal[(TokenType)i][c] = ScannerState((TokenType)i, false, false, false, false);
            }
            statesGlobal[COMMENT]['}'] = ScannerState(START, false, false, false, false);
            statesGlobal[COMMENT][EOF] = ScannerState(ERROR, false, false, false, false, "Unexpected EOF in ");
        }
        if (i == TOKEN_STRING) {
            fillWith(TOKEN_STRING, ScannerState(TOKEN_STRING), ScannerState(TOKEN_STRING), ScannerState(TOKEN_STRING));
            for (auto c: tokensStr) {
                statesGlobal[TOKEN_STRING][c] = ScannerState(TOKEN_STRING);
            }

        }

    }

    statesGlobal[NUMBER]['.'] = ScannerState(REAL_NUMBER, false, false);
    statesGlobal[NUMBER]['e'] = ScannerState(NUMBER_E, false, false);
    statesGlobal[REAL_NUMBER]['.'] = ScannerState(DOUBLE_POINT, true, true, true);
    statesGlobal[NUMBER_E]['+'] = ScannerState(NUMBER_E_POW, false, false);
    statesGlobal[NUMBER_E]['-'] = ScannerState(NUMBER_E_POW, false, false);
    statesGlobal[POINT]['.'] = ScannerState(POINT, true, true, true);
    statesGlobal[ADD]['=']  = ScannerState(EQUAL, true, true, true);
    statesGlobal[COLON]['=']  = ScannerState(EQUAL, true, true, true);
    statesGlobal[MULT]['=']  = ScannerState(EQUAL, true, true, true);
    statesGlobal[DIVISION]['=']  = ScannerState(EQUAL, true, true, true);
    statesGlobal[SUB]['='] = ScannerState(EQUAL, true, true, true);
    statesGlobal[LESS_THAN]['='] = ScannerState(EQUAL, true, true, true);
    statesGlobal[GREATER_THAN]['='] = ScannerState(EQUAL, true, true, true);
    statesGlobal[COLON]['='] = ScannerState(EQUAL, true, true, true);
    statesGlobal[LESS_THAN]['>'] = ScannerState(GREATER_THAN, true, true, true);
    statesGlobal[LESS_THAN]['<'] = ScannerState(LESS_THAN, true, true, true);
    statesGlobal[GREATER_THAN]['>'] = ScannerState(GREATER_THAN, true, true, true);
    statesGlobal[DIVISION]['/']  = ScannerState(DIVISION, true, true, true);
    statesGlobal[MULT]['*']  = ScannerState(MULT, true, true, true);
    statesGlobal[IDENTIFICATOR]['_'] = ScannerState(IDENTIFICATOR);
    statesGlobal[DIVISION]['/'] = ScannerState(SINGLE_COMMENT, false, false, false, false);
    statesGlobal[SINGLE_COMMENT]['\n'] = ScannerState(START, false, false, false, false);
    statesGlobal[SINGLE_COMMENT]['\t'] = ScannerState(START, false, false, false, false);
    statesGlobal[SINGLE_COMMENT]['\0'] = ScannerState(START, false, false, false, false);
    statesGlobal[TOKEN_STRING]['\''] = ScannerState(START, false, true, false, false);
    statesGlobal[TOKEN_STRING][EOF] = ScannerState(ERROR, false, false, false, false, "Unexpected EOF in ");

//    for (auto i : statesGlobal[TOKEN_STRING]) {
//        std::cout << i.first << "   " << enum_to_str[i.second.token] << "   " <<i.second.add << "  " << i.second.toSave << std::endl;
//    }

};

Lexem Scanner::GetLexem() {
    return last;
}

Scanner::Scanner(char* filename) {
    freopen(filename, "r", stdin);
    InitStates();
}

void Scanner::ReplaceEscapeSymbols(int replaceLen, std::string escapeSymbols) {
    toSave.pop_back();
    toSave += escapeSymbols;
    toSaveLenStr += replaceLen;
    return;
}

Lexem Scanner::TokenStrChgPos(Lexem l) {
    if (l.token == TOKEN_STRING) {
        l.pos = std::make_pair(pos.first, pos.second - toSaveLenStr + 1);
    }
    return l;
}

void Scanner::CheckSymbol(int c) {
    if ((currentState.toSave && currentState.token != START) || (toSave.back() == '\\' && c == '\'')) {
        if ((currentState.token == TOKEN_STRING && toSave.back() == '\\') || (toSave.back() == '\\' && c == '\'')) {
            if ((char)c == 'n') {
                return ReplaceEscapeSymbols(1, "\n");
            }
            if ((char)c == 't') {
                return ReplaceEscapeSymbols(1, "\t");
            }
            if ((char)c == '\'') {
                currentState = ScannerState(TOKEN_STRING);
                return ReplaceEscapeSymbols(1, "\'");
            }
            if ((char)c == '\\') {
                return ReplaceEscapeSymbols(1, "\\");
            }
            if ((char)c == 'b') {
                return ReplaceEscapeSymbols(2, "\b");
            }
            if ((char)c == 'f') {
                return ReplaceEscapeSymbols(1, "\f");
            }
            if ((char)c == 'v') {
                return ReplaceEscapeSymbols(1, "\v");
            }
        }
        toSave += (char)c;
        toSaveLenStr++;
    }
}

void Scanner::CheckError() {
    if (currentState.token == ERROR) {
        throw ScannerException(currentState.errorMsg + "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second + 1) + ")");
    }
}

Lexem Scanner::SaveLexem(ScannerState last, Lexem l) {
    if (currentState.add) {
        l.val += toSave.back();
        if (currentState.find) {
            if (doubleOperators.find(l.val) != doubleOperators.end()) {
                l.token = doubleOperators[l.val];
            }
        }
        currentState = ScannerState(START);
    }

    toSave = toSave.substr(l.val.length(), toSave.length() - l.val.length());

    if (l.token == IDENTIFICATOR) {
        if (reserveWords.find(l.val) != reserveWords.end()) {
            l.token = reserveWords[l.val];
        }
    }

    l.pos = std::make_pair(pos.first, pos.second - l.val.length() + 1);
    return TokenStrChgPos(l);
}

ScannerState Scanner::ChangeState(int c) {
    ScannerState last = currentState;
    currentState = statesGlobal[currentState.token][(char)c];
    CheckError();
    return last;
}

void Scanner::IncPos(char c) {
    pos.second++;
    if (c == '\n') {
        pos.first++;
        pos.second = 0;
    }
}

void Scanner::NextToken() {
    int c;
    Lexem l;
    toSaveLenStr = 0;
    while ((backBuffer.length() > 0 && (c = backBuffer[0])) || ((c = getchar()) != EOF)) {
        bool increnemnt = false;
        if (backBuffer.length() > 0) {
            backBuffer = backBuffer.substr(1, backBuffer.length());
            increnemnt = true;
            IncPos(c);
        }

        if (statesGlobal[currentState.token][(char)c].token == DOUBLE_POINT) {
            if (toSave.length() > 1) {
                l = Lexem(toSave.substr(0, toSave.length() - 1), NUMBER, std::make_pair(pos.first, pos.second - l.val.length()));
                toSave = toSave.substr(toSave.length() - 1, 1);
                backBuffer = toSave.back();
                this->last = l;
                return;
            }
        }

        ScannerState last = ChangeState(c);

        l = Lexem(toSave, last.token);

        CheckSymbol(c);


        bool flag = currentState.save;
        if (currentState.save) {
            l = SaveLexem(last, l);
        }

        if (!increnemnt) {
            IncPos(c);
        }

        if (flag) {
            this->last = l;
            return;
        }

    }

    eof = true;

    ScannerState last = ChangeState(c);

    if (currentState.save) {
        l = Lexem(toSave, last.token, std::make_pair(pos.first, pos.second - toSave.length() + 1));
        if (l.token == IDENTIFICATOR) {
            if (reserveWords.find(l.val) != reserveWords.end()) {
                l.token = reserveWords[l.val];
            }
        }
        this->last = TokenStrChgPos(l);
        return;
    }

    if (eof) {
        eof = false;
        this->last = Lexem("EOF", T_EOF, std::make_pair(pos.first, pos.second));
        return;
    }
}