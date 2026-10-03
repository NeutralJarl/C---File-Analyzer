// ============================================================================
// file_analyzer.cpp
//
// A simple text file analyzer.
//   - Reads a text file (works fine if the file is empty)
//   - Counts total words and different (unique) words
//   - Shows character counts, line counts, and the most frequent words
//   - Lets you search for text
//   - Lets you add, edit, and delete lines, then shows the word count
//     before and after so you can track your writing progress
//   - Can save your changes back to the file
//
// How to compile (in the VS Code terminal):
//     g++ -std=c++17 -o file_analyzer file_analyzer.cpp
//
// How to run:
//   in same folder run the .exe with the file name you want to analyze  
//   or just run it without a file name and it will ask you for one.
// ============================================================================

#include <iostream>   // cout, cin (printing and reading from the keyboard)
#include <fstream>    // ifstream, ofstream (reading and writing files)
#include <string>     // string
#include <vector>     // vector (a list that can grow)
#include <map>        // map (stores word -> count pairs)
#include <sstream>    // stringstream (splits a line into words)
#include <algorithm>  // sort, transform
#include <cctype>     // isalnum, tolower, isspace

using namespace std;  // so we don't have to write std:: everywhere


// ----------------------------------------------------------------------------
// A struct is a way to bundle related variables together.
// Used to hold all the results of one analysis.
// ----------------------------------------------------------------------------
struct Stats {
    int totalWords = 0;          // every word, repeats included
    int uniqueWords = 0;         // each different word counted once
    int lineCount = 0;           // number of lines in the document
    int blankLines = 0;          // lines with nothing on them
    int charsWithSpaces = 0;     // all characters (not counting the newline)
    int charsNoSpaces = 0;       // characters that are not spaces/tabs
    string longestWord = "";     // the longest word we found
    map<string, int> frequency;  // word -> how many times it appears
};


// ----------------------------------------------------------------------------
// toLowerCase: turns "Hello" into "hello".
// We do this so "The" and "the" count as the same word.
// ----------------------------------------------------------------------------
string toLowerCase(string text) {
    for (int i = 0; i < (int)text.size(); i++) {
        text[i] = tolower((unsigned char)text[i]);
    }
    return text;
}


// ----------------------------------------------------------------------------
// cleanWord: removes punctuation from the START and END of a word and
// makes it lowercase. Example: "Hello," -> "hello", "(don't)" -> "don't"
// Punctuation in the MIDDLE (like the ' in don't) is kept.
// If nothing is left (like the token "--"), it returns an empty string,
// which means "this wasn't really a word".
// ----------------------------------------------------------------------------
string cleanWord(string word) {
    int start = 0;
    int end = (int)word.size() - 1;

    // Move start forward until we hit a letter or number
    while (start <= end && !isalnum((unsigned char)word[start])) {
        start++;
    }
    // Move end backward until we hit a letter or number
    while (end >= start && !isalnum((unsigned char)word[end])) {
        end--;
    }

    // If start passed end, there were no letters/numbers at all
    if (start > end) {
        return "";
    }

    return toLowerCase(word.substr(start, end - start + 1));
}


// ----------------------------------------------------------------------------
// getWords: splits one line of text into a list of clean words.
// ----------------------------------------------------------------------------
vector<string> getWords(const string& line) {
    vector<string> words;
    stringstream ss(line);  // lets us read the line one "chunk" at a time
    string chunk;

    // The >> operator reads up to the next space, so each chunk is a word
    while (ss >> chunk) {
        string cleaned = cleanWord(chunk);
        if (cleaned != "") {          // skip chunks that were only punctuation
            words.push_back(cleaned);
        }
    }
    return words;
}


// ----------------------------------------------------------------------------
// countWords: quick total word count for the whole document.
// Used to show "before" and "after" numbers when editing.
// ----------------------------------------------------------------------------
int countWords(const vector<string>& lines) {
    int total = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
        total += (int)getWords(lines[i]).size();
    }
    return total;
}


