#pragma once
#ifndef __fnc__
#define __fnc__

#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include <vector>

//General Tools
namespace fnc {
	float aproach(float& v, float a, float d);
	float aproach(float&& v, float a, float d);

	float lerp(float s, float e, float p);

	typedef unsigned char	usmall;
	typedef unsigned short	ushort;
	typedef unsigned int	uint;

	typedef float			dec;
	typedef double			ldec;
	typedef const char*		text;

	template<typename ret, typename ...args>
	using func = ret(*)(args...);

	template<class c>
	class iSing {
	protected:
		static c* sInstance;
	public:
		static c* Instance() {
			if (sInstance == NULL)
				sInstance = new c();
			return sInstance;
		}
		static void Release() {
			if (sInstance == NULL)
				return;
			delete sInstance;
		}
	protected:
		iSing() {};
		virtual ~iSing() {};
	};
}

//Triggerers and Function Remote Activation Handling
namespace del {
// THIS CLASS IS BASED ON A SUGGESTION MADE BY THE USER: Dietmar Kühl
// MADE AS AN ANSWER, WHICH IS FINDABLE USING THIS LINK: https://stackoverflow.com/a/23974539/12241375

	struct EvArgs {
		EvArgs() {}
	};

	template <typename Signature>
	struct delegate;

	template <typename Args>
	struct delegate<void(Args*)>
	{
		struct base {
			virtual ~base() {}
			virtual void do_call(Args* args) = 0;
		};

		template <typename T>
		struct call : base {
			T d_callback;

			template <typename S>
			call(S&& callback) : d_callback(std::forward<S>(callback)) {}

			void do_call(Args* args) {
				this->d_callback(std::forward<Args*>(args));
				return;
			}
		};

		std::vector<std::unique_ptr<base>> d_callbacks;
		std::vector<std::unique_ptr<base>> d_tmp_callbacks;

		delegate(delegate const&) = delete;
		void operator=(delegate const&) = delete;
	public:
		delegate() {
			if (!std::is_base_of<EvArgs, Args>::value)
				throw "specified type is not derived class from EvArgs\n";
		}
		~delegate() {
			d_callbacks.clear();
			d_tmp_callbacks.clear();
		}

		template <typename T>
		delegate& operator+= (T&& callback) {
			this->d_callbacks.emplace_back(new call<T>(std::forward<T>(callback)));
			return *this;
		}

		template <typename T>
		delegate& operator<< (T&& callback) {
			this->d_tmp_callbacks.emplace_back(new call<T>(std::forward<T>(callback)));
			return *this;
		}
	};
	// ----------------------------------------------------------------------------
	template <typename RC, typename Class, typename Args>
	class member_call {
		Class* d_object;
		RC(Class::*d_member)(Args);
	public:
		member_call(Class* object, RC(Class::*member)(Args))
			: d_object(object)
			, d_member(member) {
		}

		RC operator()(Args args) {
			return (this->d_object->*this->d_member)(std::forward<Args>(args));
		}
	};

	template <typename Class, typename Args>
	member_call<void, Class, Args> mem_call(Class& object, void(Class::*member)(Args*)) {
		return member_call<void, Class, Args>(&object, member);
	}
	// ----------------------------------------------------------------------------

	template<typename Signature>
	struct action_delegate;

	template<typename Args>
	struct action_delegate<void(Args*)> : public delegate<void(Args*)>{

		delegate<void(Args*)>& getBase() {
			return *static_cast<delegate<void(Args*)>*>(this);
		}

		void operator()(Args* args) {
			for (auto& callback : this->d_callbacks)		callback->do_call(args);
			for (auto& callback : this->d_tmp_callbacks)	callback->do_call(args);
			this->d_tmp_callbacks.clear();

			delete args;
		}
	};

	// ----------------------------------------------------------------------------
	template<typename Args>
	using securedEvent = action_delegate<void(Args*)>;

