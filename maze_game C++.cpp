#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <conio.h>
#include <unordered_map>
#include <chrono>
#include <numeric> // Added header for accumulate

using namespace std;

const int MAP_WIDTH = 34;
const int MAP_HEIGHT = 20;

vector<vector<string>> mazes = {
    {


        "+##############################+",
        "|                              |",
        "|## ####### ########## ########|",
        "|   |                          |",
        "| | | |   # # ####  # # #  ### |",
        "| | ###   | |       | | |  | | |",
        "| |       | | ####  | | |  | | |",
        "| | ##### | | |     | ###  | | |",
        "| |           ####  |      | | |",
        "| |##### ##         # ###  | | |",
        "|               |              |",
        "|###   #  #######  ####### ####|",
        "|      |        |              |",
        "|# ### ####  ####### ######### |",
        "|      |                       |",
        "|### ####### ########  #### ###|",
        "|          |           |       |",
        "| ################ ##### ######|",
        "|                              |",
        "+##############################+",
    },
    {


        "+##############################+",
        "|                              |",
        "|########## ########## ####### |",
        "|        |                     |",
        "| ###### | ##### ##### ### ### |",
        "|    |   |       |       |   | |",
        "|    ## ####### # ####### ###  |",
        "|     |      |       |         |",
        "|####### ##### ### ##### ##### |",
        "|                            | |",
        "| ############################ |",
        "|              |               |",
        "| ############ ###### #########|",
        "|           |     |            |",
        "|########## ##### #### ####### |",
        "|                    |         |",
        "| ### ### # # ############ ### |",
        "| | | |   | |   |   |   |   |  |",
        "| |     |   # |   |   |   |    |",
        "+##############################+",
    },

    {
        "+##############################+",
        "|                 |            |",
        "|## #####| ############### ####|",
        "|   |    | |      |            |",
        "| | | |  | | #### # #########  |",
        "| | ###  | |    | | |       |  |",
        "| |      | #### | | | #######  |",
        "| #######| |    | | | |        |",
        "|        | #### | | | | #######|",
        "| ########      | | | |        |",
        "|          ###### | | ######## |",
        "|###  #    |          |        |",
        "|     |    ##### # ########### |",
        "|# ####    |   | |       |     |",
        "|     |      |   | ####### ####|",
        "|### ####### ##### |           |",
        "|      |         | ########### |",
        "| ###### ######### |   |   |   |",
        "|                |   |   |   | |",
        "+##############################+",
    },

    {
        "+##############################+",
        "|           |                  |",
        "|####### #### ####### #######  |",
        "|     |     | |       |        |",
        "| ### | ### | | ##### | #####  |",
        "| | | | |   |   |   | |     |  |",
        "|   ####### #  #### # #######  |",
        "| |            |               |",
        "| #########| | #### ########## |",
        "| |        | | |             | |",
        "| #########| | ############# # |",
        "| |        | |                 |",
        "| #########| ###### ###########|",
        "| |        |      |            |",
        "| # ########### #### ######### |",
        "| |              |       |     |",
        "| ### ######## # ############# |",
        "| |   |      |     |         | |",
        "|   |     |    |       |       |",
        "+##############################+"
    },

      {

        "+##############################+",
        "|                              |",
        "|## ####### ########## ########|",
        "|   |                          |",
        "| | | |   # # ####  # # #  ### |",
        "| | ###   | |       | | |  | | |",
        "| |       | | ####  | | |  | | |",
        "| | ##### | | |     | ###  | | |",
        "| |           ####  |      | | |",
        "| |##### ##         # ###  | | |",
        "|               |              |",
        "|###   #  #######  ####### ####|",
        "|      |        |              |",
        "|# ### ####  ####### ######### |",
        "|      |                       |",
        "|### ####### ########  #### ###|",
        "|          |           |       |",
        "| ################ ##### ######|",
        "|                              |",
        "+##############################+",

    },

};


char map[MAP_HEIGHT][MAP_WIDTH];

void selectRandomMaze()
{
    int randomIndex = rand() % mazes.size();
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        strcpy(map[i], mazes[randomIndex][i].c_str());
    }
}

void ShowMap()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        printf("%s\n", map[i]);
    }
}

void gotoxy(short x, short y)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};

    SetConsoleCursorPosition(hStdout, position);
}

class Position
{
public:
    int x;
    int y;

    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}

    bool operator==(const Position &other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position &other) const
    {
        return !(*this == other);
    }

    // Hash function for Position to use in unordered_map
    struct HashFunction
    {
        size_t operator()(const Position &pos) const
        {
            return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.y);
        }
    };
};

