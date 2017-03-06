#include "Scanner.h"

void fillWith(Token state, TokenType bl, TokenType sl, TokenType nums) {
    for (char c = 'A'; c <= 'Z'; c++) {
        states_global[state][c] = bl;
    }
    for (char c = 'a'; c <= 'z'; c++) {
        states_global[state][c] = sl;
    }
    for (char c = '0'; c <= '9'; c++) {
        states_global[state][c] = nums;
    }
}

void fillWith(Token state, bool add, bool save) {
    for (auto c: tokensStr) {
        states_global[state][c] = TokenType(tokenToTokenType[c], add, save);
    }
    states_global[state]['\n'] = TokenType(START, add, save, false, false);
    states_global[state]['\t'] = TokenType(START, add, save, false, false);
    states_global[state]['\0'] = TokenType(START, add, save, false, false);
    states_global[state][' '] = TokenType(START, add, save, false, false);
    states_global[state]['{'] = TokenType(COMMENT, add, save, false, false);
    states_global[state]['\''] = TokenType(TOKEN_STRING, add, save, false, false);
};

void InitStates() {
    for (int i = START; i < states_global.size(); i++) {
        if (i == START) {
            fillWith(START, TokenType(IDENTIFICATOR), TokenType(IDENTIFICATOR), TokenType(NUMBER));
            fillWith(START, false, false);
        }
        if (i == NUMBER || i == REAL_NUMBER) {
            TokenType tmp = TokenType(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((Token)i, tmp, tmp, TokenType((Token)i));
            fillWith((Token)i, false, true);
        }
        if (i == NUMBER_E) {
            TokenType tmp = TokenType(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((Token)i, tmp, tmp, TokenType(NUMBER_E));
            fillWith((Token)i, false, true);
        }
        if (i == NUMBER_E_POW) {
            TokenType tmp = TokenType(ERROR, false, false, false, false, "Unknown symbol in ");
            fillWith((Token)i, tmp, tmp, TokenType(NUMBER_E_POW));
            fillWith((Token)i, false, true);
        }
        if (i >= BIT_NOT && i < COMMENT) {
            fillWith((Token)i, TokenType(IDENTIFICATOR, false, true), TokenType(IDENTIFICATOR, false, true), TokenType(NUMBER, false, true));
            fillWith((Token)i, false, true);
        }
        if (i == IDENTIFICATOR) {
            fillWith(IDENTIFICATOR, TokenType(IDENTIFICATOR), TokenType(IDENTIFICATOR), TokenType(IDENTIFICATOR));
            fillWith(IDENTIFICATOR, false, true);
        }
        if (i == COMMENT || i == SINGLE_COMMENT) {
            fillWith((Token)i, TokenType((Token)i, false, false, false, false), TokenType((Token)i, false, false, false, false),
                     TokenType((Token)i, false, false, false, false));
            for (auto c: tokensStr) {
                states_global[(Token)i][c] = TokenType((Token)i, false, false, false, false);
            }
            states_global[COMMENT]['}'] = TokenType(START, false, false, false, false);
            states_global[COMMENT][EOF] = TokenType(ERROR, false, false, false, false, "Unexpected EOF in ");
        }
        if (i == TOKEN_STRING) {
            fillWith(TOKEN_STRING, TokenType(TOKEN_STRING), TokenType(TOKEN_STRING), TokenType(TOKEN_STRING));
            for (auto c: tokensStr) {
                states_global[TOKEN_STRING][c] = TokenType(TOKEN_STRING);
            }

        }

    }

    states_global[NUMBER]['.'] = TokenType(REAL_NUMBER, false, false);
    states_global[NUMBER]['e'] = TokenType(NUMBER_E, false, false);
    states_global[REAL_NUMBER]['.'] = TokenType(DOUBLE_POINT, true, true, true);
    states_global[NUMBER_E]['+'] = TokenType(NUMBER_E_POW, false, false);
    states_global[NUMBER_E]['-'] = TokenType(NUMBER_E_POW, false, false);
    states_global[POINT]['.'] = TokenType(POINT, true, true, true);
    states_global[ADD]['=']  = TokenType(EQUAL, true, true, true);
    states_global[COLON]['=']  = TokenType(EQUAL, true, true, true);
    states_global[MULT]['=']  = TokenType(EQUAL, true, true, true);
    states_global[DIVISION]['=']  = TokenType(EQUAL, true, true, true);
    states_global[SUB]['='] = TokenType(EQUAL, true, true, true);
    states_global[LESS_THAN]['='] = TokenType(EQUAL, true, true, true);
    states_global[GREATER_THAN]['='] = TokenType(EQUAL, true, true, true);
    states_global[COLON]['='] = TokenType(EQUAL, true, true, true);
    states_global[LESS_THAN]['>'] = TokenType(GREATER_THAN, true, true, true);
    states_global[LESS_THAN]['<'] = TokenType(LESS_THAN, true, true, true);
    states_global[GREATER_THAN]['>'] = TokenType(GREATER_THAN, true, true, true);
    states_global[DIVISION]['/']  = TokenType(DIVISION, true, true, true);
    states_global[MULT]['*']  = TokenType(MULT, true, true, true);
    states_global[IDENTIFICATOR]['_'] = TokenType(IDENTIFICATOR);
    states_global[DIVISION]['/'] = TokenType(SINGLE_COMMENT, false, false, false, false);
    states_global[SINGLE_COMMENT]['\n'] = TokenType(START, false, false, false, false);
    states_global[SINGLE_COMMENT]['\t'] = TokenType(START, false, false, false, false);
    states_global[SINGLE_COMMENT]['\0'] = TokenType(START, false, false, false, false);
    states_global[TOKEN_STRING]['\''] = TokenType(START, false, true, false, false);
    states_global[TOKEN_STRING][EOF] = TokenType(ERROR, false, false, false, false, "Unexpected EOF in ");

//    for (auto i : states_global[TOKEN_STRING]) {
//        std::cout << i.first << "   " << enum_to_str[i.second.tokenType] << "   " <<i.second.add << "  " << i.second.to_save << std::endl;
//    }

};

Scanner::Scanner(char* filename) {
    freopen(filename, "r", stdin);
}

void Scanner::CheckSymbol(int c) {
    if (current_state.to_save && current_state.tokenType != START) {
        to_save += (char)c;
    }
}

void Scanner::CheckError() {
    if (current_state.tokenType == ERROR) {
            std::cout << current_state.errorMsg << "(" << cur_pos.first << ", " << cur_pos.second + 1 << ")" << std::endl;
            exit(0);
    }
}

Lexem Scanner::SaveLexem(TokenType last, Lexem l) {
    if (current_state.add) {
        l.val += to_save.back();
        if (current_state.find) {
            if (doubleOperators.find(l.val) != doubleOperators.end()) {
                l.tokenType = doubleOperators[l.val];
            }
        }
        current_state = TokenType(START);
    }

    to_save = to_save.substr(l.val.length(), to_save.length() - l.val.length());

    if (l.tokenType == IDENTIFICATOR) {
        if (reserve_words.find(l.val) != reserve_words.end()) {
            l.tokenType = reserve_words[l.val];
        }
    }
    l.pos = std::make_pair(cur_pos.first, cur_pos.second - l.val.length() + 1);
    return l;
}

TokenType Scanner::ChangeState(int c) {
    TokenType last = current_state;
    current_state = states_global[current_state.tokenType][(char)c];
    CheckError();
    return last;
}

void Scanner::IncPos(char c) {
    cur_pos.second++;
    if (c == '\n') {
        cur_pos.first++;
        cur_pos.second = 0;
    }
}

Lexem Scanner::NextToken() {
    int c;
    Lexem l;
    while ((backbuffer.length() > 0 && (c = backbuffer[0])) || ((c = getchar()) != EOF)) {
        bool increnemnt = false;
        if (backbuffer.length() > 0) {
            backbuffer = backbuffer.substr(1, backbuffer.length());
            increnemnt = true;
            IncPos(c);
        }

        if (states_global[current_state.tokenType][(char)c].tokenType == DOUBLE_POINT) {
            if (to_save.length() > 1) {
                l = Lexem(to_save.substr(0, to_save.length() - 1), NUMBER, std::make_pair(cur_pos.first, cur_pos.second - l.val.length()));
                to_save = to_save.substr(to_save.length() - 1, 1);
                backbuffer = to_save.back();
                return l;
            }
        }

        TokenType last = ChangeState(c);

        l = Lexem(to_save, last.tokenType);

        CheckSymbol(c);


        bool flag = current_state.save;
        if (current_state.save) {
            l = SaveLexem(last, l);
        }

        if (!increnemnt) {
            IncPos(c);
        }

        if (flag) {
            return l;
        }

    }

    eof = true;

    TokenType last = ChangeState(c);

    if (current_state.save) {
        l = Lexem(to_save, last.tokenType, std::make_pair(cur_pos.first, cur_pos.second - to_save.length() + 1));
        if (l.tokenType == IDENTIFICATOR) {
            if (reserve_words.find(l.val) != reserve_words.end()) {
                l.tokenType = reserve_words[l.val];
            }
        }
        return l;
    }

    if (eof) {
        eof = false;
        return Lexem("EOF", T_EOF, std::make_pair(cur_pos.first, cur_pos.second));
    }
}



