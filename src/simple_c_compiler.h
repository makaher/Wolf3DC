#ifndef SIMPLE_C_COMPILER_H
#define SIMPLE_C_COMPILER_H

#include <string>
#include <vector>
#include <memory>
#include <map>

enum class TokenType {
    // Keywords
    KW_INT, KW_CHAR, KW_VOID, KW_FLOAT, KW_DOUBLE, KW_STRUCT, KW_UNION, KW_ENUM,
    KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_DO, KW_SWITCH, KW_CASE, KW_DEFAULT,
    KW_BREAK, KW_CONTINUE, KW_RETURN, KW_GOTO, KW_TYPEDEF, KW_EXTERN, KW_STATIC,
    KW_CONST, KW_VOLATILE, KW_UNSIGNED, KW_SIGNED,
    
    // Literals
    IDENTIFIER, NUMBER, STRING, CHAR_LITERAL,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN,
    EQ, NE, LT, GT, LE, GE,
    AND, OR, NOT, BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
    LEFT_SHIFT, RIGHT_SHIFT,
    INCREMENT, DECREMENT,
    QUESTION, COLON,
    
    // Delimiters
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    SEMICOLON, COMMA, DOT, ARROW, ELLIPSIS,
    
    // Special
    EOF_TOKEN, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

struct CompileError {
    int line;
    int column;
    std::string message;
    std::string severity; // "error", "warning"
};

class SimpleCLexer {
public:
    explicit SimpleCLexer(const std::string& source);
    std::vector<Token> tokenize();
    
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    
    char current();
    char peek(size_t offset = 1);
    void advance();
    void skipWhitespace();
    void skipComment();
    
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readString();
    Token readChar();
    Token readOperator();
    
    bool isKeyword(const std::string& word);
    TokenType getKeywordType(const std::string& word);
};

class SimpleCParser {
public:
    explicit SimpleCParser(const std::vector<Token>& tokens);
    std::vector<CompileError> parse();
    
private:
    std::vector<Token> tokens;
    size_t pos;
    std::vector<CompileError> errors;
    
    const Token& current();
    const Token& peek(size_t offset = 1);
    void advance();
    bool match(TokenType type);
    bool consume(TokenType type, const std::string& message);
    
    void parseProgram();
    void parseDeclaration();
    void parseFunction();
    void parseStatement();
    void parseExpression();
    void parseBlock();
    
    void error(const std::string& message);
};

class SimpleCCompiler {
public:
    explicit SimpleCCompiler(const std::string& source);
    std::vector<CompileError> compile();
    bool hasErrors() const;
    
private:
    std::string source;
};

#endif // SIMPLE_C_COMPILER_H
