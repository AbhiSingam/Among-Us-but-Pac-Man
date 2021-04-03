// #include <bits/stdc++.h>
#include <walls.h>
using namespace std;

Edge edge_from_wall(Wall wall)
{
    if ((int)wall.x1 != (int)wall.x2)
    {
        return Edge(min((int)wall.x1, (int)wall.x2), (int)wall.y1 - 1, min((int)wall.x1, (int)wall.x2), (int)wall.y1);
    }
    else
    {
        return Edge((int)wall.x1 - 1, min((int)wall.y1, (int)wall.y2), (int)wall.x1, min((int)wall.y1, (int)wall.y2));
    }
}

list<Wall> create_maze(int n)
{
    srand(time(NULL));
    // int n = 5; // size of grid
    // cin >> n;
    list<Wall> walls;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {

            walls.push_back(Wall(i, j, i, j + 1, n));
            walls.push_back(Wall(j, i, j + 1, i, n));
        }
    }

    // for (auto it = walls.begin(); it != walls.end(); it++)
    // {
    //     cout << (*it).x1 << " " << (*it).y1 << " " << (*it).x2 << " " << (*it).y2 << " " << (*it).edge << endl;
    // }

    // cout << "your" << endl;

    // Randomized Kruskall's Maze

    int block_indices[n - 1][n - 1];

    int index_counter = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            block_indices[i][j] = index_counter;
            index_counter++;
        }
    }

    // for (int i=0; i < ((n-1)*(n-1) - 1);)
    bool not_a_flag = true;
    while (not_a_flag)
    {
        int ind = rand() % walls.size();
        list<Wall>::iterator wall_it = walls.begin();
        // for (int i = 0; i < ind; i++)
        // {
        //     wall_it++;
        // }
        advance(wall_it, ind);
        Wall wall = *wall_it;

        // cout << "poth" << endl;

        if (!wall.edge)
        {
            if ((int)wall.x1 == (int)wall.x2)
            {
                // wall along Y
                if (block_indices[(int)wall.x1][min((int)wall.y1, (int)wall.y2)] != block_indices[(int)wall.x1 - 1][min((int)wall.y1, (int)wall.y2)])
                {
                    //replace_all(block_indices, block_indices[wall.x1][min(wall.y1, wall.y2)], block_indices[wall.x1 - 1][min(wall.y1, wall.y2)]);
                    int value = block_indices[(int)wall.x1][min((int)wall.y1, (int)wall.y2)];
                    for (int j = 0; j < n - 1; j++)
                    {
                        for (int k = 0; k < n - 1; k++)
                        {
                            if (block_indices[j][k] == value)
                            {
                                block_indices[j][k] = block_indices[(int)wall.x1 - 1][min((int)wall.y1, (int)wall.y2)];
                            }
                        }
                    }
                    edges.push_back(edge_from_wall(wall));
                    walls.erase(wall_it);
                }
            }
            else
            {
                // wall along X
                if (block_indices[min((int)wall.x1, (int)wall.x2)][(int)wall.y1] != block_indices[min((int)wall.x1, (int)wall.x2)][(int)wall.y1 - 1])
                {
                    //replace_all(block_indices, block_indices[min(wall.x1, wall.x2)][wall.y1], block_indices[min(wall.x1, wall.x2)][wall.y1 - 1]);
                    int value = block_indices[min((int)wall.x1, (int)wall.x2)][(int)wall.y1];
                    for (int j = 0; j < n - 1; j++)
                    {
                        for (int k = 0; k < n - 1; k++)
                        {
                            if (block_indices[j][k] == value)
                            {
                                block_indices[j][k] = block_indices[min((int)wall.x1, (int)wall.x2)][(int)wall.y1 - 1];
                            }
                        }
                    }
                    edges.push_back(edge_from_wall(wall));
                    walls.erase(wall_it);
                }
            }

            // cout << "sakd" << endl;

            // i++;
        }
        not_a_flag = false;

        // for (int i = 0; i < n - 1; i++)
        // {
        //     for (int j = 0; j < n - 1; j++)
        //     {
        //         cout << block_indices[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                // cout << block_indices[i][j] << " ";
                if (block_indices[i][j] != block_indices[0][0])
                {
                    not_a_flag = true;
                }
            }
        }
    }

    // cout << endl << endl;

    // for (auto it = walls.begin(); it != walls.end(); it++)
    // {
    //     cout << (*it).x1 << " " << (*it).y1 << " " << (*it).x2 << " " << (*it).y2 << " " << (*it).edge << endl;
    // }

    return walls;
}