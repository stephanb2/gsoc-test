/*
 *  @author     Stephan Bourgeois <stephanb2@hotmail.com>
 *  @version    0.1
 *  @since      2013-05-15
 *  @license    MIT License
 */

#ifndef PROTOC_JSON_PARSER_HPP
#define PROTOC_JSON_PARSER_HPP

#include <protoc/json/decoder.hpp>
#include <dynamic-cpp/dynamic.h>


namespace protoc
{
namespace json
{


class parser
{

public:
    parser(const char *begin, const char *end);
    void test_run();
    dynamic::var json_value() const;

private:
    mutable protoc::json::decoder _decoder;
    dynamic::var json_array() const;

};



} //namespace
}

#endif /* PROTOC_JSON_PARSER_HPP */
