/*
 * tic_tac_toe.c
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 *
 *  YouTube Video Link: https://youtu.be/MK4BnVNeAro
 */

#include "tic_tac_toe.h"

#define VOLT_0P6 ((int)(0.60/(3.3/4096))) // 745      // 0.6 /(3.3/4096)
#define VOLT_2P7 ((int)(2.70/(3.3/4096))) // 3352     // 2.7 /(3.3/4096)

/* Used to check position status
 * 0 - Free
 * 1 - O
 * 2 - X
 *
 * Index represents position:
 *  0  1  2
 *  3  4  5
 *  6  7  8
 *
 */
int position[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*
 * Counts number of placement of X's and O's
 *
 * Computing count % 2:
 *  - if 0, O's turn
 *  - if 1, X's turn
 */
int count = 0;

/*
 * Track of current position
 * Used as index of position array, where it starts at index 4
 *
 * Index represents position:
 *  0  1  2
 *  3  4  5
 *  6  7  8
 */
int curPos = 4;

// Used to limit one movement
int curMoved = 0;

// Used to indicate game over
int game_over = 0;

// Track of current X, Y position
uint16_t curX = CENTER_COL;
uint16_t curY = CENTER_ROW;

/*******************************************************************************
 * Function Name: tic_tac_toe_hw_init
 ********************************************************************************
 * Summary: Initializes all the hardware resources required for tic tac toe game
 *          (IO Pins, ADC14, Timer32_1, Timer32_2).
 * Returns:
 *  Nothing
 *******************************************************************************/
void tic_tac_toe_hw_init(void)
{
    // Initialize S2 on MKII
    P3->DIR &= ~BIT5;

    // Configure Timer32_1 to generate an interrupt every 10mS
    T32_1_Interrupt_10ms();

    // Configure Timer32_2 to generate an interrupt every 100mS
    T32_2_Interrupt_100ms();

    // Configure ADC14 for the joystick
    peripheral_config();

    // Initialize LCD
    lcd_init();
}

/*******************************************************************************
 * Function Name: tic_tac_toe_example_board
 ********************************************************************************
 * Summary: Prints out an example of what the tic-tac-toe board looks like
 * Returns:
 *  Nothing
 *******************************************************************************/
void tic_tac_toe_example_board(void)
{
    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL, LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);

    // Top Row
    lcd_draw_image(LEFT_COL, UPPER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_O,
    LCD_COLOR_RED,
                   LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL, UPPER_ROW, X_WIDTH, X_HEIGHT, Bitmaps_X,
    LCD_COLOR_YELLOW,
                   LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL, UPPER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_O,
    LCD_COLOR_GREEN,
                   LCD_COLOR_BLACK);

    // Center Row
    lcd_draw_image(LEFT_COL, CENTER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_X,
    LCD_COLOR_BLUE,
                   LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL, CENTER_ROW, X_WIDTH, X_HEIGHT, Bitmaps_O,
    LCD_COLOR_ORANGE,
                   LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL, CENTER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_X,
    LCD_COLOR_CYAN,
                   LCD_COLOR_BLACK);

    // Lower Row
    lcd_draw_image(LEFT_COL, LOWER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_O,
    LCD_COLOR_MAGENTA,
                   LCD_COLOR_BLACK);
    lcd_draw_image(CENTER_COL, LOWER_ROW, X_WIDTH, X_HEIGHT, Bitmaps_X,
    LCD_COLOR_GRAY,
                   LCD_COLOR_BLACK);
    lcd_draw_image(RIGHT_COL, LOWER_ROW, O_WIDTH, O_HEIGHT, Bitmaps_O,
    LCD_COLOR_BROWN,
                   LCD_COLOR_BLACK);
}

/*******************************************************************************
 * Function Name: check_O
 ********************************************************************************
 * Summary: Checks winning condition for O
 *
 * Returns:
 *      - true if win condition met
 *      - false, otherwise
 *******************************************************************************/
