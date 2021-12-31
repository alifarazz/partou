#pragma once

#include <atomic>
namespace partou::stats
{
extern std::atomic_uint64_t numRayBBoxTests;
extern std::atomic_uint64_t numRayTrianglesTests;
extern std::atomic_uint64_t numRayTrianglesIsect;
extern std::atomic_uint64_t numPrimaryRays;
}  // namespace partou::stats
