#pragma once

namespace ossia
{
struct state_execution_visitor
{
  template <typename T>
  void operator()(const T& m)
  {
    m.launch();
  }

  void operator()()
  {
  }
};
}
