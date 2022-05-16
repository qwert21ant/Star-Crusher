#ifndef UtilsH
#define UtilsH

#include <windows.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <time.h>
#include <cmath>
#include <algorithm>
#include <vector>

#include "Timer.h"
#include "resource.h"

using namespace std;

//---------------------------------------------------------------------------

template <typename T> struct Vector{
	T x, y;

	Vector<T>() : x(T()), y(T()) {};
    Vector<T>(T _x, T _y) : x(_x), y(_y) {};
	Vector<T>(const Vector<T> &v) : x(v.x), y(v.y) {};
	template <typename U> Vector<T>(const Vector<U> &v) : x(T(v.x)), y(T(v.y)) {};

	Vector<T> operator + (const Vector<T> &v) const { return {x + v.x, y + v.y}; };
	Vector<T> operator - (const Vector<T> &v) const { return {x - v.x, y - v.y}; };
	template <typename U> Vector<T> operator * (U k) const { return {T(x * k), T(y * k)}; };
	template <typename U> Vector<T> operator / (U k) const { return {T(x / k), T(y / k)}; };

	Vector<T>& operator += (const Vector<T> &v) { x += v.x; y += v.y; return *this; };
	Vector<T>& operator -= (const Vector<T> &v) { x -= v.x; y -= v.y; return *this; };
	Vector<T>& operator *= (double k) { x *= k; y *= k; return *this; };
	Vector<T>& operator /= (double k) { x /= k; y /= k; return *this; };

	Vector<T> operator -() const { return {-x, -y}; };

	bool operator == (const Vector<T> &v) const { return x == v.x && x == v.y; };
	bool operator != (const Vector<T> &v) const { return !(*this == v); };

	operator bool() const { return x || y; };
	operator TPoint() const { return TPoint(x, y); };

	String toString() const;

	double Len() const { return sqrt(x*x + y*y); };
	Vector<T> Norm() const { return (*this / Len()); };
	Vector<T> Rotate(double Angle, Vector<T> Point = Vector<T>(0, 0)) const {
		double c = cos(Angle), s = sin(Angle);
		double dx = x - Point.x, dy = y - Point.y;
		return Vector<T>(Point.x + dx * c - dy * s, Point.y + dx * s + dy * c);
	}
};

//---------------------------------------------------------------------------

template <> String Vector<int>::toString() const {
	return "X: " + IntToStr(x) + ", Y: " + IntToStr(y);
};

template <> String Vector<double>::toString() const {
	return "x: " + FloatToStrF(x, ffFixed, 4, 8) + ", y: " + FloatToStrF(y, ffFixed, 4, 8);
};

//---------------------------------------------------------------------------

typedef Vector<int> Vec2i;
typedef Vector<double> Vec2d;

//---------------------------------------------------------------------------

