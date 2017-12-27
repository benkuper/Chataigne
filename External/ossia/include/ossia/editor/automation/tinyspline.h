#ifndef TINYSPLINE_H
#define	TINYSPLINE_H

#include <stddef.h>
#include <ossia_export.h>
#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************
*                                                                             *
* System Dependent Configuration                                              *
*                                                                             *
* The following configuration values must be adjusted to your system. Some of *
* them may be configured using preprocessor definitions. The default values   *
* should be fine for most modern hardware, such as x86, x86_64, and arm.      *
*                                                                             *
******************************************************************************/
#ifdef TINYSPLINE_DOUBLE_PRECISION
typedef double tsReal;
#else
typedef float tsReal;
#endif

#define FLT_MAX_ABS_ERROR 1e-5
#define FLT_MAX_REL_ERROR 1e-8



/******************************************************************************
*                                                                             *
* Data Types                                                                  *
*                                                                             *
* The following section defines all data types available in TinySpline.       *
*                                                                             *
******************************************************************************/
/**
 * Contains all error codes used by TinySpline. The following code snippet
 * shows how to handle errors:
 *
 *      tsError err = ...       // any function call here
 *      if (err < 0) {          // or use err != TS_SUCCESS
 *          printf("we got an error!");
 *
 *          // you may want to reuse error codes
 *          return err;
 *      }
 */
typedef enum
{
  /* No error. */
  TS_SUCCESS = 0,

  /* Unable to allocate memory (using malloc/realloc). */
  TS_MALLOC = -1,

  /* The dimension of the control points are 0. */
  TS_DIM_ZERO = -2,

  /* Degree of spline (deg) >= number of control points (n_ctrlp). */
  TS_DEG_GE_NCTRLP = -3,

  /* Spline is not defined at knot value u. */
  TS_U_UNDEFINED = -4,

  /* Multiplicity of a knot (s) > order of spline.  */
  TS_MULTIPLICITY = -5,

  /* Decreasing knot vector. */
  TS_KNOTS_DECR = -6,

  /* Unexpected number of knots. */
  TS_NUM_KNOTS = -7,

  /* Spline is not derivable */
  TS_UNDERIVABLE = -8
} tsError;

/**
 * Describes how the knot vector of a spline is organized. If you don't know
 * what an opened or clamped spline is, have a look at:
 *
 *     www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html
 */
typedef enum
{
  /* Not available/Undefined. */
  TS_NONE = 0,

  /* Uniformly spaced knot vector. */
  TS_OPENED = 1,

  /* Uniformly spaced knot vector with clamped end knots. */
  TS_CLAMPED = 2,

  /* Uniformly spaced knot vector with s(u) = order of spline. */
  TS_BEZIERS = 3
} tsBSplineType;

/**
 * Represents a B-Spline which may also be used for NURBS, Bezier curves,
 * lines, and points. NURBS are represented using homogeneous coordinates where
 * the last component of a control point is its weight. Bezier curves are
 * B-Splines with 'n_ctrlp == order' and clamped knot vector making the curve
 * passing through the first and last control point. If a Bezier curve consists
 * of two control points only, we call them a line. Points, ultimately, are
 * just very short lines having only a single control point. Consequently, the
 * degree of a point is zero.
 *
 * Two dimensional control points are organized as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional control points are organized as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on. NURBS are represented using homogeneous coordinates. For
 * instance, let's say we have a NURBS in 2D consisting of 11 control points
 * where 'w_i' is the weight of the i'th control point. Then the corresponding
 * control points are organized as follows:
 *
 *     [x_0, y_0, w_0, x_1, y_1, w_1, ..., x_10, y_10, w_10]
 *
 * Note: The fields 'ctrlp' and 'knots' share the same array (similar to the
 *       approach used in 'tsDeBoorNet'). That is, the first elements of this
 *       array contain the control points of a spline whereas the last elements
 *       contain its knots. Accordingly, you should never free 'knots'
 *       explicitly. Using 'ts_bspline_free()' to free dynamically allocated
 *       memory is to be preferred anyway. If 'ctrlp' and 'knots' do not share
 *       the same array, or at least a consistent block of data, functions
 *       provided by TinySpline my fail because values are copied block wise.
 */
