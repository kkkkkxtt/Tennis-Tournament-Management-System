# Tennis-Tournament-Management-System
Exploring efficient data structures in C++ by implementing a tournament management module (Part of a system)
This program utilized static array with circular queue to handle the tournament management.

# Tennis Tournament Management System (TTMS)

*A C++ program that simulates a tennis tournament with qualifiers, round-robin, knockout, and final stages.*

---

## Table of Contents
- [Features](#-features)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [Example Output](#-example-output)

---

## Features
- **Player Management**:  
  - Tracks active/withdrawn/substitute players.
  - Reads player data from `player.txt`.
- **Tournament Stages**:  
  - **Qualifiers**: 12 → 6 players.
  - **Round Robin**: 6 → 4 players (top 4 advance).
  - **Knockout**: 4 → 2 finalists + 3rd place match.
  - **Finals**: Championship and rankings.
- **Match Simulation**:  
  - Auto-generates scores (`15-8`, `15-7`, etc.).
  - Records results in `match.txt`.
- **Rankings**:  
  - Displays final standings with original world ranks.

---

## Prerequisites
- **Compiler**: `g++` (GNU C++ Compiler) or equivalent.
- **OS**: Windows/Linux/macOS (tested on Windows).

---

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/kkkkkxtt/Tennis-Tournament-Management-System.git
   cd Tennis-Tournament-Management-System
   ```

2. Compile the program:
   ```bash
   g++ src/*.cpp -o bin/ttms
   ```

3. Run the program:
   ```bash
   ./bin/ttms
   ```

##  Example Output:

<div align="center">
  <img src="https://github.com/user-attachments/assets/b61fc8f4-bf65-4e54-8ed2-3463c2467917" width="30%">
</div>

## Output Format

### Match.txt
<div align="center">
  <img src="https://github.com/user-attachments/assets/c8eeea7f-a13c-4fa9-93b3-6ce385e4557a" width="30%">
</div>



