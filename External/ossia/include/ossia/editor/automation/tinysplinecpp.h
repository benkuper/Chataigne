#pragma once

#include "tinyspline.h"
#include <stdexcept>
#include <ossia_export.h>
#include <vector>
#include <string>

namespace tinyspline {

typedef tsReal real;

class OSSIA_EXPORT DeBoorNet {
public:
  /* Constructors & Destructors */
  DeBoorNet()
  {
    ts_deboornet_default(&deBoorNet);
  }
  DeBoorNet(const DeBoorNet &other)
  {
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
  }
  ~DeBoorNet()
  {
    ts_deboornet_free(&deBoorNet);
  }

  /* Operators */
  DeBoorNet& operator=(
  const tinyspline::DeBoorNet &other)
  {
    if (&other != this) {
      const tsError err = ts_deboornet_copy(
      &other.deBoorNet, &deBoorNet);
      if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    }
    return *this;
  }




  /* Getter */
  real u() const
  {
    return deBoorNet.u;
  }
  size_t k() const
  {
    return deBoorNet.k;
  }
  size_t s() const
  {
    return deBoorNet.s;
  }
  size_t h() const
  {
    return deBoorNet.h;
  }
  size_t dim() const
  {
    return deBoorNet.dim;
  }
  size_t nPoints() const
  {
    return deBoorNet.n_points;
  }
  std::vector<real> points() const
  {
    const tinyspline::real *begin = deBoorNet.points;
    const tinyspline::real *end = begin + deBoorNet.n_points*deBoorNet.dim;
    return std::vector<tinyspline::real>(begin, end);
  }
  std::vector<real> result() const
  {
    const tinyspline::real *begin = deBoorNet.result;
    const tinyspline::real *end = begin + deBoorNet.dim;
    return std::vector<tinyspline::real>(begin, end);
  }
  tsDeBoorNet * data()
  {
    return &deBoorNet;
  }

  /* C++11 features */
  DeBoorNet(DeBoorNet &&other)
  {
    ts_deboornet_default(&deBoorNet);
    swap(other);
  }
  DeBoorNet & operator=(
  tinyspline::DeBoorNet &&other)
  {
    if (&other != this) {
      ts_deboornet_free(&deBoorNet);
      swap(other);
    }
    return *this;
  }