	template<typename Args>
	using event = delegate<void(Args*)>;
}

//Graphics Data holders
namespace vis {
	struct Color{
		fnc::usmall r, g, b;
		bool operator ==(const Color& c);
	};
}
//Figures
namespace fig {
	template<typename t>
	struct Point {
		t x, y;
		Point(t x = t(0), t y = t(0)) : x(x), y(y) {}

		Point(const Point<t>& o) {
			this->x = o.x;
			this->y = o.y;
		}

		friend Point<t>& operator +(Point<t>& a, const Point<t>&b);
		friend Point<t>& operator -(Point<t>& a, const Point<t>&b);

		friend Point<t>& operator *(Point<t>& a, t b);
		friend Point<t>& operator /(Point<t>& a, t b);

		friend Point<t>& operator |(Point<t>&a, const Point<t>&b);

		Point<t>& operator +=(const Point<t>& b);
		Point<t>& operator -=(const Point<t>& b);

		Point<t>& operator *=(t b);
		Point<t>& operator /=(t b);

		Point<t>& operator |=(const Point<t>& b);

		virtual bool operator ==(const Point<t>& b) {
			return (this->x == b.x && this->y == b.y);
		}
	};

	struct Vector2D : public Point<float>
	{
	public:
		Vector2D(float x = 0.0f, float y = 0.0f);

		template<typename t>
		Vector2D(const fig::Point<t>& point) {
			this->x = static_cast<float>(point.x);
			this->y = static_cast<float>(point.y);
		};

		std::string tostring() const;

		friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
		friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

		Vector2D& operator *=(const Vector2D& vec);
		Vector2D& operator /=(const Vector2D& vec);

		Vector2D& operator=(const Vector2D& vec);

		static float distance(const Vector2D& v1, const Vector2D& v2);
	private:
		Vector2D& eq(const Vector2D& vec);

		Vector2D& mul(const Vector2D& vec);
		Vector2D& div(const Vector2D& vec);

		Vector2D& eq(const float vec);
	};

	struct Figure abstract {
		Vector2D pos;

		Figure() = delete;

		Figure(Vector2D pos);
		Figure(float a, float b);

		Figure(const Figure&&) = delete;
		Figure(const Figure&);

		virtual ~Figure();
	};

	struct cSphere : public Figure {
		cSphere(Vector2D pos, float r);
		float r;

		Vector2D& avg(const cSphere& sph);
		std::string tostring();
	};

	struct cRect : public Figure {
		cRect(Vector2D pos, Vector2D siz);
		Vector2D siz;

		Vector2D center() const;
		std::string tostring() const;

		cRect& operator|=(const cRect&);
		friend cRect& operator | (cRect&, const cRect&);
	private:
		cRect& cross(const cRect&);
	};

	struct cTria : public Figure {
		//Vector2D * const points[3];

		cTria(Vector2D p1, Vector2D p2, Vector2D p3);
		cTria(cTria&);

		const cRect& getboundary() const;

		std::pair<cTria, cTria> subdivide(bool dir) const;

		void getPoints(Vector2D** dst);
		void getPoints(Vector2D* dst) const;
	private:
		Vector2D p2;
		Vector2D p3;

		cRect boundary{ {},{} };
	};
}

//Collision in 2D Space
namespace space {

	enum Complexity {
		cRect = 0,
		cSphere = 1,
	};

	struct Collision {
	public:
		fig::Vector2D intersection;
		fig::Vector2D transform;

		template<typename t1, typename t2>
		static std::unique_ptr<Collision> Check(const t1&, const t2&) = delete;