bool check_O(void)
{
    // All conditions for each position
    if (curPos == 0)
    {
        if (position[1] == 1 && position[2] == 1)
        {
            return true;
        }

        if (position[3] == 1 && position[6] == 1)
        {
            return true;
        }

        if (position[4] == 1 && position[8] == 1)
        {
            return true;
        }
    }

    if (curPos == 1)
    {
        if (position[0] == 1 && position[2] == 1)
        {
            return true;
        }

        if (position[4] == 1 && position[7] == 1)
        {
            return true;
        }
    }

    if (curPos == 2)
    {
        if (position[0] == 1 && position[1] == 1)
        {
            return true;
        }

        if (position[4] == 1 && position[6] == 1)
        {
            return true;
        }

        if (position[5] == 1 && position[8] == 1)
        {
            return true;
        }
    }

    if (curPos == 3)
    {
        if (position[0] == 1 && position[6] == 1)
        {
            return true;
        }

        if (position[4] == 1 && position[5] == 1)
        {
            return true;
        }
    }

    if (curPos == 4)
    {
        if (position[0] == 1 && position[8] == 1)
        {
            return true;
        }

        if (position[1] == 1 && position[7] == 1)
        {
            return true;
        }
        if (position[2] == 1 && position[6] == 1)
        {
            return true;
        }

        if (position[3] == 1 && position[5] == 1)
        {
            return true;
        }
    }

    if (curPos == 5)
    {
        if (position[2] == 1 && position[8] == 1)
        {
            return true;
        }

        if (position[3] == 1 && position[4] == 1)
        {
            return true;
        }
    }

    if (curPos == 6)
    {
        if (position[0] == 1 && position[3] == 1)
        {
            return true;
        }

        if (position[2] == 1 && position[4] == 1)
        {
            return true;
        }
        if (position[7] == 1 && position[8] == 1)
        {
            return true;
        }
    }

    if (curPos == 7)
    {
        if (position[1] == 1 && position[4] == 1)
        {
            return true;
        }

        if (position[6] == 1 && position[8] == 1)
        {
            return true;
        }
    }

    if (curPos == 8)
    {
        if (position[2] == 1 && position[5] == 1)
        {
            return true;
        }

        if (position[0] == 1 && position[4] == 1)
        {
            return true;
        }
        if (position[6] == 1 && position[7] == 1)
        {
            return true;
        }
    }

    // None of the conditions are met, return false
    return false;
}

/*******************************************************************************
 * Function Name: check_X
 ********************************************************************************
 * Summary: Checks winning condition for X
 *
 * Returns:
 *      - true if win condition met
 *      - false, otherwise
 *******************************************************************************/
bool check_X(void)
{
    // All conditions for each position
    if (curPos == 0)
    {
        if (position[1] == 2 && position[2] == 2)
        {
            return true;
        }

        if (position[3] == 2 && position[6] == 2)
        {
            return true;
        }

        if (position[4] == 2 && position[8] == 2)
        {
            return true;
        }
    }

    if (curPos == 1)
    {
        if (position[0] == 2 && position[2] == 2)
        {
            return true;
        }

        if (position[4] == 2 && position[7] == 2)
        {
            return true;
        }
    }

    if (curPos == 2)
    {
        if (position[0] == 2 && position[1] == 2)
        {
            return true;
        }

        if (position[4] == 2 && position[6] == 2)
        {
            return true;
        }

        if (position[5] == 2 && position[8] == 2)
        {
            return true;
        }
    }

    if (curPos == 3)
    {
        if (position[0] == 2 && position[6] == 2)
        {
            return true;
        }

        if (position[4] == 2 && position[5] == 2)
        {
            return true;
        }
    }

    if (curPos == 4)
    {
        if (position[0] == 2 && position[8] == 2)
        {
            return true;
        }

        if (position[1] == 2 && position[7] == 2)
        {
            return true;
        }
        if (position[2] == 2 && position[6] == 2)
        {
            return true;
        }

        if (position[3] == 2 && position[5] == 2)
        {
            return true;
        }
    }

    if (curPos == 5)
    {
        if (position[2] == 2 && position[8] == 2)
        {
            return true;
        }

        if (position[3] == 2 && position[4] == 2)
        {
            return true;
        }
    }

    if (curPos == 6)
    {
        if (position[0] == 2 && position[3] == 2)
        {
            return true;
        }

        if (position[2] == 2 && position[4] == 2)
        {
            return true;
        }
        if (position[7] == 2 && position[8] == 2)
        {
            return true;
        }
    }

    if (curPos == 7)
    {
        if (position[1] == 2 && position[4] == 2)
        {
            return true;
        }

        if (position[6] == 2 && position[8] == 2)
        {
            return true;
        }
    }

    if (curPos == 8)
    {
        if (position[2] == 2 && position[5] == 2)
        {
            return true;
        }

        if (position[0] == 2 && position[4] == 2)
        {
            return true;
        }
        if (position[6] == 2 && position[7] == 2)
        {
            return true;
        }
    }

    // None of the conditions are met, return false
    return false;
}

/*******************************************************************************
 * Function Name: reset_board
 ********************************************************************************
 * Summary: Resets the board when game over
 *
 * Returns:
 *  Nothing
 *******************************************************************************/
