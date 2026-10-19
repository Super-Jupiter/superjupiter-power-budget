cmake_minimum_required(VERSION 3.16)
project(SuperJupiterPowerBudget VERSION 0.1.0 LANGUAGES CXX)

set(_superjupiter_power_budget_top_level OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(_superjupiter_power_budget_top_level ON)
endif()
option(SUPERJUPITER_POWER_BUDGET_BUILD_TESTS "Build PowerBudget tests" ${_superjupiter_power_budget_top_level})
option(SUPERJUPITER_POWER_BUDGET_BUILD_EXAMPLES "Build PowerBudget host example" ${_superjupiter_power_budget_top_level})
option(SUPERJUPITER_POWER_BUDGET_INSTALL "Generate PowerBudget install rules" ${_superjupiter_power_budget_top_level})

add_library(SuperJupiterPowerBudget INTERFACE)
add_library(SuperJupiter::PowerBudget ALIAS SuperJupiterPowerBudget)
set_target_properties(SuperJupiterPowerBudget PROPERTIES EXPORT_NAME PowerBudget)
target_include_directories(SuperJupiterPowerBudget INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)
target_compile_features(SuperJupiterPowerBudget INTERFACE cxx_std_11)

if(SUPERJUPITER_POWER_BUDGET_BUILD_TESTS)
  enable_testing()
  add_executable(superjupiter-power-budget-tests tests/test.cpp)
  target_link_libraries(superjupiter-power-budget-tests PRIVATE SuperJupiter::PowerBudget)
  add_test(NAME superjupiter-power-budget-tests COMMAND superjupiter-power-budget-tests)
  add_executable(superjupiter-power-budget-namespace-test tests/namespace_isolation.cpp)
  target_link_libraries(superjupiter-power-budget-namespace-test PRIVATE SuperJupiter::PowerBudget)
  add_test(NAME superjupiter-power-budget-namespace-test COMMAND superjupiter-power-budget-namespace-test)
endif()
if(SUPERJUPITER_POWER_BUDGET_BUILD_EXAMPLES)
  add_executable(superjupiter-power-budget-replay tools/replay.cpp)
  target_link_libraries(superjupiter-power-budget-replay PRIVATE SuperJupiter::PowerBudget)
endif()
if(SUPERJUPITER_POWER_BUDGET_INSTALL)
  include(CMakePackageConfigHelpers)
  set(_superjupiter_power_budget_package_dir lib/cmake/SuperJupiterPowerBudget)
  install(TARGETS SuperJupiterPowerBudget EXPORT SuperJupiterPowerBudgetTargets)
  install(FILES src/SuperJupiterPowerBudget.h DESTINATION include)
  install(EXPORT SuperJupiterPowerBudgetTargets NAMESPACE SuperJupiter::
    DESTINATION ${_superjupiter_power_budget_package_dir})
  configure_package_config_file(cmake/SuperJupiterPowerBudgetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterPowerBudgetConfig.cmake
    INSTALL_DESTINATION ${_superjupiter_power_budget_package_dir})
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterPowerBudgetConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterPowerBudgetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterPowerBudgetConfigVersion.cmake
    DESTINATION ${_superjupiter_power_budget_package_dir})
  install(FILES LICENSE DESTINATION share/licenses/SuperJupiterPowerBudget)
endif()
