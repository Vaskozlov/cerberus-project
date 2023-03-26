#pragma once

#include <ccl/flatmap.hpp>
#include <ccl/handler/cmd.hpp>
#include <ccl/lex/tokenizer.hpp>

namespace ccl
{
    enum CLexerToken : ccl::Id
    {
        DECLS = 17179869184,
        KEYWORD = 12884901888,
        NONE = 0,
        OPERATORS = 8589934592,
        SEPARATOR = 4294967296,
        EOI = 0,
        BAD_TOKEN = 1,
        COMMENT = 2,
        CURLY_OPENING = 4294967296,
        CURLY_CLOSING = 4294967297,
        LEFT_PARENTHESIS = 4294967298,
        RIGHT_PARENTHESIS = 4294967299,
        ANGLE_OPENING = 4294967300,
        ANGLE_CLOSING = 4294967301,
        COMMA = 4294967302,
        COLUMN = 4294967303,
        PREPROCESSOR = 4294967304,
        QUESTION_MARK = 4294967305,
        ADD = 8589934592,
        SUB = 8589934593,
        MUL = 8589934594,
        DIV = 8589934595,
        MOD = 8589934596,
        ASSIGN = 8589934597,
        ADD_ASSIGN = 8589934598,
        SUB_ASSIGN = 8589934599,
        MUL_ASSIGN = 8589934600,
        DIB_ASSIGN = 8589934601,
        MOD_ASSIGN = 8589934602,
        OR = 8589934603,
        AND = 8589934604,
        XOR = 8589934605,
        BITWISE_NOT = 8589934606,
        OR_ASSIGN = 8589934607,
        AND_ASSIGN = 8589934608,
        XOR_ASSIGN = 8589934609,
        LESS = 8589934610,
        GREATER = 8589934611,
        EQUAL = 8589934612,
        NOT_EQUAL = 8589934613,
        LESS_EQUAL = 8589934614,
        GREATER_EQUAL = 8589934615,
        LOGICAL_OR = 8589934616,
        LOGICAL_AND = 8589934617,
        LOGICAL_NOT = 8589934618,
        FOR = 12884901888,
        WHILE = 12884901889,
        SWITCH = 12884901890,
        CASE = 12884901891,
        RETURN = 12884901892,
        STRUCT = 12884901893,
        NUMBER = 17179869184,
        FLOAT = 17179869185,
        IDENTIFIER = 17179869186,
        CHAR = 17179869187,
        STRING = 17179869188,
    };

    // NOLINTNEXTLINE
    inline auto CLexer = ccl::lex::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {CLexerToken::COMMENT, R"( ! "//"[\n]*^ )"},
            {CLexerToken::COMMENT, R"( ! "/*" "*/"^* "*/" )"},
            {CLexerToken::CURLY_OPENING, R"( ! [{] )"},
            {CLexerToken::CURLY_CLOSING, R"( ! [}] )"},
            {CLexerToken::LEFT_PARENTHESIS, R"( ! [(] )"},
            {CLexerToken::RIGHT_PARENTHESIS, R"( ! [)] )"},
            {CLexerToken::ANGLE_OPENING, R"( ! [\[] )"},
            {CLexerToken::ANGLE_CLOSING, R"( ! [\]] )"},
            {CLexerToken::COMMA, R"( ! [,] )"},
            {CLexerToken::COLUMN, R"( ! [;] )"},
            {CLexerToken::PREPROCESSOR, R"( ! [#] )"},
            {CLexerToken::QUESTION_MARK, R"( ! [?] )"},
            {CLexerToken::ADD, R"( ! [+] )"},
            {CLexerToken::SUB, R"( ! [\-] )"},
            {CLexerToken::MUL, R"( ! [*] )"},
            {CLexerToken::DIV, R"( ! [/] )"},
            {CLexerToken::MOD, R"( ! [%] )"},
            {CLexerToken::ASSIGN, R"( ! [=] )"},
            {CLexerToken::ADD_ASSIGN, R"( ! "+=" )"},
            {CLexerToken::SUB_ASSIGN, R"( ! "-=" )"},
            {CLexerToken::MUL_ASSIGN, R"( ! "*=" )"},
            {CLexerToken::DIB_ASSIGN, R"( ! "/=" )"},
            {CLexerToken::MOD_ASSIGN, R"( ! "%=" )"},
            {CLexerToken::OR, R"( ! [|] )"},
            {CLexerToken::AND, R"( ! [&] )"},
            {CLexerToken::XOR, R"( ! [^] )"},
            {CLexerToken::BITWISE_NOT, R"( ! [~] )"},
            {CLexerToken::OR_ASSIGN, R"( ! "|=" )"},
            {CLexerToken::AND_ASSIGN, R"( ! "&=" )"},
            {CLexerToken::XOR_ASSIGN, R"( ! "^=" )"},
            {CLexerToken::LESS, R"( ! "<" )"},
            {CLexerToken::GREATER, R"( ! ">" )"},
            {CLexerToken::EQUAL, R"( ! "==" )"},
            {CLexerToken::NOT_EQUAL, R"( ! "!=" )"},
            {CLexerToken::LESS_EQUAL, R"( ! "<=" )"},
            {CLexerToken::GREATER_EQUAL, R"( ! ">=" )"},
            {CLexerToken::LOGICAL_OR, R"( ! "||" )"},
            {CLexerToken::LOGICAL_AND, R"( ! "&&" )"},
            {CLexerToken::LOGICAL_NOT, R"( ! [!] )"},
            {CLexerToken::FOR, R"( "for" )"},
            {CLexerToken::WHILE, R"( "while" )"},
            {CLexerToken::SWITCH, R"( "switch" )"},
            {CLexerToken::CASE, R"( "case" )"},
            {CLexerToken::RETURN, R"( "return" )"},
            {CLexerToken::STRUCT, R"( "struct" )"},
            {CLexerToken::NUMBER, R"( [0-9]+ ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
            {CLexerToken::FLOAT, R"( [0-9]+ "."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
            {CLexerToken::IDENTIFIER, R"( [a-zA-Z_]+ [a-zA-Z0-9_]* )"},
            {CLexerToken::CHAR, R"( ! "\'" ([']^ | "\\\'" ) "\'" )"},
            {CLexerToken::STRING, R"( ! "\"" (["]^ | "\\\"")* "\"" )"},
        }};

