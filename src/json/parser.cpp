/*
 *  @author     Stephan Bourgeois <stephanb2@hotmail.com>
 *  @version    0.1
 *  @since      2013-05-15
 *  @license    MIT License
 */

#include <iostream>
#include <protoc/json/parser.hpp>


namespace protoc
{
namespace json
{


// constructor
//
parser::parser(const char *begin, const char *end)
    : _decoder(begin, end) 
{
 
    if (_decoder.type() == json::token_eof) 
    {
        std::cout << "init eof" << std::endl;
    }
}


void parser::test_run() {

    while (_decoder.type() != json::token_eof) 
    {
        if (_decoder.type() == json::token_integer) 
        {
             std::cout << "int";
        } 
        else if (_decoder.type() == json::token_array_begin) 
        {
             std::cout << "[";
        }
        else if (_decoder.type() == json::token_array_end)
        {
             std::cout << "]";
        }

        std::cout << " ";

        _decoder.next();
    }

    std::cout << "eof" << std::endl;
}


//  BNF production:
//  Value = ( string | int64_t | double | Object | Array | true | false | null )
//  
//  will advance to the next token when it reads a ( string | number | bool | null )
//
dynamic::var parser::json_value() const
{
    using namespace dynamic;
    dynamic::var val; // val is initialised to null

    if (_decoder.type() == json::token_null)
    {
        //nothing to do, val is already initialised to null
        _decoder.next();
    }
    else if (_decoder.type() == json::token_integer)
    {
        //FIXME: dynamic-cpp needs to accept int64_t instead of int
        //this is using the overloaded assignment operator from dynamic-cpp
        val = (int) _decoder.get_integer();
        _decoder.next();
    }
    else if (_decoder.type() == json::token_string)
    {
        val = _decoder.get_string();
        _decoder.next();
    }
    else if (_decoder.type() == json::token_array_begin)
    {
        val = json_array();
    }
    
    return val;
}




//  BNF production:
//  Array = "[" ( | Value {"," Value}) "]"
//TODO: error recovery
//
dynamic::var parser::json_array() const
{
    using namespace dynamic;
    dynamic::var val = new_array(); // val is initialised to empty array
    bool empty = true;

     _decoder.next(); //skip opening bracket

    //we use negation of condition.
    //TODO: improve this for validation, this tolerates trailing or multiple commas
    /*
    if ((_decoder.type() == json::token_true) || (_decoder.type() == json::token_false) ||
        (_decoder.type() == json::token_null) || (_decoder.type() == json::token_integer) ||
        (_decoder.type() == json::token_float) || (_decoder.type() == json::token_string) ||
        (_decoder.type() == json::token_array_begin) || (_decoder.type() == json::token_object_begin))
    */
    if ((_decoder.type() != json::token_array_end) && (_decoder.type() != json::token_object_end) &&
        (_decoder.type() != json::token_comma) && (_decoder.type() != json::token_colon) &&
        (_decoder.type() != json::token_error) && (_decoder.type() != json::token_eof))
    {
        //read value and push it in the val array
        val(json_value());
        empty = false;
    }

    while (!empty && (_decoder.type() == json::token_comma))
    {
        //we have a comma, advance to the next token
        _decoder.next();
        
    	if ((_decoder.type() != json::token_array_end) && (_decoder.type() != json::token_object_end) &&
    	    (_decoder.type() != json::token_comma) && (_decoder.type() != json::token_colon) &&
    	    (_decoder.type() != json::token_error) && (_decoder.type() != json::token_eof))
        {
            val(json_value());
        }
        else
        {
            std::cout << "value missing in array" << std::endl;
        }
    }

    if (_decoder.type() == json::token_array_end)
    {
        _decoder.next();
    }
    else
    {
        //TODO: write an error message method
        std::cout << "end of array symbol expected" << std::endl;
    }

    return val;
}


} //namespace
}