typedef struct
{
  /* Degree of B-Spline basis function. */
  size_t deg;

  /* A convenience field for deg+1. */
  size_t order;

  /* Dimension of a control points. */
  size_t dim;

  /* Number of control points. */
  size_t n_ctrlp;

  /* Number of knots (n_ctrlp + deg + 1). */
  size_t n_knots;

  /* Control points of a spline. */
  tsReal *ctrlp;

  /* Knot vector of a spline (ascending order). */
  tsReal *knots;
} tsBSpline;

/**
 * Represents the output of De Boor's algorithm. De Boor's algorithm is used to
 * evaluate a spline at given knot value 'u' by iteratively computing a net of
 * intermediate values until the result is available:
 *
 *     https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
 *     https://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/de-Boor.html
 *
 * All points of the net are stored in 'points'. The resulting point of an
 * evaluation is the last point in 'points' and, for the sake of convenience,
 * may be accessed using 'result':
 *
 *     tsDeBoorNet net = ...    // evaluate an arbitrary spline and store
 *                              // resulting net of points in 'net'
 *
 *     net.result ...           // use 'result' to access resulting point
 *
 * Note: You should never free 'result' explicitly as it is just a convenient
 *       accessor for the last point in 'points'. Using 'ts_deboornet_free()'
 *       to free dynamically allocated memory is to be preferred anyway.
 *
 * Two dimensional points are organized as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional points are organized as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on.
 *
 * There is a special case in which the evaluation of a knot value 'u' returns
 * two instead of one result. It occurs when the multiplicity of 'u' ( s(u) )
 * is equals to a spline's order indicating that the spline is discontinuous at
 * 'u'. This is common practice for B-Splines (or NURBS) consisting of
 * connected Bezier curves where the endpoint of curve 'c_i' is equals to the
 * start point of curve 'c_i+1'. The end point of 'c_i' and the start point of
 * 'c_i+1' may still be completely different though, yielding to a spline
 * having a (real and visible) gap at 'u'. Consequently, De Boor's algorithm
 * must return two results if 's(u) == order' in order to give you access to
 * the desired points.  In such case, 'points' stores only the two resulting
 * points (there is no net to create) and 'result' points to the *first* point
 * in 'points' ('points' and 'result' store the same pointer). Since having
 * (real) gaps in splines is unusual, both points in 'points' are generally
 * equals making it easy to handle this special case by accessing 'result' as
 * already shown above for regular cases:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline which is discontinuous at
 *                              // at given knot value yielding to a net with
 *                              // two results
 *
 *     net.result ...           // use 'result' to access resulting point
 *
 * However, you can use both points if necessary:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline which is discontinuous at
 *                              // at given knot value yielding to a net with
 *                              // two results
 *
 *     net.result[0] ...        // 'result[0]' stores the first component of
 *                              // the first point
 *
 *     net.result[net.dim]      // 'result[net.dim]' stores the first component
 *                              // of the second point
 *
 * As if this wasn't complicated enough, there is an exception for our special
 * case yielding to exactly one result (just like the regular case) even if
 * 's(u) == order'. It occurs when 'u' is the lower or upper bound of a
 * spline's domain. For instance, if 'b' is a spline with domain [0, 1] and is
 * evaluated at 'u = 0' or 'u = 1' then 'result' is *always* a single point
 * regardless of the multiplicity of 'u'. Hence, handling this exception is
 * straightforward:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline at lower or upper bound of
 *                              // its domain, for instance, 0 or 1
 *
 *     net.result ...           // use 'result' to access resulting point
 *
 * In summary, we have three different types of evaluation. 1) The regular case
 * returning all points of the net we used to calculate the resulting point. 2)
 * The special case returning exactly two points which is required for splines
 * having (real) gaps. 3) The exception of 2) returning exactly one point even
 * if 's(u) == order'. All in all this looks quite complex (and actually it is)
 * but for most applications you don't need to bother with them. Just use
 * 'result' to access your evaluation point.
 */
