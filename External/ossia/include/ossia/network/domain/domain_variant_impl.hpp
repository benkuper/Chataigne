struct domain_base_variant
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::domain_base<ossia::impulse> m_value0;

    ossia::domain_base<bool> m_value1;

    ossia::domain_base<int> m_value2;

    ossia::domain_base<float> m_value3;

    ossia::domain_base<char> m_value4;

    ossia::domain_base<std::string> m_value5;

    ossia::vector_domain m_value6;

    ossia::vecf_domain<2> m_value7;

    ossia::vecf_domain<3> m_value8;

    ossia::vecf_domain<4> m_value9;

    ossia::domain_base<ossia::value> m_value10;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Type6,
    Type7,
    Type8,
    Type9,
    Type10,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      case Type::Type0:
        m_impl.m_value0.~domain_base();
        break;
      case Type::Type1:
        m_impl.m_value1.~domain_base();
        break;
      case Type::Type2:
        m_impl.m_value2.~domain_base();
        break;
      case Type::Type3:
        m_impl.m_value3.~domain_base();
        break;
      case Type::Type4:
        m_impl.m_value4.~domain_base();
        break;
      case Type::Type5:
        m_impl.m_value5.~domain_base();
        break;
      case Type::Type6:
        m_impl.m_value6.~vector_domain();
        break;
      case Type::Type7:
        m_impl.m_value7.~vecf_domain();
        break;
      case Type::Type8:
        m_impl.m_value8.~vecf_domain();
        break;
      case Type::Type9:
        m_impl.m_value9.~vecf_domain();
        break;
      case Type::Type10:
        m_impl.m_value10.~domain_base();
        break;
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  domain_base_variant() : m_type{Npos}
  {
  }
  ~domain_base_variant()
  {
    destruct_impl();
  }
  domain_base_variant(const ossia::domain_base<ossia::impulse>& v)
      : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::domain_base<ossia::impulse>{v};
  }
  domain_base_variant(ossia::domain_base<ossia::impulse>&& v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::domain_base<ossia::impulse>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<bool>& v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::domain_base<bool>{v};
  }
  domain_base_variant(ossia::domain_base<bool>&& v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::domain_base<bool>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<int>& v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::domain_base<int>{v};
  }
  domain_base_variant(ossia::domain_base<int>&& v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::domain_base<int>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<float>& v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::domain_base<float>{v};
  }
  domain_base_variant(ossia::domain_base<float>&& v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::domain_base<float>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<char>& v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::domain_base<char>{v};
  }
  domain_base_variant(ossia::domain_base<char>&& v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::domain_base<char>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<std::string>& v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::domain_base<std::string>{v};
  }
  domain_base_variant(ossia::domain_base<std::string>&& v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::domain_base<std::string>{std::move(v)};
  }
  domain_base_variant(const ossia::vector_domain& v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::vector_domain{v};
  }
  domain_base_variant(ossia::vector_domain&& v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::vector_domain{std::move(v)};
  }
  domain_base_variant(const ossia::vecf_domain<2>& v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::vecf_domain<2>{v};
  }
  domain_base_variant(ossia::vecf_domain<2>&& v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::vecf_domain<2>{std::move(v)};
  }
  domain_base_variant(const ossia::vecf_domain<3>& v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::vecf_domain<3>{v};
  }
  domain_base_variant(ossia::vecf_domain<3>&& v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::vecf_domain<3>{std::move(v)};
  }
  domain_base_variant(const ossia::vecf_domain<4>& v) : m_type{Type9}
  {
    new (&m_impl.m_value9) ossia::vecf_domain<4>{v};
  }
  domain_base_variant(ossia::vecf_domain<4>&& v) : m_type{Type9}
  {
    new (&m_impl.m_value9) ossia::vecf_domain<4>{std::move(v)};
  }
  domain_base_variant(const ossia::domain_base<ossia::value>& v)
      : m_type{Type10}
  {
    new (&m_impl.m_value10) ossia::domain_base<ossia::value>{v};
  }
  domain_base_variant(ossia::domain_base<ossia::value>&& v) : m_type{Type10}
  {
    new (&m_impl.m_value10) ossia::domain_base<ossia::value>{std::move(v)};
  }
  domain_base_variant(const domain_base_variant& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::domain_base<ossia::impulse>{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::domain_base<bool>{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::domain_base<int>{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::domain_base<float>{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::domain_base<char>{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::domain_base<std::string>{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::vector_domain{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::vecf_domain<2>{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::vecf_domain<3>{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::vecf_domain<4>{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10)
            ossia::domain_base<ossia::value>{other.m_impl.m_value10};
        break;
      default:
        break;
    }
  }
  domain_base_variant(domain_base_variant&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::domain_base<ossia::impulse>{
            std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::domain_base<bool>{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::domain_base<int>{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::domain_base<float>{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::domain_base<char>{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::domain_base<std::string>{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::vector_domain{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::vecf_domain<2>{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::vecf_domain<3>{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9)
            ossia::vecf_domain<4>{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::domain_base<ossia::value>{
            std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
  }
  domain_base_variant& operator=(const domain_base_variant& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::domain_base<ossia::impulse>{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::domain_base<bool>{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::domain_base<int>{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::domain_base<float>{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::domain_base<char>{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::domain_base<std::string>{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::vector_domain{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::vecf_domain<2>{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::vecf_domain<3>{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::vecf_domain<4>{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10)
            ossia::domain_base<ossia::value>{other.m_impl.m_value10};
        break;
      default:
        break;
    }
    return *this;
  }
  domain_base_variant& operator=(domain_base_variant&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::domain_base<ossia::impulse>{
            std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::domain_base<bool>{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::domain_base<int>{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::domain_base<float>{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::domain_base<char>{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::domain_base<std::string>{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::vector_domain{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::vecf_domain<2>{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::vecf_domain<3>{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9)
            ossia::vecf_domain<4>{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::domain_base<ossia::value>{
            std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::domain_base<ossia::impulse>*
domain_base_variant::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::domain_base<bool>* domain_base_variant::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::domain_base<int>* domain_base_variant::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::domain_base<float>* domain_base_variant::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::domain_base<char>* domain_base_variant::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::domain_base<std::string>*
domain_base_variant::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::vector_domain* domain_base_variant::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::vecf_domain<2>* domain_base_variant::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::vecf_domain<3>* domain_base_variant::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::vecf_domain<4>* domain_base_variant::target() const
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline const ossia::domain_base<ossia::value>*
domain_base_variant::target() const
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline ossia::domain_base<ossia::impulse>* domain_base_variant::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::domain_base<bool>* domain_base_variant::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::domain_base<int>* domain_base_variant::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::domain_base<float>* domain_base_variant::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::domain_base<char>* domain_base_variant::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::domain_base<std::string>* domain_base_variant::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::vector_domain* domain_base_variant::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::vecf_domain<2>* domain_base_variant::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::vecf_domain<3>* domain_base_variant::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::vecf_domain<4>* domain_base_variant::target()
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline ossia::domain_base<ossia::value>* domain_base_variant::target()
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline const ossia::domain_base<ossia::impulse>&
domain_base_variant::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<bool>& domain_base_variant::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<int>& domain_base_variant::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<float>& domain_base_variant::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<char>& domain_base_variant::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<std::string>& domain_base_variant::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::vector_domain& domain_base_variant::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::vecf_domain<2>& domain_base_variant::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::vecf_domain<3>& domain_base_variant::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::vecf_domain<4>& domain_base_variant::get() const
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline const ossia::domain_base<ossia::value>& domain_base_variant::get() const
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<ossia::impulse>& domain_base_variant::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<bool>& domain_base_variant::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<int>& domain_base_variant::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<float>& domain_base_variant::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<char>& domain_base_variant::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<std::string>& domain_base_variant::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::vector_domain& domain_base_variant::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::vecf_domain<2>& domain_base_variant::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::vecf_domain<3>& domain_base_variant::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::vecf_domain<4>& domain_base_variant::get()
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <>
inline ossia::domain_base<ossia::value>& domain_base_variant::get()
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("domain_variant_impl: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const domain_base_variant& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case domain_base_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case domain_base_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case domain_base_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case domain_base_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case domain_base_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case domain_base_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case domain_base_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case domain_base_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    case domain_base_variant::Type::Type9:
      return functor(var.m_impl.m_value9);
    case domain_base_variant::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("domain_variant_impl: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, domain_base_variant& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case domain_base_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case domain_base_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case domain_base_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case domain_base_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case domain_base_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case domain_base_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case domain_base_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case domain_base_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    case domain_base_variant::Type::Type9:
      return functor(var.m_impl.m_value9);
    case domain_base_variant::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("domain_variant_impl: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, domain_base_variant&& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case domain_base_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case domain_base_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case domain_base_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case domain_base_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case domain_base_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case domain_base_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case domain_base_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case domain_base_variant::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case domain_base_variant::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case domain_base_variant::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      throw std::runtime_error("domain_variant_impl: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const domain_base_variant& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case domain_base_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case domain_base_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case domain_base_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case domain_base_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case domain_base_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case domain_base_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case domain_base_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case domain_base_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    case domain_base_variant::Type::Type9:
      return functor(var.m_impl.m_value9);
    case domain_base_variant::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, domain_base_variant& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case domain_base_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case domain_base_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case domain_base_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case domain_base_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case domain_base_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case domain_base_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case domain_base_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case domain_base_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    case domain_base_variant::Type::Type9:
      return functor(var.m_impl.m_value9);
    case domain_base_variant::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, domain_base_variant&& var)
{
  switch (var.m_type)
  {
    case domain_base_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case domain_base_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case domain_base_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case domain_base_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case domain_base_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case domain_base_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case domain_base_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case domain_base_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case domain_base_variant::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case domain_base_variant::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case domain_base_variant::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      return functor();
  }
}
inline bool
operator==(const domain_base_variant& lhs, const domain_base_variant& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case domain_base_variant::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case domain_base_variant::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case domain_base_variant::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case domain_base_variant::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case domain_base_variant::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case domain_base_variant::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case domain_base_variant::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case domain_base_variant::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      case domain_base_variant::Type::Type8:
        return lhs.m_impl.m_value8 == rhs.m_impl.m_value8;
      case domain_base_variant::Type::Type9:
        return lhs.m_impl.m_value9 == rhs.m_impl.m_value9;
      case domain_base_variant::Type::Type10:
        return lhs.m_impl.m_value10 == rhs.m_impl.m_value10;
      default:
        return true;
    }
  }
  return false;
}
inline bool
operator!=(const domain_base_variant& lhs, const domain_base_variant& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case domain_base_variant::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case domain_base_variant::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case domain_base_variant::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case domain_base_variant::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case domain_base_variant::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case domain_base_variant::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case domain_base_variant::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case domain_base_variant::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    case domain_base_variant::Type::Type8:
      return lhs.m_impl.m_value8 != rhs.m_impl.m_value8;
    case domain_base_variant::Type::Type9:
      return lhs.m_impl.m_value9 != rhs.m_impl.m_value9;
    case domain_base_variant::Type::Type10:
      return lhs.m_impl.m_value10 != rhs.m_impl.m_value10;
    default:
      return false;
  }
  return true;
}
inline bool operator==(
    const domain_base_variant& lhs,
    const ossia::domain_base<ossia::impulse>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type0)
         && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(
    const ossia::domain_base<ossia::impulse>& lhs,
    const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type0)
         && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(
    const domain_base_variant& lhs,
    const ossia::domain_base<ossia::impulse>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type0)
         || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(
    const ossia::domain_base<ossia::impulse>& lhs,
    const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type0)
         || (rhs.m_impl.m_value0 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::domain_base<bool>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type1)
         && (lhs.m_impl.m_value1 == rhs);
}
inline bool
operator==(const ossia::domain_base<bool>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type1)
         && (rhs.m_impl.m_value1 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::domain_base<bool>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type1)
         || (lhs.m_impl.m_value1 != rhs);
}
inline bool
operator!=(const ossia::domain_base<bool>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type1)
         || (rhs.m_impl.m_value1 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::domain_base<int>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type2)
         && (lhs.m_impl.m_value2 == rhs);
}
inline bool
operator==(const ossia::domain_base<int>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type2)
         && (rhs.m_impl.m_value2 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::domain_base<int>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type2)
         || (lhs.m_impl.m_value2 != rhs);
}
inline bool
operator!=(const ossia::domain_base<int>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type2)
         || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(
    const domain_base_variant& lhs, const ossia::domain_base<float>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type3)
         && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(
    const ossia::domain_base<float>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type3)
         && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(
    const domain_base_variant& lhs, const ossia::domain_base<float>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type3)
         || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(
    const ossia::domain_base<float>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type3)
         || (rhs.m_impl.m_value3 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::domain_base<char>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type4)
         && (lhs.m_impl.m_value4 == rhs);
}
inline bool
operator==(const ossia::domain_base<char>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type4)
         && (rhs.m_impl.m_value4 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::domain_base<char>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type4)
         || (lhs.m_impl.m_value4 != rhs);
}
inline bool
operator!=(const ossia::domain_base<char>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type4)
         || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(
    const domain_base_variant& lhs, const ossia::domain_base<std::string>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type5)
         && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(
    const ossia::domain_base<std::string>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type5)
         && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(
    const domain_base_variant& lhs, const ossia::domain_base<std::string>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type5)
         || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(
    const ossia::domain_base<std::string>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type5)
         || (rhs.m_impl.m_value5 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::vector_domain& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type6)
         && (lhs.m_impl.m_value6 == rhs);
}
inline bool
operator==(const ossia::vector_domain& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type6)
         && (rhs.m_impl.m_value6 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::vector_domain& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type6)
         || (lhs.m_impl.m_value6 != rhs);
}
inline bool
operator!=(const ossia::vector_domain& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type6)
         || (rhs.m_impl.m_value6 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::vecf_domain<2>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type7)
         && (lhs.m_impl.m_value7 == rhs);
}
inline bool
operator==(const ossia::vecf_domain<2>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type7)
         && (rhs.m_impl.m_value7 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::vecf_domain<2>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type7)
         || (lhs.m_impl.m_value7 != rhs);
}
inline bool
operator!=(const ossia::vecf_domain<2>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type7)
         || (rhs.m_impl.m_value7 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::vecf_domain<3>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type8)
         && (lhs.m_impl.m_value8 == rhs);
}
inline bool
operator==(const ossia::vecf_domain<3>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type8)
         && (rhs.m_impl.m_value8 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::vecf_domain<3>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type8)
         || (lhs.m_impl.m_value8 != rhs);
}
inline bool
operator!=(const ossia::vecf_domain<3>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type8)
         || (rhs.m_impl.m_value8 != lhs);
}
inline bool
operator==(const domain_base_variant& lhs, const ossia::vecf_domain<4>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type9)
         && (lhs.m_impl.m_value9 == rhs);
}
inline bool
operator==(const ossia::vecf_domain<4>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type9)
         && (rhs.m_impl.m_value9 == lhs);
}
inline bool
operator!=(const domain_base_variant& lhs, const ossia::vecf_domain<4>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type9)
         || (lhs.m_impl.m_value9 != rhs);
}
inline bool
operator!=(const ossia::vecf_domain<4>& lhs, const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type9)
         || (rhs.m_impl.m_value9 != lhs);
}
inline bool operator==(
    const domain_base_variant& lhs,
    const ossia::domain_base<ossia::value>& rhs)
{
  return (lhs.m_type == domain_base_variant::Type::Type10)
         && (lhs.m_impl.m_value10 == rhs);
}
inline bool operator==(
    const ossia::domain_base<ossia::value>& lhs,
    const domain_base_variant& rhs)
{
  return (rhs.m_type == domain_base_variant::Type::Type10)
         && (rhs.m_impl.m_value10 == lhs);
}
inline bool operator!=(
    const domain_base_variant& lhs,
    const ossia::domain_base<ossia::value>& rhs)
{
  return (lhs.m_type != domain_base_variant::Type::Type10)
         || (lhs.m_impl.m_value10 != rhs);
}
inline bool operator!=(
    const ossia::domain_base<ossia::value>& lhs,
    const domain_base_variant& rhs)
{
  return (rhs.m_type != domain_base_variant::Type::Type10)
         || (rhs.m_impl.m_value10 != lhs);
}
template <typename Functor>
auto apply(
    Functor&& functor, domain_base_variant& arg0,
    const value_variant_type& arg1)
{
  switch (arg0.m_type)
  {
    case domain_base_variant::Type::Type0:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type1:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type2:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type3:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type4:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type5:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type6:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type7:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type8:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type9:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case domain_base_variant::Type::Type10:
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
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    default:
      throw std::runtime_error("domain_variant_impl: bad type");
  }
}
template <typename Functor>
auto apply(
    Functor&& functor, const value_variant_type& arg0,
    const domain_base_variant& arg1)
{
  switch (arg0.m_type)
  {
    case value_variant_type::Type::Type0:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value0, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type1:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value1, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type2:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value2, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type3:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value3, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type4:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value4, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type5:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value5, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type6:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value6, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type7:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value7, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type8:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value8, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    case value_variant_type::Type::Type9:
    {
      switch (arg1.m_type)
      {
        case domain_base_variant::Type::Type0:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value0);
        }
        case domain_base_variant::Type::Type1:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value1);
        }
        case domain_base_variant::Type::Type2:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value2);
        }
        case domain_base_variant::Type::Type3:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value3);
        }
        case domain_base_variant::Type::Type4:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value4);
        }
        case domain_base_variant::Type::Type5:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value5);
        }
        case domain_base_variant::Type::Type6:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value6);
        }
        case domain_base_variant::Type::Type7:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value7);
        }
        case domain_base_variant::Type::Type8:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value8);
        }
        case domain_base_variant::Type::Type9:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value9);
        }
        case domain_base_variant::Type::Type10:
        {
          return functor(arg0.m_impl.m_value9, arg1.m_impl.m_value10);
        }
        default:
          throw std::runtime_error("domain_variant_impl: bad type");
      }
    }
    default:
      throw std::runtime_error("domain_variant_impl: bad type");
  }
}
