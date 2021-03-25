#include <stdafx.hpp>
#include "raycasting.hpp"

bool getIntersection(const Ray &ray, const Segment &segment, Vector2D &intersection) {

  Vector2D ray_delta;
  ray_delta.x = ray.pos.x + cos(ray.dir)*ray.length;
  ray_delta.y = ray.pos.y + sin(ray.dir)*ray.length;

  Vector2D seg_delta;
  seg_delta.x = segment.end.x - segment.begin.x;
  seg_delta.y = segment.end.y - segment.begin.y;

  double T2 = (ray_delta.x * (segment.begin.y - ray.pos.y) + ray_delta.y * (ray.pos.x - segment.begin.x)) / (seg_delta.x * ray_delta.y - seg_delta.y * ray_delta.x);
  double T1 = (segment.begin.x + seg_delta.x * T2 - ray.pos.x) / ray_delta.x;

  if (T1 < 0)
    return false;

  if (T2 < 0 || T2 > 1)
    return false;

  intersection.x = ray.pos.x + ray_delta.x * T1;
  intersection.y = ray.pos.y + ray_delta.y * T1;

  return true;
}
