# Overview

As a software engineer, I wanted to deepen my understanding of C++ by building something practical that touches the core parts of the language: working with strings, reading and writing files, and organizing data in containers. Rather than studying each concept in isolation, I set out to build a complete program where those pieces have to work together.

The software I wrote is a **text file analyzer** that runs in the terminal. It reads a text file (including an empty one), counts the total number of words and the number of different words, and displays statistics such as line counts, character counts, the longest word, and the ten most frequently used words. It can also search the document for any word or phrase, and it lets you add, edit, and delete lines, showing the word count before and after each change. This means it can be used to check your word count, keep writing, and check again at the end. Changes can be saved back to the file.

My purpose for writing this software was to practice the C++ fundamentals in a realistic project: `std::string` handling, file input and output with `fstream`, `std::vector` and `std::map`, structs, functions, references, and basic input validation. I also wanted a tool that is genuinely useful for tracking progress on anything I'm writing.

[Software Demo Video](https://youtu.be/X5AMQaq9hp8)

# Development Environment

- **Editor:** Visual Studio Code
- **Compiler:** g++ (MinGW-w64, installed through MSYS2) on Windows
- **Terminal:** PowerShell, inside VS Code's integrated terminal
- **Language:** C++ (C++17 standard)
- **Libraries:** Only the C++ Standard Library:
  - `<iostream>` for keyboard input and screen output
  - `<fstream>` for reading and writing text files
  - `<string>` and `<sstream>` for working with text and splitting lines into words
  - `<vector>` and `<map>` for storing the document and counting word frequencies
  - `<algorithm>` for sorting the most-used words
  - `<cctype>` for character checks and converting text to lowercase

To build and run:

```
g++ -std=c++17 -o file_analyzer file_analyzer.cpp
.\file_analyzer.exe myfile.txt
```

# Useful Websites

- [cppreference.com](https://en.cppreference.com)
- [LearnCpp.com](https://www.learncpp.com)
- [VS Code: Using GCC with MinGW](https://code.visualstudio.com/docs/cpp/config-mingw)
- [MSYS2](https://www.msys2.org)

# Future Work

- Add a "replace word" feature to change every occurrence of a word at once
- Add an option to search for whole words only, instead of matching any text
- Let the user insert a new line at a chosen position, not just at the end
- Add an undo option for edits and deletions
- Add a "Save As" option and an automatic backup before overwriting a file
- Let the user filter common words (like "the" and "and") out of the top-words list
- Add a writing goal feature that shows progress toward a target word count
- Improve support for Unicode text and accented characters