// ----------------------------------------------------------------------------
// analyze: goes through every line and fills in a Stats struct.
// ----------------------------------------------------------------------------
Stats analyze(const vector<string>& lines) {
    Stats stats;
    stats.lineCount = (int)lines.size();

    for (int i = 0; i < (int)lines.size(); i++) {
        const string& line = lines[i];

        // --- Character counts ---
        stats.charsWithSpaces += (int)line.size();
        for (int c = 0; c < (int)line.size(); c++) {
            if (!isspace((unsigned char)line[c])) {
                stats.charsNoSpaces++;
            }
        }

        // --- Word counts ---
        vector<string> words = getWords(line);

        if (words.empty()) {
            stats.blankLines++;   // no words on this line
        }

        for (int w = 0; w < (int)words.size(); w++) {
            stats.totalWords++;
            stats.frequency[words[w]]++;   // add 1 to this word's count

            if (words[w].size() > stats.longestWord.size()) {
                stats.longestWord = words[w];
            }
        }
    }

    // Every different word is one entry in the map, so its size is
    // the number of unique words.
    stats.uniqueWords = (int)stats.frequency.size();
    return stats;
}


// ----------------------------------------------------------------------------
// printTopWords: shows the most frequently used words.
// ----------------------------------------------------------------------------
void printTopWords(const Stats& stats, int howMany) {
    if (stats.frequency.empty()) {
        cout << "  (no words to show)\n";
        return;
    }

    // A map can't be sorted by value, so we copy everything into a vector
    // of (word, count) pairs and sort that instead.
    vector<pair<string, int> > sorted(stats.frequency.begin(), stats.frequency.end());

    // This "lambda" tells sort() how to compare two entries:
    // higher counts come first, and ties are sorted alphabetically.
    sort(sorted.begin(), sorted.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             if (a.second != b.second) {
                 return a.second > b.second;
             }
             return a.first < b.first;
         });

    // Don't try to print more words than we actually have
    if (howMany > (int)sorted.size()) {
        howMany = (int)sorted.size();
    }

    for (int i = 0; i < howMany; i++) {
        cout << "  " << (i + 1) << ". " << sorted[i].first
             << " (" << sorted[i].second << " time"
             << (sorted[i].second == 1 ? "" : "s") << ")\n";
    }
}


// ----------------------------------------------------------------------------
// printReport: displays the full analysis. Handles an empty document too.
// ----------------------------------------------------------------------------
void printReport(const vector<string>& lines) {
    Stats stats = analyze(lines);

    cout << "\n========== ANALYSIS RESULTS ==========\n";

    // Special message if there's nothing in the document
    if (stats.totalWords == 0) {
        cout << "The document is empty (no words found).\n";
    }

    cout << "Total words:        " << stats.totalWords << "\n";
    cout << "Different words:    " << stats.uniqueWords << "\n";
    cout << "Lines:              " << stats.lineCount
         << " (" << stats.blankLines << " with no words)\n";
    cout << "Characters:         " << stats.charsWithSpaces << " with spaces, "
         << stats.charsNoSpaces << " without\n";

    if (stats.totalWords > 0) {
        cout << "Longest word:       " << stats.longestWord
             << " (" << stats.longestWord.size() << " letters)\n";

        // Average word length = total letters / number of words.
        // charsNoSpaces includes punctuation, so this is only a rough number.
        double average = (double)stats.charsNoSpaces / stats.totalWords;
        cout << "Avg. word length:   " << average << " (approx.)\n";

        cout << "\nTop 10 most used words:\n";
        printTopWords(stats, 10);
    }
    cout << "======================================\n";
}


// ----------------------------------------------------------------------------
// printDocument: shows the document with line numbers.
// ----------------------------------------------------------------------------
void printDocument(const vector<string>& lines) {
    cout << "\n---------- DOCUMENT ----------\n";
    if (lines.empty()) {
        cout << "(empty)\n";
    }
    for (int i = 0; i < (int)lines.size(); i++) {
        cout << (i + 1) << ": " << lines[i] << "\n";
    }
    cout << "------------------------------\n";
}


