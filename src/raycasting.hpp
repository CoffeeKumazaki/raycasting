#pragma once

struct Vector2D {
  double x, y;
};

struct Ray {
  Vector2D pos;
  double dir;     // rad.
  double length;
};

struct Segment {
  Vector2D begin;
  Vector2D end;
};

struct Circle {
  Vector2D center;
  double radius;
};

bool getIntersection(const Ray &ray, const Segment &segment, Vector2D &intersection);
bool getIntersection(const Ray &ray, const Circle &circle, Vector2D &intersection);