typedef struct
{
  /* The evaluated knot value. */
  tsReal u;

  /* The index [u_k, u_k+1) */
  size_t k;

  /* Multiplicity of u_k. */
  size_t s;

  /* How many times u must be inserted to get the resulting point. */
  size_t h;

  /* Dimension of a control point. */
  size_t dim;

  /* Number of points in 'points'. */
  size_t n_points;

  /* Points of the net used to evaluate u_k. */
  tsReal *points;

  /* A convenient pointer to the result in 'points'. */
  tsReal *result;
} tsDeBoorNet;



/******************************************************************************
*                                                                             *
* Constructors, Destructors, Copy, and Move Functions                         *
*                                                                             *
* The following section contains all functions used to create and delete      *
* instances of the data types listed above. Additionally, each data type has  *
* a copy and move function.                                                   *
*                                                                             *
******************************************************************************/
/**
 * The default constructor of tsBSpline.
 *
 * All values of \bspline are set to 0/NULL.
 */
OSSIA_EXPORT
void ts_bspline_default(tsBSpline *bspline);

/**
 * The copy constructor of tsBSpline.
 *
 * Creates a deep copy of \original and stores the result in \copy. This
 * function does not free already allocated memory in \copy. If you want to
 * reuse an instance of tsBSpline by using it in multiple calls of this
 * function, make sure to call ::ts_bspline_free beforehand.
 *
 * On error all values of \copy are 0/NULL. The function does nothing if
 * \original == \result
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if allocating memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_copy(
  const tsBSpline *original,
  tsBSpline *copy
);

/**
 * The move constructor of tsBSpline.
 *
 * Moves all values from \from to \to and calls ::ts_bspline_default on \from
 * afterwards. Does nothing if \from == \to.
 */
OSSIA_EXPORT
void ts_bspline_move(tsBSpline *from, tsBSpline *to);

/**
 * A convenient constructor for tsBSpline.
 *
 * Create a new spline of degree \deg with dimension \dim and \n_ctrlp
 * many control points. This function automatically calculates the necessary
 * number of knots and fills the knot vector according to \type.
 *
 * On error all values of \bspline are 0/NULL.
 *
 * @return TS_SUCCESS          on success.
 * @return TS_DIM_ZERO         if \deg == 0.
 * @return TS_DEG_GE_NCTRLP    if \deg >= \n_ctrlp.
 * @return TS_NUM_KNOTS        if \type == TS_BEZIERS and \n_ctrlp % \deg+1 != 0
 * @return TS_MALLOC           if allocating memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_new(
  const size_t deg, const size_t dim, const size_t n_ctrlp,
  const tsBSplineType type,
  tsBSpline *bspline
);

/**
 * Performs a cubic spline interpolation using thomas algorithm.
 * https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
 * http://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf
 * http://www.bakoma-tex.com/doc/generic/pst-bspline/pst-bspline-doc.pdf
 *
 * The resulting spline is a sequence of bezier curves connecting each point in
 * \points. Each bezier curve is of degree 3 with dimension \dim. The total
 * number of control points is (n-1)*4.
 *
 * This function does not free already allocated memory in \bspline.
 * If you want to reuse an instance of tsBSpline by using it in multiple
 * calls of this function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error all values of \bspline are 0/NULL.
 *
 * Note: \n is the number of points in \points and not the length of \points.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_DIM_ZERO          if \dim == 0.
 * @return TS_DEG_GE_NCTRLP     if \n < 2.
 * @return TS_MALLOC            if allocating memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_interpolate_cubic(
  const tsReal *points, const size_t n, const size_t dim,
  tsBSpline *bspline
);

/**
 * The destructor of tsBSpline.
 *
 * Frees all dynamically allocated memory and calls ::ts_deboornet_free
 * afterwards.
 */
OSSIA_EXPORT
void ts_bspline_free(tsBSpline *bspline);

/**
 * The default constructor of tsDeBoorNet.
 *
 * All values of \deBoorNet are set to 0/NULL.
 */
OSSIA_EXPORT
void ts_deboornet_default(tsDeBoorNet *deBoorNet);

/**
 * The copy constructor of tsDeBoorNet.
 *
 * Creates a deep copy of \original and stores the result in \copy. This
 * function does not free already allocated memory in \copy. If you want to
 * reuse an instance of tsDeBoorNet by using it in multiple calls of this
 * function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error all values of \copy are 0/NULL. The function does nothing if
 * \original == \result
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if allocating memory failed.
 */