// ----------------------------------------------------------------------------
// loadFile: reads the file into our vector of lines.
// Returns true if the file opened, false if it couldn't be opened.
// ----------------------------------------------------------------------------
bool loadFile(const string& filename, vector<string>& lines) {
    ifstream file(filename);   // try to open the file for reading

    if (!file.is_open()) {
        return false;          // couldn't open it (maybe it doesn't exist)
    }

    lines.clear();             // throw away anything already stored
    string line;
    while (getline(file, line)) {   // read one full line at a time
        lines.push_back(line);
    }

    file.close();
    return true;
}


// ----------------------------------------------------------------------------
// saveFile: writes all our lines back into the file.
// ----------------------------------------------------------------------------
bool saveFile(const string& filename, const vector<string>& lines) {
    ofstream file(filename);   // this replaces the old contents of the file

    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < (int)lines.size(); i++) {
        file << lines[i] << "\n";
    }

    file.close();
    return true;
}


// ----------------------------------------------------------------------------
// readNumber: asks for a number and keeps asking until the user types one.
// We read the whole line as text first, then convert it. This avoids the
// classic problem where typing letters into cin >> int breaks the program.
// ----------------------------------------------------------------------------
int readNumber(const string& prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);

        try {
            return stoi(input);   // stoi = "string to int"
        } catch (...) {
            cout << "Please type a whole number.\n";
        }
    }
}


// ----------------------------------------------------------------------------
// searchText: finds every line containing the search text (ignoring case)
// and reports how many times it was found.
// ----------------------------------------------------------------------------
void searchText(const vector<string>& lines) {
    cout << "Search for: ";
    string target;
    getline(cin, target);

    if (target == "") {
        cout << "Nothing to search for.\n";
        return;
    }

    string lowerTarget = toLowerCase(target);
    int totalMatches = 0;

    for (int i = 0; i < (int)lines.size(); i++) {
        string lowerLine = toLowerCase(lines[i]);

        // Count how many times the target appears in this line
        int matchesOnLine = 0;
        size_t position = lowerLine.find(lowerTarget);   // npos means "not found"
        while (position != string::npos) {
            matchesOnLine++;
            // Search again starting just after the match we found
            position = lowerLine.find(lowerTarget, position + 1);
        }

        if (matchesOnLine > 0) {
            cout << "  Line " << (i + 1) << " (" << matchesOnLine << "x): "
                 << lines[i] << "\n";
            totalMatches += matchesOnLine;
        }
    }

    if (totalMatches == 0) {
        cout << "\"" << target << "\" was not found.\n";
    } else {
        cout << "Found \"" << target << "\" " << totalMatches << " time(s).\n";
    }
}


// ----------------------------------------------------------------------------
// addText: lets the user keep typing new lines onto the end of the document.
// Shows the word count before and after.
// Returns true if anything was added (so main knows there are unsaved changes).
// ----------------------------------------------------------------------------
bool addText(vector<string>& lines) {
    int before = countWords(lines);
    cout << "Word count before: " << before << "\n";
    cout << "Type your text. Press Enter for a new line.\n";
    cout << "Type .done on its own line when you are finished.\n";

    bool addedSomething = false;
    string line;
    while (true) {
        getline(cin, line);
        if (line == ".done") {
            break;                 // stop when the user types .done
        }
        lines.push_back(line);
        addedSomething = true;
    }

    int after = countWords(lines);
    cout << "Word count after:  " << after
         << " (" << (after - before) << " added)\n";
    return addedSomething;
}


// ----------------------------------------------------------------------------
// editLine: replaces one existing line with new text.
// ----------------------------------------------------------------------------
bool editLine(vector<string>& lines) {
    if (lines.empty()) {
        cout << "The document is empty, so there is nothing to edit.\n";
        return false;
    }

    printDocument(lines);
    int number = readNumber("Which line number do you want to edit? ");

    // Line numbers shown to the user start at 1, but vector positions start at 0
    if (number < 1 || number > (int)lines.size()) {
        cout << "That line doesn't exist.\n";
        return false;
    }

    int before = countWords(lines);
    cout << "Current text: " << lines[number - 1] << "\n";
    cout << "New text (type it in full): ";
    string newText;
    getline(cin, newText);

    lines[number - 1] = newText;

    int after = countWords(lines);
    cout << "Word count before: " << before << ", after: " << after << "\n";
    return true;
}


