# SuperJupiter PowerBudget

<!-- SUPERJUPITER-CONSOLIDATION-MIGRATION:START -->
## Consolidated library available

For new projects, start with **[SuperJupiter Embedded Reliability](https://github.com/Super-Jupiter/superjupiter-embedded-reliability)**. This standalone package and its published releases remain available. Existing installations need no change.

If migrating with PlatformIO, replace the standalone dependency with:

```ini
lib_deps = superjupiter/SuperJupiterEmbeddedReliability@0.1.0
```

Replace this standalone include:

```cpp
#include <SuperJupiterPowerBudget.h>
```

with this consolidated include:

```cpp
#include <SuperJupiter/EmbeddedReliability/utilities/PowerBudget.h>
```

The component namespace and behaviour are preserved from standalone **0.1.1**. If using an earlier version, review its API migration requirements first. Use one implementation of this component per application; do not mix standalone and consolidated copies.

PowerBudget is a supporting utility. Use the explicit header above; it is excluded from the convenience header.

See the [consolidated migration guide](https://github.com/Super-Jupiter/superjupiter-embedded-reliability/blob/v0.1.0/docs/MIGRATION.md) and consolidated README for Arduino/CMake installation. **Hardware validation pending.**
<!-- SUPERJUPITER-CONSOLIDATION-MIGRATION:END -->

Transparent battery-current duty-cycle and runtime estimates.

**Version 0.1.1: evolving preview. Hardware validation pending.**

Original MIT-licensed portable C++11 by [SuperJupiter](https://www.superjupiter.net).
No mandatory service, hidden clock reads, network calls or heap allocation in the core.

## What it does

Profile entries specify battery-side current in mA and durationMs for mutually exclusive operating states.
The supplied profile is one repeating cycle; average current is duration-weighted, with an optional constant background load.
usableFraction includes caller-estimated capacity derating and reserve. RuntimeHours=capacityMah*usableFraction/averageMa.
All nonzero-duration states contribute. Zero load reports NoLoad rather than a finite lifetime.
No battery chemistry, temperature, ageing, regulator or self-discharge model is inferred. Estimates require measured profiles; hardware validation pending.

## Install

PlatformIO exact release (after registry publication):

```ini
lib_deps = superjupiter/SuperJupiterPowerBudget@0.1.1
```

Arduino: use the versioned [GitHub release ZIP](https://github.com/Super-Jupiter/superjupiter-power-budget/releases/tag/v0.1.1).
Library Manager registration and index propagation are separate steps; check the official index before relying on IDE availability.

## Example

```cpp
#include <SuperJupiterPowerBudget.h>
using namespace superjupiter::power_budget;
int main() {
State profile[]={{80,100},{0.1,9900}};
Result battery=estimate(profile,2,2000,0.8);
(void)battery;
return 0;
}

```

For Arduino, open `examples/Minimal`. It uses synthetic data only.
Results are deliberately local so applications can inspect outcomes before adding I/O.

## CMake

```cmake
add_subdirectory(path/to/superjupiter-power-budget)
target_link_libraries(your_app PRIVATE SuperJupiter::PowerBudget)
```

For an installed package use `find_package(SuperJupiterPowerBudget 0.1 CONFIG REQUIRED)`.
Top-level CMake builds tests/examples; a subproject does not enable developer tools by default.

## Documentation and verification

- [API and boundaries](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/docs/API.md)
- [Tests](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/tests/test.cpp)
- [Validation status](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/docs/VALIDATION.md)
- [Shared conventions](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/docs/CONVENTIONS.md)
- [Migration policy](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/docs/MIGRATION.md)

```sh
cmake -S . -B build/cmake
cmake --build build/cmake
ctest --test-dir build/cmake --output-on-failure
python3 tools/check_release.py --tag v0.1.1
```

Keep raw observations and their provenance in your application. Derived results are not physical truth.
Do not use fast-math or finite-math-only compiler assumptions. AVR double may have float precision.
Software tests, board compilation and physical validation are reported separately.

## Fits the SuperJupiter measurement portfolio

Complements [SensorQuality](https://github.com/Super-Jupiter/superjupiter-sensor-quality) and
[SampleWindow](https://github.com/Super-Jupiter/superjupiter-sample-window) without a required dependency.
Each component can be used independently. Domain models, customer data and proprietary applications are excluded.

## Support and licence

MIT. Report reproducible bugs through repository issues; private security reports: info@superjupiter.net.
Include version, configuration, inputs, expected and observed behaviour. No credentials or private customer data.


## Compatibility review 0.1.1

This patch includes independent regression tests and a reviewed [interoperability contract](https://github.com/Super-Jupiter/superjupiter-power-budget/blob/v0.1.1/docs/INTEROPERABILITY.md). Hardware validation pending.

- Removed unused duplicated math helpers and unnecessary includes.
- Positive subnormal loads cannot be reported as no load; avoid runtime underflow when final result is representable.