class Entity
{
public:
    Entity(int x, int y) : pos(x, y) {}

    void move(int dx, int dy)
    {
        if (map[pos.y + dy][pos.x + dx] == ' ')
        {
            pos.x += dx;
            pos.y += dy;
        }
    }

    int getX() const { return pos.x; }
    int getY() const { return pos.y; }

    void draw(char symbol)
    {
        gotoxy(pos.x, pos.y);
        printf("%c", symbol);
    }

private:
    Position pos;
};

struct Node
{
    Position pos;
    int cost;
    int priority;

    bool operator>(const Node &other) const
    {
        return priority > other.priority;
    }
};

vector<Position> reconstructPath(unordered_map<Position, Position, Position::HashFunction> &cameFrom, Position &current)
{
    vector<Position> totalPath = {current};
    while (cameFrom.find(current) != cameFrom.end())
    {
        current = cameFrom[current];
        totalPath.push_back(current);
    }
    reverse(totalPath.begin(), totalPath.end());
    return totalPath;
}

vector<Position> AStarSearch(const Position &start, const Position &goal)
{
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    openSet.push({start, 0, 0});

    unordered_map<Position, Position, Position::HashFunction> cameFrom;
    unordered_map<Position, int, Position::HashFunction> costSoFar;
    costSoFar[start] = 0;

    while (!openSet.empty())
    {
        Position current = openSet.top().pos;
        openSet.pop();

        if (current == goal)
        {
            return reconstructPath(cameFrom, current);
        }

        vector<Position> neighbors = {
            {current.x + 1, current.y}, {current.x - 1, current.y},
            {current.x, current.y + 1}, {current.x, current.y - 1}};

        for (Position next : neighbors)
        {
            if (map[next.y][next.x] != ' ' && map[next.y][next.x] != 'H')
            {
                continue;
            }

            int newCost = costSoFar[current] + 1;
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next])
            {
                costSoFar[next] = newCost;
                int priority = newCost + abs(next.x - goal.x) + abs(next.y - goal.y);
                openSet.push({next, newCost, priority});
                cameFrom[next] = current;
            }
        }
    }

    return {};
}

vector<Position> GreedySearch(const Position &start, const Position &goal)
{
    queue<Node> openSet;
    openSet.push({start, 0, abs(start.x - goal.x) + abs(start.y - goal.y)});

    unordered_map<Position, Position, Position::HashFunction> cameFrom;
    unordered_map<Position, int, Position::HashFunction> costSoFar;
    costSoFar[start] = 0;

    while (!openSet.empty())
    {
        Position current = openSet.front().pos;
        openSet.pop();

        if (current == goal)
        {
            return reconstructPath(cameFrom, current);
        }

        vector<Position> neighbors = {
            {current.x + 1, current.y}, {current.x - 1, current.y},
            {current.x, current.y + 1}, {current.x, current.y - 1}};

        for (Position next : neighbors)
        {
            if (map[next.y][next.x] != ' ' && map[next.y][next.x] != 'H')
            {
                continue;
            }

            int newCost = abs(next.x - goal.x) + abs(next.y - goal.y);
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next])
            {
                costSoFar[next] = newCost;
                openSet.push({next, newCost, newCost});
                cameFrom[next] = current;
            }
        }
    }

    return {};
}

const int totalTrials = 100;
int successfulTrials = 0;
const int timeLimit = 300; // Example time limit in game frames or moves

