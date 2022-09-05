#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <iostream>

namespace ccl::lex::parser
{
    auto CcllParser::parse() -> bool
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::IDENTIFIER:
                token_stack.push(std::move(token));

                if (not parseDeclaration()) {
                    recoverFromError();
                }

                break;

            case GenToken::CURLY_OPENING:
                if (not parseBlockDefinition()) {
                    recoverFromError();
                }

                break;

            case GenToken::NEW_LINE:
                break;

            case GenToken::EOI:
                return true;

            default:
                parsingError("identifier or block definition (`{`)", token_id);
                recoverFromError();
                break;
            }
        }
    }

    auto CcllParser::parseDeclaration() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::COLUMN:
            return parseRuleDeclaration();

        case GenToken::ASSIGN:
            return parseDirectiveDeclaration();

        default:
            parsingError("column (`:`) or assign (`=`)", token_id);
            return false;
        }
    }

    auto CcllParser::parseRuleDeclaration() -> bool
    {
        auto iterator_copy = tokenizer.getIterator();
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::RULE_DECLARATION) {
            iterator_copy.setEnd(token.getRepr().end());
            checkRule(iterator_copy);

            token_stack.push(std::move(token));
            completeRuleDeclaration();

            return true;
        }

        parsingError("regular expression", token_id);
        return false;
    }

    auto CcllParser::parseDirectiveDeclaration() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::IDENTIFIER || token_id == GenToken::STRING) {
            token_stack.push(std::move(token));
            completeDirectiveDeclaration();
            return true;
        }

        parsingError("identifier", token_id);
        return false;
    }

    auto CcllParser::completeRuleDeclaration() -> void
    {
        auto rule = token_stack.top();
        token_stack.pop();

        auto name = token_stack.top();
        token_stack.pop();

        rules.emplace_back(current_block, blocks[current_block], name.getValue(), rule.getValue());

        expectRuleEnd();
    }

    auto CcllParser::completeDirectiveDeclaration() -> void
    {
        auto directive_value = token_stack.top();
        token_stack.pop();

        auto directive = token_stack.top();
        token_stack.pop();

        directives[directive.getValue()] = directive_value.getValue();

        expectRuleEnd();
    }

    auto CcllParser::checkRule(text::TextIterator &rule) -> void
    {
        try {
            auto container = dot_item::Container{ std::move(rule), special_items, 2 };
        } catch (const UnrecoverableError & /* unused */) {}
    }

    auto CcllParser::parseBlockDefinition() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::IDENTIFIER) {
            token_stack.push(std::move(token));
            return parseBlockEnding();
        }

        parsingError("identifier", token_id);
        return false;
    }

    auto CcllParser::parseBlockEnding() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::CURLY_CLOSING) {
            completeBlock();
            return true;
        }

        parsingError("expected block end (`}`)", token_id);
        return false;
    }

    auto CcllParser::completeBlock() -> void
    {
        auto block_name = token_stack.top();
        token_stack.pop();

        current_block = block_name.getValue();

        if (not blocks.contains(current_block)) {
            blocks.insert({ current_block, { static_cast<u16>(last_block_id++), 0 } });
        }

        expectRuleEnd();
    }

    auto CcllParser::expectRuleEnd() -> void
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::EOI:
        case GenToken::NEW_LINE:
            break;
        default:
            parsingError("new line or end of input", token_id);
        }
    }

    auto CcllParser::recoverFromError() -> void
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::EOI:
            case GenToken::NEW_LINE:
                return;

            default:
                break;
            }
        }
    }

    auto CcllParser::parsingError(
        string_view expected_types, GenToken given_token, string_view suggestion) -> void
    {
        auto error_message =
            fmt::format("expected {}, got {}", expected_types, GenToken::toString(given_token));

        tokenizer.throwException(ExceptionCriticality::PANIC, error_message, suggestion);
    }
}// namespace ccl::lex::parser