# Refactoring & Enhancement Diary

## Phase 1: Assessment & Planning

**Date:** 2026-01-04

- Initial assessment of the "Brainy Warrior" project.
- Identified issues: Memory leaks in `Node_State`, heavy reliance on global variables, mixed IO/Logic concerns, and TUI bugs.

## Phase 2: Core Refactoring

**Date:** 2026-01-05

- **Fix Memory Leaks:** [DONE]
  - Modified `Node_State` to use `std::shared_ptr`.
- **Clean Architecture:** [DONE]
  - Created `GameSession` singleton to encapsulate game state.
- **TUI IO Fixes:** [DONE]
  - Updated `level()` to use `getch()` and `printw()`.

## Phase 3: Centering & Coloring (UI/UX Foundation)

**Date:** 2026-01-06

- **Objectives:**
  - Initialize ncurses color pairs for better visualization. [DONE]
  - Implement centered board printing in `helper_functions.cpp`. [DONE]

## Phase 4: Computer Play Implementation

**Date:** 2026-01-08

- **Objectives:**
  - Implement algorithm selection menu. [DONE]
  - Complete the `computer_play()` flow. [DONE]

## Phase 5: Statistics & Filtering

**Date:** 2026-01-11

- **Objectives:**
  - Correctly load and display statistics. [DONE]
  - Filter stats by the current user's name. [DONE]

## Phase 6: Final Polish & UX

**Date:** 2026-01-13

- **Objectives:**
  - Improve win/lose screens. [DONE]
  - Fix the "print path" feature layout. [DONE]
  - Center all instructions and algorithm status messages under the game board. [DONE]

## Phase 7: Ease of Use

**Date:** 2026-01-14

- **Objectives:**
  - Create `run_game.sh` to automate build and execution. [DONE]

## Phase 8: Robustness & Crash Fixes

**Date:** 2026-01-14

- **Objectives:**
  - Fixed `Board::getPlayerTile()` fallback to prevent crashes. [DONE]

## Phase 9: Search Logic & Wizard Optimization

**Date:** 2026-01-21

- **Objectives:**
  - **Fix Wizard Move (Z):** Refactored `moveWizard` to correctly handle tile swaps in the board grid. [DONE]
  - **Advanced Level Solving:** Optimized A* heuristic and state hashing to solve levels 15, 16, and 17. [DONE]

## Phase 10: Performance & Hardening

**Date:** 2026-01-21

- **Objectives:**
  - **Closed List Implementation:** Updated UCS and A* to correctly track state costs, ensuring optimal path finding. [DONE]
  - **Search Speedup:** Optimized TUI refresh rate during search (100 states per update) to handle complex levels without lag. [DONE]
  - **Enhanced A* Heuristic:**Prioritized target count reduction in A* to guide the search through hard levels (15-17). [DONE]
