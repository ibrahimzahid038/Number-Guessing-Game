#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const string BEST_SCORE_FILE = "best_score.txt";
const int NO_BEST = 1000000000;  
int loadBestScore() {
    ifstream in(BEST_SCORE_FILE.c_str());
    int best;
    if (in >> best && best > 0) {
        in.close();
        return best;
    }
    return NO_BEST;
} 
void saveBestScore(int best) {
    ofstream out(BEST_SCORE_FILE.c_str());
    if (out) {
        out << best << '\n';
        out.close();
    }
} 
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Invalid input. Please enter an integer.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
 
char readYesNo(const string& prompt) {
    char c;
    while (true) {
        cout << prompt;
        if (cin >> c) {
            cin.ignore(10000, '\n');
            if (c == 'y' || c == 'Y' || c == 'n' || c == 'N')
                return c;
        }
        cout << "Please enter 'y' or 'n'.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
 
int computeDefaultAttempts(long long rangeSize) { 
    if (rangeSize <= 1) return 3;
    long long p = 1;
    int count = 0;
    while (p < rangeSize && count < 60) { 
        p *= 2;
        count++;
    }
    if (count < 3) count = 3;
    return count + 1;  
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  
    cout << "=== Number Guessing Game ===\n\n";

    int bestScore = loadBestScore();
    if (bestScore == NO_BEST) {
        cout << "Best score: N/A (no previous successful game)\n\n";
    }
    else {
        cout << "Best score (fewest attempts): " << bestScore << "\n\n";
    }

    bool playAgain = true;
    while (playAgain) {
        int minVal, maxVal;
        while (true) {
            minVal = readInt("Enter minimum value of range: ");
            maxVal = readInt("Enter maximum value of range: ");
            if (minVal < maxVal) break;
            cout << "Minimum must be less than maximum. Try again.\n";
        }

        long long rangeSize = static_cast<long long>(maxVal) - static_cast<long long>(minVal) + 1;

        cout << "Enter maximum number of guesses allowed (enter 0 for recommended default): ";
        int maxAttempts;
        while (true) {
            if (cin >> maxAttempts) {
                cin.ignore(10000, '\n');
                if (maxAttempts >= 0) break;
            }
            else {
                cout << "Invalid input. Please enter a non-negative integer: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        if (maxAttempts == 0) {
            maxAttempts = computeDefaultAttempts(rangeSize);
            cout << "Using recommended attempts: " << maxAttempts << "\n";
        }
         
        int secret;
        if (rangeSize <= RAND_MAX) {
            secret = (rand() % static_cast<int>(rangeSize)) + minVal;
        }
        else { 
            long long r = (static_cast<long long>(rand()) << 15) ^ rand();
            secret = static_cast<int>((r % rangeSize) + minVal);
        }

        cout << "\nI've chosen a number between " << minVal << " and " << maxVal << ".\n";
        cout << "You have " << maxAttempts << " attempts to guess it.\n";

        bool guessed = false;
        int attemptsUsed = 0;

        while (attemptsUsed < maxAttempts && !guessed) {
            attemptsUsed++;
            string prompt = "Attempt " + to_string(attemptsUsed) + " - Enter your guess: ";
            int guess = readInt(prompt);

            if (guess < minVal || guess > maxVal) {
                cout << "Your guess is outside the chosen range. Try a number between "
                    << minVal << " and " << maxVal << ".\n";
            }

            if (guess == secret) {
                cout << "Congratulations! You guessed the correct number in " << attemptsUsed
                    << (attemptsUsed > 1 ? " attempts!\n" : " attempt!\n");
                guessed = true;

                if (attemptsUsed < bestScore) {
                    bestScore = attemptsUsed;
                    saveBestScore(bestScore);
                    cout << "New best score! (" << bestScore << " attempts)\n";
                }
                else if (bestScore != NO_BEST) {
                    cout << "Best score remains " << bestScore << ".\n";
                }
                else {
                    bestScore = attemptsUsed;
                    saveBestScore(bestScore);
                    cout << "Best score recorded: " << bestScore << " attempts\n";
                }
                break;
            }
            else if (guess < secret) {
                cout << "Too low.\n";
            }
            else {
                cout << "Too high.\n";
            }

            int attemptsLeft = maxAttempts - attemptsUsed;
            if (!guessed && attemptsLeft > 0) {
                cout << attemptsLeft << " attempt" << (attemptsLeft > 1 ? "s" : "") << " left.\n";
            }
        }

        if (!guessed) {
            cout << "\nYou've run out of attempts. The correct number was: " << secret << "\n";
            if (bestScore != NO_BEST)
                cout << "Best score (fewest attempts from previous games): " << bestScore << "\n";
            else
                cout << "No recorded best score yet.\n";
        }

        char c = readYesNo("\nPlay again? (y/n): ");
        playAgain = (c == 'y' || c == 'Y');
        cout << "\n";
    }

    cout << "Thanks for playing! Final best score: ";
    if (bestScore == NO_BEST) cout << "N/A\n";
    else cout << bestScore << " attempts\n";
    return 0;
}
