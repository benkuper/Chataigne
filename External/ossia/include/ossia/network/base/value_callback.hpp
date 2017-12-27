#pragma once
#include <functional>
namespace ossia
{
class value;
using value_callback = std::function<void(const value&)>;
}