OSSIA_EXPORT
tsError ts_deboornet_copy(
  const tsDeBoorNet *original,
  tsDeBoorNet *copy
);

/**
 * The move constructor of tsDeBoorNet.
 *
 * Moves all values from \from to \to and calls ::ts_deboornet_default on
 * \from afterwards. Does nothing if \from == \to.
 */
OSSIA_EXPORT
void ts_deboornet_move(tsDeBoorNet *from, tsDeBoorNet *to);

/**
 * Evaluates \bspline at knot value \u and stores result in \deBoorNet.
 *
 * This function does not free already allocated memory in \deBoorNet.
 * If you want to reuse an instance of tsDeBoorNet by using it in multiple
 * calls of this function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error all values of \deBoorNet are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if allocating memory failed.
 * @return TS_MULTIPLICITY      if multiplicity of \u > order of \bspline.
 * @return TS_U_UNDEFINED       if \bspline is not defined at \u.
 */
OSSIA_EXPORT
tsError ts_bspline_evaluate(
  const tsBSpline *bspline, const tsReal u,
  tsDeBoorNet *deBoorNet
);

/**
 * The destructor of tsDeBoorNet.
 *
 * Frees all dynamically allocated memory and calls ::ts_deboornet_default
 * afterwards.
 */
OSSIA_EXPORT
void ts_deboornet_free(tsDeBoorNet *deBoorNet);



/******************************************************************************
*                                                                             *
* Transformation functions                                                    *
*                                                                             *
* TinySpline is a library focusing on transformations. That is, most          *
* functions are used to transform splines by modifying their state, e.g.,     *
* their number of control points, their degree, and so on. Accordingly, each  *
* transformation functions specifies an input and output parameter (along     *
* with the other parameters required to calculate the actual transformation). *
* By passing a different pointer to the output parameter, the transformation  *
* result is calculated and stored without changing the state of the input.    *
* This is in particular useful when dealing with errors as the original state *
* will never be modified. For instance, let's have a look at the following    *
* code snippet:                                                               *
*                                                                             *
*     tsBSpline in = ...    // an arbitrary spline                            *
*     tsBSpline out;        // result of transformation                       *
*                                                                             *
*     // Subdivide 'in' into sequence of bezier curves and store the result   *
*     // in 'out'. Does not change 'in' in any way.                           *
*     tsError err = ts_bspline_to_beziers(&in, &out);                         *
*     if (err != TS_SUCCESS) {                                                *
*         // fortunately, 'in' has not been changed                           *
*     }                                                                       *
*                                                                             *
* Even if 'ts_bspline_to_beziers' fails, the state of 'in' has not been       *
* changed allowing you to handle the error properly.                          *
*                                                                             *
* Unless stated otherwise, the order of the parameters for transformation     *
* functions is:                                                               *
*                                                                             *
*     function(input, [additional_input], output, [additional_output])        *
*                                                                             *
* 'additional_input' are parameters required to calculate the actual          *
* transformation. 'additional_output' are parameters to store further         *
* results.                                                                    *
*                                                                             *
* Note: None of TinySpline's transformation functions frees the memory of the *
*       output parameter. Thus, when using the same output parameter multiple *
*       times, make sure to free memory before each call. Otherwise, you will *
*       have a bad time with memory leaks:                                    *
*                                                                             *
*     tsBSpline in = ...                  // an arbitrary spline              *
*     tsBSpline out;                      // result of transformations        *
*                                                                             *
*     ts_bspline_to_beziers(&in, &out);   // first transformation             *
*     ...                                 // some code                        *
*     ts_bspline_free(&out);              // avoid memory leak.               *
*     ts_bspline_buckle(&in, &out);       // next transformation              *
*                                                                             *
* If you want to modify your input directly without having a separate output  *
* just pass it as input and output at once:                                   *
*                                                                             *
*     tsBSpline s = ...                       // an arbitrary spline          *
*     tsReal *knots = ...                     // a knot vector                *
*                                                                             *
*     ts_bspline_set_knots(&s, knots, &s);    // copy 'knots' into 's'        *
*                                                                             *
* Note: If a transformation function fails *and* input != output, all fields  *
*       of the output parameter are set to 0/NULL. If input == output, your   *
*       input may have an invalid state in case of errors.                    *
*                                                                             *
******************************************************************************/
/**
 * Computes the derivative of \original.
 * http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-derv.html
 *
 * The derivative of a spline \original of degree \original->deg with
 * \original->n_ctrlp control points and \original->n_knots knots
 * is another spline of degree \original->deg-1 with \original->n_ctrl-1
 * control points and \original->n_knots-2 knots, defined over \original as
 * C'(u) = Sum_{i=0}^{n-1} N_{i+1,p-1}(u) * (P_{i+1} - P_{i}) * p / (u_{i+p+1}-u_{i+1})
 *       = Sum_{i=1}^{n} N_{i,p-1}(u) * (P_{i} - P_{i-1}) * p / (u_{i+p}-u_{i})
 * If \original is clamped, it can be shown that
 * C'(u) = Sum_{i=0}^{n-1} N_{i,p-1}(u) * (P_{i+1} - P_{i}) * p / (u_{i+p+1}-u_{i+1})
 * where the multiplicity of the first and the last u is p rather than p+1.
 *
 * This function does not free already allocated memory in \derivative.
 * If you want to reuse an instance of tsBSpline by using it in multiple
 * calls of this function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error (and if \original != \derivative) all values of \derivative are
 * 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if allocating memory failed.
 * @return TS_UNDERIVABLE       if \original->deg < 1 or
 *                              if \original->n_ctrlp < 2 or
 *                              if the multiplicity of an internal knot of
 *                              \original is greater than the degree of
 *                              \original. NOTE: This will be fixed in the
 *                              future.
 */
