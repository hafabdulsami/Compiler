#include <iostream>
#include <vector>
#include <map>
#include <cctype>
#include <string>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_WHILE,
    T_FOR,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_LT, // Add this line for '<' operator
    T_EOF,
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line;
    vector<string> comments; // Vector to store comments

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->pos = 0;
        this->line = 1; // Initialize line number to 1
    }

    vector<Token> tokenize()
    {
        vector<Token> tokens;

        while (pos < src.size())
        {
            char current = src[pos];

            if (isspace(current))
            {
                if (current == '\n')
                {
                    line++; // Increment line number on new line
                }
                pos++;
                continue;
            }

            // Single-line comment
            if (current == '/' && pos + 1 < src.size() && src[pos + 1] == '/')
            {
                comments.push_back(consumeSingleLineComment());
                continue;
            }

            // Multi-line comment
            if (current == '/' && pos + 1 < src.size() && src[pos + 1] == '*')
            {
                comments.push_back(consumeMultiLineComment());
                continue;
            }

            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }

            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "float")
                    tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, line});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line});
                break;
            case '<':                                     // Add this case
                tokens.push_back(Token{T_LT, "<", line}); // Assuming you want to handle '<'
                break;
            default:
                cout << "Unexpected character: " << current << " at line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

    string consumeSingleLineComment()
    {
        size_t start = pos;
        pos += 2; // Skip '//'
        while (pos < src.size() && src[pos] != '\n')
        {
            pos++;
        }
        string comment = src.substr(start, pos - start);
        return comment;
    }

    string consumeMultiLineComment()
    {
        size_t start = pos;
        pos += 2; // Skip '/*'
        while (pos + 1 < src.size() && !(src[pos] == '*' && src[pos + 1] == '/'))
        {
            if (src[pos] == '\n')
                line++;
            pos++;
        }
        pos += 2; // Skip '*/'
        string comment = src.substr(start, pos - start);
        return comment;
    }

    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos]))
        {
            if (src[pos] == '\n')
                line++;
            pos++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
        {
            if (src[pos] == '\n')
                line++;
            pos++;
        }
        return src.substr(start, pos - start);
    }

    void printComments()
    {
        for (const auto &comment : comments)
        {
            cout << "Comment: " << comment << endl;
        }
    }
};

class Parser
{
public:
    Parser(const vector<Token> &tokens)
    {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

    void parseStatement()
    {
        if (tokens[pos].type == T_INT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_FLOAT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }
    void parseDeclaration()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT)
        {
            pos++;        // Consume the type token (int/float)
            expect(T_ID); // Consume the identifier (e.g., i)

            // Check if there's an assignment
            if (tokens[pos].type == T_ASSIGN)
            {
                pos++;             // Consume the '=' token
                parseExpression(); // Parse the expression (e.g., 10)
            }

            expect(T_SEMICOLON); // Ensure the statement ends with a semicolon
        }
        else
        {
            cout << "Syntax error: expected type declaration but found '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }
    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression(); // Parse the condition
        expect(T_RPAREN);
        parseStatement(); // Parse the statement or block inside the while loop
    }

    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        parseDeclaration(); // Initialization (e.g., int i = 0)
        parseExpression();  // Condition (e.g., i < 10)
        expect(T_SEMICOLON);
        parseExpression(); // Increment (e.g., i = i + 1 or i++)
        expect(T_RPAREN);
        parseStatement(); // Parse the statement or block inside the for loop
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        // Handle possible assignments
        if (tokens[pos].type == T_ID && tokens[pos + 1].type == T_ASSIGN)
        {
            pos++;             // consume identifier
            pos++;             // consume '='
            parseExpression(); // parse the right-hand side of the assignment
            return;            // exit after handling assignment expression
        }

        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT || tokens[pos].type == T_LT)
        {
            pos++;
            parseExpression();
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++;
            parseFactor();
        }
    }

    void parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected token type " << type << " but found '" << tokens[pos].value + tokens[pos + 1].value + tokens[pos - 1].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }
};
int main()
{
    string sourceCode = R"(
    int x = 5;
    float y;
    y = 3;
    if (x > y) {
        x = x + 1;
    }
    while (x > y) {
        x = x - 1;
    }
    for (int i = 0; i < 10; i = i + 1) {
        x = x + i;
    }
    )"; // Properly closed the raw string literal here

    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();
    lexer.printComments(); // Print skipped comments

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
