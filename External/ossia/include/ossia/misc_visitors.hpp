#pragma once
#include <ossia/editor/curve/behavior.hpp>
#include <ossia/network/value/value.hpp>
namespace ossia
{
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0,
    const behavior_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case behavior_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case behavior_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("misc_visitors: bad type");
      }
    }
    default:
      throw std::runtime_error("misc_visitors: bad type");
  }
}
}
