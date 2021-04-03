#include "main.h"
#include "timer.h"
#include "ball.h"
#include "maze.h"
#include "player.h"
#include "walls.h"
#include "unistd.h"
#include "common.h"
#include "vaporise.h"
#include "star.h"
#include "powerup.h"
#include <string>
#include <time.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Maze maze;
Player player;
Player impostor;
Vaporise vaporub;
Vaporise drop;
Star star;
Powerup powers[6];
list<Wall> walls;
vector<pair<int, int>> adj[20][20];
int maze_size = 20;
int ticks = 0;
int freq = 20;
int score = 0;
int completed_tasks = 0;
time_t start_time = time(NULL);
list<Edge> edges;
bool imp_live = true;
bool game_end = false;
bool dropped = false;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */

pair<int, int> mod_dfs(int x, int y, pair<int, int> parent)
{
    int value = 1;
    if (x == player.position.x && y == player.position.y)
    {
        value += 1000000;
    }
    vector<pair<int, pair<int, int>>> weights;
    for (int i = 0; i < adj[x][y].size(); i++)
    {
        int nx = adj[x][y][i].first;
        int ny = adj[x][y][i].second;
        if (nx != parent.first || ny != parent.second)
        {
            weights.push_back(make_pair(mod_dfs(nx, ny, make_pair(x, y)).first, make_pair(nx, ny)));
        }
    }

    if (weights.size() == 0)
    {
        return make_pair(value, 0);
    }

    int min = 999999, min_million = 999999999;
    pair<int, int> min_dir = make_pair(-1, -1);
    pair<int, int> min_mil_dir = make_pair(-1, -1);

    for (int i = 0; i < weights.size(); i++)
    {
        if (weights[i].first < min)
        {
            min = weights[i].first;
            min_dir = weights[i].second;
        }
        else if (weights[i].first > 1000000 && weights[i].first < min_million)
        {
            min_million = weights[i].first;
            min_mil_dir = weights[i].second;
        }
    }

    if (min_million != 999999999)
    {
        int dirn = 0;
        if (x < min_mil_dir.first)
        {
            dirn = 1;
        }
        else if (x > min_mil_dir.first)
        {
            dirn = -1;
        }
        else if (y < min_mil_dir.second)
        {
            dirn = 2;
        }
        else if (y > min_mil_dir.second)
        {
            dirn = -2;
        }
        return make_pair(min_million + value, dirn);
    }
    else
    {
        int dirn = 0;
        if (x < min_dir.first)
        {
            dirn = 1;
        }
        else if (x > min_dir.first)
        {
            dirn = -1;
        }
        else if (y < min_dir.second)
        {
            dirn = 2;
        }
        else if (y > min_dir.second)
        {
            dirn = -2;
        }
        return make_pair(min + value, dirn);
    }
}

