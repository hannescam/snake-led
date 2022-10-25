
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <chrono>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include "led-matrix.h"
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include <random>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

#define height_num 32
#define width_num 64
int height = height_num;
int width = width_num;
unsigned long offsetT = 0;
float gameSpeed = 15;      // Higher numbers are faster
bool start = true;         // will not start without say-so
unsigned long offsetM = 0; // time delay for main loop
int xold;
int yold;
bool move_finished = true;
float gs;
int headX = 1; // coordinates for head
int headY = 1;
#define length_num (height_num * width_num) + 2
int length = length_num;
int beenHeadX[length_num]; // coordinates to clear later
int beenHeadY[length_num];
int changeX = 0; // the direction of the snake
int changeY = 0;
bool lastMoveH = false; // to keep from going back on oneself
int score = 1;
int foodX; // coordinates of food
int foodY;
bool eaten = true; // if true a new food will be made
int foodX2;        // coordinates of food
int foodY2;
bool eaten2 = true; // if true a new food will be made
int foodX3;         // coordinates of food
int foodY3;
bool eaten3 = true; // if true a new food will be made
int foodX4;         // coordinates of food
int foodY4;
bool eaten4 = true; // if true a new food will be made
int foodX5;         // coordinates of food
int foodY5;
bool eaten5 = true; // if true a new food will be made
int foodX6;         // coordinates of food
int foodY6;
bool eaten6 = true; // if true a new food will be made
int loopCount = 0;  // number of times the loop has run
int clearPoint = 0; // when the loopCount is reset
bool clearScore = false;

using namespace std;

struct termios t;
struct termios tty;

char *intToStr(int data)
{
    std::string strData = std::to_string(data);

    char *temp = new char[strData.length() + 1];
    strcpy(temp, strData.c_str());

    return temp;
}

void printScore(bool ok)
{
    if (ok)
    {
        // future use
    }
    // printf("Score: ");
    // printf(intToStr(score - 1));
}

void endGame()
{
    printScore(true);
    eaten = true; // new food will be created
    headX = 1;    // reset snake
    headY = 1;
    changeX = 0;
    changeY = 1;
    printf("Game over!!!");
    lastMoveH = false;
    memset(beenHeadX, 0, 470); // clear the beenHead arrays
    memset(beenHeadY, 0, 470); // probably not necessary
    loopCount = 0;
    clearScore = true;
    start = false; // stops game
    while (!interrupt_received)
        ;
}

void drawDot(Canvas *canvas, int x, int y)
{
    canvas->SetPixel((x * 2), (y * 2), 255, 255, 255);
    canvas->SetPixel((x * 2) + 1, (y * 2), 255, 255, 255);
    canvas->SetPixel((x * 2), (y * 2) + 1, 255, 255, 255);
    canvas->SetPixel((x * 2) + 1, (y * 2) + 1, 255, 255, 255);

    /*if (xold != changeX or yold != changeY) {
      Serial.println(xold);
      Serial.println(changeX);
      Serial.println(yold);
      Serial.println(changeY);
      if (xold == 1 && changeX == 0 && yold == 0 && changeY == 1) {
        y--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/9.jpg", LittleFS);
        y++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
      } else if (xold == 0 && changeX == 1 && yold == 1 && changeY == 0) {
        x--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/7.jpg", LittleFS);
        x++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
      } else if (xold == 1 && changeX == 0 && yold == 0 && changeY == -1) {
        y++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/6.jpg", LittleFS);
        y--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
      } else if (xold == 0 && changeX == 1 && yold == -1 && changeY == 0) {
        x--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/8.jpg", LittleFS);
        x++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
      } else if (xold == 0 && changeX == -1 && yold == -1 && changeY == 0) {
        x++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/9.jpg", LittleFS);
        x--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
      } else if (xold == -1 && changeX == 0 && yold == 0 && changeY == -1) {
        y++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/7.jpg", LittleFS);
        y--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
      } else if (xold == 0 && changeX == -1 && yold == 1 && changeY == 0) {
        x++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/6.jpg", LittleFS);
        x--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
      } else if (xold == -1 && changeX == 0 && yold == 0 && changeY == 1) {
        y--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/8.jpg", LittleFS);
        y++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
      }
    } else {
      if (changeX == 1 && changeY == 0) {
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/2.jpg", LittleFS);
        x--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
        x++;
      } else if (changeX == -1 && changeY == 0) {
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/3.jpg", LittleFS);
        x++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/1.jpg", LittleFS);
        x--;
      } else if (changeX == 0 && changeY == -1) {
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/4.jpg", LittleFS);
        y++;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
        y--;
      } else if (changeX == 0 && changeY == 1) {
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/5.jpg", LittleFS);
        y--;
        TJpgDec.drawFsJpg(12 * x + 6, 12 * y + 23, "/10.jpg", LittleFS);
        y++;
      }
    }
    xold = changeX;
    yold = changeY;*/
    // old memmories, todo: make the direction thing work
}

