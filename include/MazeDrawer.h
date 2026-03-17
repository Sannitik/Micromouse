#pragma once

#include <Arduino.h>
#include "Maze.h"
#include "Solver.h"

#define ICON_WALL_VERTICAL "!"
#define ICON_WALL_HORIZONTAL "---"
#define ICON_UNKNOWN_HORIZONTAL " . "
#define ICON_UNKNOWN_VERTICAL "."
#define ICON_OPEN_HORIZONTAL "   "
#define ICON_OPEN_VERTICAL " "

// Function to draw the maze using ASCII art
void draw_maze(int width, int height) {
  // Iterate through each row
  for (int y = 0; y < height; y++) {
    // Draw the top walls of the cells in the current row
    for (int x = 0; x < width; x++) {
      Maze::CellWalls cell = maze.getWalls(Vec2{x, y});
      Serial.print("+");
      if (cell.north == Maze::WALL) {
        Serial.print(ICON_WALL_HORIZONTAL);
      } else if (cell.north == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_HORIZONTAL);
      } else {
        Serial.print(ICON_OPEN_HORIZONTAL);
      }
    }
    Serial.println("+");

    // Draw the left and right walls of the cells in the current row
    for (int x = 0; x < width; x++) {
      Maze::CellWalls cell = maze.getWalls(Vec2{x, y});
      if (cell.west == Maze::WALL) {
        Serial.print(ICON_WALL_VERTICAL);
      } else if (cell.west == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_VERTICAL);
      } else {
        Serial.print(ICON_OPEN_VERTICAL);
      }
      Serial.print(ICON_OPEN_HORIZONTAL);  // Space for the cell content
    }
    // Draw the rightmost wall of the last cell in the row
    Maze::CellWalls lastCell = maze.getWalls(Vec2{width - 1, y});
    if (lastCell.east == Maze::WALL) {
      Serial.println(ICON_WALL_VERTICAL);
    } else if (lastCell.east == Maze::UNKNOWN) {
      Serial.println(ICON_UNKNOWN_VERTICAL);
    } else {
      Serial.println(ICON_OPEN_VERTICAL);
    }
  }

  // Draw the bottom walls of the last row
  for (int x = 0; x < width; x++) {
    Maze::CellWalls cell = maze.getWalls(Vec2{x, height - 1});
    Serial.print("+");
    if (cell.south == Maze::WALL) {
      Serial.print(ICON_WALL_HORIZONTAL);
    } else if (cell.south == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_HORIZONTAL);
    } else {
      Serial.print(ICON_OPEN_HORIZONTAL);
    }
  }
  Serial.println("+");
}

// Function to draw the maze using ASCII art
void draw_maze_with_solver(int width, int height) {
  // Iterate through each row
  for (int y = 0; y < height; y++) {
    // Draw the top walls of the cells in the current row
    for (int x = 0; x < width; x++) {
      Maze::CellWalls cell = maze.getWalls(Vec2{x, y});
      Serial.print("+");
      if (cell.north == Maze::WALL) {
        Serial.print(ICON_WALL_HORIZONTAL);
      } else if (cell.north == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_HORIZONTAL);
      } else {
        Serial.print(ICON_OPEN_HORIZONTAL);
      }
    }
    Serial.println("+");

    // Draw the left and right walls of the cells in the current row
    for (int x = 0; x < width; x++) {
      Maze::CellWalls cell = maze.getWalls(Vec2{x, y});
      if (cell.west == Maze::WALL) {
        Serial.print(ICON_WALL_VERTICAL);
      } else if (cell.west == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_VERTICAL);
      } else {
        Serial.print(ICON_OPEN_VERTICAL);
      }
      Serial.print(" ");
      Serial.print(solver_get_where_from(Vec2{int16_t(x), int16_t(y)}));  // Space for the cell content
      Serial.print(" ");
    }
    // Draw the rightmost wall of the last cell in the row
    Maze::CellWalls lastCell = maze.getWalls(Vec2{width - 1, y});
    if (lastCell.east == Maze::WALL) {
      Serial.println(ICON_WALL_VERTICAL);
    } else if (lastCell.east == Maze::UNKNOWN) {
      Serial.println(ICON_UNKNOWN_VERTICAL);
    } else {
      Serial.println(ICON_OPEN_VERTICAL);
    }
  }

  // Draw the bottom walls of the last row
  for (int x = 0; x < width; x++) {
    Maze::CellWalls cell = maze.getWalls(Vec2{x, height - 1});
    Serial.print("+");
    if (cell.south == Maze::WALL) {
      Serial.print(ICON_WALL_HORIZONTAL);
    } else if (cell.south == Maze::UNKNOWN) {
        Serial.print(ICON_UNKNOWN_HORIZONTAL);
    } else {
      Serial.print(ICON_OPEN_HORIZONTAL);
    }
  }
  Serial.println("+");
}