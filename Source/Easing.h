/*
  ==============================================================================

    Easing.h
    Created: 11 Dec 2016 1:29:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef EASING_H_INCLUDED
#define EASING_H_INCLUDED

#include "ControllableContainer.h"

class EasingUI;

class Easing :
	public ControllableContainer
{
public:
	enum Type { LINEAR, BEZIER, HOLD };

	Easing(Type type);
	virtual ~Easing();
	
	Type type;
	virtual float getValue(const float &start, const float &end, const float &weight) = 0;//must be overriden

};


class LinearEasing :
	public Easing
{
public:
	LinearEasing() : Easing(LINEAR) {}

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		return start + (end - start)*weight;
	}

};

class HoldEasing :
	public Easing
{
public:
	HoldEasing() : Easing(HOLD) {}

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		if (weight >= 1) return end;
		return start;
	}
};

class CubicEasing :
	public Easing
{
public:
	CubicEasing();
	Point2DParameter * anchor1;
	Point2DParameter * anchor2;

	virtual float getValue(const float &start, const float &end, const float &weight) override
	{
		if (weight <= 0) return start;
		if (weight >= 1) return end;

		//Point<float> a1 = anchor1->getPoint();
		//Point<float> a2 = anchor2->getPoint();
		//a1.y = jmap<float>(a1.y, start, end);
		//a2.y = jmap<float>(a2.y, start, end);

		float v1 = start;
		float v2 = end;
		float vx1 = jmap<float>(anchor1->getPoint().x, start, end);
		float vx2 = jmap<float>(anchor2->getPoint().x, start, end);

		float a = v2 + 3 * (vx1 - vx2) - v1;
		float b = 3 * (v1 - 2 * vx1 + vx2);
		float c = 3 * (vx1 - v1);
		float d = v1;

		Array<float> result = solveCubic(a, b, c, -weight * (v2 - v1));

		if (result.size() < 1) return start;
		float realRatio  = result[0];

		
		float vy1 = v1;
		float vy2 = v2;

		float a2 = v2 + 3 * (vy1 - vy2) - v1;
		float b2 = 3 * (v1 - 2 * vy1 + vy2);
		float c2 = 3 * (vy1 - v1);
		float d2 = v1;

		float val = a2 * powf(realRatio, 3) + b2 * powf(realRatio, 2) + c2 * realRatio + d2;

		return val;
	}

	Array<float> solveCubic(float a, float b, float c, float d)
	{
		float u;
		float D;
		
		Array<float> result;

		if (abs(a) < 1e-8) { // Quadratic case, ax^2+bx+c=0
			a = b; b = c; c = d;
			if (abs(a) < 1e-8) { // Linear case, ax+b=0
				a = b; b = c;

				if (abs(a) < 1e-8)
				{
					// Degenerate case
					return result;
				}

				result.add(-b / a);
				return result;
			}

			D = b*b - 4 * a*c;
			if (abs(D) < 1e-8)
			{
				result.add(-b / (2 * a));
				return result;
			}
			else if (D > 0)
			{
				result.add((-b + sqrtf(D)) / (2 * a));
				result.add((-b - sqrtf(D)) / (2 * a));
				return result;
			}
				
			return result;
		}

		// Convert to depressed cubic t^3+pt+q = 0 (subst x = t - b/3a)
		float p = (3 * a * c - b * b) / (3 * a * a);
		float q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);


		if (abs(p) < 1e-8) { // p = 0 -> t^3 = -q -> t = -q^1/3
			result.add(cbrtf(-q));

		}
		else if (abs(q) < 1e-8) { // q = 0 -> t^3 + pt = 0 -> t(t^2+p)=0
			result.add(0);
			if (p < 0)
			{
				result.add(sqrtf(-p));
				result.add(sqrtf(-p));
			}

		}
		else {
			D = q * q / 4 + p * p * p / 27;

			if (abs(D) < 1e-8) {       // D = 0 -> two roots
				result.add(-1.5 * q / p);
				result.add(3 * q / p);

			}
			else if (D > 0) {             // Only one real root
				u = cbrtf(-q / 2 - sqrt(D));

				result.add(u - p / (3 * u));

			}
			else {                        // D < 0, three roots, but needs to use complex numbers/trigonometric solution
				u = 2 * sqrt(-p / 3);
				float t = acosf(3 * q / p / u) / 3;  // D < 0 implies p < 0 and acos argument in [-1..1]
				float k = 2 * float_Pi / 3;
				result.add(u*cosf(t));
				result.add(u*cosf(t - k));
				result.add(u*cosf(t - 2 * k));
			}
		}

		// Convert back from depressed cubic
		for (int i = 0; i < result.size(); i++)
		{
			result.set(i, result[i] - b / (3 * a));
		}

		return result;
	}
};



#endif  // EASING_H_INCLUDED
