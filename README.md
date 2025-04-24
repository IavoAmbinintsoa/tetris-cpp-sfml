# tetris-cpp-sfml

---

# 🎮 TETIRISY — C++ + SFML

A **custom Tetris game**, developed in C++ using the **SFML** graphics library. It handles piece movement, collisions, scoring, best score saving, and a simple graphical interface.

###Version : 0.1
---

## 🧱 Project Structure

### 🔹 Libraries Used

- **SFML**: For graphical rendering.
- **<queue>, <vector>, <iostream>, <fstream>, <cmath>, <random>**: Data structures, I/O, math, and utilities.

### 🔹 Dependencies

- **SFML 2.x**
- Required files:
  - `tetris.png` → Background image
  - `square.png` → Block sprite
  - `Arial.ttf` → Font file

---

## 📂 Key Files

| File          | Role                                      |
|---------------|-------------------------------------------|
| `.back.txt`   | Text file storing the best score          |
| `tetris.png`  | Game board texture                        |
| `square.png`  | Block sprite                              |
| `Arial.ttf`   | Font used for text display                |

---

## ▶️ Running the Game

```bash
g++ -o tetris tetris.cpp -lsfml-graphics -lsfml-window -lsfml-system
./tetris
```

⚠️ Make sure the image and font files are in the correct folder.

---

## 🎮 Game Controls

| Key           | Action                                 |
|---------------|----------------------------------------|
| **←**         | Move the piece left                    |
| **→**         | Move the piece right                   |
| **↓**         | Speed up the descent                   |
| **↑**         | Temporarily slow down the descent      |
| **Space**     | (Not used yet)                         |
| **X**         | Close the window                       |

---

## 🧩 Code Functionality

### 🎲 Piece Generation
- `fillListCell()` creates a list of all shapes (I, O, T, S, Z, L, J) with their rotations.
- `randomcell()` generates a random piece with a color and sprite.

### 🧠 Tetris Logic
- **Collision**: Checked before each movement via `isOnColision()`.
- **Full lines**: Detected and removed using `checkTetrisMatrix()`.
- **Score**: Increased by `+10` per completed line.
- **Game Over**: Triggered if a new piece touches the top row.

### 📝 Score Saving
- On launch, the best score is read from `.back.txt`.
- On Game Over, it is updated if the current score is higher.

---

## 🖼 Graphical Interface

- A `480x480` pixel window with:
  - A background (`tetris.png`)
  - Game cells using `square.png`
  - **Score** and **Best Score** displayed at the bottom right

---
## Model
![model](https://github.com/user-attachments/assets/dd2c0a92-e06b-4bcf-a865-d7ad541abb15)


