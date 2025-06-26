# TicTacToe Game
## Overview
This project is an advanced implementation of the classic Tic Tac Toe game developed in C++ with a focus on modular architecture, user interaction, and personalized gameplay experiences. It features both the standard 3×3 Tic Tac Toe and an extended Mega Tic Tac Toe mode for enhanced complexity and fun.
The game includes a modern graphical user interface built using Qt, along with SQLite integration for persistent user profiles and game history. It supports secure user authentication using bcrypt password hashing, and offers two game modes: Player vs Player and Player vs AI (with difficulty levels implemented using the Minimax algorithm).
To ensure software quality, the project incorporates unit testing with Google Test, follows a clean C++ class structure, and uses GitHub Actions for CI (continuous integration). The codebase is organized for maintainability and extensibility, making it suitable for learning or further development.
## Objectives
- **Engaging Gameplay :** Provide a user-friendly and visually appealing experience through a responsive Qt GUI.

- **Multiple Game Modes :** Support both standard and Mega Tic Tac Toe boards with Player vs Player and Player vs AI modes.

- **Expanded Game Modes :** Support both the classic 3×3 grid and an extended Mega Tic Tac Toe mode to increase challenge.

- **Replay System :** Allow users to review past games through a built-in replay feature.

- **User Profiles :** Enable user authentication with secure password handling using bcrypt.

- **Game History :** Store complete game sessions in a persistent SQLite database.

- **Testing and CI :** Use Google Test for reliable unit testing and GitHub Actions for automated CI.

- **Clean Architecture :** Use object-oriented design and modular C++ code for maintainability and scalability.

## Features
- **🎮Classic 3×3 Tic Tac Toe :** The standard version of the game.

- **🧩Mega Tic Tac Toe Mode :** An extended version for more strategic and complex gameplay.

- **🤝Multiplayer Support :** Play in Player vs Player or challenge the AI.

- **🧠AI Opponent with Difficulty Levels :** Implemented using the Minimax algorithm, offering Easy, Normal, and Hard levels.

- **🔐Secure User Authentication :** Passwords are hashed using bcrypt for safe login and registration.

- **🕹️Game History Tracking :** Stores all past matches in a SQLite database, including the outcome, players, and timestamp.

- **💾Move-Level Storage :** Every move is recorded in a separate table to enable game replay functionality.

- **🔁Replay System :** Players can replay previously played games step-by-step, reviewing all moves.

- **🖼️Graphical User Interface (GUI) :** Built with Qt, offering a clean and interactive interface.

- **🧪Unit Testing :** Key components are tested using Google Test.

- **🔁Continuous Integration (CI) :** Uses GitHub Actions for automated builds and testing on every commit.

## Tech Stack / Tools Used

| Category           | Tool / Library      | Purpose                                              |
|--------------------|---------------------|------------------------------------------------------|
| Language           | C++                 | Core game logic and backend                          |
| GUI Framework      | Qt                  | Building cross-platform graphical user interface     |
| Database           | SQLite              | Storing user data, game results, and move history    |
| Authentication     | bcrypt (C++ library)| Secure password hashing                              |
| AI Algorithm       | Minimax             | Powering AI opponent logic (with difficulty levels)  |
| Testing Framework  | Google Test         | Unit testing for core functionality                  |
| CI/CD              | GitHub Actions      | Automated testing and build on each push             |
| Build System       | CMake               | Managing cross-platform compilation                  |
| Version Control    | Git + GitHub        | Code collaboration, tracking, and publishing         |

## Documentation

- **SRS Document :** [Software Requirements Specification (SRS)](docs/SRS.pdf)

- **SDS Document :** [Software Design Specification (SDS)](docs/SDS.pdf)
  
- **Testing Document :** [Testing Report](docs/testing%20report.pdf)

## Installation & Running

### Option 1: Build from Source (Developer Mode)


Please refer to the  [Installation Guide](#-installation-guide-for-developers)  section below for full steps on building the project using Qt, CMake, and other dependencies.

### Option 2: Run Executable (User Mode)

To download the the executable file of the game [download](https://github.com/MohGendy/tic-tac-toe/actions/runs/15898027166/artifacts/3408836376)


---


## 🛠 Installation Guide (For Developers)

This section explains how to build and run the project from source.



### 🔧 Prerequisites

Make sure you have the following tools installed:

| Tool              | Purpose                            |
|------------------|------------------------------------|
| C++ Compiler      | Supports C++17 (e.g., GCC, Clang)  |
| Qt 6.x            | GUI framework                      |
| CMake ≥ 3.16      | Build system                       |
| Git               | Cloning the project                |
| Google Test       | (Optional) Unit testing            |

Note : SQLite and bcrypt are already included in the project repository. You do not need to install them separately.

### Build Instructions
#### clone Repository

```sh
git clone https://github.com/MohGendy/tic-tac-toe.git
cd tic-tac-toe
```
#### If Qt is not found
```sh
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64/lib/cmake" ..
```
#### To Build Use Next Commands

```sh
cd ./build  
make clean  
cmake .. -G "MinGW Makefiles"  
cmake --build .
```
#### after building run the excutable

```sh
my_program.exe
```
#### To Run Tests

```sh
runTests.exe
```
#### steps to pack

```sh
cmake --build .
cmake --install . --prefix "C:\path\to\install\directory"
cpack
```



## Game snippets
### login window

![Image](https://github.com/user-attachments/assets/9b5d1514-7687-434d-9a9a-9f5a0e8b0939)

### Main window

![Image](https://github.com/user-attachments/assets/7f66f4ea-fb82-4f62-bc15-3b60e3f89428)


### Select to play the classic mode(3x3) or mega mode window

![Image](https://github.com/user-attachments/assets/e6933bc7-0813-4d77-aba8-2516f4a99932)

### Classic mode window (player Vs player)  

![Image](https://github.com/user-attachments/assets/a9572724-840b-46a6-b5d5-35e77d7f1562)




### The mega game window (player Vs Player)

![Image](https://github.com/user-attachments/assets/52bd9e2c-3e20-4605-86f2-bae233917793)

### Choose the AI difficulty window

![Image](https://github.com/user-attachments/assets/500bb3cf-8acd-49a9-b725-df8118842b03)

### Show the history and stats of the player window

![Image](https://github.com/user-attachments/assets/ac4654ba-fa4b-4b6d-b864-f1eb1f167695)

![Image](https://github.com/user-attachments/assets/0301ff01-2000-48f4-8623-de890e661a07)

### Replay game window

![Image](https://github.com/user-attachments/assets/c4317719-af80-424d-a06d-a271d5dff0b7)

### Database snippets

![Image](https://github.com/user-attachments/assets/1c59bd4e-8498-44b0-89bc-b41dd6fb97c3)

![Image](https://github.com/user-attachments/assets/8777ed8c-5e09-4706-8195-e81055156b0a)

## Contributors

Basma Khaled  [GitHub Profile](https://github.com/basmakh45)

Yasmeen Haitham [GitHub Profile](https://github.com/Yasmeen283)

Omar Ahmed  [GitHub Profile](https://github.com/OmarAhmed2772004)

Muhamed Sami  [GitHub Profile](https://github.com/muhameddsami)

Zeyad Mustafa  [GitHub Profile](https://github.com/zeyad2411)

Muhamed Gendy [GitHub Profile](https://github.com/MohGendy)




