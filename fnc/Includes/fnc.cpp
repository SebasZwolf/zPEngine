#include "fnc.h"
#include <math.h>

#include <iostream>

namespace {
	template<typename t>
	t min(t a, t b) {
		return a < b ? a : b;
	}
	template<typename t>
	t max(t a, t b) {
		return a > b ? a : b;
	}
}

namespace fnc {
	float aproach(float & v, float a, float d) {
		v = v < d ? min(d, v + a) : max(d, v - a);
		return v;
	}
	float aproach(float && v, float a, float d) {
		return v < d ? min(d, v + a) : max(d, v - a);
	}

	float lerp(float s, float e, float p) {
		return s + p * (e - s);
	}

}
namespace fig {

	template<typename t>
	Point<t>& operator +(Point<t>& a, const Point<t>&b) {
		a.x += b.x;
		a.y += b.y;
		return a;
	}
	template<typename t>
	Point<t>& operator -(Point<t>& a, const Point<t>&b) {
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}
	template<typename t>
	Point<t>& operator *(Point<t>& a, t b) {
		a.x *= b;
		a.y *= b;
		return a;
	}

	template<typename t>
	Point<t>& operator/(Point<t>& a, t b) {
		a.x /= b;
		a.y /= b;
		return a;
	}

	template<typename t>
	Point<t>& operator |(Point<t>& a, const Point<t>&b) {
		a.x = (a.x + b.x) / 2;
		a.y = (a.y + b.y) / 2;
		return a;
	}

	template<typename t>
	Point<t>& Point<t>::operator+=(const Point<t>& b) {
		this->x += b.x;
		this->y += b.y;
		return *this;
	}
	template<typename t>
	Point<t>& Point<t>::operator-=(const Point<t>& b) {
		this->x -= b.x;
		this->y -= b.y;
		return *this;
	}
	template<typename t>
	Point<t>& Point<t>::operator*=(t b) {
		this->x *= t;
		this->y *= t;
		return *this;
	}

	template<typename t>
	Point<t>& Point<t>::operator/=(t b) {
		this->x /= b;
		this->y /= b;
		return *this;
	}

	template<typename t>
	Point<t>& Point<t>::operator|=(const Point<t>& b) {
		this->x = (this->x + b.x) / 2;
		this->y = (this->x + b.y) / 2;
		return *this;
	}

