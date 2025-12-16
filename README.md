# SatiChecker - Resolution-Based SAT Solver


*A user-friendly Windows console-based SAT solver for propositional logic satisfiability checking and CNF conversion, implemented in C.*

## Overview

SatiChecker is a modular SAT (Satisfiability) solver designed to determine the satisfiability of propositional logic clauses using resolution and to convert formulas to Conjunctive Normal Form (CNF). It features an interactive, visually appealing console interface with colored output, ASCII art, and arrow-key navigation. Developed as a team project, it emphasizes maintainability, performance optimizations, and user experience.

Key capabilities:
- **Satisfiability Checking**: Applies resolution to detect empty clauses (unsatisfiable) or confirm satisfiability.
- **CNF Conversion**: Handles implications (`>`), equivalences (`=`), negations (`!`), and parentheses for grouping.
- **Input Handling**: Reads clauses from text files; supports up to 26 variables (A-Z).
- **Timeout Protection**: 3-minute limit for large inputs to prevent hangs.

The project was developed in June 2025 and is Windows-specific, leveraging console APIs for enhanced UI.

## Team Logic
- **Mekentichi NejemEddine**
- **Kahlouche Youcef**
- **Benbada Ayoub**
- **Bouderbala Heythem**

## Features

- **Modular Architecture**: Clean separation of concerns for easy maintenance.
- **Dynamic Memory Management**: Uses `malloc`/`realloc` for flexible input sizes; leak-free with `freeClause`.
- **Efficient Resolution**: Avoids duplicates, caps iterations at 100, and includes timeout for practicality.
- **Input Validation**: Syntax checks with user-friendly error messages (color-coded: red for errors, green for success).
- **Visual Interface**: ASCII art splash screens, centered menus, and step-by-step CNF traces.
- **Documentation Integration**: In-app workflow and format guides.

## System Requirements

- **OS**: Windows (uses `<windows.h>` and `<conio.h>` for console features).
- **Compiler**: GCC (MinGW) or Microsoft Visual C++.
- **No external libraries**: Pure standard C with basic headers.

## Installation & Building

1. Clone or download the repository.
2. Open a command prompt in the project directory.
3. Compile with:
   ```
   gcc -o SatiChecker.exe *.c -std=c99
   ```
   (Adjust for your compiler; ensure `windows.h` and `conio.h` are available.)

4. Run: `./SatiChecker.exe` (or double-click the EXE).

No installation required—it's a standalone console app.

## Usage

### Launching
- Run the executable to see the team splash screen, followed by the main menu.

### Main Menu Options
Use arrow keys to navigate and Enter to select:
1. **Check Satisfiability using Resolution**: Input a `.txt` file with CNF clauses (e.g., `A|B|!C & D|!E & F`).
2. **CNF Conversion**: Input a `.txt` file with propositional formulas (e.g., `(A>B) || (B>C)`).
3. **Read Documentation**: View in-app guides on formats and workflows.
4. **View Algorithm Workflow**: Step-by-step resolution and conversion explanations.
5. **Exit Program**.

### Input Formats
- **For Satisfiability**:
  - One clause per line, separated by `&`.
  - Literals: Uppercase letters (A-Z), negated with `!` (e.g., `A|B|!C`).
  - Example file (`input.txt`):
    ```
    A|B|!C
    D|!E
    F
    ```

- **For CNF Conversion**:
  - Supports `|`(OR), `&`(AND), `!`(NOT), `>`(implication), `=`(equivalence), `( )` for grouping.
  - Example: `(A>B) || (B=C)` → Outputs `(!A|B) & (!B|C)` (step-by-step trace shown).

- **Output**:
  - Satisfiability: "Satisfiable" (green) or "Unsatisfiable" (red), with resolution trace.
  - CNF: Step-by-step conversion (eliminate `=`, `>`, push negations, distribute OR over AND).

Press `Q` to return to the menu from any screen.

### Example Runs
- **Satisfiability Input**: `A|B|!C & D|!E & F` → **Result**: Satisfiable (no empty clause).
- **CNF Input**: `(A>B) || (B>C)` → **Output**: `(!A|B) & (!B|C)`.

## Code Structure
The project is organized into modular C files:

| File              | Purpose |
|-------------------|---------|
| `fileHandlers.c`  | File I/O for reading clauses from text files. |
| `interface.c`     | Console UI with colors, ASCII art, and key navigation. |
| `stringHandlers.c`| Clause parsing, resolution logic, and CNF conversion. |
| `main.c`          | Program orchestration and menu flow. |

- **Data Structures**: `Clause` struct (string + variable array up to 26).
- **Key Functions**:
  - `setResolution()`: Core resolution with duplicate checks.
  - `findMatchingParen()`: Handles nested parentheses.
  - `verifySet()` / `verifySetWithConversion()`: Input validation.

## Algorithms
### Resolution for SAT
1. Parse clauses into sets.
2. Generate resolvents by resolving complementary literals.
3. Repeat until empty clause (unsat) or no new clauses (sat).
4. Timeout: "Likely satisfiable" after 3 minutes.

### CNF Conversion
1. Eliminate equivalences (`P=Q` → `(P→Q) & (Q→P)`).
2. Eliminate implications (`P→Q` → `!P|Q`).
3. Push negations inward (`!(P&Q)` → `!P|!Q`).
4. Distribute OR over AND (Morgan's laws).

## Optimizations & Challenges
- **Performance**: Duplicate avoidance (`setHas`), iteration limits, dynamic allocation.
- **Challenges Solved**:
  - Nested parentheses: `findMatchingParen`.
  - Memory leaks: Strict `freeClause` usage.
  - UX: Color-coded errors and step-by-step traces.

## Screenshots
*(Placeholders; add actual images to `/screenshots/` folder in the repo.)*

- **Main Menu**: ASCII art header with options.
- **Team Splash**: Developer credits.
- **CNF Screen**: File input and step-by-step output.
- **Resolution Screen**: Trace with "Satisfiable" result.
- **Documentation**: Format guides.
- **Workflow**: Algorithm steps.

## Future Improvements
- Support for >26 variables (e.g., numbered vars).
- Advanced resolution strategies (e.g., unit propagation).
- Cross-platform compatibility (Linux/Mac).
- GUI version or web interface.

## Contact
For questions, reach out via the team members or open an issue on the repo.

---

*Report generated June 12, 2025. Code available in this repository.*
