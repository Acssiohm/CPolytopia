#pragma once
#include <iostream>
#include <tuple>
template < typename T >
class Vec2 {
public:
	constexpr Vec2(T x = 0, T y = 0): m_x(x), m_y(y) {}
	template <typename T2>
	constexpr Vec2(const Vec2<T2> v) : m_x((T)v.x()), m_y((T)v.y()) {}

	T x() const {
		return m_x;
	}
	T y() const {
		return m_y;
	}
	std::tuple<T, T> to_tuple() const {
		return std::make_tuple(m_x, m_y);
	}
	operator std::tuple<T, T>() const {
		return to_tuple();
	}
private:
	T m_x;
	T m_y;
};
template <typename T> 
bool operator< (const Vec2<T>& v1, const Vec2<T>& v2) {
	return v1.to_tuple() < v2.to_tuple();
}
template <typename T>
bool operator== (const Vec2<T>& v1, const Vec2<T>& v2) {
	return v1.to_tuple() == v2.to_tuple();
}
template <typename T>
bool operator!= (const Vec2<T>& v1, const Vec2<T>& v2) {
	return v1.to_tuple() != v2.to_tuple();
}
constexpr Vec2<int> unit(1, 1);
constexpr Vec2<int> unit_x(1, 0);
constexpr Vec2<int> unit_y(0, 1);

template <typename T1, typename T2>
Vec2<T1> operator+ (Vec2<T1> a, Vec2<T2> b) {
	return Vec2<T1>(a.x() + (T1)b.x(), a.y() + (T1)b.y());
}
template <typename T1, typename T2>
Vec2<T1> operator- (Vec2<T1> a, Vec2<T2> b) {
	return a + b * (-1);
}
template <typename T>
Vec2<T> operator* (Vec2<T> a, double s) {
	return Vec2<T>((T)(a.x() * s), (T)(a.y() * s));
}
template <typename T>
Vec2<T> operator* (double s, Vec2<T> a) {
	return Vec2((T)(a.x() * s), (T)(a.y() * s));
}
template <typename T>
Vec2<T> operator* (Vec2<T> a, Vec2<T> b) {
	return Vec2(a.x() * b.x(), a.y() * b.y());
}
template<typename T>
std::ostream& operator<< (std::ostream& os, Vec2<T> a) {
	return os << "( " << a.x() << ", " << a.y() << " )";
}
