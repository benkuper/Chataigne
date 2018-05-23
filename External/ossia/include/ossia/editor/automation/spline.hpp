#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia/network/value/destination.hpp>

// Courtesy of tinyspline library, MIT license.
#include <ossia/editor/automation/tinysplinecpp.h>

namespace ossia
{
struct spline_point
{
  double x() const
  {
    return m_x;
  }
  double y() const
  {
    return m_y;
  }

  double m_x;
  double m_y;

  friend bool operator==(const spline_point& lhs, const spline_point& rhs)
  {
    return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y;
  }
  friend bool operator!=(const spline_point& lhs, const spline_point& rhs)
  {
    return lhs.m_x != rhs.m_x || lhs.m_y != rhs.m_y;
  }
};

struct spline_data
{
  std::vector<spline_point> points;

  friend bool operator==(const spline_data& lhs, const spline_data& rhs)
  {
    return lhs.points == rhs.points;
  }
  friend bool operator!=(const spline_data& lhs, const spline_data& rhs)
  {
    return lhs.points != rhs.points;
  }
};

class OSSIA_EXPORT spline_automation : public ossia::time_process
{
public:
  spline_automation();

  void set_translation(double x, double y);
  void set_scale(double x, double y);
  void set_destination(ossia::destination a);
  void set_spline(const spline_data& t);

  ossia::state_element state(ossia::time_value date, double pos) override;

  void start(ossia::state& st) override;
  void stop() override;
  void pause() override;
  void resume() override;

private:
  ossia::optional<ossia::destination> m_parameter;
  tinyspline::BSpline m_spline;
  double m_x{}, m_y{}, m_scaleX{1.}, m_scaleY{1.};
};
}
