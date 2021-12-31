#include <atomic>
namespace partou::stats
{
std::atomic_uint64_t numRayTrianglesTests = 0;
std::atomic_uint64_t numRayTrianglesIsect = 0;
std::atomic_uint64_t numPrimaryRays = 0;
}  // namespace partou::stats
