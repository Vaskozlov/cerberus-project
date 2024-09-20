#include <ast-lang/ast-lang.hpp>

namespace astlang
{
    constexpr isl::string_view AstLangGrammar = R"(
%PARSER%

GOAL:
    GLOBAL_DECLARATIONS

GLOBAL_DECLARATIONS:
    GLOBAL_DECLARATION GLOBAL_DECLARATIONS
    | GLOBAL_DECLARATION
    | EPSILON

GLOBAL_DECLARATION:
    FUNCTION_DEFINITION
    | PARSER_RULE
    | LEXER_RULE
    | VARIABLE_DECLARATION ';'
    | TYPE_VARIABLE_DECLARATION ';'

LEXER_RULE:
    "lexer" IDENTIFIER
    '{'
        LEXER_RULE_BODY
    '}'

LEXER_RULE_BODY:
    LEXER_RULE_BODY ',' EXPRESSION
    | EXPRESSION
    | EPSILON

PARSER_RULE:
    "parser" IDENTIFIER
    '{'
        PARSER_RULE_BODY
    '}'

PARSER_RULE_BODY:
    PARSER_SYMBOLS ';' PARSER_FUNCTIONS

PARSER_FUNCTIONS:
    PARSER_FUNCTIONS FUNCTION_DEFINITION
    | FUNCTION_DEFINITION
    | EPSILON

PARSER_SYMBOLS:
    PARSER_SYMBOLS ',' EXPRESSION
    | EXPRESSION
    | EPSILON

FUNCTION_DEFINITION:
    "def" IDENTIFIER '(' FUNCTION_DEFINITION_ARGUMENTS ')' FUNCTION_RETURN_TYPE
    '{'
        STATEMENTS
    '}'

FUNCTION_DEFINITION_ARGUMENTS:
    FUNCTION_DEFINITION_ARGUMENT ',' FUNCTION_DEFINITION_ARGUMENTS
    | FUNCTION_DEFINITION_ARGUMENT
    | EPSILON

FUNCTION_DEFINITION_ARGUMENT:
    TYPE IDENTIFIER

FUNCTION_RETURN_TYPE:
    '->' TYPE
    | EPSILON

STATEMENTS:
    STATEMENT STATEMENTS
    | STATEMENT
    | EPSILON

STATEMENT:
    LOGICAL_OR_EXPRESSION ';'
    | VARIABLE_DECLARATION ';'
    | TYPE_VARIABLE_DECLARATION ';'
    | FOR_LOOP
    | IF_STATEMENT
    | RETURN_STATEMENT ';'

RETURN_STATEMENT:
    "return" LOGICAL_OR_EXPRESSION
    | "return"

IF_STATEMENT:
    IF_BLOCK
    ELIF_BLOCK
    ELSE_BLOCK

IF_BLOCK:
    "if" LOGICAL_OR_EXPRESSION
    '{'
        STATEMENTS
    '}'

ELIF_BLOCK:
    "elif" LOGICAL_OR_EXPRESSION
    '{'
        STATEMENTS
    '}'
    ELIF_BLOCK
    | EPSILON

ELSE_BLOCK:
    "else"
    '{'
        STATEMENTS
    '}'
    | EPSILON


LOGICAL_OR_EXPRESSION:
    LOGICAL_OR_EXPRESSION '||' LOGICAL_AND_EXPRESSION
    | LOGICAL_AND_EXPRESSION

LOGICAL_AND_EXPRESSION:
    LOGICAL_AND_EXPRESSION '&&' BITWISE_OR_EXPRESSION
    | BITWISE_OR_EXPRESSION

BITWISE_OR_EXPRESSION:
    BITWISE_OR_EXPRESSION '|' BITWISE_XOR_EXPRESSION
    | BITWISE_XOR_EXPRESSION

BITWISE_XOR_EXPRESSION:
    BITWISE_XOR_EXPRESSION '^' BITWISE_AND_EXPRESSION
    | BITWISE_AND_EXPRESSION

BITWISE_AND_EXPRESSION:
    BITWISE_AND_EXPRESSION '&' EQUALITY
    | EQUALITY

EQUALITY:
    EQUALITY '==' COMPARISON
    | EQUALITY '!=' COMPARISON
    | COMPARISON

COMPARISON:
    COMPARISON '<' EXPRESSION
    | COMPARISON '>' EXPRESSION
    | COMPARISON '<=' EXPRESSION
    | COMPARISON '>=' EXPRESSION
    | EXPRESSION

EXPRESSION:
    EXPRESSION '+' TERM
    | EXPRESSION '-' TERM
    | TERM

TERM:
    TERM '*' VALUE
    | TERM '/' VALUE
    | VALUE

VALUE:
    '-' VALUE
    | '+' VALUE
    | FACTOR

VARIABLE_DECLARATION:
    "var" IDENTIFIER '=' LOGICAL_OR_EXPRESSION

TYPE_VARIABLE_DECLARATION:
    "var" IDENTIFIER ':' TYPE '=' LOGICAL_OR_EXPRESSION

FOR_LOOP:
    "for" IDENTIFIER "in" LOGICAL_OR_EXPRESSION
    '{'
        STATEMENTS
    '}'

FUNCTION_CALL:
    IDENTIFIER '(' FUNCTION_CALL_ARGUMENTS ')'

FUNCTION_CALL_ARGUMENTS:
    LOGICAL_OR_EXPRESSION ',' FUNCTION_CALL_ARGUMENTS
    | LOGICAL_OR_EXPRESSION

METHOD_CALL:
    FACTOR '.' IDENTIFIER '(' FUNCTION_CALL_ARGUMENTS ')'

FACTOR:
    NUMBER
    | ARRAY
    | "true"
    | "false"
    | IDENTIFIER
    | STRING
    | FUNCTION_CALL
    | METHOD_CALL
    | '(' LOGICAL_OR_EXPRESSION ')'

ARRAY:
    '[' ARRAY_ELEMENTS ']'

ARRAY_ELEMENTS:
    LOGICAL_OR_EXPRESSION ',' ARRAY_ELEMENTS
    | LOGICAL_OR_EXPRESSION

TYPE:
    IDENTIFIER
    | IDENTIFIER '[' TYPE ']'

%LEXER%
IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*
NUMBER:     [0-9]+
STRING:     ! ["] ("\\\"" | ["]^)* ["]
)";
}