void drawDotRed(Canvas *canvas, int x, int y)
{
    canvas->SetPixel((x * 2), (y * 2), 255, 0, 0);
    canvas->SetPixel((x * 2) + 1, (y * 2), 255, 0, 0);
    canvas->SetPixel((x * 2), (y * 2) + 1, 255, 0, 0);
    canvas->SetPixel((x * 2) + 1, (y * 2) + 1, 255, 0, 0);
}

void eraseDot(Canvas *canvas, int x, int y)
{
    canvas->SetPixel((x * 2), (y * 2), 0, 0, 0);
    canvas->SetPixel((x * 2) + 1, (y * 2), 0, 0, 0);
    canvas->SetPixel((x * 2), (y * 2) + 1, 0, 0, 0);
    canvas->SetPixel((x * 2) + 1, (y * 2) + 1, 0, 0, 0);
}

void input_enter_off()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void *input_listner(void *)
{
    // auto start_time = std::chrono::high_resolution_clock::now();
    /*int serial_port = open("/dev/ttyUSB2", O_RDWR);
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }
    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;         // Clear all bits that set the data size
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }*/
    char read_buf;
    while (true)
    {
        memset(&read_buf, '\0', 1);
        // int num_bytes = read(serial_port, &read_buf, 1);
        scanf("%c", &read_buf);
        if (true)
        {
            if (lastMoveH and read_buf == 'w' and move_finished)
            {
                changeX = 0;
                changeY = -1;
                lastMoveH = false;
                move_finished = false;
            }

            if (lastMoveH and read_buf == 's' and move_finished)
            {
                changeX = 0;
                changeY = 1;
                lastMoveH = false;
                move_finished = false;
            }

            if (!lastMoveH and read_buf == 'a' and move_finished)
            {
                changeX = -1;
                changeY = 0;
                lastMoveH = true;
                move_finished = false;
            }

            if (!lastMoveH and read_buf == 'd' and move_finished)
            {
                changeX = 1;
                changeY = 0;
                lastMoveH = true;
                move_finished = false;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::random_device randomizer;
    std::uniform_int_distribution<int> x_rand(2, width-1);
    std::uniform_int_distribution<int> y_rand(2, height-1);
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = true;
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL)
        return 1;

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    canvas->Fill(0, 0, 0);
    input_enter_off();
    memset(beenHeadX, 0, length); // initiate beenHead with a bunch of zeros
    memset(beenHeadY, 0, length);
    auto start_time = std::chrono::high_resolution_clock::now();
    pthread_t tId;
    (void)pthread_create(&tId, 0, input_listner, 0);
    while (!interrupt_received)
    {
        if (clearScore and start)
        {                      // resets score from last game, won't clear
            score = 1;         // until new game starts so you can show off
            printScore(false); // your own score
            clearScore = false;
        }
        //sleep(1); //awfoly slow for debugging its like inglestepping
        usleep(250000); //normal speed
        // auto current_time = std::chrono::high_resolution_clock::now();
        // if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() - offsetM > gs and start)
        //{
        beenHeadX[loopCount] = headX; // adds current head coordinates to be
        beenHeadY[loopCount] = headY; // covered later

        headX = headX + (changeX); // head moved
        headY = headY + (changeY);
        move_finished = true;

        if (headX - foodX == 0 and headY - foodY == 0)
        { // food
            score += 1;
            printScore(false);
            eaten = true;
        }
        if (headX - foodX2 == 0 and headY - foodY2 == 0)
        { // food
            score += 1;
            printScore(false);
            eaten2 = true;
        }
        if (headX - foodX3 == 0 and headY - foodY3 == 0)
        { // food
            score += 1;
            printScore(false);
            eaten3 = true;
        }
        if (headX - foodX4 == 0 and headY - foodY4 == 0)
        { // food
            score += 1;
            printScore(false);
            eaten4 = true;
        }
        if (headX - foodX5 == 0 and headY - foodY5 == 0)
        { // food
            score += 1;
            printScore(false);
            eaten5 = true;
        }
        if (headX - foodX6 == 0 and headY - foodY6 == 0)
        { // food
            score += 1;
            printScore(false);
            eaten6 = true;
        }

        loopCount += 1; // loopCount used for addressing, mostly

        if (loopCount > length - 3)         // 467
        {                                   // if loopCount exceeds size of
            clearPoint = loopCount - score; // beenHead arrays, reset to zero
            loopCount = 0;
        }

        drawDot(canvas, headX, headY); // head is drawn
        printf("\nLoop Count: ");
        printf(intToStr(loopCount));
        printf(" Score: ");
        printf(intToStr(score));
        printf(" edx: ");
        printf(intToStr(beenHeadX[loopCount - score]));
        printf(" edy: ");
        printf(intToStr(beenHeadY[loopCount - score]));
        printf("\n");
        if (loopCount - score >= 0)
        { // if array has not been reset
            eraseDot(canvas, beenHeadX[loopCount - score], beenHeadY[loopCount - score]);
        } // covers end of tail
        else
        {
            eraseDot(canvas, beenHeadX[clearPoint], beenHeadY[clearPoint]);
            clearPoint += 1;
        }

        if (eaten)
        {                                // randomly create a new piece of food if last was eaten
            foodX = x_rand(randomizer);  // random(2, 26);
            foodY = y_rand(randomizer); // random(2, 18);
            eaten = false;
        }

        if (eaten2)
        {                                 // randomly create a new piece of food if last was eaten
            foodX2 = x_rand(randomizer);  // random(2, 26);
            foodY2 = y_rand(randomizer); // random(2, 18);
            while (foodX2 == foodX && foodY2 == foodY)
            {
                foodX2 = x_rand(randomizer);  // random(2, 26);
                foodY2 = y_rand(randomizer); // random(2, 18);
            }
            eaten2 = false;
        }

        if (eaten3)
        {                                 // randomly create a new piece of food if last was eaten
            foodX3 = x_rand(randomizer);  // random(2, 26);
            foodY3 = y_rand(randomizer); // random(2, 18);
            while (foodX3 == foodX && foodY3 == foodY | foodX3 == foodX2 && foodY3 == foodY2)
            {
                foodX3 = x_rand(randomizer);  // random(2, 26);
                foodY3 = y_rand(randomizer); // random(2, 18);
            }
            eaten3 = false;
        }

        if (eaten4)
        {                                 // randomly create a new piece of food if last was eaten
            foodX4 = x_rand(randomizer);  // random(2, 26);
            foodY4 = y_rand(randomizer); // random(2, 18);
            while (foodX4 == foodX && foodY4 == foodY | foodX4 == foodX3 && foodY4 == foodY3 | foodX4 == foodX2 && foodY4 == foodY2)
            {
                foodX4 = x_rand(randomizer);  // random(2, 26);
                foodY4 = y_rand(randomizer); // random(2, 18);
            }
            eaten4 = false;
        }

        if (eaten5)
        {                                 // randomly create a new piece of food if last was eaten
            foodX5 = x_rand(randomizer);  // random(2, 26);
            foodY5 = y_rand(randomizer); // random(2, 18);
            while (foodX5 == foodX && foodY5 == foodY | foodX5 == foodX4 && foodY5 == foodY4 | foodX5 == foodX3 && foodY5 == foodY3 | foodX5 == foodX2 && foodY5 == foodY2)
            {
                foodX5 = x_rand(randomizer);  // random(2, 26);
                foodY5 = y_rand(randomizer); // random(2, 18);
            }
            eaten5 = false;
        }

        if (eaten6)
        {                                 // randomly create a new piece of food if last was eaten
            foodX6 = x_rand(randomizer);  // random(2, 26);
            foodY6 = y_rand(randomizer); // random(2, 18);
            while (foodX6 == foodX && foodY6 == foodY | foodX6 == foodX5 && foodY6 == foodY5 | foodX6 == foodX4 && foodY6 == foodY4 | foodX6 == foodX3 && foodY6 == foodY3 | foodX6 == foodX2 && foodY6 == foodY2)
            {
                foodX6 = x_rand(randomizer);  // random(2, 26);
                foodY6 = y_rand(randomizer); // random(2, 18);
            }
            eaten6 = false;
        }

        drawDotRed(canvas, foodX, foodY);   // draw the food
        drawDotRed(canvas, foodX2, foodY2); // draw the food
        drawDotRed(canvas, foodX3, foodY3); // draw the food
        drawDotRed(canvas, foodX4, foodY4); // draw the food
        drawDotRed(canvas, foodX5, foodY5); // draw the food
        drawDotRed(canvas, foodX6, foodY6); // draw the food
        if (headY > height - 1)
        {
            headY = - 1;
            printScore(true);
        }
        else if (headY < 0)
        {
            headY = height;
            printScore(true);
        }
        else if (headX > width - 1)
        {
            headX = - 1;
            printScore(true);
        }
        else if (headX < 0)
        {
            headX = width;
            printScore(true);
        }
        // endGame();

        if (loopCount - score < 0)
        { // check to see if head is on tail
            for (int j = 0; j < loopCount; j++)
            {
                if (headX == beenHeadX[j] and headY == beenHeadY[j])
                {
                    endGame();
                }
            }
            for (int k = clearPoint; k < length - 3; k++)
            {
                if (headX == beenHeadX[k] and headY == beenHeadY[k])
                {
                    endGame();
                }
            }
        }
        else
        {
            for (int i = loopCount - (score - 1); i < loopCount; i++)
            {
                if (headX == beenHeadX[i] and headY == beenHeadY[i])
                {
                    endGame();
                }
            }
        }
        auto current_time = std::chrono::high_resolution_clock::now();
        offsetM = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count(); // reset game loop timer
        //}
    }
    canvas->Clear();
    delete canvas;
    printf("\nStopping...\n");
    return 0;
}