struct angle
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::degree m_value0;

    ossia::radian m_value1;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  angle() : m_type{Npos}
  {
  }
  ~angle()
  {
    destruct_impl();
  }
  angle(ossia::degree v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::degree{v};
  }
  angle(ossia::radian v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::radian{v};
  }
  angle(const angle& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian{other.m_impl.m_value1};
        break;
      default:
        break;
    }
  }
  angle(angle&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian{std::move(other.m_impl.m_value1)};
        break;
      default:
        break;
    }
  }
  angle& operator=(const angle& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian{other.m_impl.m_value1};
        break;
      default:
        break;
    }
    return *this;
  }
  angle& operator=(angle&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian{std::move(other.m_impl.m_value1)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::degree* angle::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::radian* angle::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::degree* angle::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::radian* angle::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::degree& angle::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::radian& angle::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::degree& angle::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::radian& angle::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const angle& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, angle& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, angle&& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case angle::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const angle& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, angle& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, angle&& var)
{
  switch (var.m_type)
  {
    case angle::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case angle::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    default:
      return functor();
  }
}
inline bool operator==(const angle& lhs, const angle& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case angle::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case angle::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const angle& lhs, const angle& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case angle::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case angle::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const angle& lhs, const ossia::degree& rhs)
{
  return (lhs.m_type == angle::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::degree& lhs, const angle& rhs)
{
  return (rhs.m_type == angle::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const angle& lhs, const ossia::degree& rhs)
{
  return (lhs.m_type != angle::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::degree& lhs, const angle& rhs)
{
  return (rhs.m_type != angle::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const angle& lhs, const ossia::radian& rhs)
{
  return (lhs.m_type == angle::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::radian& lhs, const angle& rhs)
{
  return (rhs.m_type == angle::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const angle& lhs, const ossia::radian& rhs)
{
  return (lhs.m_type != angle::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::radian& lhs, const angle& rhs)
{
  return (rhs.m_type != angle::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
struct color
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::argb m_value0;

    ossia::rgba m_value1;

    ossia::rgb m_value2;

    ossia::bgr m_value3;

    ossia::argb8 m_value4;

    ossia::hsv m_value5;

    ossia::cmy8 m_value6;

    ossia::xyz m_value7;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  color() : m_type{Npos}
  {
  }
  ~color()
  {
    destruct_impl();
  }
  color(ossia::argb v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::argb{v};
  }
  color(ossia::rgba v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::rgba{v};
  }
  color(ossia::rgb v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::rgb{v};
  }
  color(ossia::bgr v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::bgr{v};
  }
  color(ossia::argb8 v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::argb8{v};
  }
  color(ossia::hsv v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::hsv{v};
  }
  color(ossia::cmy8 v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::cmy8{v};
  }
  color(ossia::xyz v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::xyz{v};
  }
  color(const color& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz{other.m_impl.m_value7};
        break;
      default:
        break;
    }
  }
  color(color&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
  }
  color& operator=(const color& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz{other.m_impl.m_value7};
        break;
      default:
        break;
    }
    return *this;
  }
  color& operator=(color&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::argb* color::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::rgba* color::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::rgb* color::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::bgr* color::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::argb8* color::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::hsv* color::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::cmy8* color::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::xyz* color::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::argb* color::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::rgba* color::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::rgb* color::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::bgr* color::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::argb8* color::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::hsv* color::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::cmy8* color::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::xyz* color::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::argb& color::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::rgba& color::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::rgb& color::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::bgr& color::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::argb8& color::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::hsv& color::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::cmy8& color::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::xyz& color::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::argb& color::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::rgba& color::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::rgb& color::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::bgr& color::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::argb8& color::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::hsv& color::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::cmy8& color::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::xyz& color::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const color& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, color& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, color&& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case color::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case color::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case color::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case color::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case color::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case color::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case color::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const color& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, color& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, color&& var)
{
  switch (var.m_type)
  {
    case color::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case color::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case color::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case color::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case color::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case color::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case color::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case color::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      return functor();
  }
}
inline bool operator==(const color& lhs, const color& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case color::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case color::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case color::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case color::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case color::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case color::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case color::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case color::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const color& lhs, const color& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case color::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case color::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case color::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case color::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case color::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case color::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case color::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case color::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const color& lhs, const ossia::argb& rhs)
{
  return (lhs.m_type == color::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::argb& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::argb& rhs)
{
  return (lhs.m_type != color::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::argb& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const color& lhs, const ossia::rgba& rhs)
{
  return (lhs.m_type == color::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::rgba& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::rgba& rhs)
{
  return (lhs.m_type != color::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::rgba& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const color& lhs, const ossia::rgb& rhs)
{
  return (lhs.m_type == color::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::rgb& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::rgb& rhs)
{
  return (lhs.m_type != color::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::rgb& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const color& lhs, const ossia::bgr& rhs)
{
  return (lhs.m_type == color::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::bgr& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::bgr& rhs)
{
  return (lhs.m_type != color::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::bgr& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const color& lhs, const ossia::argb8& rhs)
{
  return (lhs.m_type == color::Type::Type4) && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::argb8& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type4) && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::argb8& rhs)
{
  return (lhs.m_type != color::Type::Type4) || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::argb8& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type4) || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const color& lhs, const ossia::hsv& rhs)
{
  return (lhs.m_type == color::Type::Type5) && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::hsv& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type5) && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::hsv& rhs)
{
  return (lhs.m_type != color::Type::Type5) || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::hsv& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type5) || (rhs.m_impl.m_value5 != lhs);
}
inline bool operator==(const color& lhs, const ossia::cmy8& rhs)
{
  return (lhs.m_type == color::Type::Type6) && (lhs.m_impl.m_value6 == rhs);
}
inline bool operator==(const ossia::cmy8& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type6) && (rhs.m_impl.m_value6 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::cmy8& rhs)
{
  return (lhs.m_type != color::Type::Type6) || (lhs.m_impl.m_value6 != rhs);
}
inline bool operator!=(const ossia::cmy8& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type6) || (rhs.m_impl.m_value6 != lhs);
}
inline bool operator==(const color& lhs, const ossia::xyz& rhs)
{
  return (lhs.m_type == color::Type::Type7) && (lhs.m_impl.m_value7 == rhs);
}
inline bool operator==(const ossia::xyz& lhs, const color& rhs)
{
  return (rhs.m_type == color::Type::Type7) && (rhs.m_impl.m_value7 == lhs);
}
inline bool operator!=(const color& lhs, const ossia::xyz& rhs)
{
  return (lhs.m_type != color::Type::Type7) || (lhs.m_impl.m_value7 != rhs);
}
inline bool operator!=(const ossia::xyz& lhs, const color& rhs)
{
  return (rhs.m_type != color::Type::Type7) || (rhs.m_impl.m_value7 != lhs);
}
struct distance
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::meter m_value0;

    ossia::kilometer m_value1;

    ossia::decimeter m_value2;

    ossia::centimeter m_value3;

    ossia::millimeter m_value4;

    ossia::micrometer m_value5;

    ossia::nanometer m_value6;

    ossia::picometer m_value7;

    ossia::inch m_value8;

    ossia::foot m_value9;

    ossia::mile m_value10;

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
  distance() : m_type{Npos}
  {
  }
  ~distance()
  {
    destruct_impl();
  }
  distance(ossia::meter v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::meter{v};
  }
  distance(ossia::kilometer v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::kilometer{v};
  }
  distance(ossia::decimeter v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::decimeter{v};
  }
  distance(ossia::centimeter v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::centimeter{v};
  }
  distance(ossia::millimeter v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::millimeter{v};
  }
  distance(ossia::micrometer v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::micrometer{v};
  }
  distance(ossia::nanometer v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::nanometer{v};
  }
  distance(ossia::picometer v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::picometer{v};
  }
  distance(ossia::inch v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::inch{v};
  }
  distance(ossia::foot v) : m_type{Type9}
  {
    new (&m_impl.m_value9) ossia::foot{v};
  }
  distance(ossia::mile v) : m_type{Type10}
  {
    new (&m_impl.m_value10) ossia::mile{v};
  }
  distance(const distance& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::kilometer{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decimeter{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::centimeter{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::millimeter{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::micrometer{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::nanometer{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::picometer{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile{other.m_impl.m_value10};
        break;
      default:
        break;
    }
  }
  distance(distance&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::kilometer{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decimeter{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::centimeter{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::millimeter{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::micrometer{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::nanometer{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::picometer{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile{std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
  }
  distance& operator=(const distance& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::kilometer{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decimeter{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::centimeter{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::millimeter{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::micrometer{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::nanometer{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::picometer{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile{other.m_impl.m_value10};
        break;
      default:
        break;
    }
    return *this;
  }
  distance& operator=(distance&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::kilometer{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decimeter{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::centimeter{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::millimeter{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::micrometer{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::nanometer{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::picometer{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile{std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::meter* distance::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::kilometer* distance::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::decimeter* distance::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::centimeter* distance::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::millimeter* distance::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::micrometer* distance::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::nanometer* distance::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::picometer* distance::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::inch* distance::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::foot* distance::target() const
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline const ossia::mile* distance::target() const
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline ossia::meter* distance::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::kilometer* distance::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::decimeter* distance::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::centimeter* distance::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::millimeter* distance::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::micrometer* distance::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::nanometer* distance::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::picometer* distance::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::inch* distance::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::foot* distance::target()
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline ossia::mile* distance::target()
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline const ossia::meter& distance::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::kilometer& distance::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::decimeter& distance::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::centimeter& distance::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::millimeter& distance::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::micrometer& distance::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::nanometer& distance::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::picometer& distance::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::inch& distance::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::foot& distance::get() const
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::mile& distance::get() const
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::meter& distance::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::kilometer& distance::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::decimeter& distance::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::centimeter& distance::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::millimeter& distance::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::micrometer& distance::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::nanometer& distance::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::picometer& distance::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::inch& distance::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::foot& distance::get()
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::mile& distance::get()
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const distance& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, distance& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, distance&& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case distance::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case distance::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case distance::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case distance::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case distance::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case distance::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case distance::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case distance::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case distance::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case distance::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const distance& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, distance& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, distance&& var)
{
  switch (var.m_type)
  {
    case distance::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case distance::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case distance::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case distance::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case distance::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case distance::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case distance::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case distance::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case distance::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case distance::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case distance::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      return functor();
  }
}
inline bool operator==(const distance& lhs, const distance& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case distance::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case distance::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case distance::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case distance::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case distance::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case distance::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case distance::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case distance::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      case distance::Type::Type8:
        return lhs.m_impl.m_value8 == rhs.m_impl.m_value8;
      case distance::Type::Type9:
        return lhs.m_impl.m_value9 == rhs.m_impl.m_value9;
      case distance::Type::Type10:
        return lhs.m_impl.m_value10 == rhs.m_impl.m_value10;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const distance& lhs, const distance& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case distance::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case distance::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case distance::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case distance::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case distance::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case distance::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case distance::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case distance::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    case distance::Type::Type8:
      return lhs.m_impl.m_value8 != rhs.m_impl.m_value8;
    case distance::Type::Type9:
      return lhs.m_impl.m_value9 != rhs.m_impl.m_value9;
    case distance::Type::Type10:
      return lhs.m_impl.m_value10 != rhs.m_impl.m_value10;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const distance& lhs, const ossia::meter& rhs)
{
  return (lhs.m_type == distance::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::meter& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::meter& rhs)
{
  return (lhs.m_type != distance::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::meter& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::kilometer& rhs)
{
  return (lhs.m_type == distance::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::kilometer& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::kilometer& rhs)
{
  return (lhs.m_type != distance::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::kilometer& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::decimeter& rhs)
{
  return (lhs.m_type == distance::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::decimeter& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::decimeter& rhs)
{
  return (lhs.m_type != distance::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::decimeter& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::centimeter& rhs)
{
  return (lhs.m_type == distance::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::centimeter& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::centimeter& rhs)
{
  return (lhs.m_type != distance::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::centimeter& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::millimeter& rhs)
{
  return (lhs.m_type == distance::Type::Type4) && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::millimeter& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type4) && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::millimeter& rhs)
{
  return (lhs.m_type != distance::Type::Type4) || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::millimeter& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type4) || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::micrometer& rhs)
{
  return (lhs.m_type == distance::Type::Type5) && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::micrometer& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type5) && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::micrometer& rhs)
{
  return (lhs.m_type != distance::Type::Type5) || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::micrometer& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type5) || (rhs.m_impl.m_value5 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::nanometer& rhs)
{
  return (lhs.m_type == distance::Type::Type6) && (lhs.m_impl.m_value6 == rhs);
}
inline bool operator==(const ossia::nanometer& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type6) && (rhs.m_impl.m_value6 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::nanometer& rhs)
{
  return (lhs.m_type != distance::Type::Type6) || (lhs.m_impl.m_value6 != rhs);
}
inline bool operator!=(const ossia::nanometer& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type6) || (rhs.m_impl.m_value6 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::picometer& rhs)
{
  return (lhs.m_type == distance::Type::Type7) && (lhs.m_impl.m_value7 == rhs);
}
inline bool operator==(const ossia::picometer& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type7) && (rhs.m_impl.m_value7 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::picometer& rhs)
{
  return (lhs.m_type != distance::Type::Type7) || (lhs.m_impl.m_value7 != rhs);
}
inline bool operator!=(const ossia::picometer& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type7) || (rhs.m_impl.m_value7 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::inch& rhs)
{
  return (lhs.m_type == distance::Type::Type8) && (lhs.m_impl.m_value8 == rhs);
}
inline bool operator==(const ossia::inch& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type8) && (rhs.m_impl.m_value8 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::inch& rhs)
{
  return (lhs.m_type != distance::Type::Type8) || (lhs.m_impl.m_value8 != rhs);
}
inline bool operator!=(const ossia::inch& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type8) || (rhs.m_impl.m_value8 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::foot& rhs)
{
  return (lhs.m_type == distance::Type::Type9) && (lhs.m_impl.m_value9 == rhs);
}
inline bool operator==(const ossia::foot& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type9) && (rhs.m_impl.m_value9 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::foot& rhs)
{
  return (lhs.m_type != distance::Type::Type9) || (lhs.m_impl.m_value9 != rhs);
}
inline bool operator!=(const ossia::foot& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type9) || (rhs.m_impl.m_value9 != lhs);
}
inline bool operator==(const distance& lhs, const ossia::mile& rhs)
{
  return (lhs.m_type == distance::Type::Type10)
         && (lhs.m_impl.m_value10 == rhs);
}
inline bool operator==(const ossia::mile& lhs, const distance& rhs)
{
  return (rhs.m_type == distance::Type::Type10)
         && (rhs.m_impl.m_value10 == lhs);
}
inline bool operator!=(const distance& lhs, const ossia::mile& rhs)
{
  return (lhs.m_type != distance::Type::Type10)
         || (lhs.m_impl.m_value10 != rhs);
}
inline bool operator!=(const ossia::mile& lhs, const distance& rhs)
{
  return (rhs.m_type != distance::Type::Type10)
         || (rhs.m_impl.m_value10 != lhs);
}
struct gain
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::linear m_value0;

    ossia::midigain m_value1;

    ossia::decibel m_value2;

    ossia::decibel_raw m_value3;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  gain() : m_type{Npos}
  {
  }
  ~gain()
  {
    destruct_impl();
  }
  gain(ossia::linear v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::linear{v};
  }
  gain(ossia::midigain v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::midigain{v};
  }
  gain(ossia::decibel v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::decibel{v};
  }
  gain(ossia::decibel_raw v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::decibel_raw{v};
  }
  gain(const gain& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::midigain{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decibel{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::decibel_raw{other.m_impl.m_value3};
        break;
      default:
        break;
    }
  }
  gain(gain&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::midigain{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decibel{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::decibel_raw{std::move(other.m_impl.m_value3)};
        break;
      default:
        break;
    }
  }
  gain& operator=(const gain& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::midigain{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decibel{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::decibel_raw{other.m_impl.m_value3};
        break;
      default:
        break;
    }
    return *this;
  }
  gain& operator=(gain&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::midigain{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decibel{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::decibel_raw{std::move(other.m_impl.m_value3)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::linear* gain::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::midigain* gain::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::decibel* gain::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::decibel_raw* gain::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::linear* gain::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::midigain* gain::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::decibel* gain::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::decibel_raw* gain::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::linear& gain::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::midigain& gain::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::decibel& gain::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::decibel_raw& gain::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::linear& gain::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::midigain& gain::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::decibel& gain::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::decibel_raw& gain::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const gain& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, gain& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, gain&& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case gain::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case gain::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case gain::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const gain& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, gain& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, gain&& var)
{
  switch (var.m_type)
  {
    case gain::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case gain::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case gain::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case gain::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    default:
      return functor();
  }
}
inline bool operator==(const gain& lhs, const gain& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case gain::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case gain::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case gain::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case gain::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const gain& lhs, const gain& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case gain::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case gain::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case gain::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case gain::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const gain& lhs, const ossia::linear& rhs)
{
  return (lhs.m_type == gain::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::linear& lhs, const gain& rhs)
{
  return (rhs.m_type == gain::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const gain& lhs, const ossia::linear& rhs)
{
  return (lhs.m_type != gain::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::linear& lhs, const gain& rhs)
{
  return (rhs.m_type != gain::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const gain& lhs, const ossia::midigain& rhs)
{
  return (lhs.m_type == gain::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::midigain& lhs, const gain& rhs)
{
  return (rhs.m_type == gain::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const gain& lhs, const ossia::midigain& rhs)
{
  return (lhs.m_type != gain::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::midigain& lhs, const gain& rhs)
{
  return (rhs.m_type != gain::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const gain& lhs, const ossia::decibel& rhs)
{
  return (lhs.m_type == gain::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::decibel& lhs, const gain& rhs)
{
  return (rhs.m_type == gain::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const gain& lhs, const ossia::decibel& rhs)
{
  return (lhs.m_type != gain::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::decibel& lhs, const gain& rhs)
{
  return (rhs.m_type != gain::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const gain& lhs, const ossia::decibel_raw& rhs)
{
  return (lhs.m_type == gain::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::decibel_raw& lhs, const gain& rhs)
{
  return (rhs.m_type == gain::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const gain& lhs, const ossia::decibel_raw& rhs)
{
  return (lhs.m_type != gain::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::decibel_raw& lhs, const gain& rhs)
{
  return (rhs.m_type != gain::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
struct orientation
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::quaternion m_value0;

    ossia::euler m_value1;

    ossia::axis m_value2;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  orientation() : m_type{Npos}
  {
  }
  ~orientation()
  {
    destruct_impl();
  }
  orientation(ossia::quaternion v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::quaternion{v};
  }
  orientation(ossia::euler v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::euler{v};
  }
  orientation(ossia::axis v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::axis{v};
  }
  orientation(const orientation& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::quaternion{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis{other.m_impl.m_value2};
        break;
      default:
        break;
    }
  }
  orientation(orientation&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::quaternion{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis{std::move(other.m_impl.m_value2)};
        break;
      default:
        break;
    }
  }
  orientation& operator=(const orientation& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::quaternion{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis{other.m_impl.m_value2};
        break;
      default:
        break;
    }
    return *this;
  }
  orientation& operator=(orientation&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::quaternion{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis{std::move(other.m_impl.m_value2)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::quaternion* orientation::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::euler* orientation::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::axis* orientation::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::quaternion* orientation::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::euler* orientation::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::axis* orientation::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::quaternion& orientation::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::euler& orientation::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::axis& orientation::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::quaternion& orientation::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::euler& orientation::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::axis& orientation::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const orientation& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, orientation& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, orientation&& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case orientation::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case orientation::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const orientation& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, orientation& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, orientation&& var)
{
  switch (var.m_type)
  {
    case orientation::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case orientation::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case orientation::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    default:
      return functor();
  }
}
inline bool operator==(const orientation& lhs, const orientation& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case orientation::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case orientation::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case orientation::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const orientation& lhs, const orientation& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case orientation::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case orientation::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case orientation::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const orientation& lhs, const ossia::quaternion& rhs)
{
  return (lhs.m_type == orientation::Type::Type0)
         && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::quaternion& lhs, const orientation& rhs)
{
  return (rhs.m_type == orientation::Type::Type0)
         && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const orientation& lhs, const ossia::quaternion& rhs)
{
  return (lhs.m_type != orientation::Type::Type0)
         || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::quaternion& lhs, const orientation& rhs)
{
  return (rhs.m_type != orientation::Type::Type0)
         || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const orientation& lhs, const ossia::euler& rhs)
{
  return (lhs.m_type == orientation::Type::Type1)
         && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::euler& lhs, const orientation& rhs)
{
  return (rhs.m_type == orientation::Type::Type1)
         && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const orientation& lhs, const ossia::euler& rhs)
{
  return (lhs.m_type != orientation::Type::Type1)
         || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::euler& lhs, const orientation& rhs)
{
  return (rhs.m_type != orientation::Type::Type1)
         || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const orientation& lhs, const ossia::axis& rhs)
{
  return (lhs.m_type == orientation::Type::Type2)
         && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::axis& lhs, const orientation& rhs)
{
  return (rhs.m_type == orientation::Type::Type2)
         && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const orientation& lhs, const ossia::axis& rhs)
{
  return (lhs.m_type != orientation::Type::Type2)
         || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::axis& lhs, const orientation& rhs)
{
  return (rhs.m_type != orientation::Type::Type2)
         || (rhs.m_impl.m_value2 != lhs);
}
struct position
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::cartesian_3d m_value0;

    ossia::cartesian_2d m_value1;

    ossia::spherical m_value2;

    ossia::polar m_value3;

    ossia::opengl m_value4;

    ossia::cylindrical m_value5;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  position() : m_type{Npos}
  {
  }
  ~position()
  {
    destruct_impl();
  }
  position(ossia::cartesian_3d v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::cartesian_3d{v};
  }
  position(ossia::cartesian_2d v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::cartesian_2d{v};
  }
  position(ossia::spherical v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::spherical{v};
  }
  position(ossia::polar v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::polar{v};
  }
  position(ossia::opengl v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::opengl{v};
  }
  position(ossia::cylindrical v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::cylindrical{v};
  }
  position(const position& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::cartesian_3d{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::cartesian_2d{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::spherical{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::cylindrical{other.m_impl.m_value5};
        break;
      default:
        break;
    }
  }
  position(position&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::cartesian_3d{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::cartesian_2d{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::spherical{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::cylindrical{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
  }
  position& operator=(const position& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::cartesian_3d{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::cartesian_2d{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::spherical{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::cylindrical{other.m_impl.m_value5};
        break;
      default:
        break;
    }
    return *this;
  }
  position& operator=(position&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::cartesian_3d{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::cartesian_2d{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::spherical{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::cylindrical{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::cartesian_3d* position::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::cartesian_2d* position::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::spherical* position::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::polar* position::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::opengl* position::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::cylindrical* position::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::cartesian_3d* position::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::cartesian_2d* position::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::spherical* position::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::polar* position::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::opengl* position::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::cylindrical* position::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::cartesian_3d& position::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::cartesian_2d& position::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::spherical& position::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::polar& position::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::opengl& position::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::cylindrical& position::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::cartesian_3d& position::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::cartesian_2d& position::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::spherical& position::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::polar& position::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::opengl& position::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::cylindrical& position::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const position& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, position& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, position&& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case position::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case position::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case position::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case position::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case position::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const position& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, position& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, position&& var)
{
  switch (var.m_type)
  {
    case position::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case position::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case position::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case position::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case position::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case position::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      return functor();
  }
}
inline bool operator==(const position& lhs, const position& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case position::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case position::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case position::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case position::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case position::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case position::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const position& lhs, const position& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case position::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case position::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case position::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case position::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case position::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case position::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const position& lhs, const ossia::cartesian_3d& rhs)
{
  return (lhs.m_type == position::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::cartesian_3d& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::cartesian_3d& rhs)
{
  return (lhs.m_type != position::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::cartesian_3d& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const position& lhs, const ossia::cartesian_2d& rhs)
{
  return (lhs.m_type == position::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::cartesian_2d& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::cartesian_2d& rhs)
{
  return (lhs.m_type != position::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::cartesian_2d& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const position& lhs, const ossia::spherical& rhs)
{
  return (lhs.m_type == position::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::spherical& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::spherical& rhs)
{
  return (lhs.m_type != position::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::spherical& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const position& lhs, const ossia::polar& rhs)
{
  return (lhs.m_type == position::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::polar& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::polar& rhs)
{
  return (lhs.m_type != position::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::polar& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const position& lhs, const ossia::opengl& rhs)
{
  return (lhs.m_type == position::Type::Type4) && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::opengl& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type4) && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::opengl& rhs)
{
  return (lhs.m_type != position::Type::Type4) || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::opengl& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type4) || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const position& lhs, const ossia::cylindrical& rhs)
{
  return (lhs.m_type == position::Type::Type5) && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::cylindrical& lhs, const position& rhs)
{
  return (rhs.m_type == position::Type::Type5) && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const position& lhs, const ossia::cylindrical& rhs)
{
  return (lhs.m_type != position::Type::Type5) || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::cylindrical& lhs, const position& rhs)
{
  return (rhs.m_type != position::Type::Type5) || (rhs.m_impl.m_value5 != lhs);
}
struct speed
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::meter_per_second m_value0;

    ossia::miles_per_hour m_value1;

    ossia::kilometer_per_hour m_value2;

    ossia::knot m_value3;

    ossia::foot_per_second m_value4;

    ossia::foot_per_hour m_value5;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  speed() : m_type{Npos}
  {
  }
  ~speed()
  {
    destruct_impl();
  }
  speed(ossia::meter_per_second v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::meter_per_second{v};
  }
  speed(ossia::miles_per_hour v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::miles_per_hour{v};
  }
  speed(ossia::kilometer_per_hour v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::kilometer_per_hour{v};
  }
  speed(ossia::knot v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::knot{v};
  }
  speed(ossia::foot_per_second v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::foot_per_second{v};
  }
  speed(ossia::foot_per_hour v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::foot_per_hour{v};
  }
  speed(const speed& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter_per_second{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::miles_per_hour{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::foot_per_second{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::foot_per_hour{other.m_impl.m_value5};
        break;
      default:
        break;
    }
  }
  speed(speed&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::miles_per_hour{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::foot_per_second{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::foot_per_hour{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
  }
  speed& operator=(const speed& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter_per_second{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::miles_per_hour{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::foot_per_second{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::foot_per_hour{other.m_impl.m_value5};
        break;
      default:
        break;
    }
    return *this;
  }
  speed& operator=(speed&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::miles_per_hour{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::foot_per_second{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::foot_per_hour{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::meter_per_second* speed::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::miles_per_hour* speed::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::kilometer_per_hour* speed::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::knot* speed::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::foot_per_second* speed::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::foot_per_hour* speed::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::meter_per_second* speed::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::miles_per_hour* speed::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::kilometer_per_hour* speed::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::knot* speed::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::foot_per_second* speed::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::foot_per_hour* speed::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::meter_per_second& speed::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::miles_per_hour& speed::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::kilometer_per_hour& speed::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::knot& speed::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::foot_per_second& speed::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::foot_per_hour& speed::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::meter_per_second& speed::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::miles_per_hour& speed::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::kilometer_per_hour& speed::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::knot& speed::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::foot_per_second& speed::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::foot_per_hour& speed::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const speed& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, speed& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, speed&& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case speed::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case speed::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case speed::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case speed::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case speed::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const speed& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, speed& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, speed&& var)
{
  switch (var.m_type)
  {
    case speed::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case speed::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case speed::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case speed::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case speed::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case speed::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      return functor();
  }
}
inline bool operator==(const speed& lhs, const speed& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case speed::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case speed::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case speed::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case speed::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case speed::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case speed::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const speed& lhs, const speed& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case speed::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case speed::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case speed::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case speed::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case speed::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case speed::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const speed& lhs, const ossia::meter_per_second& rhs)
{
  return (lhs.m_type == speed::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::meter_per_second& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::meter_per_second& rhs)
{
  return (lhs.m_type != speed::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::meter_per_second& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const speed& lhs, const ossia::miles_per_hour& rhs)
{
  return (lhs.m_type == speed::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::miles_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::miles_per_hour& rhs)
{
  return (lhs.m_type != speed::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::miles_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const speed& lhs, const ossia::kilometer_per_hour& rhs)
{
  return (lhs.m_type == speed::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::kilometer_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::kilometer_per_hour& rhs)
{
  return (lhs.m_type != speed::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::kilometer_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const speed& lhs, const ossia::knot& rhs)
{
  return (lhs.m_type == speed::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::knot& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::knot& rhs)
{
  return (lhs.m_type != speed::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::knot& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const speed& lhs, const ossia::foot_per_second& rhs)
{
  return (lhs.m_type == speed::Type::Type4) && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::foot_per_second& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type4) && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::foot_per_second& rhs)
{
  return (lhs.m_type != speed::Type::Type4) || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::foot_per_second& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type4) || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const speed& lhs, const ossia::foot_per_hour& rhs)
{
  return (lhs.m_type == speed::Type::Type5) && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::foot_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type == speed::Type::Type5) && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const speed& lhs, const ossia::foot_per_hour& rhs)
{
  return (lhs.m_type != speed::Type::Type5) || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::foot_per_hour& lhs, const speed& rhs)
{
  return (rhs.m_type != speed::Type::Type5) || (rhs.m_impl.m_value5 != lhs);
}
struct timing
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::second m_value0;

    ossia::bark m_value1;

    ossia::bpm m_value2;

    ossia::cent m_value3;

    ossia::frequency m_value4;

    ossia::mel m_value5;

    ossia::midi_pitch m_value6;

    ossia::millisecond m_value7;

    ossia::playback_speed m_value8;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
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
  timing() : m_type{Npos}
  {
  }
  ~timing()
  {
    destruct_impl();
  }
  timing(ossia::second v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::second{v};
  }
  timing(ossia::bark v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::bark{v};
  }
  timing(ossia::bpm v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::bpm{v};
  }
  timing(ossia::cent v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::cent{v};
  }
  timing(ossia::frequency v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::frequency{v};
  }
  timing(ossia::mel v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::mel{v};
  }
  timing(ossia::midi_pitch v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::midi_pitch{v};
  }
  timing(ossia::millisecond v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::millisecond{v};
  }
  timing(ossia::playback_speed v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::playback_speed{v};
  }
  timing(const timing& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::frequency{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::midi_pitch{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::millisecond{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::playback_speed{other.m_impl.m_value8};
        break;
      default:
        break;
    }
  }
  timing(timing&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::frequency{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::midi_pitch{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::millisecond{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::playback_speed{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
  }
  timing& operator=(const timing& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::frequency{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::midi_pitch{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::millisecond{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::playback_speed{other.m_impl.m_value8};
        break;
      default:
        break;
    }
    return *this;
  }
  timing& operator=(timing&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::frequency{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::midi_pitch{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::millisecond{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::playback_speed{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::second* timing::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::bark* timing::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::bpm* timing::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::cent* timing::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::frequency* timing::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::mel* timing::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::midi_pitch* timing::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::millisecond* timing::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::playback_speed* timing::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::second* timing::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::bark* timing::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::bpm* timing::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::cent* timing::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::frequency* timing::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::mel* timing::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::midi_pitch* timing::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::millisecond* timing::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::playback_speed* timing::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::second& timing::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::bark& timing::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::bpm& timing::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::cent& timing::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::frequency& timing::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::mel& timing::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::midi_pitch& timing::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::millisecond& timing::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::playback_speed& timing::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::second& timing::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::bark& timing::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::bpm& timing::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::cent& timing::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::frequency& timing::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::mel& timing::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::midi_pitch& timing::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::millisecond& timing::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::playback_speed& timing::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const timing& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, timing& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, timing&& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case timing::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case timing::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case timing::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case timing::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case timing::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case timing::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case timing::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case timing::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const timing& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, timing& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, timing&& var)
{
  switch (var.m_type)
  {
    case timing::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case timing::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case timing::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case timing::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case timing::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case timing::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case timing::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case timing::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case timing::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      return functor();
  }
}
inline bool operator==(const timing& lhs, const timing& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case timing::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case timing::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case timing::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case timing::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case timing::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case timing::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case timing::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case timing::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      case timing::Type::Type8:
        return lhs.m_impl.m_value8 == rhs.m_impl.m_value8;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const timing& lhs, const timing& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case timing::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case timing::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case timing::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case timing::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case timing::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case timing::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case timing::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case timing::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    case timing::Type::Type8:
      return lhs.m_impl.m_value8 != rhs.m_impl.m_value8;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const timing& lhs, const ossia::second& rhs)
{
  return (lhs.m_type == timing::Type::Type0) && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::second& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type0) && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::second& rhs)
{
  return (lhs.m_type != timing::Type::Type0) || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::second& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type0) || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::bark& rhs)
{
  return (lhs.m_type == timing::Type::Type1) && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::bark& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type1) && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::bark& rhs)
{
  return (lhs.m_type != timing::Type::Type1) || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::bark& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type1) || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::bpm& rhs)
{
  return (lhs.m_type == timing::Type::Type2) && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::bpm& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type2) && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::bpm& rhs)
{
  return (lhs.m_type != timing::Type::Type2) || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::bpm& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type2) || (rhs.m_impl.m_value2 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::cent& rhs)
{
  return (lhs.m_type == timing::Type::Type3) && (lhs.m_impl.m_value3 == rhs);
}
inline bool operator==(const ossia::cent& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type3) && (rhs.m_impl.m_value3 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::cent& rhs)
{
  return (lhs.m_type != timing::Type::Type3) || (lhs.m_impl.m_value3 != rhs);
}
inline bool operator!=(const ossia::cent& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type3) || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::frequency& rhs)
{
  return (lhs.m_type == timing::Type::Type4) && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::frequency& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type4) && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::frequency& rhs)
{
  return (lhs.m_type != timing::Type::Type4) || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::frequency& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type4) || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::mel& rhs)
{
  return (lhs.m_type == timing::Type::Type5) && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::mel& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type5) && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::mel& rhs)
{
  return (lhs.m_type != timing::Type::Type5) || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::mel& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type5) || (rhs.m_impl.m_value5 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::midi_pitch& rhs)
{
  return (lhs.m_type == timing::Type::Type6) && (lhs.m_impl.m_value6 == rhs);
}
inline bool operator==(const ossia::midi_pitch& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type6) && (rhs.m_impl.m_value6 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::midi_pitch& rhs)
{
  return (lhs.m_type != timing::Type::Type6) || (lhs.m_impl.m_value6 != rhs);
}
inline bool operator!=(const ossia::midi_pitch& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type6) || (rhs.m_impl.m_value6 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::millisecond& rhs)
{
  return (lhs.m_type == timing::Type::Type7) && (lhs.m_impl.m_value7 == rhs);
}
inline bool operator==(const ossia::millisecond& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type7) && (rhs.m_impl.m_value7 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::millisecond& rhs)
{
  return (lhs.m_type != timing::Type::Type7) || (lhs.m_impl.m_value7 != rhs);
}
inline bool operator!=(const ossia::millisecond& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type7) || (rhs.m_impl.m_value7 != lhs);
}
inline bool operator==(const timing& lhs, const ossia::playback_speed& rhs)
{
  return (lhs.m_type == timing::Type::Type8) && (lhs.m_impl.m_value8 == rhs);
}
inline bool operator==(const ossia::playback_speed& lhs, const timing& rhs)
{
  return (rhs.m_type == timing::Type::Type8) && (rhs.m_impl.m_value8 == lhs);
}
inline bool operator!=(const timing& lhs, const ossia::playback_speed& rhs)
{
  return (lhs.m_type != timing::Type::Type8) || (lhs.m_impl.m_value8 != rhs);
}
inline bool operator!=(const ossia::playback_speed& lhs, const timing& rhs)
{
  return (rhs.m_type != timing::Type::Type8) || (rhs.m_impl.m_value8 != lhs);
}
struct strong_value_variant
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::value m_value0;

    ossia::distance m_value1;

    ossia::position m_value2;

    ossia::speed m_value3;

    ossia::orientation m_value4;

    ossia::angle m_value5;

    ossia::color m_value6;

    ossia::gain m_value7;

    ossia::timing m_value8;

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
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      case Type::Type0:
        m_impl.m_value0.~value();
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
  strong_value_variant() : m_type{Npos}
  {
  }
  ~strong_value_variant()
  {
    destruct_impl();
  }
  strong_value_variant(const ossia::value& v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::value{v};
  }
  strong_value_variant(ossia::value&& v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::value{std::move(v)};
  }
  strong_value_variant(ossia::distance v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::distance{v};
  }
  strong_value_variant(ossia::position v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::position{v};
  }
  strong_value_variant(ossia::speed v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::speed{v};
  }
  strong_value_variant(ossia::orientation v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::orientation{v};
  }
  strong_value_variant(ossia::angle v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::angle{v};
  }
  strong_value_variant(ossia::color v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::color{v};
  }
  strong_value_variant(ossia::gain v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::gain{v};
  }
  strong_value_variant(ossia::timing v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::timing{v};
  }
  strong_value_variant(const strong_value_variant& other)
      : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::value{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::distance{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::position{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::speed{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::orientation{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::angle{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::color{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::gain{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::timing{other.m_impl.m_value8};
        break;
      default:
        break;
    }
  }
  strong_value_variant(strong_value_variant&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::value{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::distance{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::position{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::speed{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::orientation{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::angle{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::color{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::gain{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::timing{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
  }
  strong_value_variant& operator=(const strong_value_variant& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::value{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::distance{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::position{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::speed{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::orientation{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::angle{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::color{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::gain{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::timing{other.m_impl.m_value8};
        break;
      default:
        break;
    }
    return *this;
  }
  strong_value_variant& operator=(strong_value_variant&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::value{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::distance{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::position{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::speed{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::orientation{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::angle{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::color{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::gain{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::timing{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::value* strong_value_variant::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::distance* strong_value_variant::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::position* strong_value_variant::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::speed* strong_value_variant::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::orientation* strong_value_variant::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::angle* strong_value_variant::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::color* strong_value_variant::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::gain* strong_value_variant::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::timing* strong_value_variant::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::value* strong_value_variant::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::distance* strong_value_variant::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::position* strong_value_variant::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::speed* strong_value_variant::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::orientation* strong_value_variant::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::angle* strong_value_variant::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::color* strong_value_variant::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::gain* strong_value_variant::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::timing* strong_value_variant::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::value& strong_value_variant::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::distance& strong_value_variant::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::position& strong_value_variant::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::speed& strong_value_variant::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::orientation& strong_value_variant::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::angle& strong_value_variant::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::color& strong_value_variant::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::gain& strong_value_variant::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline const ossia::timing& strong_value_variant::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::value& strong_value_variant::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::distance& strong_value_variant::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::position& strong_value_variant::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::speed& strong_value_variant::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::orientation& strong_value_variant::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::angle& strong_value_variant::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::color& strong_value_variant::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::gain& strong_value_variant::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <>
inline ossia::timing& strong_value_variant::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_strong_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const strong_value_variant& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case strong_value_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case strong_value_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case strong_value_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case strong_value_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case strong_value_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case strong_value_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case strong_value_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case strong_value_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, strong_value_variant& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case strong_value_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case strong_value_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case strong_value_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case strong_value_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case strong_value_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case strong_value_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case strong_value_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case strong_value_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, strong_value_variant&& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case strong_value_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case strong_value_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case strong_value_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case strong_value_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case strong_value_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case strong_value_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case strong_value_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case strong_value_variant::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      throw std::runtime_error("dataspace_strong_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const strong_value_variant& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case strong_value_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case strong_value_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case strong_value_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case strong_value_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case strong_value_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case strong_value_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case strong_value_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case strong_value_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, strong_value_variant& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case strong_value_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case strong_value_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case strong_value_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case strong_value_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case strong_value_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case strong_value_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case strong_value_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    case strong_value_variant::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, strong_value_variant&& var)
{
  switch (var.m_type)
  {
    case strong_value_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case strong_value_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case strong_value_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case strong_value_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case strong_value_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case strong_value_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case strong_value_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case strong_value_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case strong_value_variant::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      return functor();
  }
}
inline bool
operator==(const strong_value_variant& lhs, const strong_value_variant& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case strong_value_variant::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case strong_value_variant::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case strong_value_variant::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case strong_value_variant::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case strong_value_variant::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case strong_value_variant::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case strong_value_variant::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case strong_value_variant::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      case strong_value_variant::Type::Type8:
        return lhs.m_impl.m_value8 == rhs.m_impl.m_value8;
      default:
        return true;
    }
  }
  return false;
}
inline bool
operator!=(const strong_value_variant& lhs, const strong_value_variant& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case strong_value_variant::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case strong_value_variant::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case strong_value_variant::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case strong_value_variant::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case strong_value_variant::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case strong_value_variant::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case strong_value_variant::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case strong_value_variant::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    case strong_value_variant::Type::Type8:
      return lhs.m_impl.m_value8 != rhs.m_impl.m_value8;
    default:
      return false;
  }
  return true;
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::value& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type0)
         && (lhs.m_impl.m_value0 == rhs);
}
inline bool
operator==(const ossia::value& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type0)
         && (rhs.m_impl.m_value0 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::value& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type0)
         || (lhs.m_impl.m_value0 != rhs);
}
inline bool
operator!=(const ossia::value& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type0)
         || (rhs.m_impl.m_value0 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::distance& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type1)
         && (lhs.m_impl.m_value1 == rhs);
}
inline bool
operator==(const ossia::distance& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type1)
         && (rhs.m_impl.m_value1 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::distance& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type1)
         || (lhs.m_impl.m_value1 != rhs);
}
inline bool
operator!=(const ossia::distance& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type1)
         || (rhs.m_impl.m_value1 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::position& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type2)
         && (lhs.m_impl.m_value2 == rhs);
}
inline bool
operator==(const ossia::position& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type2)
         && (rhs.m_impl.m_value2 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::position& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type2)
         || (lhs.m_impl.m_value2 != rhs);
}
inline bool
operator!=(const ossia::position& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type2)
         || (rhs.m_impl.m_value2 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::speed& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type3)
         && (lhs.m_impl.m_value3 == rhs);
}
inline bool
operator==(const ossia::speed& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type3)
         && (rhs.m_impl.m_value3 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::speed& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type3)
         || (lhs.m_impl.m_value3 != rhs);
}
inline bool
operator!=(const ossia::speed& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type3)
         || (rhs.m_impl.m_value3 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::orientation& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type4)
         && (lhs.m_impl.m_value4 == rhs);
}
inline bool
operator==(const ossia::orientation& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type4)
         && (rhs.m_impl.m_value4 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::orientation& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type4)
         || (lhs.m_impl.m_value4 != rhs);
}
inline bool
operator!=(const ossia::orientation& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type4)
         || (rhs.m_impl.m_value4 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::angle& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type5)
         && (lhs.m_impl.m_value5 == rhs);
}
inline bool
operator==(const ossia::angle& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type5)
         && (rhs.m_impl.m_value5 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::angle& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type5)
         || (lhs.m_impl.m_value5 != rhs);
}
inline bool
operator!=(const ossia::angle& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type5)
         || (rhs.m_impl.m_value5 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::color& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type6)
         && (lhs.m_impl.m_value6 == rhs);
}
inline bool
operator==(const ossia::color& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type6)
         && (rhs.m_impl.m_value6 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::color& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type6)
         || (lhs.m_impl.m_value6 != rhs);
}
inline bool
operator!=(const ossia::color& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type6)
         || (rhs.m_impl.m_value6 != lhs);
}
inline bool operator==(const strong_value_variant& lhs, const ossia::gain& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type7)
         && (lhs.m_impl.m_value7 == rhs);
}
inline bool operator==(const ossia::gain& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type7)
         && (rhs.m_impl.m_value7 == lhs);
}
inline bool operator!=(const strong_value_variant& lhs, const ossia::gain& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type7)
         || (lhs.m_impl.m_value7 != rhs);
}
inline bool operator!=(const ossia::gain& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type7)
         || (rhs.m_impl.m_value7 != lhs);
}
inline bool
operator==(const strong_value_variant& lhs, const ossia::timing& rhs)
{
  return (lhs.m_type == strong_value_variant::Type::Type8)
         && (lhs.m_impl.m_value8 == rhs);
}
inline bool
operator==(const ossia::timing& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type == strong_value_variant::Type::Type8)
         && (rhs.m_impl.m_value8 == lhs);
}
inline bool
operator!=(const strong_value_variant& lhs, const ossia::timing& rhs)
{
  return (lhs.m_type != strong_value_variant::Type::Type8)
         || (lhs.m_impl.m_value8 != rhs);
}
inline bool
operator!=(const ossia::timing& lhs, const strong_value_variant& rhs)
{
  return (rhs.m_type != strong_value_variant::Type::Type8)
         || (rhs.m_impl.m_value8 != lhs);
}
