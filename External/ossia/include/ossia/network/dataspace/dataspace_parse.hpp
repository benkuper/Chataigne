#pragma once
#include <ossia/network/dataspace/dataspace.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

#include <ossia/editor/state/message.hpp>

namespace ossia
{
using unit_parse_symbols_t
    = boost::spirit::qi::symbols<char, ossia::destination_qualifiers>;

OSSIA_EXPORT
const unit_parse_symbols_t& get_unit_parser();
}
