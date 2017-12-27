#pragma once
namespace ossia
{
namespace expressions
{

//! Represents a comparison between expressions
enum class comparator
{
  EQUAL,         //! ==
  DIFFERENT,     //! !=
  GREATER,       //! >
  LOWER,         //! <
  GREATER_EQUAL, //! >=
  LOWER_EQUAL    //! <=
};

//! Represents a binary operation between expressions
enum class binary_operator
{
  AND, //! &&
  OR,  //! ||
  XOR  //! ^
};
}
}
