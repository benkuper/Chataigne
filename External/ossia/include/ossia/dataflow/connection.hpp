#pragma once
#include <ossia/dataflow/data.hpp>

namespace ossia
{

struct immediate_glutton_connection
{
};
struct immediate_strict_connection
{
  enum required_sides_t
  {
    inbound = 1 << 0,
    outbound = 1 << 1,
    both = inbound | outbound
  };

  required_sides_t required_sides{both};
};
struct temporal_glutton_connection
{
};
struct delayed_glutton_connection
{
  // delayed at the source or at the target
  delay_line_type buffer;
  std::size_t pos{};
};
struct delayed_strict_connection
{
  // same
  delay_line_type buffer;
  std::size_t pos{};
};
struct reduction_connection
{
};
struct replacing_connection
{
};

// An explicit dependency required by the composer.
struct dependency_connection
{
};

using connection = eggs::variant<
immediate_glutton_connection,
immediate_strict_connection,
delayed_glutton_connection,
delayed_strict_connection,/*
reduction_connection,
replacing_connection,*/
dependency_connection>;
}