// ----------------------------------------------------------------------------
// deleteLine: removes one line from the document.
// ----------------------------------------------------------------------------
bool deleteLine(vector<string>& lines) {
    if (lines.empty()) {
        cout << "The document is empty, so there is nothing to delete.\n";
        return false;
    }

    printDocument(lines);
    int number = readNumber("Which line number do you want to delete? ");

    if (number < 1 || number > (int)lines.size()) {
        cout << "That line doesn't exist.\n";
        return false;
    }

    int before = countWords(lines);
    lines.erase(lines.begin() + (number - 1));   // remove that one entry
    int after = countWords(lines);

    cout << "Line deleted. Word count before: " << before
         << ", after: " << after << "\n";
    return true;
}


// ----------------------------------------------------------------------------
// printMenu: shows the list of things the user can do.
// ----------------------------------------------------------------------------
void printMenu() {
    cout << "\n=========== MENU ===========\n";
    cout << " 1. Analyze document\n";
    cout << " 2. View document\n";
    cout << " 3. Add text to the end\n";
    cout << " 4. Edit a line\n";
    cout << " 5. Delete a line\n";
    cout << " 6. Search\n";
    cout << " 7. Save\n";
    cout << " 0. Quit\n";
    cout << "============================\n";
}


// ----------------------------------------------------------------------------
// main: where the program starts.
// argc = number of command line arguments, argv = the arguments themselves.
// argv[0] is the program name, argv[1] is the file name if one was given.
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    vector<string> lines;      // the whole document, one entry per line
    string filename;
    bool unsavedChanges = false;

    // --- Step 1: figure out which file to open ---
    if (argc > 1) {
        filename = argv[1];    // file name typed after the program name
    } else {
        cout << "Enter the name of the text file: ";
        getline(cin, filename);
    }

    // --- Step 2: try to load it ---
    if (loadFile(filename, lines)) {
        cout << "Loaded \"" << filename << "\" (" << lines.size() << " lines).\n";
    } else {
        cout << "Could not open \"" << filename << "\".\n";
        cout << "Start a new empty document with that name? (y/n): ";
        string answer;
        getline(cin, answer);

        if (answer == "y" || answer == "Y") {
            // lines is already empty, so we just carry on
            cout << "Starting a new document.\n";
        } else {
            cout << "Goodbye!\n";
            return 1;          // non-zero means "something went wrong"
        }
    }

    // Show the analysis right away, like the assignment describes
    printReport(lines);

    // --- Step 3: the main menu loop ---
    int choice = -1;
    while (choice != 0) {
        printMenu();
        choice = readNumber("Choose an option: ");

        if (choice == 1) {
            printReport(lines);
        } else if (choice == 2) {
            printDocument(lines);
        } else if (choice == 3) {
            if (addText(lines)) {
                unsavedChanges = true;
            }
        } else if (choice == 4) {
            if (editLine(lines)) {
                unsavedChanges = true;
            }
        } else if (choice == 5) {
            if (deleteLine(lines)) {
                unsavedChanges = true;
            }
        } else if (choice == 6) {
            searchText(lines);
        } else if (choice == 7) {
            if (saveFile(filename, lines)) {
                cout << "Saved to \"" << filename << "\".\n";
                unsavedChanges = false;
            } else {
                cout << "Error: could not save the file.\n";
            }
        } else if (choice == 0) {
            // Warn about unsaved work before quitting
            if (unsavedChanges) {
                cout << "You have unsaved changes. Save before quitting? (y/n): ";
                string answer;
                getline(cin, answer);
                if (answer == "y" || answer == "Y") {
                    if (saveFile(filename, lines)) {
                        cout << "Saved to \"" << filename << "\".\n";
                    } else {
                        cout << "Error: could not save the file.\n";
                    }
                }
            }
        } else {
            cout << "That's not a menu option. Try again.\n";
        }
    }

    // --- Step 4: final summary so you can see your finishing word count ---
    cout << "\nFinal word count: " << countWords(lines) << "\n";
    cout << "Goodbye!\n";
    return 0;   // 0 means the program finished normally
}