OSSIA_EXPORT
tsError ts_bspline_derive(
  const tsBSpline *original,
  tsBSpline *derivative
);

/**
 * Creates a deep copy of \bspline (only if \bspline != \result) and copies the
 * first bspline->n_ctrlp * bspline->dim control points from \ctrlp to \result
 * using memmove. The behaviour of this function is undefined if the length of
 * \ctrlp is less than bspline->n_ctrlp * bspline->dim.
 *
 * On error (and if \bspline != \result) all values of \result are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \bspline != \result and allocating
 *                              memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_set_ctrlp(
  const tsBSpline *bspline, const tsReal *ctrlp,
  tsBSpline *result
);

/**
 * Creates a deep copy of \bspline (only if \bspline != \result) and copies the
 * the first bspline->n_knots from \knots to \result using memmove. The
 * behaviour of this function is undefined if the length of \knots is less than
 * bspline->n_knots.
 *
 * On error (and if \bspline != \result) all values of \result are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \bspline != \result and allocating
 *                              memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_set_knots(
  const tsBSpline *bspline, const tsReal *knots,
  tsBSpline *result
);

/**
 * Fills the knot vector of \original according to \type with minimum knot
 * value \min to maximum knot value \max and stores the result in \result.
 *
 * This function creates a deep copy of \original, if \original != \result.
 *
 * On error all values of \result are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \original != \result and allocating
 *                              memory failed.
 * @return TS_DEG_GE_NCTRLP     if \original->n_knots < 2*(\original->deg+1).
 * (We can reuse the error code TS_DEG_GE_NCTRLP because n_knots < 2*(deg+1)
 * implies deg >= n_ctrlp. Furthermore, using TS_DEG_GE_NCTRLP instead of
 * TS_NUM_KNOTS ensures that TS_NUM_KNOTS is not used twice for this function.
 * To be more fail-safe, \original->deg+1 instead of \original->order is used,
 * to make sure that \original->deg+1 >= 1)
 *
 * @return TS_NUM_KNOTS         if \type == TS_BEZIERS and n_knots % order != 0
 * @return TS_KNOTS_DECR        if \min >= \max.
 * (The function uses ::ts_fequals in order to determine if \min == \max)
 */
OSSIA_EXPORT
tsError ts_bspline_fill_knots(
  const tsBSpline *original, const tsBSplineType type, const tsReal min,
  const tsReal max,
  tsBSpline *result
);
OSSIA_EXPORT
tsError ts_bspline_insert_knot(
  const tsBSpline *bspline, const tsReal u, const size_t n,
  tsBSpline *result, size_t *k
);

