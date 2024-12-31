# Seam Carving Project

## Overview

Seam carving is a content-aware image resizing technique that reduces an image's size by removing paths of least importance, known as "seams," while preserving critical features. Unlike traditional methods like cropping or scaling, seam carving maintains the integrity of the image's significant content, making it a popular technique in modern image editing tools. This project is part of ESC190 (Data Structures and Algorithms) at the University of Toronto.

This project implements the seam carving algorithm using C. The implementation includes calculating pixel energy, identifying minimum energy seams, and removing those seams iteratively to resize an image.

---

## Features

1. **Dual-Gradient Energy Function**: Calculates the energy of each pixel in an image based on color gradients, identifying areas of high importance.
2. **Dynamic Seam Identification**: Uses dynamic programming to efficiently find vertical seams with the smallest energy sum.
3. **Seam Removal**: Removes seams from the image, reducing its width without affecting critical visual elements.
4. **Iterative Seam Removal**: Allows repeated seam removal to achieve the desired image dimensions.

---

## Implementation Details

### 1. Dual-Gradient Energy Calculation
The function `calc_energy()` computes the energy of each pixel using the dual-gradient energy function:
$$\text{Energy}(y, x) = \sqrt{\Delta^2_x(y, x) + \Delta^2_y(y, x)}$$
Where:
- $\Delta^2_x$ and $\Delta^2_y$ are the squared color gradients along the x and y axes.
- Wrapping around the edges is handled to ensure seamless calculations.

The computed energy values are stored in a grayscale image, where higher energy pixels are brighter.

---

### 2. Dynamic Seam Identification
The function `dynamic_seam()` constructs a dynamic programming table to compute the minimum cumulative energy for seams from the top to the current row. This table enables efficient seam identification with $O(H \times W)$ time complexity, where $H$ is the height and $W$ is the width of the image.

---

### 3. Seam Recovery
The function `recover_path()` traces the path of the minimum energy seam from the bottom to the top of the image. This seam is represented as an array of x-coordinates, one for each row.

---

### 4. Seam Removal
The function `remove_seam()` removes the identified seam from the image, creating a new image with reduced width. This process is repeated iteratively to achieve the desired resizing.

---

## How to Run

1. **Compilation**: Compile the program with the command:
   ```bash
   gcc -o seam_carving seamcarving.c -lm
   ```

2. **Execution**: Run the program using the test harness provided in the `main()` function. For example:
   ```bash
   ./seam_carving
   ```

3. **Input/Output**: The program reads binary image files (`.bin`) and writes resized binary images after removing seams.

---

## Example

Given an input image `HJoceanSmall.bin`, the program can remove seams iteratively:
- Input Image: $505 \times 287$
- Output Image (after 150 vertical seams): $355 \times 287$

---

## Files

- **`seamcarving.c`**: Implements all core functions.
- **`seamcarving.h`**: Header file defining function prototypes.
- **`rgb_img.h`**: Utility functions for image handling (e.g., reading/writing binary images).
- **`main.c`**: Sample program demonstrating iterative seam removal.

---

## Additional Notes

- **Optimization**: The implementation leverages dynamic programming for efficient seam identification and removal.
- **Applications**: This technique is widely used in image editing software to resize images for web applications and digital displays while preserving essential content.
