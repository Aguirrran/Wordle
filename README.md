# Wordle

A C++ Wordle recommender that narrows down possible answers based on feedback and suggests strong next guesses using an entropy-based strategy. This project also includes a lightweight local web interface so the game can be played more visually instead of only through the terminal.

## Features

- Loads a dictionary of valid five-letter words
- Generates Wordle-style feedback patterns
- Filters remaining candidate words after each guess
- Recommends the next guess using entropy
- Supports both a command-line version and a local web version
- Includes unit tests for Wordle logic and BSTSet functionality

## Tech Stack

- **C++**
- **Custom BSTSet**
- **Google Test / Google Mock**
- **cpp-httplib**
- **HTML, CSS, JavaScript**

## How It Works

1. Load a dictionary of possible words.
2. Choose a guess that provides strong information gain.
3. Receive feedback from the player.
4. Remove impossible words from the remaining set.
5. Repeat until the answer is found.

## Feedback Format

### Command Line
The terminal version uses numeric feedback:

- `2` = green
- `1` = yellow
- `0` = gray

Example:

```txt
22222