void reset_board(void)
{
    int i;

    // Reset position array
    for (i = 0; i < 9; i++)
    {
        position[i] = 0;
    }
    curPos = 4;
    curX = CENTER_COL;
    curY = CENTER_ROW;
    count = 0;
    curMoved = 0;
    game_over = 0;

    // Initialize hardware
    tic_tac_toe_hw_init();

    // Set up game board again
    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL, LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);

    // Clear all boxes to black
    lcd_draw_rectangle(LEFT_COL, UPPER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(LEFT_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(LEFT_COL, LOWER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(CENTER_COL, UPPER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(CENTER_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(CENTER_COL, LOWER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(RIGHT_COL, UPPER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(RIGHT_COL, CENTER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
    lcd_draw_rectangle(RIGHT_COL, LOWER_ROW, SQUARE_SIZE, SQUARE_SIZE,
    LCD_COLOR_BLACK);
}

/*******************************************************************************
 * Function Name: tic_tac_toe_play_game
 ********************************************************************************
 * Summary: Enters the code that plays the tic_tac_toe game.  Once called, this
 *          function should never return!
 * Returns:
 *  Nothing
 *******************************************************************************/
void tic_tac_toe_play_game(void)
{
    // Initialize hardware
    tic_tac_toe_hw_init();

    // Set up game board
    // Horizontal Lines
    lcd_draw_rectangle(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(SCREEN_CENTER_COL, LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH,
    LINE_WIDTH,
                       LCD_COLOR_BLUE);

    //Vertical Lines
    lcd_draw_rectangle(LEFT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);
    lcd_draw_rectangle(RIGHT_HORIZONTAL_LINE_X, SCREEN_CENTER_ROW, LINE_WIDTH,
    LINE_LENGTH,
                       LCD_COLOR_BLUE);

    // Game starts here
    while (1)
    {
        if (game_over == 0)
        {
            // X direction check
            // Move right
            if (PS2_X_DIR > VOLT_2P7)
            {
                // Current position is at the left most column
                if (curX == LEFT_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }

                        curX = CENTER_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 1;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 4;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 7;
                        }
                    }
                }

                // Current position is at the center column
                else if (curX == CENTER_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curX = RIGHT_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 2;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 5;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 8;
                        }
                    }
                }

                // Current position is at the right most column
                else if (curX == RIGHT_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curX = LEFT_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 0;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 3;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 6;
                        }
                    }
                }
            }

            // Move left
            else if (PS2_X_DIR < VOLT_0P6)
            {
                // Current position is at the left most column
                if (curX == LEFT_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curX = RIGHT_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 2;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 5;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 8;
                        }
                    }
                }

                // Current position is at the center column
                else if (curX == CENTER_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curX = LEFT_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 0;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 3;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 6;
                        }
                    }
                }

                // Current position is at the right most column
                else if (curX == RIGHT_COL)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curX = CENTER_COL;
                        curMoved = 1;

                        // Update curPos
                        if (curY == UPPER_ROW)
                        {
                            curPos = 1;
                        }

                        else if (curY == CENTER_ROW)
                        {
                            curPos = 4;
                        }

                        else if (curY == LOWER_ROW)
                        {
                            curPos = 7;
                        }
                    }
                }

            }

            // Y direction check
            // Move upwards
            if (PS2_Y_DIR > VOLT_2P7)
            {
                // Current position is at the upper most row
                if (curY == UPPER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = LOWER_ROW;
                        curMoved = 1;

                        // Update curPos
                        if (curX == LEFT_COL)
                        {
                            curPos = 6;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 7;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 8;
                        }
                    }
                }

                // Current position is at the lower most row
                else if (curY == LOWER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = CENTER_ROW;
                        curMoved = 1;

                        // Update curPos
                        if (curX == LEFT_COL)
                        {
                            curPos = 3;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 4;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 5;
                        }

                    }
                }

                // Current position is at the center row
                else if (curY == CENTER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = UPPER_ROW;
                        curMoved = 1;

                        // Update curPos
                        if (curX == LEFT_COL)
                        {
                            curPos = 0;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 1;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 2;
                        }
                    }
                }

            }

            // Move downwards
            else if (PS2_Y_DIR < VOLT_0P6)
            {
                // Current position is at the upper most row
                if (curY == UPPER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = CENTER_ROW;
                        curMoved = 1;

                        if (curX == LEFT_COL)
                        {
                            curPos = 3;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 4;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 5;
                        }
                    }
                }

                // Current position is at the lower most row
                else if (curY == LOWER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = UPPER_ROW;
                        curMoved = 1;

                        if (curX == LEFT_COL)
                        {
                            curPos = 0;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 1;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 2;
                        }
                    }
                }

                // Current position is at the center row
                else if (curY == CENTER_ROW)
                {
                    if (curMoved == 0)
                    {
                        // Free spot, do nothing
                        if (position[curPos] == 0)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                        }

                        // Claimed spot by O
                        else if (position[curPos] == 1)
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT,
                                           Bitmaps_O,
                                           LCD_COLOR_CYAN,
                                           LCD_COLOR_BLACK);
                        }

                        // Claimed spot by X
                        else
                        {
                            lcd_draw_rectangle(curX, curY, SQUARE_SIZE,
                            SQUARE_SIZE,
                                               LCD_COLOR_BLACK);
                            lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT,
                                           Bitmaps_X,
                                           LCD_COLOR_YELLOW,
                                           LCD_COLOR_BLACK);
                        }
                        curY = LOWER_ROW;
                        curMoved = 1;

                        if (curX == LEFT_COL)
                        {
                            curPos = 6;
                        }

                        else if (curX == CENTER_COL)
                        {
                            curPos = 7;
                        }

                        else if (curX == RIGHT_COL)
                        {
                            curPos = 8;
                        }
                    }
                }
            }
        }

        // When button is pressed
        if (ALERT_S2_PRESSED)
        {
            ALERT_S2_PRESSED = false;

            // Reset everything and restart the game
            if (game_over == 1)
            {
                reset_board();
                continue;
            }

            // O's turn
            if (count % 2 == 0)
            {
                // If free spot, placement available
                if (position[curPos] == 0)
                {
                    lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT, Bitmaps_O,
                    LCD_COLOR_CYAN,
                                   LCD_COLOR_BLACK);
                    position[curPos] = 1;
                    count++;

                    // Check for O's winning condition
                    if (check_O())
                    {
                        lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                        LCD_COLOR_BLACK);
                        lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT, Bitmaps_O,
                        LCD_COLOR_CYAN,
                                       LCD_COLOR_BLACK);
                        lcd_O_wins();   // Print out winning message
                        game_over = 1;  // Indicate game over
                        continue;
                    }
                }
            }

            // X's turn
            else
            {
                // If free spot, placement available
                if (position[curPos] == 0)
                {
                    lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT, Bitmaps_X,
                    LCD_COLOR_YELLOW,
                                   LCD_COLOR_BLACK);
                    position[curPos] = 2;
                    count++;

                    // Check for X's winning condition
                    if (check_X())
                    {

                        lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                        LCD_COLOR_BLACK);
                        lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT, Bitmaps_X,
                        LCD_COLOR_YELLOW,
                                       LCD_COLOR_BLACK);
                        lcd_X_wins();   // Print out winning message
                        game_over = 1;  // Indicate game over
                        continue;
                    }
                }

            }

            // Check for tie condition
            if (count == 9)
            {
                lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                LCD_COLOR_BLACK);
                lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT, Bitmaps_O,
                LCD_COLOR_CYAN,
                               LCD_COLOR_BLACK);
                lcd_tie();  // Print out tie message
                game_over = 1;  // Indicate game over
                continue;

            }
        }

        // Stop updating LCD if game over
        if (ALERT_LCD_UPDATE && game_over == 0)
        {
            ALERT_LCD_UPDATE = false;

            // If current position claimed by O, active square must be red and O should be displayed
            if (position[curPos] == 1)
            {
                lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                LCD_COLOR_RED);
                lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT, Bitmaps_O,
                LCD_COLOR_BLACK,
                               LCD_COLOR_RED);
            }

            // If current position claimed by X, active square must be red and X should be displayed
            else if (position[curPos] == 2)
            {
                lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                LCD_COLOR_RED);
                lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT, Bitmaps_X,
                LCD_COLOR_BLACK,
                               LCD_COLOR_RED);
            }

            // If current position is free, active square is green with current player - O or X
            else
            {
                // Check if current turn is O or X
                if (count % 2 == 0)
                {
                    lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                    LCD_COLOR_GREEN);
                    lcd_draw_image(curX, curY, O_WIDTH, O_HEIGHT, Bitmaps_O,
                    LCD_COLOR_BLACK,
                                   LCD_COLOR_GREEN);
                }
                else
                {
                    lcd_draw_rectangle(curX, curY, SQUARE_SIZE, SQUARE_SIZE,
                    LCD_COLOR_GREEN);
                    lcd_draw_image(curX, curY, X_WIDTH, X_HEIGHT, Bitmaps_X,
                    LCD_COLOR_BLACK,
                                   LCD_COLOR_GREEN);
                }
            }
        }

        // Update curMoved to allow additional moves
        if (PS2_Y_DIR < VOLT_2P7 && PS2_Y_DIR > VOLT_0P6 && PS2_X_DIR > VOLT_0P6
                && PS2_X_DIR < VOLT_2P7)
        {
            curMoved = 0;
        }
    }
}

