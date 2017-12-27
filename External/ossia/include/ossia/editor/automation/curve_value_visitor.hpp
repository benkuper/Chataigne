#pragma once
#include <ossia/editor/curve/behavior.hpp>
#include <ossia/editor/curve/curve.hpp>
#include <ossia/network/value/value.hpp>
#include <chobo/static_vector.hpp>
namespace ossia
{
namespace detail
{

using vec_behavior = chobo::static_vector<curve<double, float>*, 4>;
static vec_behavior
list_convertible_to_vec(const std::vector<ossia::behavior>& t)
{
  const auto n = t.size();

  chobo::static_vector<curve<double, float>*, 4> arr;
  bool ok = false;
  if (n >= 2 && n <= 4)
  {
    for (const ossia::behavior& v : t)
    {
      auto target = v.target<std::shared_ptr<ossia::curve_abstract>>();
      if (!target)
        return {};

      ossia::curve_abstract* c = target->get();
      if (!c)
        return {};

      auto t = c->get_type();
      if (t.first == ossia::curve_segment_type::DOUBLE
          && t.second == ossia::curve_segment_type::FLOAT)
      {
        arr.push_back(static_cast<ossia::curve<double, float>*>(c));
        ok = true;
        continue;
      }
      else
      {
        ok = false;
        break;
      }
    }

    if (ok)
      return arr;
    else
      return {};
  }
  return {};
}

template <int N>
ossia::value make_filled_vec(
    ossia::curve_abstract* base_curve, ossia::curve_segment_type t,
    double position)
{
  switch (t)
  {
    case ossia::curve_segment_type::FLOAT:
    {
      auto c = static_cast<curve<double, float>*>(base_curve);
      return fill_vec<N>(float(c->value_at(position)));
    }
    case ossia::curve_segment_type::INT:
    {
      auto c = static_cast<curve<double, int>*>(base_curve);
      return fill_vec<N>(int(c->value_at(position)));
    }
    case ossia::curve_segment_type::BOOL:
    {
      auto c = static_cast<curve<double, bool>*>(base_curve);
      return fill_vec<N>(bool(c->value_at(position)));
    }
    case ossia::curve_segment_type::DOUBLE:
    case ossia::curve_segment_type::ANY:
      break;
  }

  return {};
}

struct compute_value_uninformed_visitor
{
  double position;

  ossia::value error() const
  {
    throw invalid_value_type_error(
        "computeValue_visitor: "
        "Unhandled drive value type.");
    return {};
  }

  template <typename T>
  ossia::value operator()(const T&) const
  {
    return error();
  }

  ossia::value operator()() const
  {
    return error();
  }

  ossia::value operator()(const curve_ptr& c) const
  {
    auto base_curve = c.get();
    if (!base_curve)
    {
      throw invalid_value_type_error(
          "computeValue_visitor: "
          "invalid Behavior");
      return {};
    }

    auto t = base_curve->get_type();
    switch (t.second)
    {
      case ossia::curve_segment_type::FLOAT:
        return float{static_cast<curve<double, float>*>(base_curve)
                         ->value_at(position)};
      case ossia::curve_segment_type::INT:
        return int32_t{
            static_cast<curve<double, int>*>(base_curve)->value_at(position)};
      case ossia::curve_segment_type::BOOL:
        return bool{
            static_cast<curve<double, bool>*>(base_curve)->value_at(position)};
      case ossia::curve_segment_type::DOUBLE:
        break;
      case ossia::curve_segment_type::ANY:
        // TODO we need a specific handling for destination.
        return static_cast<constant_curve*>(base_curve)->value();
    }

    throw invalid_value_type_error(
        "computeValue_visitor: "
        "drive curve type is not DOUBLE");
    return {};
  }

  ossia::value operator()(const std::vector<ossia::behavior>& b)
  {
    vec_behavior arr = list_convertible_to_vec(b);

    // VecNf case.
    switch (arr.size())
    {
      case 2:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position));
      case 3:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position),
            arr[2]->value_at(position));
      case 4:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position),
            arr[2]->value_at(position), arr[3]->value_at(position));
      default:
        break;
    }

    // General list case
    std::vector<ossia::value> t;
    t.reserve(b.size());

    for (const auto& e : b)
    {
      if (e)
        t.push_back(ossia::apply_nonnull(*this, e));
      else
        return {};
    }

    return std::move(t);
  }
};

struct compute_value_visitor
{
  double position;
  ossia::val_type driven_type;

  ossia::value error() const
  {
    throw invalid_value_type_error(
        "computeValue_visitor: "
        "Unhandled drive value type.");
    return {};
  }

  template <typename T>
  ossia::value operator()(const T&) const
  {
    return error();
  }

  ossia::value operator()() const
  {
    return error();
  }

  ossia::value operator()(const curve_ptr& c) const
  {
    auto base_curve = c.get();
    if (!base_curve)
    {
      throw invalid_value_type_error(
          "computeValue_visitor: "
          "invalid Behavior");
      return {};
    }

    auto t = base_curve->get_type();
    if (t.first == ossia::curve_segment_type::DOUBLE)
    {
      switch (driven_type)
      {
        case ossia::val_type::FLOAT:
        case ossia::val_type::INT:
        case ossia::val_type::BOOL:
        {
          switch (t.second)
          {
            case ossia::curve_segment_type::FLOAT:
              return float{static_cast<curve<double, float>*>(base_curve)
                               ->value_at(position)};
            case ossia::curve_segment_type::INT:
              return int32_t{static_cast<curve<double, int>*>(base_curve)
                                 ->value_at(position)};
            case ossia::curve_segment_type::BOOL:
              return bool{static_cast<curve<double, bool>*>(base_curve)
                              ->value_at(position)};
            case ossia::curve_segment_type::DOUBLE:
              break;
            case ossia::curve_segment_type::ANY:
              // TODO we need a specific handling for destination.
              return static_cast<constant_curve*>(base_curve)->value();
          }
          break;
        }
        case ossia::val_type::VEC2F:
          return make_filled_vec<2>(base_curve, t.second, position);
        case ossia::val_type::VEC3F:
          return make_filled_vec<3>(base_curve, t.second, position);
        case ossia::val_type::VEC4F:
          return make_filled_vec<4>(base_curve, t.second, position);
        case ossia::val_type::LIST:
        {
          return {}; // TODO
        }

        default:
          break;
      }
    }

    throw invalid_value_type_error(
        "computeValue_visitor: "
        "drive curve type is not DOUBLE");
    return {};
  }

  ossia::value operator()(const std::vector<ossia::behavior>& b)
  {
    vec_behavior arr = list_convertible_to_vec(b);

    // VecNf case.
    switch (arr.size())
    {
      case 2:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position));
      case 3:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position),
            arr[2]->value_at(position));
      case 4:
        return ossia::make_vec(
            arr[0]->value_at(position), arr[1]->value_at(position),
            arr[2]->value_at(position), arr[3]->value_at(position));
      default:
        break;
    }

    // General list case
    std::vector<ossia::value> t;
    t.reserve(b.size());

    for (const auto& e : b)
    {
      if (e)
        t.push_back(ossia::apply_nonnull(
            compute_value_uninformed_visitor{position}, e));
      else
        return {};
    }

    return std::move(t);
  }
};
}
}
