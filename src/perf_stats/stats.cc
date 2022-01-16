#include <atomic>
namespace partou::stats
{
std::atomic_uint64_t numRayBBoxTests {};
std::atomic_uint64_t numRayTrianglesTests {};
std::atomic_uint64_t numRayTrianglesIsect {};
std::atomic_uint64_t numPrimaryRays {};
std::atomic_uint64_t numNaNpixels {};
}  // namespace partou::stats