template <typename T> struct Rect{
	T left, top, right, bottom;

	Rect<T>() :
		left(T()), top(T()), right(T()), bottom(T()) {};
	Rect<T>(T _left, T _top, T _right, T _bottom) :
		left(_left), top(_top), right(_right), bottom(_bottom) {};
	Rect<T>(Vector<T> a, Vector<T> b) :
		left(a.x), top(a.y), right(b.x), bottom(b.y) {};
	Rect<T>(Vector<T> size) :
		left(T()), top(T()), right(size.x), bottom(size.y) {};
	Rect<T>(const Rect<T> &r) :
		left(r.left), top(r.top), right(r.right), bottom(r.bottom) {};
	template <typename U> Rect<T>(const Rect<U> &r) :
		left(T(r.left)), top(T(r.top)), right(T(r.right)), bottom(T(r.bottom)) {};

	Rect<T> operator + (const Vector<T> &v){
		return {left + v.x, top + v.y, right + v.x, bottom + v.y};
	};

	Rect<T> operator - (const Vector<T> &v){
		return {left - v.x, top - v.y, right - v.x, bottom - v.y};
	};

	Rect<T>& operator += (const Vector<T> &v){
		left += v.x; top += v.y;
		right += v.x; bottom += v.y;
		return *this;
	};

	Rect<T>& operator -= (const Vector<T> &v){
		left -= v.x; top -= v.y;
		right -= v.x; bottom -= v.y;
		return *this;
	};

	bool operator == (const Rect<T> &r){
		return left == r.left && top == r.top && right == r.right && bottom == r.bottom;
	};

	bool operator != (const Rect<T> &r) { return !(*this == r); };

	operator TRect() { return {left, top, right, bottom}; };

	String toString();

	Vector<T> LT() const { return {left, top}; };
	Vector<T> RT() const { return {right, top}; };
	Vector<T> RB() const { return {right, bottom}; };
	Vector<T> LB() const { return {left, bottom}; };
	Vector<T> Center() const { return {Width()/2 + left, Height()/2 + top}; };

	T Width() const { return right - left; };
	T Height() const { return bottom - top; };
	Vector<T> Size() const { return {Width(), Height()}; };

	bool PtInRect(Vector<T> v) const { return v.x > left && v.x < right && v.y > top && v.y < bottom; };
	bool IsCollision(Rect<T> r) const {
		return right >= r.left && r.right >= left &&
			   bottom >= r.top && r.bottom >= top;
	};

	Rect<T> Scale(Vector<T> v, double k){
		Rect<T> r;
		r.left = v.x - (v.x - left) * k;
		r.top = v.y - (v.y - top) * k;
		r.right = v.x + (right - v.x) * k;
		r.bottom = v.y + (bottom - v.y) * k;
		return r;
	}

	static Rect<T> Bounds(T x, T y, T width, T height){
		Rect<T> r;
		r.left = x;
		r.top = y;
		r.right = x + width;
		r.bottom = y + height;
		return r;
	};

	static Rect<T> Bounds(Vector<T> pos, Vector<T> size){
		Rect<T> r;
		r.left = pos.x;
		r.top = pos.y;
		r.right = pos.x + size.x;
		r.bottom = pos.y + size.y;
		return r;
	};

	static Rect<T> BoundsCenter(Vector<T> Center, Vector<T> Size){
		return Bounds(Center, Size) - Vector<T>(Size / 2);
	};
};

//---------------------------------------------------------------------------

template <> String Rect<int>::toString(){
	return LT().toString() +
		   " W: " + IntToStr(Width()) + " H: " + IntToStr(Height());
};

template <> String Rect<double>::toString(){
	return LT().toString() +
		   " W: " + FloatToStrF(right, ffFixed, 4, 8) + " H: " + FloatToStrF(bottom, ffFixed, 4, 8);
};

//---------------------------------------------------------------------------

typedef Rect<int> Rect4i;
typedef Rect<double> Rect4d;

//---------------------------------------------------------------------------

struct Range{
	int begin, end;

	Range() : begin(0), end(0) {};
	Range(int Begin, int End) : begin(Begin), end(End) {};
	Range(const Range &r) : begin(r.begin), end(r.end) {};

	int Size() { return end - begin; };
	int Middle() { return Size() / 2; };
};

//---------------------------------------------------------------------------

double RadToGrad(double Rad){
	return Rad * 180 / (double)M_PI;
}

double GradToRad(double Angle){
	return Angle * M_PI / 180.f;
}

//---------------------------------------------------------------------------

void InfoBox(String Str){
	Application->MessageBox(Str.w_str(), L"Info", MB_OK | MB_ICONINFORMATION);
}

//---------------------------------------------------------------------------

int Random(Range r){
    if(!r.Size()) return 0;
	int x = rand() % r.Size() + r.begin;
	return x;
}

int Random(int begin, int end){
	return Random(Range(begin, end));
}

//---------------------------------------------------------------------------

string BStoS(String S){
	return string(AnsiString(S).c_str());
}

String StoBS(string s){
    return String(s.c_str());
}

//---------------------------------------------------------------------------
#endif
