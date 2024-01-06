# Centipede Game

## Overview

This project represents the culmination of my efforts in the first semester of the 'Programming Fundamentals' course at my university. I am delighted to present a contemporary remake of the classic Centipede game. I easily spent more than 100 hours on this project but result was quite pleasing for me at this stage

## How to Run

To execute the code, enter the following command in the terminal:

```bash
g++ -c Centipede.cpp && g++ Centipede.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system && ./sfml-app
```

Ensure that you have downloaded SFML.

## Features

- All creatures from the original Centipede are present in this game.
- Pressing Shift while playing the game will activate a speed booster for both the player and bullets.
- The game is divided into three difficulty levels (Beginner, Intermediate, and Advanced) which can be selected in the main menu.
- The game includes an escape menu with resume, restart, settings, help, return to main, and exit game functionalities.
- The settings section allows users to toggle music and sound effects and adjust their volume.

## Additional Features

- Laser hits on a centipede segment result in the segment splitting into two pieces, each continuing independently.
- The centipede starts at a random location on the screen and travels to the left side.
- Once the centipede reaches the bottom of the screen, it moves back and forth within the player area, with one-segment "head" centipedes periodically appearing from the side.
- If the fighter hits the centipede with the laser, it creates a poisonous mushroom. Contact with the poisonous mushroom results in the fighter's demise.
- The speed of the centipede doubles in each advanced level.
- If a centipede fragment consumes 5 mushrooms without being hit by the laser, its size doubles.
- Upon destroying all centipede segments, a new level begins, introducing a new centipede at the top of the screen and additional mushrooms.
- Each level features 20% more mushrooms than the previous level, with higher levels covering the upper half of the screen entirely in mushrooms, causing the centipede to descend rapidly.
- The first-level centipede has 12 segments, and each subsequent level detaches one segment, creating a separate fast-moving "head" centipede.

Enjoy the game!
