#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

using namespace std;

// Token types
enum class TokenType {
    IDENTIFIER,
    INTEGER,
    REAL_NUMBER,
    KEYWORD,
    OPERATOR,
    SEPARATOR,
    END_OF_LINE
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// Keywords
unordered_set<string> keywords = { "while", "if", "thus" };

// Operators
unordered_set<char> operators_set = { '+', '-', '*', '/', '>', '<', '=', '!', '&', '|' };
unordered_set<string> operators = { "<=", ">=", "==", "!=" };

// Separators
unordered_set<char> separators = { '(', ')', '[', ']', '{', '}', ';' };

// Lexer class
class Lexer {
public:
    Lexer(const string& filename) : input(filename) {}

    // Function to tokenize the input string
    vector<Token> tokenize() {
        vector<Token> tokens;
        char currentChar;

        // Loop through the input file character by character
        while (input.get(currentChar)) {
            // Skip whitespace characters
            if (isspace(currentChar)) {
                continue;
            }
            // Identify identifiers or keywords
            else if (isalpha(currentChar)) {
                string identifier = "";
                identifier += currentChar;
                while (input.get(currentChar) && (isalnum(currentChar) || currentChar == '_')) {
                    identifier += currentChar;
                }
                // Put back the last character read
                input.putback(currentChar);

                // Check if the identifier is a keyword
                if (keywords.find(identifier) != keywords.end()) {
                    tokens.push_back({ TokenType::KEYWORD, identifier });
                }
                else {
                    tokens.push_back({ TokenType::IDENTIFIER, identifier });
                }
            }
            // Identify integers and real numbers
            else if (isdigit(currentChar) || currentChar == '-') {
                string number = "";
                number += currentChar;
                bool isRealNumber = false;

                while (input.get(currentChar) && (isdigit(currentChar) || currentChar == '.')) {
                    if (currentChar == '.') {
                        isRealNumber = true;
                    }
                    number += currentChar;
                }

                // Put back the last character read
                input.putback(currentChar);

                if (isRealNumber) {
                    tokens.push_back({ TokenType::REAL_NUMBER, number });
                }
                else {
                    tokens.push_back({ TokenType::INTEGER, number });
                }
            }
            // Identify operators
            else if (operators_set.find(currentChar) != operators_set.end()) {
                string op = "";
                op += currentChar;
                while (input.get(currentChar) && operators_set.find(currentChar) != operators_set.end()) {
                    op += currentChar;
                }
                // Put back the last character read
                input.putback(currentChar);

                // Check for multi-character operators
                if (operators.find(op) != operators.end()) {
                    tokens.push_back({ TokenType::OPERATOR, op });
                }
                else {
                    tokens.push_back({ TokenType::OPERATOR, string(1, op[0]) });
                }
            }
            // Identify separators
            else if (separators.find(currentChar) != separators.end()) {
                if (currentChar == ';') {
                    tokens.push_back({ TokenType::END_OF_LINE, ";" });
                }
                else {
                    tokens.push_back({ TokenType::SEPARATOR, string(1, currentChar) });
                }
            }


        }

        return tokens;
    }

private:
    ifstream input;
};

int main() {
    // Hardcoded input file name
    string filename = "input_scode.txt";

    Lexer lexer(filename);
    vector<Token> tokens = lexer.tokenize();

    // Write tokens and lexemes to output file
    ofstream output("output.txt");
    if (output.is_open()) {
        for (const auto& token : tokens) {
            string tokenType;
            switch (token.type) {
            case TokenType::IDENTIFIER:
                tokenType = "IDENTIFIER";
                break;
            case TokenType::INTEGER:
                tokenType = "INTEGER";
                break;
            case TokenType::REAL_NUMBER:
                tokenType = "REAL_NUMBER";
                break;
            case TokenType::KEYWORD:
                tokenType = "KEYWORD";
                break;
            case TokenType::OPERATOR:
                tokenType = "OPERATOR";
                break;
            case TokenType::SEPARATOR:
                tokenType = "SEPARATOR";
                break;
            case TokenType::END_OF_LINE:
                tokenType = "END_OF_LINE";
                break;

            }
            output << tokenType << "\t" << token.value << endl;
        }
        output.close();
        cout << "Tokens and lexemes have been written to output.txt" << endl;
    }
    else {
        cerr << "Unable to open output file!" << endl;
        return 1;
    }

    return 0;
}
