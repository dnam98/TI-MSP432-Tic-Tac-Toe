/*
 * tic_tac_toe.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 *
 *  YouTube Video Link: https://youtu.be/MK4BnVNeAro
 */
#ifndef __TIC_TAC_TOE_H__
#define __TIC_TAC_TOE_H__

#include <stdbool.h>
#include "lcd.h"
#include "ps2.h"
#include "timer32.h"
#include "tic_tac_toe_images.h"

#define SCREEN_X            132
#define SCREEN_Y            132

#define SCREEN_CENTER_COL    ((SCREEN_X/2)-1)
#define SCREEN_CENTER_ROW    ((SCREEN_Y/2)-1)

#define LINE_WIDTH          4
#define LINE_LENGTH         (SCREEN_X - 1)

#define SQUARE_SIZE         32

#define PADDING             2

#define LEFT_COL                      (SCREEN_CENTER_COL - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_COL                    (SCREEN_CENTER_COL)
#define RIGHT_COL                     (SCREEN_CENTER_COL + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_ROW                     (SCREEN_CENTER_ROW - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_ROW                    (SCREEN_CENTER_ROW)
#define LOWER_ROW                     (SCREEN_CENTER_ROW + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define LOWER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#define LEFT_HORIZONTAL_LINE_X      (SCREEN_CENTER_COL - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define RIGHT_HORIZONTAL_LINE_X     (SCREEN_CENTER_COL + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)



/*******************************************************************************
* Function Name: tic_tac_toe_hw_init
********************************************************************************
* Summary: Initializes all the hardware resources required for tic tac toe game
*          (IO Pins, ADC14, Timer32_1, Timer32_2).
* Returns:
*  Nothing
*******************************************************************************/ 
void tic_tac_toe_hw_init(void);

/*******************************************************************************
* Function Name: tic_tac_toe_example_board
********************************************************************************
* Summary: Prints out an example of what the tic-tac-toe board looks like
* Returns:
*  Nothing
*******************************************************************************/
void tic_tac_toe_example_board(void);

/*******************************************************************************
 * Function Name: check_O
 ********************************************************************************
 * Summary: Checks winning condition for O
 *
 * Returns:
 *      - true if win condition met
 *      - false, otherwise
 *******************************************************************************/
bool check_O(void);

/*******************************************************************************
 * Function Name: check_X
 ********************************************************************************
 * Summary: Checks winning condition for X
 *
 * Returns:
 *      - true if win condition met
 *      - false, otherwise
 *******************************************************************************/
bool check_X(void);

/*******************************************************************************
 * Function Name: reset_board
 ********************************************************************************
 * Summary: Resets the board when game over
 *
 * Returns:
 *  Nothing
 *******************************************************************************/
void reset_board(void);

/*******************************************************************************
* Function Name: tic_tac_toe_play_game
********************************************************************************
* Summary: Enters the code that plays the tic_tac_toe game.  Once called, this
*          function should never return!
* Returns:
*  Nothing
*******************************************************************************/
void tic_tac_toe_play_game(void);
#endif