void initializeGame(int &x, int &y, int &ex, int &ey, int &pts, int &timer, int &frame, bool &running, bool &paused, bool &useAStar, int &speedmod)
{
    // Select a random maze
    selectRandomMaze();

    // Random spawn points for hero and eater
    do {
        x = rand() % (MAP_WIDTH - 2) + 1;
        y = rand() % (MAP_HEIGHT - 2) + 1;
    } while (map[y][x] != ' ');

    do {
        ex = rand() % (MAP_WIDTH - 2) + 1;
        ey = rand() % (MAP_HEIGHT - 2) + 1;
    } while (map[ey][ex] != ' ' || (abs(x - ex) + abs(y - ey) < 10));

    pts = 0;
    timer = 0;
    frame = 0;
    running = true;
    paused = false;

    // Difficulty settings
    printf("Instructions:\n1. Arrow Keys to move your hero\n2. Eat the dots produced by the Eater to gain points\n3. Don't get caught by the Eater\n\n");
    printf("H -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

    char diffi;
    cin >> diffi;
    if (diffi == 'H' || diffi == 'h')
    {
        speedmod = 1;
        useAStar = true;
    }
    else
    {
        speedmod = 3;
        useAStar = false;
    }

    system("cls");
    ShowMap();

    gotoxy(x, y);
    cout << "H";
}

bool gameStep(int &x, int &y, int &ex, int &ey, int &pts, int &timer, int &frame, bool &useAStar, int speedmod,
              vector<int> &moveTimes, vector<int> &pathLengths, int &totalMoves)
{
    static chrono::time_point<chrono::steady_clock> start, end;
    static int moveTime;

    if (_kbhit())
    {
        char key = _getch();
        if (key == 'P' || key == 'p')
        {
            return false; // Pause the game
        }
    }

    gotoxy(x, y);
    cout << " ";

    int old_x = x;
    int old_y = y;

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if (map[y - 1][x] == ' ')
            y--;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if (map[y + 1][x] == ' ')
            y++;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if (map[y][x - 1] == ' ')
            x--;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if (map[y][x + 1] == ' ')
            x++;
    }

    gotoxy(x, y);
    cout << "P";

    if (x == ex && y == ey)
    {
        end = chrono::steady_clock::now();
        moveTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        moveTimes.push_back(moveTime);
        totalMoves++;
        return true; // Player caught by Eater
    }

    if (frame % speedmod == 0)
    {
        gotoxy(ex, ey);
        cout << "o";
        vector<Position> path;
        Position eaterPos(ex, ey);
        Position heroPos(x, y);
        start = chrono::steady_clock::now();
        if (useAStar)
        {
            path = AStarSearch(eaterPos, heroPos);
        }
        else
        {
            path = GreedySearch(eaterPos, heroPos);
        }
        end = chrono::steady_clock::now();
        moveTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        moveTimes.push_back(moveTime);
        pathLengths.push_back(path.size());
        totalMoves++;

        if (!path.empty())
        {
            gotoxy(ex, ey);
            cout << ".";

            ex = path[1].x;
            ey = path[1].y;
        }

        gotoxy(ex, ey);
        cout << "E";
    }

    if (old_x != x || old_y != y)
    {
        pts++;
    }

    gotoxy(34, 1);
    printf("Points : %d", pts);
    gotoxy(34, 2);
    printf("Timer  : %d", timer);

    frame++;
    if (frame % 10 == 0)
    {
        timer++;
    }

    return false;
}

void displayMenu()
{
    printf("\nGame Over!\n");
    printf("1. Restart\n");
    printf("2. Quit\n");
    printf("Choose an option: ");
}

void displayMetrics(const std::vector<int> &moveTimes, const std::vector<int> &pathLengths, int totalMoves, int timer)
{
    double totalMoveTime = std::accumulate(moveTimes.begin(), moveTimes.end(), 0.0);
    double avgPathLength = std::accumulate(pathLengths.begin(), pathLengths.end(), 0.0) / pathLengths.size();

    double avgMoveTime = static_cast<double>(totalMoves) / timer;

    printf("\nTime Metrics:\n");
    printf("Time to Catch Player: %d seconds\n", timer);
    printf("Average Time Per Move: %.2f milliseconds\n", avgMoveTime);

    printf("\nPath Metrics:\n");
    printf("Number of Moves: %d\n", totalMoves);
    printf("Average Path Length: %.2f\n", avgPathLength);
}

int main()
{
    int x, y, ex, ey, pts, timer, frame;
    bool running, paused, useAStar = false;
    int speedmod;
    srand(time(NULL));

    while (true)
    {
        initializeGame(x, y, ex, ey, pts, timer, frame, running, paused, useAStar, speedmod);

        vector<int> moveTimes;
        vector<int> pathLengths;
        int totalMoves = 0;

        while (running)
        {
            bool gameOver = gameStep(x, y, ex, ey, pts, timer, frame, useAStar, speedmod, moveTimes, pathLengths, totalMoves);
            if (gameOver)
            {
                printf("\nGame Over! You were caught by the Eater.\n");
                running = false;
            }

            if (timer >= timeLimit)
            {
                printf("\nTime's up! You survived for %d seconds.\n", timeLimit);
                running = false;
                successfulTrials++; // Counting a successful trial if the player survives till the time limit.
            }

            Sleep(100); // Control game speed
        }

        displayMetrics(moveTimes, pathLengths, totalMoves, timer);

        displayMenu();
        int choice;
        cin >> choice;
        if (choice == 2)
        {
            break; // Quit the game
        }

        system("cls");
    }

    return 0;
}