void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(((float)maze_size - 1) / 2, ((float)maze_size - 1) / 2, 0.31 * (float)maze_size);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target(((float)maze_size - 1) / 2, ((float)maze_size - 1) / 2, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up(0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP; // MVP = Projection * View * Model

    // Scene render
    // ball1.draw(VP);

    maze.draw(VP);
    for (int i = 0; i < 6; i++)
    {
        if (powers[i].value != 0 && !game_end)
        {
            // cout << powers[i].position.x << ' ' << powers[i].position.y << endl;
            powers[i].draw(VP);
        }
    }
    if (imp_live)
    {
        if (!game_end)
        {
            vaporub.draw(VP);
        }
        impostor.draw(VP);
    }
    player.draw(VP);
    if (!dropped && !game_end)
    {
        drop.draw(VP);
    }

    if(completed_tasks == 2 && !game_end)
    {
        star.draw(VP);
    }
}

void end_game(bool win)
{
    game_end = true;
    float fac = (float)maze_size / 20.0;
    if (!win)
    {
        walls = {
            Wall(fac * 4.0, fac * 11.0, fac * 4.85, fac * 10.0, maze_size),
            Wall(fac * 5.7, fac * 11.0, fac * 4.85, fac * 10.0, maze_size),
            Wall(fac * 4.85, fac * 9.0, fac * 4.85, fac * 10.0, maze_size),

            Wall(fac * 6.0, fac * 11.0, fac * 6.0, fac * 9.0, maze_size),
            Wall(fac * 7.7, fac * 11.0, fac * 7.7, fac * 9.0, maze_size),
            Wall(fac * 6.0, fac * 11.0, fac * 7.7, fac * 11.0, maze_size),
            Wall(fac * 6.0, fac * 9.0, fac * 7.7, fac * 9.0, maze_size),

            Wall(fac * 8.0, fac * 11.0, fac * 8.0, fac * 9.0, maze_size),
            Wall(fac * 8.0, fac * 9.0, fac * 9.7, fac * 9.0, maze_size),
            Wall(fac * 9.7, fac * 11.0, fac * 9.7, fac * 9.0, maze_size),

            Wall(10.3, 11.0, 10.3, 9.0, maze_size),
            Wall(10.3, 9.0, 11.3, 9.0, maze_size),

            Wall(fac * 11.6, fac * 11.0, fac * 11.6, fac * 9.0, maze_size),
            Wall(fac * 13.3, fac * 11.0, fac * 13.3, fac * 9.0, maze_size),
            Wall(fac * 11.6, fac * 11.0, fac * 13.3, fac * 11.0, maze_size),
            Wall(fac * 11.6, fac * 9.0, fac * 13.3, fac * 9.0, maze_size),

            Wall(fac * 13.6, fac * 9.0, fac * 14.6, fac * 9.0, maze_size),
            Wall(fac * 14.6, fac * 10.0, fac * 14.6, fac * 9.0, maze_size),
            Wall(fac * 13.6, fac * 10.0, fac * 13.6, fac * 11.0, maze_size),
            Wall(fac * 13.6, fac * 10.0, fac * 14.6, fac * 10.0, maze_size),
            Wall(fac * 13.6, fac * 11.0, fac * 14.6, fac * 11.0, maze_size),

            Wall(fac * 14.9, fac * 11.0, fac * 14.9, fac * 9.0, maze_size),
            Wall(fac * 14.9, fac * 11.0, fac * 16.0, fac * 11.0, maze_size),
            Wall(fac * 14.9, fac * 9.0, fac * 16.0, fac * 9.0, maze_size),
            Wall(fac * 14.9, fac * 10.0, fac * 16.0, fac * 10.0, maze_size)};
    }
    else
    {
        walls = {
            Wall(fac * 4.0, fac * 11.0, fac * 4.85, fac * 10.0, maze_size),
            Wall(fac * 5.7, fac * 11.0, fac * 4.85, fac * 10.0, maze_size),
            Wall(fac * 4.85, fac * 9.0, fac * 4.85, fac * 10.0, maze_size),

            Wall(fac * 6.0, fac * 11.0, fac * 6.0, fac * 9.0, maze_size),
            Wall(fac * 7.7, fac * 11.0, fac * 7.7, fac * 9.0, maze_size),
            Wall(fac * 6.0, fac * 11.0, fac * 7.7, fac * 11.0, maze_size),
            Wall(fac * 6.0, fac * 9.0, fac * 7.7, fac * 9.0, maze_size),

            Wall(fac * 8.0, fac * 11.0, fac * 8.0, fac * 9.0, maze_size),
            Wall(fac * 8.0, fac * 9.0, fac * 9.7, fac * 9.0, maze_size),
            Wall(fac * 9.7, fac * 11.0, fac * 9.7, fac * 9.0, maze_size),

            Wall(fac * 10.3, fac * 9.0, fac * 11.15, fac * 10.0, maze_size),
            Wall(fac * 12.0, fac * 9.0, fac * 11.15, fac * 10.0, maze_size),
            Wall(fac * 12.0, fac * 11.0, fac * 12.0, fac * 9.0, maze_size),
            Wall(fac * 10.3, fac * 11.0, fac * 10.3, fac * 9.0, maze_size),

            Wall(fac * 13.0, fac * 11.0, fac * 13.0, fac * 9.0, maze_size),
            Wall(fac * 12.3, fac * 11.0, fac * 13.7, fac * 11.0, maze_size),
            Wall(fac * 12.3, fac * 9.0, fac * 13.7, fac * 9.0, maze_size),

            Wall(fac * 14.0, fac * 11.0, fac * 14.0, fac * 9.0, maze_size),
            Wall(fac * 15.0, fac * 11.0, fac * 15.0, fac * 9.0, maze_size),
            Wall(fac * 14.0, fac * 11.0, fac * 15.0, fac * 9.0, maze_size),
        };
    }
    maze = Maze(0, 0, walls);
    // draw();
    // sleep(5);
    // quit(window);
}

void tick_input(GLFWwindow *window)
{
    // int left = glfwGetKey(window, GLFW_KEY_LEFT);
    // int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    // int up = glfwGetKey(window, GLFW_KEY_UP);
    // int down = glfwGetKey(window, GLFW_KEY_DOWN);
    // if (left)
    // {

    // }
    // else if (right)
    // {

    // }
    // else if (up)
    // {

    // }
    // else if (down)
    // {

    // }
}

void tick_elements()
{
    // ball1.tick();
    // camera_rotation_angle += 1;
    ticks++;
    if (ticks % freq == 0 && imp_live && !game_end)
    {
        impostor.move(mod_dfs(impostor.position.x, impostor.position.y, make_pair(-1, -1)).second);
    }
    ticks = ticks % freq;

    if (player.position.x == vaporub.position.x && player.position.y == vaporub.position.y && imp_live)
    {
        imp_live = false;
        completed_tasks++;
    }

    if (player.position.x == impostor.position.x && player.position.y == impostor.position.y && imp_live)
    {
        end_game(false);
        // end_game(true);
    }

    if (player.position.x == drop.position.x && player.position.y == drop.position.y && !dropped)
    {
        dropped = true;
        // drop the powerups
        int x = 0, y = 17;
        srand(time(NULL));
        for (int i = 0; i < 6; i++)
        {
            int x2 = rand() % 3;
            int y2 = rand() % 17;
            int value = -30 + 100 * (rand() % 2);
            powers[i] = Powerup(x + 3 * i + x2, y - y2, value);
        }
        // cout<<"they dropped!!"<<endl;
        completed_tasks++;
    }

    for (int i = 0; i < 6; i++)
    {
        if (player.position.x == powers[i].position.x && player.position.y == powers[i].position.y && !game_end)
        {
            score += powers[i].value;
            powers[i].value = 0;
        }
    }

    if (player.position.x == star.position.x && player.position.y == star.position.y && !game_end && completed_tasks == 2)
    {
        end_game(true);
        // end_game(true);
    }

    // cout<<"score: "<<score<<endl;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    maze = Maze(0, 0, walls);
    player = Player(0, 0, COLOR_RED);
    srand(time(NULL));
    float impx = (float)maze_size - 2.0 - (float)(rand() % ((int)maze_size / 3));
    float impy = (float)maze_size - 2.0 - (float)(rand() % ((int)maze_size / 3));
    impostor = Player(impx, impy, COLOR_BLACK);

    float vapx = (float)maze_size - 9.0 - (float)(rand() % ((int)maze_size / 3));
    float vapy = (float)maze_size - 9.0 - (float)(rand() % ((int)maze_size / 3));
    vaporub = Vaporise(vapx, vapy, COLOR_GREEN);

    float dropx = (float)maze_size - 2.0 - (float)(rand() % ((2 * (int)maze_size / 3)));
    float dropy = (float)maze_size - 2.0 - (float)(rand() % ((2 * (int)maze_size / 3)));

    if (dropx == vapx && dropy == vapy)
    {
        if (dropx < maze_size)
        {
            dropx += 1;
        }
        else
        {
            dropx -= 1;
        }
    }

    drop = Vaporise(dropx, dropy, COLOR_GREY);
    star = Star(maze_size-2, maze_size-2, COLOR_YELLOW);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void make_adj_list(list<Edge> edges)
{
    for (auto it = edges.begin(); it != edges.end(); it++)
    {
        Edge edge = *it;
        adj[edge.x1][edge.y1].push_back(make_pair(edge.x2, edge.y2));
        adj[edge.x2][edge.y2].push_back(make_pair(edge.x1, edge.y1));
    }
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 800;
    int height = 800;

    walls = create_maze(maze_size);
    make_adj_list(edges);

    window = initGLFW(width, height);

    initGL(window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands
            draw();

            // Initialize glText
            gltInit();

            // Creating text
            GLTtext *text = gltCreateText();

            int time_played = (int)(time(NULL) - start_time);
            int time_remaining = 60;
            if(!game_end)
            {
                time_remaining -= time_played;
            }
            else
            {
                time_remaining = 0;
            }
            if(time_remaining < 0)
            {
                time_remaining = 0;
            }

            char *to_write = (char *)malloc(sizeof(char)*200);
            sprintf(to_write, "Score (Health): %d\nTasks: %d/2\nLight: On\nTime (in seconds): %d", score, completed_tasks, time_remaining);

            // cout << "Score (Health): " + to_string(score) + "\nTasks: " + to_string(completed_tasks) + "/2\nLight: On\nTime (in seconds): " + to_string(time_remaining) << endl;

            gltSetText(text, to_write);

            // Begin text drawing (this for instance calls glUseProgram)
            gltBeginDraw();

            // Draw any amount of text between begin and end
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(text, 1, 19, 1.0);

            // Finish drawing text
            gltEndDraw();

            // Deleting text
            gltDeleteText(text);

            // Destroy glText
            gltTerminate();

            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);

            if (time_remaining <= 0)
            {
                end_game(false);
            }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
