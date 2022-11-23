#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parser.hpp>

using namespace ccl;

// NOLINTNEXTLINE
CCL_ENUM(// NOLINTNEXTLINE
    TestToken, size_t, EOI, BAD_TOKEN, ROOT, FUNC, ID, NUM, MUL, ADD, NOT, ANGLE_OPENING,
    ANGLE_CLOSING, FACTOR, EXPR);

// NOLINTNEXTLINE
lex::LexicalAnalyzer LexicalAnalyzer(
    handler::Cmd::instance(),
    {
        {TestToken::ID, "[a-zA-Z_][a-zA-Z0-9_]*"},
        {TestToken::NUM, "[0-9]+"},
        {TestToken::MUL, "! [*]"},
        {TestToken::ADD, "! [+]"},
        {TestToken::NOT, "! [!]"},
        {TestToken::ANGLE_CLOSING, R"(! "]" )"},
        {TestToken::ANGLE_OPENING, R"(! "[" )"},
    });

struct FactorWithValue : parser::Factor
{
    FactorConstructor(FactorWithValue, TestToken::FACTOR);
};

struct ExpressionWithValue : parser::ValueExpression
{
    ValueExpressionConstructor(ExpressionWithValue, TestToken::EXPR);
};

struct UnaryExpression : parser::UnaryExpression
{
    UnaryExpressionConstructor(UnaryExpression, TestToken::FACTOR);
};

struct BinaryExpression : parser::BinaryExpression
{
    BinaryExpressionConstructor(BinaryExpression, TestToken::EXPR);
};

struct Root : parser::ValueExpression
{
    ValueExpressionConstructor(Root, TestToken::ROOT);
};

auto constructFactorFromID(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, FactorWithValue>(stack.pop());
}

auto constructExpressionFromFactor(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, ExpressionWithValue>(stack.pop());
}

auto constructUnaryExpression(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, UnaryExpression>(stack.pop(), stack.pop());
}

auto constructBinaryExpression(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, BinaryExpression>(stack.pop(), stack.pop(), stack.pop());
}

auto constructRoot(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<Root>(stack.pop());
}

auto constructIndexOperator(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    auto angle_closing = stack.pop();
    auto expression = stack.pop();
    auto angle_opening = stack.pop();
    auto factor = stack.pop();

    return makeUnique<parser::Node, UnaryExpression>(std::move(factor), std::move(expression));
}

// NOLINTBEGIN
const ccl::UnorderedMap<ccl::Id, size_t> PrecedenceTable(//
    {{TestToken::MUL, 30}, {TestToken::ADD, 10}});
// NOLINTEND

BOOST_AUTO_TEST_CASE(CclParser)
{
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::EOI), TestToken::EOI);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::BAD_TOKEN), TestToken::BAD_TOKEN);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::ROOT), TestToken::ROOT);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::ID), TestToken::ID);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::NUM), TestToken::NUM);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::MUL), TestToken::MUL);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::ADD), TestToken::ADD);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::NOT), TestToken::NOT);
    fmt::print("{:<8}{}\n", TestToken::toString(TestToken::EXPR), TestToken::EXPR);

    auto rules = parser::ParsingRules(
        PrecedenceTable,
        {CCL_PARSING_RULE(TestToken::FACTOR, constructFactorFromID, TestToken::NUM),
         CCL_PARSING_RULE(TestToken::FACTOR, constructFactorFromID, TestToken::ID),
         CCL_PARSING_RULE(
             TestToken::FACTOR, constructIndexOperator, TestToken::FACTOR, TestToken::ANGLE_OPENING,
             TestToken::EXPR, TestToken::ANGLE_CLOSING),
         CCL_PARSING_RULE(TestToken::EXPR, constructExpressionFromFactor, TestToken::FACTOR),
         CCL_PARSING_RULE(
             TestToken::EXPR, constructBinaryExpression, TestToken::EXPR, TestToken::ADD,
             TestToken::EXPR),
         CCL_PARSING_RULE(
             TestToken::EXPR, constructBinaryExpression, TestToken::EXPR, TestToken::MUL,
             TestToken::EXPR),
         CCL_PARSING_RULE(
             TestToken::EXPR, constructUnaryExpression, TestToken::NOT, TestToken::EXPR),
         CCL_PARSING_RULE(TestToken::ROOT, constructRoot, TestToken::EXPR, TestToken::EOI)});


    auto tokenizer = LexicalAnalyzer.getTokenizer("!10");

    auto parser = parser::Parser(rules, tokenizer);
    parser.parse();
}
