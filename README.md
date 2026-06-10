# Trail of Tinghir - 2D Endless Runner Game

A 2D endless runner game developed in **C++** using the **SFML 2.6** library. This project was created as part of the Object-Oriented Programming (OOP) course at the Faculty of Sciences Semlalia (Cadi Ayyad University).

## 📖 Project Overview

Trail of Tinghir is inspired by the real-life **Trail de Tinghir** event in Morocco. Players must avoid obstacles, survive for a limited time, and progress through two unique levels inspired by Moroccan landscapes.

### Level 1
🏜️ Dadès Oasis

### Level 2
🏙️ Marrakech City

Each level introduces different backgrounds, obstacles, and difficulty settings.

---

## 🎮 Gameplay

The objective is simple:

- Avoid obstacles
- Survive until the timer reaches zero
- Keep your lives as long as possible

### Controls

| Key | Action |
|------|---------|
| Space / ↑ | Jump |
| ↓ | Slide |
| P | Pause / Resume |

### Features

- 2 playable levels
- Infinite scrolling background
- Obstacle spawning system
- Life management system
- Real-time HUD
- Pause functionality
- Player animations
- Object Pool optimization

---

## 🏗️ Project Architecture

The game is built using Object-Oriented Programming principles.

### Main Classes

#### Game
Responsible for:
- Main game loop
- Window management
- State management
- Event handling
- Rendering

#### Player
Responsible for:
- Movement
- Jumping
- Sliding
- Animations
- Collision interactions

#### ObstacleManager
Responsible for:
- Obstacle generation
- Object Pool management
- Memory optimization

#### Background
Responsible for:
- Infinite scrolling effect
- Level backgrounds

#### HUD
Displays:
- Score
- Lives
- Remaining time
- Progress bar
- Current speed

---

## ⚙️ Technologies Used

- C++
- SFML 2.6.0
- Object-Oriented Programming
- Object Pool Pattern

---

## 🧠 OOP Concepts Applied

This project demonstrates:

- Encapsulation
- Inheritance
- Polymorphism
- Modular design
- Resource management
- Real-time event processing

---

## 📸 Screenshots

Add your screenshots here:

### Main Menu

![Menu](screenshots/menu.png)

### Gameplay

![Gameplay](screenshots/gameplay.png)

### Level 1 - Dadès Oasis

![Level1](screenshots/level1.png)

### Level 2 - Marrakech

![Level2](screenshots/level2.png)

---

## 🚀 Installation

### Requirements

- C++17 or later
- SFML 2.6.0
- CMake (optional)

### Clone the Repository

```bash
git clone https://github.com/your-username/trail-of-tinghir.git
cd trail-of-tinghir
