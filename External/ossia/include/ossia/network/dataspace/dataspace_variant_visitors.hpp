#pragma once
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/dataspace/value_with_unit.hpp>
namespace ossia
{
template <typename Functor>
auto apply(
    Functor&& functor, const strong_value_variant& arg0,
    const unit_variant& arg1)
{
  switch (arg0.m_type)
  {
    case strong_value_variant::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case strong_value_variant::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case unit_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case unit_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case unit_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case unit_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case unit_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case unit_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case unit_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case unit_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const angle_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const color_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const distance_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const gain_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0,
    const orientation_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const position_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const speed_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0, const timing_u& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const angle& arg0, const angle_u& arg1)
{
  switch (arg0.m_type)
  {
    case angle::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case angle::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case angle_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case angle_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const color& arg0, const color_u& arg1)
{
  switch (arg0.m_type)
  {
    case color::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case color_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case color_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case color_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case color_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case color_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case color_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case color_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case color_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const distance& arg0, const distance_u& arg1)
{
  switch (arg0.m_type)
  {
    case distance::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type10:
    {
      switch (arg1.m_type)
      {
        case distance_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value0);
        }
        case distance_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value1);
        }
        case distance_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value2);
        }
        case distance_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value3);
        }
        case distance_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value4);
        }
        case distance_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value5);
        }
        case distance_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value6);
        }
        case distance_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value7);
        }
        case distance_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value8);
        }
        case distance_u::Type::Type9:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value9);
        }
        case distance_u::Type::Type10:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const gain& arg0, const gain_u& arg1)
{
  switch (arg0.m_type)
  {
    case gain::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case gain_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case gain_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case gain_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case gain_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const orientation& arg0, const orientation_u& arg1)
{
  switch (arg0.m_type)
  {
    case orientation::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case orientation::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case orientation::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case orientation_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case orientation_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case orientation_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const position& arg0, const position_u& arg1)
{
  switch (arg0.m_type)
  {
    case position::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case position_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case position_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case position_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case position_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case position_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case position_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const speed& arg0, const speed_u& arg1)
{
  switch (arg0.m_type)
  {
    case speed::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case speed_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case speed_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case speed_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case speed_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case speed_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case speed_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const timing& arg0, const timing_u& arg1)
{
  switch (arg0.m_type)
  {
    case timing::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case timing_u::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case timing_u::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case timing_u::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case timing_u::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case timing_u::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case timing_u::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case timing_u::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case timing_u::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case timing_u::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const angle& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case angle::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case angle::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const color& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case color::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case color::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const distance& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case distance::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case distance::Type::Type10:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value10, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(Functor&& functor, const gain& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case gain::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case gain::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const orientation& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case orientation::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case orientation::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case orientation::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const position& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case position::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case position::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const speed& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case speed::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case speed::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const timing& arg0, const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case timing::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    case timing::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case value_variant_type::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case value_variant_type::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case value_variant_type::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case value_variant_type::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case value_variant_type::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case value_variant_type::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case value_variant_type::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case value_variant_type::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case value_variant_type::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        case value_variant_type::Type::Type9:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value9);
        }
        default:
          throw std::runtime_error("dataspace_variant: bad type");
      }
    }
    default:
      throw std::runtime_error("dataspace_variant: bad type");
  }
}
}
