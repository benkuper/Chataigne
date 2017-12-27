#pragma once
#include <ossia/detail/any_map.hpp>
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/common/complex_type.hpp>
#include <ossia/network/common/extended_types.hpp>
#include <ossia/network/domain/domain.hpp>

namespace ossia
{
namespace net
{
/**
 * @brief The data that can be found inside a parameter
 *
 * Used to create a new address / node.
 * Unlike parameter_base and its child classes, can be copied, moved, etc.
 *
 * Name is the name of the node only.
 */
struct parameter_data
{
  parameter_data() = default;
  parameter_data(const parameter_data&) = default;
  parameter_data(parameter_data&&) = default;
  parameter_data& operator=(const parameter_data&) = default;
  parameter_data& operator=(parameter_data&&) = default;

  parameter_data(std::string n) : name{std::move(n)}
  {
  }

  std::string name;
  ossia::value value;
  ossia::complex_type type;
  ossia::optional<ossia::domain> domain;
  ossia::optional<ossia::access_mode> access;
  ossia::optional<ossia::bounding_mode> bounding;
  ossia::optional<ossia::repetition_filter> rep_filter;
  ossia::optional<bool> disabled{};
  ossia::optional<bool> muted{};
  ossia::optional<bool> critical{};
  unit_t unit;

  extended_attributes extended;

  operator const ossia::extended_attributes&() const
  {
    return extended;
  }
  operator ossia::extended_attributes&()
  {
    return extended;
  }
};

/**
 * @brief Full information about a parameter.
 *
 * address is an osc-like address, e.g.
 *
 * `/foo/bar/baz.3/fib`
 *
 * in parameter_data, name would be `fib`
 */
struct full_parameter_data
{
  public:
    full_parameter_data() = default;
    full_parameter_data(const full_parameter_data&) = default;
    full_parameter_data(full_parameter_data&&) = default;
    full_parameter_data& operator=(const full_parameter_data&) = default;
    full_parameter_data& operator=(full_parameter_data&&) = default;

    full_parameter_data(std::string n) : address{std::move(n)}
    {
    }

    std::string address;

  private:
    ossia::value val;
    ossia::value previous_val;

  public:
    ossia::val_type type{};
    ossia::domain domain;
    ossia::access_mode access{};
    ossia::bounding_mode bounding{};
    ossia::repetition_filter rep_filter{};
    ossia::unit_t unit;
    bool disabled{};
    bool muted{};
    bool critical{};

    extended_attributes extended;

    operator const ossia::extended_attributes&() const
    {
      return extended;
    }
    operator ossia::extended_attributes&()
    {
      return extended;
    }

    void set_value(const ossia::value& v) {
      previous_val = std::move(val);
      val = v;
    }
    void set_value(ossia::value&& v) {
      previous_val = std::move(val);
      val = std::move(v);
    }

    bool filter_value(const ossia::value& v) const
    {
      return disabled || muted ||
          (get_repetition_filter() == ossia::repetition_filter::ON && v == previous_val);
    }

    const ossia::value& value() const { return this->val; }
    ossia::val_type get_value_type() const { return type; }
    access_mode get_access() const { return access; }
    const ossia::domain& get_domain() const { return domain; }
    bounding_mode get_bounding() const { return bounding; }
    repetition_filter get_repetition_filter() const { return rep_filter; }
    const ossia::unit_t& get_unit() const { return unit; }
    bool get_disabled() const { return muted; }
    bool get_muted() const { return muted; }
    bool get_critical() const { return critical; }
};

OSSIA_EXPORT
inline const std::string& osc_parameter_string(const ossia::net::full_parameter_data& d)
{
  return d.address;
}
}
}