  void swap(DeBoorNet &other)
  {
    if (&other != this) {
      std::swap(deBoorNet.u, other.deBoorNet.u);
      std::swap(deBoorNet.k, other.deBoorNet.k);
      std::swap(deBoorNet.s, other.deBoorNet.s);
      std::swap(deBoorNet.h, other.deBoorNet.h);
      std::swap(deBoorNet.dim, other.deBoorNet.dim);
      std::swap(deBoorNet.n_points, other.deBoorNet.n_points);
      std::swap(deBoorNet.points, other.deBoorNet.points);
      std::swap(deBoorNet.result, other.deBoorNet.result);
    }
  }
  friend void swap(DeBoorNet &left, DeBoorNet &right)
  {
    left.swap(right);
  }
private:
  tsDeBoorNet deBoorNet;
};

class OSSIA_EXPORT BSpline {
public:
  /* Constructors & Destructors */
  BSpline()
  {
    ts_bspline_default(&bspline);
  }
  BSpline(const BSpline &other)
  {
    const tsError err = ts_bspline_copy(&other.bspline, &bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
  }
  BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
  const tsBSplineType type)
  {
    const tsError err = ts_bspline_new(deg, dim, nCtrlp, type, &bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
  }
  ~BSpline()
  {
    ts_bspline_free(&bspline);
  }

  /* Operators */
  BSpline & operator=(
  const tinyspline::BSpline &other)
  {
    if (&other != this) {
      const tsError err = ts_bspline_copy(&other.bspline, &bspline);
      if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    }
    return *this;
  }

  DeBoorNet operator()(
  const tinyspline::real u) const
  {
    return evaluate(u);
  }


  /* Getter */
  size_t deg() const
  {
    return bspline.deg;
  }
  size_t order() const
  {
    return bspline.order;
  }
  size_t dim() const
  {
    return bspline.dim;
  }
  size_t nCtrlp() const
  {
    return bspline.n_ctrlp;
  }
  size_t nKnots() const
  {
    return bspline.n_knots;
  }
  std::vector<real> ctrlp() const
  {
    const tinyspline::real *begin  = bspline.ctrlp;
    const tinyspline::real *end = begin + bspline.n_ctrlp*bspline.dim;
    return std::vector<tinyspline::real>(begin, end);
  }
  std::vector<real> knots() const
  {
    const tinyspline::real *begin = bspline.knots;
    const tinyspline::real *end = begin + bspline.n_knots;
    return std::vector<tinyspline::real>(begin, end);
  }
  tsBSpline * data()
  {
    return &bspline;
  }
  DeBoorNet evaluate(const real u) const
  {
    tinyspline::DeBoorNet deBoorNet;
    const tsError err = ts_bspline_evaluate(&bspline, u, deBoorNet.data());
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return deBoorNet;
  }

  /* Modifications */
  void setCtrlp(const std::vector<real> &ctrlp)
  {
    if (ctrlp.size() != nCtrlp() * dim()) {
      throw std::runtime_error("The number of values must be equals"
      "to the spline's number of control points multiplied"
      "by the dimension of each control point.");
    }
    const tsError err = ts_bspline_set_ctrlp(
    &bspline, ctrlp.data(), &bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
  }
  void setKnots(const std::vector<real> &knots)
  {
    if (knots.size() != nKnots()) {
      throw std::runtime_error("The number of values must be equals"
      "to the spline's number of knots.");
    }
    const tsError err = ts_bspline_set_knots(
    &bspline, knots.data(), &bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
  }

  /* Transformations */
  BSpline fillKnots(const tsBSplineType type, const real min,
  const real max) const
  {
    tinyspline::BSpline bs;
    const tsError err = ts_bspline_fill_knots(
    &bspline, type, min, max, &bs.bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline insertKnot(const real u, const size_t n) const
  {
    tinyspline::BSpline bs;
    size_t k;
    const tsError err = ts_bspline_insert_knot(
    &bspline, u, n, &bs.bspline, &k);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline resize(const int n, const int back) const
  {
    tinyspline::BSpline bs;
    const tsError err = ts_bspline_resize(&bspline, n, back, &bs.bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline split(const real u) const
  {
    tinyspline::BSpline bs;
    size_t k;
    const tsError err = ts_bspline_split(&bspline, u, &bs.bspline, &k);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline buckle(const real b) const
  {
    tinyspline::BSpline bs;
    const tsError err = ts_bspline_buckle(&bspline, b, &bs.bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline toBeziers() const
  {
    tinyspline::BSpline bs;
    const tsError err = ts_bspline_to_beziers(&bspline, &bs.bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }
  BSpline derive() const
  {
    tinyspline::BSpline bs;
    const tsError err = ts_bspline_derive(&bspline, &bs.bspline);
    if (err < 0)
      throw std::runtime_error(ts_enum_str(err));
    return bs;
  }

  /* C++11 features */
  BSpline(tinyspline::BSpline &&other)
  {
    ts_bspline_default(&bspline);
    swap(other);
  }

  BSpline& operator=(
  tinyspline::BSpline &&other)
  {
    if (&other != this) {
      ts_bspline_free(&bspline);
      swap(other);
    }
    return *this;
  }
  void swap(BSpline &other)
  {
    if (&other != this) {
      std::swap(bspline.deg, other.bspline.deg);
      std::swap(bspline.order, other.bspline.order);
      std::swap(bspline.dim, other.bspline.dim);
      std::swap(bspline.n_ctrlp, other.bspline.n_ctrlp);
      std::swap(bspline.n_knots, other.bspline.n_knots);
      std::swap(bspline.ctrlp, other.bspline.ctrlp);
      std::swap(bspline.knots, other.bspline.knots);
    }
  }
  friend void swap(BSpline &left, BSpline &right)
  {
    left.swap(right);
  }

private:
  tsBSpline bspline;
};

class Utils {
public:
  static BSpline interpolateCubic(const std::vector<real> *points,
  const size_t dim);
  static bool fequals(const real x, const real y);
  static std::string enum_str(const tsError err);
  static tsError str_enum(const std::string str);

private:
  Utils() {}
};

}