		template<>
		static std::unique_ptr<Collision> Check<fig::cRect, fig::cRect>(const fig::cRect& rA, const fig::cRect& rB) {
			if (!(
				rA.pos.x + rA.siz.x >= rB.pos.x && rA.pos.y + rA.siz.y >= rB.pos.y &&
				rB.pos.x + rB.siz.x >= rA.pos.x && rB.pos.y + rB.siz.y >= rA.pos.y
				)) return nullptr;

			std::unique_ptr<Collision> collision = std::make_unique<Collision>();

			float cdx = rA.center().x - rB.center().x;
			float cdy = rA.center().y - rB.center().y;

			fig::cRect rA_rB(rA);
			rA_rB |= rB;

			collision->intersection = rA_rB.center();

			if (std::abs(cdx) > std::abs(cdy))
				collision->transform.x = (std::signbit(cdx) ? -.5f : .5f)*(rA_rB.siz.x);
			else
				collision->transform.y = (std::signbit(cdy) ? -.5f : .5f)*(rA_rB.siz.y);

			return std::move(collision);
		}

		template<>
		static std::unique_ptr<Collision> Check<fig::cSphere, fig::cSphere>(const fig::cSphere& rA, const fig::cSphere& rB) {
			if (!(std::pow(rA.pos.x - rB.pos.x, 2) + std::pow(rA.pos.y - rB.pos.y, 2) < std::pow(rA.r + rB.r, 2)))
				return nullptr;

			std::unique_ptr<Collision> collision = std::make_unique<Collision>();

			float dist = fig::Vector2D::distance(rA.pos, rB.pos);
			float overlap = dist - rA.r - rB.r;

			collision->intersection = rA.pos;
			collision->intersection |= rB.pos;

			collision->transform.x = (rB.pos.x - rA.pos.x) * overlap * .5 / dist;
			collision->transform.y = (rB.pos.y - rA.pos.y) * overlap * .5 / dist;

			return std::move(collision);
		}

		template<>
		static std::unique_ptr<Collision> Check<fig::cRect, fig::cSphere>(const fig::cRect& rA, const fig::cSphere& sB) {
			fig::Vector2D closest_point{
				 std::fmaxf(rA.pos.x, std::fminf(rA.pos.x + rA.siz.x, sB.pos.x)),
				 std::fmaxf(rA.pos.y, std::fminf(rA.pos.y + rA.siz.y, sB.pos.y))
			};

			if (fig::Vector2D::distance(closest_point, { sB.pos.x, sB.pos.y }) > sB.r)
				return nullptr;

			std::unique_ptr<Collision> collision = std::make_unique<Collision>();

			float cdx = rA.center().x - sB.pos.x;
			float cdy = rA.center().y - sB.pos.y;

			if (std::abs(cdx) > std::abs(cdy))
				collision->transform.x = (std::signbit(cdx) ? -.5f : .5f)*(std::sqrt(std::pow(sB.r, 2) - std::pow(closest_point.y - sB.pos.y, 2)) - std::abs(closest_point.x - sB.pos.x));
			else
				collision->transform.y = (std::signbit(cdy) ? -.5f : .5f)*(std::sqrt(std::pow(sB.r, 2) - std::pow(closest_point.x - sB.pos.x, 2)) - std::abs(closest_point.y - sB.pos.y));

			return std::move(collision);
		}
	private:

	};
}

//control interfacez
namespace bac {
	class Transform abstract {
	protected:
	public:

		fig::Vector2D position;
		float angle{ 0.0f };
		fig::Vector2D scale{ 1.0f,1.0f };

		virtual fig::Vector2D	getPos()	const = 0;
		virtual fig::Vector2D	getScale()	const = 0;
		virtual float			getAngle()	const = 0;

		struct _local {
			fig::Vector2D& pos;
			fig::Vector2D& scale;
			float& angle;
		}local{ position, scale, angle };
	};

	class Collider abstract {
	protected:
		bac::Transform* transform;

		std::pair<space::Complexity, fig::Figure*> *figures;
		fnc::usmall nfigures;
	public:
		virtual fig::cRect getBoundary() = 0;
		virtual std::pair<space::Complexity, fig::Figure*>* getFigures() const = 0;
	};
}
#endif // !__fnc__

