#pragma once
#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/string_view.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/exceptions.hpp>
#include <exception>

namespace ossia
{
namespace minuit
{
enum class minuit_command : char
{
  Request = '?',
  Answer = ':',
  Error = '!'
};

enum class minuit_operation : char
{
  Listen = 'l',
  Namespace = 'n',
  Get = 'g'
};

enum class minuit_action : int
{
  NamespaceRequest,
  NamespaceReply,
  NamespaceError,
  GetRequest,
  GetReply,
  GetError,
  ListenRequest,
  ListenReply,
  ListenError
};

enum class minuit_type : char
{
  Application = 'A',
  Container = 'C',
  Data = 'D',
  None = 'n',
  ModelInfo = 'M',
  UiInfo = 'U',
  PresetManager = 'P'
};

enum class minuit_attribute
{
  Value,
  Type,
  Service,
  RangeBounds,
  RangeClipMode,
  Description,
  RepetitionFilter,
  Tags,
  Active,
  ValueDefault,
  Priority,
  Dataspace,
  DataspaceUnit,
  RampFunction,
  RampDrive,
  ValueStepSize,
  RampFunctionParameters
};

OSSIA_EXPORT
ossia::string_view to_minuit_attribute_text(minuit_attribute str);

OSSIA_EXPORT
minuit_attribute get_attribute(ossia::string_view str);

OSSIA_EXPORT
ossia::string_view to_minuit_type_text(const ossia::value& val);

inline ossia::string_view to_minuit_type_text(ossia::val_type val)
{
  // integer, decimal, string, generic, boolean, none, array.
  switch (val)
  {
    case val_type::IMPULSE:
      return make_string_view("none");
    case val_type::INT:
      return make_string_view("integer");
    case val_type::FLOAT:
      return make_string_view("decimal");
    case val_type::BOOL:
      return make_string_view("boolean");
    case val_type::CHAR:
    case val_type::STRING:
      return make_string_view("string");
    case val_type::VEC2F:
    case val_type::VEC3F:
    case val_type::VEC4F:
    case val_type::LIST:
      return make_string_view("array");
    default:
      throw invalid_value_type_error("to_minuit_type_text: Invalid type");
  }
  return {};
}

inline ossia::value value_from_minuit_type_text(ossia::string_view str)
{
  // integer, decimal, string, generic, boolean, none, array.
  // we can differentiate them by the first character

  switch (str[0])
  {
    case 'i': // integer
      return int32_t{};
    case 'd': // decimal
      return float{};
    case 's': // string
      return std::string{};
    case 'b': // boolean
      return bool{};
    case 'n': // none
      return ossia::impulse{};
    case 'a': // array
    case 'g': // generic
      return std::vector<ossia::value>{};
    default:
      return {};
  }
}

inline ossia::val_type type_from_minuit_type_text(ossia::string_view str)
{
  // integer, decimal, string, generic, boolean, none, array.
  // we can differentiate them by the first character

  switch (str[0])
  {
    case 'i': // integer
      return ossia::val_type::INT;
    case 'd': // decimal
      return ossia::val_type::FLOAT;
    case 's': // string
      return ossia::val_type::STRING;
    case 'b': // boolean
      return ossia::val_type::BOOL;
    case 'n': // none
      return ossia::val_type::IMPULSE;
    case 'a': // array
    case 'g': // generic
      return ossia::val_type::LIST;
    default:
      return {};
  }
}

inline ossia::string_view to_minuit_service_text(ossia::access_mode acc)
{
  switch (acc)
  {
    case ossia::access_mode::BI:
      return make_string_view("parameter");
    case ossia::access_mode::GET:
      return make_string_view("return");
    case ossia::access_mode::SET:
      return make_string_view("message");
    default:
      throw parse_error("to_minuit_service_text: Invalid access mode");
  }
  return {};
}

inline ossia::access_mode from_minuit_service_text(ossia::string_view str)
{
  switch (str[0])
  {
    case 'p':
      return ossia::access_mode::BI;
    case 'r':
      return ossia::access_mode::GET;
    case 'm':
      return ossia::access_mode::SET;
    default:
      throw parse_error("from_minuit_service_text: Invalid access mode");
  }
  return {};
}

inline ossia::string_view to_minuit_bounding_text(ossia::bounding_mode b)
{
  switch (b)
  {
    case ossia::bounding_mode::FREE:
      return make_string_view("none");
    case ossia::bounding_mode::CLIP:
      return make_string_view("both");
    case ossia::bounding_mode::WRAP:
      return make_string_view("wrap");
    case ossia::bounding_mode::FOLD:
      return make_string_view("fold");
    case ossia::bounding_mode::LOW:
      return make_string_view("low");
    case ossia::bounding_mode::HIGH:
      return make_string_view("high");
    default:
      throw parse_error("to_minuit_bounding_text: Invalid bounding mode");
  }
  return {};
}

inline ossia::bounding_mode from_minuit_bounding_text(ossia::string_view str)
{
  switch (str[0])
  {
    case 'n': // none
      return ossia::bounding_mode::FREE;
    case 'b': // both
      return ossia::bounding_mode::CLIP;
    case 'w': // wrap
      return ossia::bounding_mode::WRAP;
    case 'f': // fold
      return ossia::bounding_mode::FOLD;
    case 'l': // low
      return ossia::bounding_mode::LOW;
    case 'h': // high
      return ossia::bounding_mode::HIGH;
    default:
      throw parse_error("from_minuit_bounding_text: Invalid bounding mode");
  }
  return {};
}

inline minuit_command get_command(char str)
{
  switch (str)
  {
    case '?':
    case ':':
    case '!':
      return static_cast<minuit_command>(str);
    default:
      throw parse_error("get_command: unhandled command");
  }
  return {};
}

inline minuit_type get_type(char str)
{
  switch (str)
  {
    case 'A':
    case 'C':
    case 'D':
    case 'n':
    case 'M':
    case 'U':
    case 'P':
      return static_cast<minuit_type>(str);
    default:
      return minuit_type::None;
  }
  return minuit_type::None;
}

inline minuit_operation get_operation(char str)
{
  switch (str)
  {
    case 'l':
    case 'n':
    case 'g':
      return static_cast<minuit_operation>(str);
    default:
      throw parse_error("get_operation: unhandled operation");
  }
  return {};
}

inline minuit_operation get_operation(ossia::string_view str)
{
  return get_operation(str[0]);
}
}
}
