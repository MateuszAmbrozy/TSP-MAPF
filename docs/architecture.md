# Architecture and verification

## Core

`map::Graph` stores a rectangular grid indexed as `cells[x][y]`. `Agent` stores capacity, position, assigned tasks and a timed path. `TaskGroup` combines pickup cells, service times, a shared dropoff and a capacity requirement.

`TSP::solveTSP` constructs a Euclidean distance matrix and applies repeated 2-opt improvements to an open route from the agent through the pickups to the dropoff. The returned values are zero-based pickup indices. A* separately computes grid distances; it is not currently used to populate the TSP matrix.

`CA_Environment` and `WCA_Environment` assign work and advance agents. `CooperativeA` plans timed routes; `WindowCooperativeA` replans using agent-specific window settings. `Reservation` stores vertex and directed-edge reservations, including a reverse-edge conflict query.

## Desktop application

The Qt application has General, Editor and Animation views. It uses Qt's JSON classes for scenario files and the graphics scene framework for rendering. The console demo uses neither Qt nor a JSON library.

The root CMake project builds `tsp_mapf`, `tsp_mapf_demo`, optional `tsp_mapf_gui`, and `tsp_mapf_tests` when testing is enabled. `TSP_MAPF_BUILD_GUI=OFF` permits a dependency-free core build. GUI resources are embedded through Qt's resource compiler.

## Verification

Run the commands in the README. The regression executable uses explicit failure checks, including in Release builds:

- A* obstacle detour, identical endpoints, invalid/blocked endpoints, empty graph and unreachable goal;
- vertex reservations, time separation, reverse-edge conflicts and reservation removal;
- a known 2-opt improvement, empty task ordering, default task state and stream output.

CTest also runs the console demo. CI is configured to build/test the core on Linux and Windows, and to compile the Qt application on Linux. A green core job alone does not verify the GUI or multi-agent coordination.

## Remaining engineering work

- Validate complete JSON scenarios before mutating the current simulation; reject overlapping starts, blocked locations and inconsistent dimensions.
- Add bounded, deterministic multi-agent scenarios checking vertex conflicts, edge swaps, task completion and unreachable-task behavior for both planners.
- Consolidate random generation behind a configurable seed before publishing benchmark comparisons.
- Extend RAII ownership through graphics helpers and auxiliary task windows.
- Investigate grid-distance-aware task ordering, with explicit handling of unreachable pickups.
- Record real GUI screenshots or a short demonstration and benchmark results after the scenarios have been verified.

## Repository cleanup

Generated Windows/CMake output, executables and user-specific IDE files were removed. The non-building historical console benchmark was replaced with a self-contained example. Unused Environment/Task/Reserve/TSP-MAPD prototypes and empty Space-Time-AStar placeholders were removed; their originals remain in Git history. The original class diagram was moved into `docs/`.

The retained PDF references are third-party material. No project-wide license has been inferred or assigned on the author's behalf.