/**
 * Resizes \bspline by \n (number of control points) and stores the result in
 * \resized. If \back != 0 the resulting splines is resized at the end. If
 * \back == 0 the resulting spline is resized at the front.
 *
 * Resizing a spline at the end is usually faster, because the current values
 * don't need to be copied.
 *
 * This function creates a deep copy of \bspline, if \bspline != \resized.
 *
 * On error (and if \bspline != \resized) all values of \resized are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_DEG_GE_NCTRLP     if the degree of \resized would be >= the number
 *                              of the control points of \resized.
 * @return TS_OVER_UNDERFLOW    if adding \n to the current length of the
 *                              control point/knot array would cause a
 *                              over/underflow.
 * @return TS_MALLOC            if (re)allocating memory failed.
 *
 * if \bspline != \resized:
 *      @return TS_DIM_ZERO         if the degree of \bspline == 0.
 *      @return TS_DEG_GE_NCTRLP    if the degree of \bspline >= the number of
 *                                  the control points of \bspline.
 *      @return TS_MALLOC           if allocating memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_resize(
  const tsBSpline *bspline, const int n, const int back,
  tsBSpline *resized
);
OSSIA_EXPORT
tsError ts_bspline_split(
  const tsBSpline *bspline, const tsReal u,
  tsBSpline *split, size_t *k
);

/**
 * Buckles \original by \b and stores the result in \buckled.
 *
 * This function is based on:
 *      Holten, Danny. "Hierarchical edge bundles: Visualization of adjacency
 *      relations in hierarchical data." Visualization and Computer Graphics,
 *      IEEE Transactions on 12.5 (2006): 741-748.
 * Holten calls it "straightening" (page 744, equation 1).
 *
 * Usually the range of \b is: 0 <= \b <= 1, where 0 results in a line between
 * the first and the last control point (no buckle) and 1 keeps the original
 * shape (maximum buckle). If \b < 0 or \b > 1 the behaviour is undefined,
 * though it will not result in an error.
 *
 * This function creates a deep copy of \original, if \original != \buckled
 * and will never fail if \original == \buckled (always returns TS_SUCCESS).
 *
 * On error all values of \buckled are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \original != \buckled and allocating
 *                              memory failed.
 */
OSSIA_EXPORT
tsError ts_bspline_buckle(
  const tsBSpline *original, const tsReal b,
  tsBSpline *buckled
);
OSSIA_EXPORT
tsError ts_bspline_to_beziers(
  const tsBSpline *bspline,
  tsBSpline *beziers
);



/******************************************************************************
*                                                                             *
* Utility Functions                                                           *
*                                                                             *
* The following section contains utility functions used by TinySpline which   *
* also may be helpful when working with this library.                         *
*                                                                             *
******************************************************************************/
/**
 * Compares the tsReal values \x and \y by using absolute and relative
 * epsilon.
 *
 * @return 1    if \x is equals to \y.
 * @return 0    otherwise.
 */
OSSIA_EXPORT
int ts_fequals(const tsReal x, const tsReal y);

/**
 * Returns the error message associated to \err. Returns "unknown error" if
 * \err is not associated (indicating a bug) or is TS_SUCCESS (which is not an
 * actual error).
 */
OSSIA_EXPORT
const char* ts_enum_str(const tsError err);

/**
 * Returns the error code associated to \str or TS_SUCCESS if \str is not
 * associated. Keep in mind that by concept "unknown error" is not associated,
 * though, TS_SUCCESS is returned.
 */
OSSIA_EXPORT
tsError ts_str_enum(const char *str);

/**
 * Fills the given array \arr with \val from \arr+0 to \arr+\num (exclusive).
 */
void ts_arr_fill(tsReal *arr, const size_t num, const tsReal val);

/**
 * Returns the euclidean distance of \x and \y consisting of \dim components,
 * respectively.
 *
 * @return  the euclidean of \x and \y.
 */
tsReal ts_ctrlp_dist2(const tsReal *x, const tsReal *y, const size_t dim);



#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