    inline constexpr ccl::StaticFlatmap<ccl::Id, ccl::string_view, 52> ToStringCLexerToken{
        {CLexerToken::EOI, "EOI"},
        {CLexerToken::BAD_TOKEN, "BAD_TOKEN"},
        {CLexerToken::COMMENT, "COMMENT"},
        {CLexerToken::CURLY_OPENING, "CURLY_OPENING"},
        {CLexerToken::CURLY_CLOSING, "CURLY_CLOSING"},
        {CLexerToken::LEFT_PARENTHESIS, "LEFT_PARENTHESIS"},
        {CLexerToken::RIGHT_PARENTHESIS, "RIGHT_PARENTHESIS"},
        {CLexerToken::ANGLE_OPENING, "ANGLE_OPENING"},
        {CLexerToken::ANGLE_CLOSING, "ANGLE_CLOSING"},
        {CLexerToken::COMMA, "COMMA"},
        {CLexerToken::COLUMN, "COLUMN"},
        {CLexerToken::PREPROCESSOR, "PREPROCESSOR"},
        {CLexerToken::QUESTION_MARK, "QUESTION_MARK"},
        {CLexerToken::ADD, "ADD"},
        {CLexerToken::SUB, "SUB"},
        {CLexerToken::MUL, "MUL"},
        {CLexerToken::DIV, "DIV"},
        {CLexerToken::MOD, "MOD"},
        {CLexerToken::ASSIGN, "ASSIGN"},
        {CLexerToken::ADD_ASSIGN, "ADD_ASSIGN"},
        {CLexerToken::SUB_ASSIGN, "SUB_ASSIGN"},
        {CLexerToken::MUL_ASSIGN, "MUL_ASSIGN"},
        {CLexerToken::DIB_ASSIGN, "DIB_ASSIGN"},
        {CLexerToken::MOD_ASSIGN, "MOD_ASSIGN"},
        {CLexerToken::OR, "OR"},
        {CLexerToken::AND, "AND"},
        {CLexerToken::XOR, "XOR"},
        {CLexerToken::BITWISE_NOT, "BITWISE_NOT"},
        {CLexerToken::OR_ASSIGN, "OR_ASSIGN"},
        {CLexerToken::AND_ASSIGN, "AND_ASSIGN"},
        {CLexerToken::XOR_ASSIGN, "XOR_ASSIGN"},
        {CLexerToken::LESS, "LESS"},
        {CLexerToken::GREATER, "GREATER"},
        {CLexerToken::EQUAL, "EQUAL"},
        {CLexerToken::NOT_EQUAL, "NOT_EQUAL"},
        {CLexerToken::LESS_EQUAL, "LESS_EQUAL"},
        {CLexerToken::GREATER_EQUAL, "GREATER_EQUAL"},
        {CLexerToken::LOGICAL_OR, "LOGICAL_OR"},
        {CLexerToken::LOGICAL_AND, "LOGICAL_AND"},
        {CLexerToken::LOGICAL_NOT, "LOGICAL_NOT"},
        {CLexerToken::FOR, "FOR"},
        {CLexerToken::WHILE, "WHILE"},
        {CLexerToken::SWITCH, "SWITCH"},
        {CLexerToken::CASE, "CASE"},
        {CLexerToken::RETURN, "RETURN"},
        {CLexerToken::STRUCT, "STRUCT"},
        {CLexerToken::NUMBER, "NUMBER"},
        {CLexerToken::FLOAT, "FLOAT"},
        {CLexerToken::IDENTIFIER, "IDENTIFIER"},
        {CLexerToken::CHAR, "CHAR"},
        {CLexerToken::STRING, "STRING"},
    };
}// namespace ccl
