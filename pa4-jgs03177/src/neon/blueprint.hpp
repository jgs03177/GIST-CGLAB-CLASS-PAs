#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <memory>

// Forward declarations
// blueprint of neon
namespace ne {

namespace abstract {
class Material;
class Rendable;
} // namespace abstract

namespace core {
class Integrator;
} // namespace core

class Image;
class TileIterator;
class Sphere;
class Intersection;
class Ray;
class Metal;
class Lambertian;
class DiffuseLight;
class Camera;
class Scene;

// alias
using MaterialPointer = std::shared_ptr<ne::abstract::Material>;
using RendablePointer = std::shared_ptr<ne::abstract::Rendable>;

namespace utils {
class Timer;
class Progressbar;
} // namespace utils

} // namespace ne

#endif // __CONFIG_H_
