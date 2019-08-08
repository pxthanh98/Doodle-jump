#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>
#include<sstream>

using namespace sf;
using namespace std;

int window_width = 400;
int window_height = 533;
int doodle_size = 80;
int platform_width = 68;
int platform_height = 14;
int jump = 10;
int pos_x = 100;
int pos_y = 100;
int height = 200;
int score = 0;
float dy = 0;
string background = "images/background.png";
string platform = "images/platform.png";
string doodle = "images/doodle.png";

struct point
{
    int x, y;
};

bool gameOver(int y)
{
    if (y > window_height)
        return true;
    else
        return false;
}

int gameLogic(point plat[], int& pos_x, int& pos_y)
{
    // Draw random platforms from height range = [0; 200]
    if (pos_y < height)
        for (int i = 0; i < 10; i++)
        {
            pos_y = height;
            plat[i].y = plat[i].y - dy;
            if (plat[i].y > window_height)
            {
                plat[i].y = 0;
                plat[i].x = rand() % window_width;
            }
        }

    // Doodle jump
    for (int i = 0; i < 10; i++)
        if ((pos_x + 50 > plat[i].x) && (pos_x + 20 < plat[i].x + platform_width)
                && (pos_y + 70 > plat[i].y) && (pos_y + 70 < plat[i].y + platform_height) && (dy > 0))
        {
            dy = -jump;
            score += 50;
        }
    return pos_x, pos_y;
}

void navigation(int& pos_x, int& pos_y)
{
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        pos_x += 4;
        if (pos_x >= (window_height - doodle_size))
            pos_x = 0 - doodle_size/2;
    }

    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        pos_x -= 4;
        if (pos_x <= 0 - doodle_size)
            pos_x = window_height - doodle_size/0.5;
    }

    // Create falling feeling with velocity dy
    dy += 0.2;
    pos_y += dy;
}

void drawPlatforms(RenderWindow& app, Sprite& sPlat, point plat[])
{
    for (int i = 0; i < 10; i++)
    {
        sPlat.setPosition(plat[i].x, plat[i].y);
        app.draw(sPlat);
    }
}

void gameClose(RenderWindow& app)
{
    Event e;
    while (app.pollEvent(e))
    {
        if (e.type == Event::Closed)
            app.close();
    }
}

void gamePlay(RenderWindow& app, Texture t1, Texture t2, Texture t3)
{
    Sprite sBackground(t1), sPlat(t2), sDoo(t3);

    // Create random platforms
    point plat[20];
    for (int i = 0; i < 10; i++)
    {
        plat[i].x=rand() % window_width;
        plat[i].y=rand() % window_height;
    }

    // Game loop
    do
    {
        navigation(pos_x, pos_y);
        gameLogic(plat, pos_x, pos_y);
        sDoo.setPosition(pos_x, pos_y);
        app.draw(sBackground);
        app.draw(sDoo);
        drawPlatforms(app, sPlat, plat);
        app.display();
        gameClose(app);
    }
    while (app.isOpen() && (gameOver(pos_y) == false));
    cout << "Game Over !!!" << endl;
    cout << "Your Score: " << score << endl;
}

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(window_width, window_height), "Doodle Jump");
    app.setFramerateLimit(60);

    Texture t1,t2,t3;
    t1.loadFromFile(background);
    t2.loadFromFile(platform);
    t3.loadFromFile(doodle);
    gamePlay(app, t1, t2, t3);

    return 0;
}
