#include "simple_c_compiler.h"
#include "logger.h"
#include <cctype>
#include <algorithm>
#include <sstream>

// ============= SimpleCLexer =============

SimpleCLexer::SimpleCLexer(const std::string& source)
    : source(source), pos(0), line(1), column(1) {}

char SimpleCLexer::current() {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char SimpleCLexer::peek(size_t offset) {
    if (pos + offset >= source.length()) return '\0';
    return source[pos + offset];
}

void SimpleCLexer::advance() {
    if (pos < source.length()) {
        if (source[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

void SimpleCLexer::skipWhitespace() {
    while (std::isspace(current())) {
        advance();
    }
}

void SimpleCLexer::skipComment() {
    if (current() == '/' && peek() == '/') {
        while (current() != '\n' && current() != '\0') {
            advance();
        }
    } else if (current() == '/' && peek() == '*') {
        advance();
        advance();
        while (current() != '\0') {
            if (current() == '*' && peek() == '/') {
                advance();
                advance();
                break;
            }
            advance();
        }
    }
}

bool SimpleCLexer::isKeyword(const std::string& word) {
    static const std::vector<std::string> keywords = {
        "int", "char", "void", "float", "double", "struct", "union", "enum",
        "if", "else", "while", "for", "do", "switch", "case", "default",
        "break", "continue", "return", "goto", "typedef", "extern", "static",
        "const", "volatile", "unsigned", "signed"
    };
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

TokenType SimpleCLexer::getKeywordType(const std::string& word) {
    static const std::map<std::string, TokenType> keywordMap = {
        {"int", TokenType::KW_INT},
        {"char", TokenType::KW_CHAR},
        {"void", TokenType::KW_VOID},
        {"float", TokenType::KW_FLOAT},
        {"double", TokenType::KW_DOUBLE},
        {"struct", TokenType::KW_STRUCT},
        {"union", TokenType::KW_UNION},
        {"enum", TokenType::KW_ENUM},
        {"if", TokenType::KW_IF},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE},
        {"for", TokenType::KW_FOR},
        {"do", TokenType::KW_DO},
        {"switch", TokenType::KW_SWITCH},
        {"case", TokenType::KW_CASE},
        {"default", TokenType::KW_DEFAULT},
        {"break", TokenType::KW_BREAK},
        {"continue", TokenType::KW_CONTINUE},
        {"return", TokenType::KW_RETURN},
        {"goto", TokenType::KW_GOTO},
        {"typedef", TokenType::KW_TYPEDEF},
        {"extern", TokenType::KW_EXTERN},
        {"static", TokenType::KW_STATIC},
        {"const", TokenType::KW_CONST},
        {"volatile", TokenType::KW_VOLATILE},
        {"unsigned", TokenType::KW_UNSIGNED},
        {"signed", TokenType::KW_SIGNED}
    };
    auto it = keywordMap.find(word);
    return it != keywordMap.end() ? it->second : TokenType::IDENTIFIER;
}

Token SimpleCLexer::readIdentifierOrKeyword() {
    int startLine = line, startCol = column;
    std::string value;
    
    while (std::isalnum(current()) || current() == '_') {
        value += current();
        advance();
    }
    
    Token token;
    token.line = startLine;
    token.column = startCol;
    token.value = value;
    
    if (isKeyword(value)) {
        token.type = getKeywordType(value);
    } else {
        token.type = TokenType::IDENTIFIER;
    }
    
    return token;
}

Token SimpleCLexer::readNumber() {
    int startLine = line, startCol = column;
    std::string value;
    
    while (std::isdigit(current()) || current() == '.') {
        value += current();
        advance();
    }
    
    if (current() == 'x' || current() == 'X') {
        value += current();
        advance();
        while (std::isxdigit(current())) {
            value += current();
            advance();
        }
    }
    
    Token token;
    token.type = TokenType::NUMBER;
    token.value = value;
    token.line = startLine;
    token.column = startCol;
    return token;
}

Token SimpleCLexer::readString() {
    int startLine = line, startCol = column;
    char quote = current();
    advance();
    std::string value;
    
    while (current() != quote && current() != '\0') {
        if (current() == '\\') {
            advance();
            value += current();
            advance();
        } else {
            value += current();
            advance();
        }
    }
    
    if (current() == quote) {
        advance();
    }
    
    Token token;
    token.type = TokenType::STRING;
    token.value = value;
    token.line = startLine;
    token.column = startCol;
    return token;
}

Token SimpleCLexer::readChar() {
    int startLine = line, startCol = column;
    advance();
    std::string value;
    
    while (current() != '\'' && current() != '\0') {
        if (current() == '\\') {
            value += current();
            advance();
            value += current();
            advance();
        } else {
            value += current();
            advance();
        }
    }
    
    if (current() == '\'') {
        advance();
    }
    
    Token token;
    token.type = TokenType::CHAR_LITERAL;
    token.value = value;
    token.line = startLine;
    token.column = startCol;
    return token;
}

Token SimpleCLexer::readOperator() {
    int startLine = line, startCol = column;
    char ch = current();
    Token token;
    token.line = startLine;
    token.column = startCol;
    
    advance();
    
    if (ch == '+') {
        if (current() == '+') { advance(); token.type = TokenType::INCREMENT; }
        else if (current() == '=') { advance(); token.type = TokenType::PLUS_ASSIGN; }
        else { token.type = TokenType::PLUS; }
    } else if (ch == '-') {
        if (current() == '-') { advance(); token.type = TokenType::DECREMENT; }
        else if (current() == '=') { advance(); token.type = TokenType::MINUS_ASSIGN; }
        else if (current() == '>') { advance(); token.type = TokenType::ARROW; }
        else { token.type = TokenType::MINUS; }
    } else if (ch == '*') {
        token.type = TokenType::MULTIPLY;
    } else if (ch == '/') {
        token.type = TokenType::DIVIDE;
    } else if (ch == '%') {
        token.type = TokenType::MODULO;
    } else if (ch == '=') {
        if (current() == '=') { advance(); token.type = TokenType::EQ; }
        else { token.type = TokenType::ASSIGN; }
    } else if (ch == '!') {
        if (current() == '=') { advance(); token.type = TokenType::NE; }
        else { token.type = TokenType::NOT; }
    } else if (ch == '<') {
        if (current() == '=') { advance(); token.type = TokenType::LE; }
        else if (current() == '<') { advance(); token.type = TokenType::LEFT_SHIFT; }
        else { token.type = TokenType::LT; }
    } else if (ch == '>') {
        if (current() == '=') { advance(); token.type = TokenType::GE; }
        else if (current() == '>') { advance(); token.type = TokenType::RIGHT_SHIFT; }
        else { token.type = TokenType::GT; }
    } else if (ch == '&') {
        if (current() == '&') { advance(); token.type = TokenType::AND; }
        else { token.type = TokenType::BIT_AND; }
    } else if (ch == '|') {
        if (current() == '|') { advance(); token.type = TokenType::OR; }
        else { token.type = TokenType::BIT_OR; }
    } else if (ch == '^') {
        token.type = TokenType::BIT_XOR;
    } else if (ch == '~') {
        token.type = TokenType::BIT_NOT;
    } else if (ch == '?') {
        token.type = TokenType::QUESTION;
    } else if (ch == ':') {
        token.type = TokenType::COLON;
    } else if (ch == '.') {
        if (current() == '.' && peek() == '.') {
            advance();
            advance();
            token.type = TokenType::ELLIPSIS;
        } else {
            token.type = TokenType::DOT;
        }
    } else {
        token.type = TokenType::UNKNOWN;
    }
    
    return token;
}

std::vector<Token> SimpleCLexer::tokenize() {
    std::vector<Token> tokens;
    
    while (current() != '\0') {
        skipWhitespace();
        
        if (current() == '/' && (peek() == '/' || peek() == '*')) {
            skipComment();
            continue;
        }
        
        if (current() == '\0') break;
        
        Token token;
        
        if (std::isalpha(current()) || current() == '_') {
            token = readIdentifierOrKeyword();
        } else if (std::isdigit(current())) {
            token = readNumber();
        } else if (current() == '"') {
            token = readString();
        } else if (current() == '\'') {
            token = readChar();
        } else if (current() == '(') {
            token.type = TokenType::LPAREN;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == ')') {
            token.type = TokenType::RPAREN;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == '{') {
            token.type = TokenType::LBRACE;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == '}') {
            token.type = TokenType::RBRACE;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == '[') {
            token.type = TokenType::LBRACKET;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == ']') {
            token.type = TokenType::RBRACKET;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == ';') {
            token.type = TokenType::SEMICOLON;
            token.line = line;
            token.column = column;
            advance();
        } else if (current() == ',') {
            token.type = TokenType::COMMA;
            token.line = line;
            token.column = column;
            advance();
        } else {
            token = readOperator();
        }
        
        if (token.type != TokenType::UNKNOWN) {
            tokens.push_back(token);
        }
    }
    
    Token eofToken;
    eofToken.type = TokenType::EOF_TOKEN;
    eofToken.line = line;
    eofToken.column = column;
    tokens.push_back(eofToken);
    
    return tokens;
}

// ============= SimpleCParser =============

SimpleCParser::SimpleCParser(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0) {}

const Token& SimpleCParser::current() {
    if (pos >= tokens.size()) {
        return tokens.back();
    }
    return tokens[pos];
}

const Token& SimpleCParser::peek(size_t offset) {
    if (pos + offset >= tokens.size()) {
        return tokens.back();
    }
    return tokens[pos + offset];
}

void SimpleCParser::advance() {
    if (pos < tokens.size()) {
        pos++;
    }
}

bool SimpleCParser::match(TokenType type) {
    return current().type == type;
}

bool SimpleCParser::consume(TokenType type, const std::string& message) {
    if (match(type)) {
        advance();
        return true;
    }
    error(message);
    return false;
}

void SimpleCParser::error(const std::string& message) {
    CompileError err;
    err.line = current().line;
    err.column = current().column;
    err.message = message;
    err.severity = "error";
    errors.push_back(err);
}

void SimpleCParser::parseProgram() {
    while (!match(TokenType::EOF_TOKEN)) {
        parseDeclaration();
    }
}

void SimpleCParser::parseDeclaration() {
    if (match(TokenType::KW_EXTERN) || match(TokenType::KW_STATIC) ||
        match(TokenType::KW_CONST) || match(TokenType::KW_VOLATILE)) {
        advance();
    }
    
    // Parse type
    if (match(TokenType::KW_INT) || match(TokenType::KW_CHAR) ||
        match(TokenType::KW_VOID) || match(TokenType::KW_FLOAT) ||
        match(TokenType::KW_DOUBLE) || match(TokenType::KW_STRUCT) ||
        match(TokenType::KW_UNION)) {
        advance();
        
        if (match(TokenType::IDENTIFIER)) {
            advance();
            
            if (match(TokenType::LPAREN)) {
                parseFunction();
            } else {
                while (match(TokenType::COMMA) || match(TokenType::LBRACKET)) {
                    if (match(TokenType::COMMA)) {
                        advance();
                        if (match(TokenType::IDENTIFIER)) advance();
                    } else if (match(TokenType::LBRACKET)) {
                        advance();
                        while (!match(TokenType::RBRACKET) && !match(TokenType::EOF_TOKEN)) {
                            advance();
                        }
                        if (match(TokenType::RBRACKET)) advance();
                    }
                }
                
                if (match(TokenType::ASSIGN)) {
                    advance();
                    parseExpression();
                }
                
                if (match(TokenType::SEMICOLON)) {
                    advance();
                } else {
                    error("Expected ';' after declaration");
                }
            }
        }
    } else if (!match(TokenType::EOF_TOKEN)) {
        advance();
    }
}

void SimpleCParser::parseFunction() {
    // Skip past function parameters
    if (match(TokenType::LPAREN)) {
        advance();
        int parenCount = 1;
        
        while (parenCount > 0 && !match(TokenType::EOF_TOKEN)) {
            if (match(TokenType::LPAREN)) parenCount++;
            else if (match(TokenType::RPAREN)) parenCount--;
            
            if (parenCount > 0) advance();
        }
        
        if (match(TokenType::RPAREN)) advance();
    }
    
    if (match(TokenType::LBRACE)) {
        parseBlock();
    } else if (match(TokenType::SEMICOLON)) {
        advance();
    }
}

void SimpleCParser::parseBlock() {
    if (!consume(TokenType::LBRACE, "Expected '{'")) return;
    
    int braceCount = 1;
    while (braceCount > 0 && !match(TokenType::EOF_TOKEN)) {
        if (match(TokenType::LBRACE)) braceCount++;
        else if (match(TokenType::RBRACE)) braceCount--;
        
        if (braceCount > 0) {
            parseStatement();
        }
    }
    
    if (match(TokenType::RBRACE)) advance();
}

void SimpleCParser::parseStatement() {
    if (match(TokenType::SEMICOLON)) {
        advance();
    } else if (match(TokenType::LBRACE)) {
        parseBlock();
    } else if (match(TokenType::KW_IF)) {
        advance();
        if (match(TokenType::LPAREN)) {
            advance();
            parseExpression();
            if (match(TokenType::RPAREN)) advance();
        }
        parseStatement();
        if (match(TokenType::KW_ELSE)) {
            advance();
            parseStatement();
        }
    } else if (match(TokenType::KW_WHILE) || match(TokenType::KW_FOR) ||
               match(TokenType::KW_DO)) {
        advance();
        if (match(TokenType::LPAREN)) {
            int parenCount = 1;
            advance();
            while (parenCount > 0 && !match(TokenType::EOF_TOKEN)) {
                if (match(TokenType::LPAREN)) parenCount++;
                else if (match(TokenType::RPAREN)) parenCount--;
                if (parenCount > 0) advance();
            }
            if (match(TokenType::RPAREN)) advance();
        }
        parseStatement();
    } else if (match(TokenType::KW_RETURN)) {
        advance();
        if (!match(TokenType::SEMICOLON)) {
            parseExpression();
        }
        if (match(TokenType::SEMICOLON)) advance();
    } else if (match(TokenType::KW_BREAK) || match(TokenType::KW_CONTINUE)) {
        advance();
        if (match(TokenType::SEMICOLON)) advance();
    } else {
        parseExpression();
        if (match(TokenType::SEMICOLON)) advance();
    }
}

void SimpleCParser::parseExpression() {
    while (!match(TokenType::SEMICOLON) && !match(TokenType::RPAREN) &&
           !match(TokenType::RBRACE) && !match(TokenType::COMMA) &&
           !match(TokenType::EOF_TOKEN)) {
        advance();
    }
}

std::vector<CompileError> SimpleCParser::parse() {
    parseProgram();
    return errors;
}

// ============= SimpleCCompiler =============

SimpleCCompiler::SimpleCCompiler(const std::string& source)
    : source(source) {}

std::vector<CompileError> SimpleCCompiler::compile() {
    SimpleCLexer lexer(source);
    auto tokens = lexer.tokenize();
    
    SimpleCParser parser(tokens);
    auto errors = parser.parse();
    
    return errors;
}

bool SimpleCCompiler::hasErrors() const {
    SimpleCCompiler compiler(source);
    auto errors = compiler.compile();
    return !errors.empty();
}
