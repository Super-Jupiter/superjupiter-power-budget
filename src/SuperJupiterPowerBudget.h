// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SuperJupiter
#ifndef SUPERJUPITER_POWER_BUDGET_H
#define SUPERJUPITER_POWER_BUDGET_H
#if defined(__FAST_MATH__) || (defined(__FINITE_MATH_ONLY__) && __FINITE_MATH_ONLY__ > 0)
#error "SuperJupiter requires IEEE non-finite checks; disable fast-math"
#endif
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#define SUPERJUPITER_POWER_BUDGET_VERSION "0.1.0"
namespace superjupiter {
namespace power_budget {

namespace detail {
inline bool finite(double value) { return value == value && value >= -DBL_MAX && value <= DBL_MAX; }
inline double oneMinusExpNegative(double ratio) {
  // Stable at small elapsed/tau without requiring AVR's unavailable expm1.
  if (ratio < 0.01) {
    double term = ratio, total = ratio;
    for (unsigned n = 2; n <= 12; ++n) {
      term *= -ratio / n;
      total += term;
    }
    return total;
  }
  return 1 - ::exp(-ratio);
}
} // namespace detail

enum class Status { Ok, NoLoad, InvalidInput, NumericError };
struct State {
  double currentMa;
  uint32_t durationMs;
};
struct Result {
  Status status;
  uint64_t cycleMs;
  double averageMa, mahPerDay, runtimeHours;
};
inline Result estimate(const State *states, size_t count, double capacityMah,
                       double usableFraction = 0.8, double backgroundMa = 0) {
  Result r = {Status::InvalidInput, 0, 0, 0, 0};
  if (!states || !count || !detail::finite(capacityMah) || capacityMah <= 0 ||
      !detail::finite(usableFraction) || usableFraction <= 0 || usableFraction > 1 ||
      !detail::finite(backgroundMa) || backgroundMa < 0)
    return r;
  for (size_t i = 0; i < count; ++i) {
    if (!detail::finite(states[i].currentMa) || states[i].currentMa < 0 ||
        r.cycleMs > UINT64_MAX - states[i].durationMs)
      return r;
    r.cycleMs += states[i].durationMs;
  }
  if (!r.cycleMs)
    return r;
  r.averageMa = backgroundMa;
  for (size_t i = 0; i < count; ++i)
    r.averageMa += states[i].currentMa *
                   (static_cast<double>(states[i].durationMs) / static_cast<double>(r.cycleMs));
  if (!detail::finite(r.averageMa)) {
    r.status = Status::NumericError;
    return r;
  }
  if (r.averageMa == 0) {
    r.status = Status::NoLoad;
    return r;
  }
  r.mahPerDay = r.averageMa * 24;
  r.runtimeHours = (capacityMah * usableFraction) / r.averageMa;
  r.status = detail::finite(r.mahPerDay) && detail::finite(r.runtimeHours) ? Status::Ok
                                                                           : Status::NumericError;
  return r;
}
} // namespace power_budget
} // namespace superjupiter
#endif