	Vector2D::Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}
	std::string Vector2D::tostring() const
	{
		std::string a;
		a.reserve(9);
		a.push_back('<');
		a.append(std::to_string(this->x).substr(0, std::to_string(this->x).find(".") + 2 + 1));
		a.push_back(',');
		a.append(std::to_string(this->y).substr(0, std::to_string(this->y).find(".") + 2 + 1));
		a.push_back('>');
		return a;
	}

	Vector2D & operator*(Vector2D & v1, const Vector2D & v2) {
		return v1.mul(v2);
	}

	Vector2D & operator/(Vector2D & v1, const Vector2D & v2) {
		return v1.div(v2);
	}

	cRect & operator|(cRect & rA, const cRect & rB) {
		return rA.cross(rB);
	}

	Vector2D & Vector2D::operator=(const Vector2D & vec) {
		return this->eq(vec);
	}

	Vector2D & Vector2D::operator*=(const Vector2D & vec) {
		return this->mul(vec);
	}
	Vector2D & Vector2D::operator/=(const Vector2D & vec) {
		return this->div(vec);
	}


	float Vector2D::distance(const Vector2D & v1, const Vector2D & v2) {
		return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
	}

	//rvalue = v2d
	Vector2D & Vector2D::eq(const Vector2D & vec) {
		this->x = vec.x;
		this->y = vec.y;
		return *this;
	}
	/*Vector2D & Vector2D::add(const Vector2D & vec){
		this->x += vec.x;
		this->y += vec.y;
		return *this;
	}*/
	/*Vector2D & Vector2D::sub(const Vector2D & vec){
		this->x -= vec.x;
		this->y -= vec.y;
		return *this;
	}*/
	Vector2D & Vector2D::mul(const Vector2D & vec) {
		this->x *= vec.x;
		this->y *= vec.y;
		return *this;
	}
	Vector2D & Vector2D::div(const Vector2D & vec) {
		this->x /= vec.x;
		this->y /= vec.y;
		return *this;
	}
	/*Vector2D & Vector2D::avg(const Vector2D & vec) {
		this->x = (this->x + vec.x) / 2;
		this->y = (this->y + vec.y) / 2;
		return *this;
	}*/

	//rvalue = float
	Vector2D & Vector2D::eq(const float vec) {
		this->x += vec;	this->y += vec;
		return *this;
	}
	/*Vector2D & Vector2D::mul(const float vec) {
		this->x *= vec;	this->y *= vec;
		return *this;
	}*/


	Figure::Figure(Vector2D pos) : pos(pos) {}

	fig::Figure::Figure(float a, float b) : pos({ a,b }) {}

	Figure::Figure(const Figure & f) : pos(f.pos) {}

	Figure::~Figure() {}


	cSphere::cSphere(Vector2D pos, float r) : Figure(pos) {
		this->r = r;
	}

	Vector2D & cSphere::avg(const cSphere & sph) {
		this->pos.x = (this->pos.x + sph.pos.x) / 2;
		this->pos.y = (this->pos.y + sph.pos.y) / 2;
		return this->pos;
	}

	std::string cSphere::tostring() {
		std::string a = "";
		a.push_back('[');
		a.append(this->pos.tostring());
		a.push_back(',');
		a.push_back('(');
		a.append(std::to_string(this->r).substr(0, std::to_string(this->r).find(".") + 2 + 1));
		a.push_back(')');
		a.push_back(']');
		return a;
	}

	cRect::cRect(Vector2D pos, Vector2D siz) : Figure(pos), siz(siz) {
	}

	Vector2D cRect::center() const {
		Vector2D a{ pos.x + siz.x / 2, pos.y + siz.y / 2 };
		return a;
	}

	std::string cRect::tostring() const {
		std::string a = "";
		a.push_back('[');
		a.append(this->pos.tostring());
		a.push_back(',');
		a.append(this->siz.tostring());
		a.push_back(']');
		return a;
	}

	cRect & cRect::operator|=(const cRect & rB) {
		return this->cross(rB);
	}

	cRect & cRect::cross(const cRect& rB) {
		this->pos = {
			max<float>(pos.x, rB.pos.x),
			max<float>(pos.y, rB.pos.y)
		};
		this->siz = {
			min<float>(pos.x + siz.x, rB.pos.x + rB.siz.x) - this->pos.x,
			min<float>(pos.y + siz.y, rB.pos.y + rB.siz.y) - this->pos.y
		};
		return *this;
	}

	cTria::cTria(Vector2D _p1, Vector2D _p2, Vector2D _p3) : Figure(_p1) {
		pos = _p1;
		this->p2 = p2;
		this->p3 = p3;

		boundary.pos.x = min(_p1.x, min(_p2.x, _p3.x));
		boundary.pos.y = min(_p1.y, min(_p2.y, _p3.y));
		boundary.siz.x = max(_p1.x, max(_p2.x, _p3.x)) - boundary.pos.x;
		boundary.siz.y = max(_p1.y, max(_p2.y, _p3.y)) - boundary.pos.y;
	}
	cTria::cTria(cTria & a) : Figure(a.pos) {
		this->pos = a.pos;
		this->p2 = a.p2;
		this->p3 = a.p3;

		boundary.pos.x = min(pos.x, min(p2.x, p3.x));
		boundary.pos.y = min(pos.y, min(p2.y, p3.y));
		boundary.siz.x = max(pos.x, max(p2.x, p3.x)) - boundary.pos.x;
		boundary.siz.y = max(pos.y, max(p2.y, p3.y)) - boundary.pos.y;
	}
	const fig::cRect& cTria::getboundary() const {
		return boundary;
	}
	std::pair<cTria, cTria> cTria::subdivide(bool dir = true) const {
		cTria t1{ {},{},{} };
		cTria t2{ {},{},{} };
		Vector2D np;
		fnc::usmall chosen;

		Vector2D points[3];
		getPoints(points);

		if (dir) {
			//HORIZONAL DIV
			/*
			for (fnc::usmall i = 0; i < 3; i++) {
				if (std::signbit(points[i]->x - points[(i + 1) % 3]->x) != std::signbit(points[i]->x - points[(i + 2) % 3]->x)) {
					chosen = i;
					np = {
						points[i]->x,
						fnc::lerp(
							points[(i + 1) % 3]->y,
							points[(i + 2) % 3]->y,
							(points[(i) % 3]->x - 1.0f * points[(i + 1) % 3]->x) / (points[(i + 2) % 3]->x - points[(i + 1) % 3]->x)
						)
					};
					t1.pos = *points[i];
					t1.p2 = *points[(i + 1) % 3];
					t1.p3 = np;

					t2.pos = *points[i];
					t2.p2 = *points[(i + 2) % 3];
					t2.p3 = np;
					break;
				}
			}
			*/
			for (fnc::usmall i = 0; i < 3; i++) {
				if (std::signbit(points[i].x - points[(i + 1) % 3].x) != std::signbit(points[i].x - points[(i + 2) % 3].x)) {
					chosen = i;
					np = {
						points[i].x,
						fnc::lerp(
							points[(i + 1) % 3].y,
							points[(i + 2) % 3].y,
							(points[(i) % 3].x - 1.0f * points[(i + 1) % 3].x) / (points[(i + 2) % 3].x - points[(i + 1) % 3].x)
						)
					};
					t1.pos = points[i];
					t1.p2 = points[(i + 1) % 3];
					t1.p3 = np;

					t2.pos = points[i];
					t2.p2 = points[(i + 2) % 3];
					t2.p3 = np;
					break;
				}
			}
		}
		else {
			//VERTICAL DIV
			/*for (fnc::usmall i = 0; i < 3; i++) {
				if (std::signbit(points[i]->y - points[(i + 1) % 3]->y) != std::signbit(points[i]->y - points[(i + 2) % 3]->y)) {
					np = {
						fnc::lerp(
							points[(i + 1) % 3]->x,
							points[(i + 2) % 3]->x,
							(points[(i) % 3]->y - 1.0f * points[(i + 1) % 3]->y) / (points[(i + 2) % 3]->y - points[(i + 1) % 3]->y)
						),
						points[i]->y
					};
					t1.pos = *points[i];
					t1.p2 = *points[(i + 1) % 3];
					t1.p3 = np;

					t2.pos = *points[i];
					t2.p2 = *points[(i + 2) % 3];
					t2.p3 = np;
					break;
				}
			}*/
			for (fnc::usmall i = 0; i < 3; i++) {
				if (std::signbit(points[i].y - points[(i + 1) % 3].y) != std::signbit(points[i].y - points[(i + 2) % 3].y)) {
					np = {
						fnc::lerp(
							points[(i + 1) % 3].x,
							points[(i + 2) % 3].x,
							(points[(i) % 3].y - 1.0f * points[(i + 1) % 3].y) / (points[(i + 2) % 3].y - points[(i + 1) % 3].y)
						),
						points[i].y
					};
					t1.pos = points[i];
					t1.p2 = points[(i + 1) % 3];
					t1.p3 = np;

					t2.pos = points[i];
					t2.p2 = points[(i + 2) % 3];
					t2.p3 = np;
					break;
				}
			}
		}
		return { t1, t2 };
	}
	void cTria::getPoints(Vector2D ** dst) {
		dst[0] = &pos;
		dst[1] = &p2;
		dst[2] = &p3;
	}
	void cTria::getPoints(Vector2D * dst) const {
		dst[0] = pos;
		dst[1] = p2;
		dst[2] = p3;
	}
}