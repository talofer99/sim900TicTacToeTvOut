// CODE BY : TALOFER99@HOTMAIL.COM

//https://github.com/nootropicdesign/arduino-tvout 

// include the library
#include <TVout.h>
#include <fontALL.h>
// decalre the object
TVout TV;
// set up kb pins
int keyDown = 2;
int keyUp = 3;
int keyLeft = 4;
int keyRight = 5;
int keySelect = 6;
// define the cursor Row/Col
int cursorColumn ;
int cursorRow ;
boolean refreshDisplay = true; // refresh the screen ?
// tile size
int tileSize = 26;
int pixtocenter = 16;
// holds what is which box (X=3 // O=4)
int tiles[3][3];
// Player PLace holder
int placeHolder[2] = {3, 4};
// who playes 0/1
int CurrentPlayer;
// is gae over
int isGameOver;
int whoWon; // tricky ... 0 = no onw .. 1 = 0 2 -1 !!!:)

// **********************************************************************************************************************
// SETUP
// **********************************************************************************************************************
void setup() {
  // for debug - start serial
  Serial.begin(115200);
  Serial.println("System started");
  // random seed
  randomSeed(analogRead(2));
  // defien the analg 5 as the input select button
  pinMode(keySelect, INPUT_PULLUP);

  pinMode(keyDown, INPUT_PULLUP);
  pinMode(keyUp, INPUT_PULLUP);
  pinMode(keyLeft, INPUT_PULLUP);
  pinMode(keyRight, INPUT_PULLUP);
  // TV OUT SETUP
  TV.begin(PAL, 120, 96);
  TV.select_font(font6x8);
  // reset the game
  resetgame();
  Serial.print("done setting up");
} // END void setup




void move_up() {
  if (cursorRow > 0)
  { cursorRow = cursorRow - 1;
    refreshDisplay = true;
  } // end if (corsur_y >0)
} //end move_up

void move_down() {
  if (cursorRow < 2)
  { cursorRow = cursorRow + 1;
    refreshDisplay = true;
  } // end  if (cursorRow < 2)
} //end move down

void move_left() {
  if (cursorColumn > 0)
  { cursorColumn = cursorColumn - 1;
    refreshDisplay = true;
  }  // end if (corcur_x > 0)
} //end move_left


void move_right() {
  if (cursorColumn < 2)
  { cursorColumn = cursorColumn + 1;
    refreshDisplay = true;
  } // end if (cursorColumn <2)
} //end if

void checkAndExecuteTrun() {
  if (tiles[cursorRow][cursorColumn]  == 0 && CurrentPlayer == 0 )
  {
    playMove(cursorRow, cursorColumn);
  } //end if
} //end checkAndExecuteTrun

// **********************************************************************************************************************
// LOOP
// **********************************************************************************************************************
void  loop() {
  // **********************************************************************************************************************
  // process Serial
  // **********************************************************************************************************************
  if (Serial.available()) {
    char getChar = Serial.read();
    if (getChar == '8') {
      move_down();
    } //end if

    else if (getChar == '2') {
      move_up();
    } //end if

    else if (getChar == '4') {
      move_left();
    } //end if

    else if (getChar == '6') {
      move_right();
    } //end if

    else if (getChar == '5') {
      checkAndExecuteTrun();
    } //end if
    
  } //end if




  // **********************************************************************************************************************
  // check for key strokes
  // **********************************************************************************************************************
  if (!digitalRead(keyUp))  // UP
  {
    move_up();
  } // end  if (digitalRead(keyUp))

  if (!digitalRead(keyDown)) //DOWN
  {
    move_down();
  } // end  if (digitalRead(keyDown))

  if (!digitalRead(keyLeft))  //LEFT
  {
    move_left();
  } // end  if (digitalRead(keyLeft))

  if (!digitalRead(keyRight)) //RIGHT
  {
    move_right();
  } // end  if (digitalRead(keyRight))

  // if PALYER = 1 = computer  comput next move ...(leave here .. if not player move will nto refresh till computer one is done ...
  if (CurrentPlayer == 1)
  {
    computNextMove();
  }


  // select key
  if (!digitalRead(keySelect))
  { // if the place is avilabe (ie =0) we allow to palce the number
    checkAndExecuteTrun();
  } // end    if (digitalRead(keySelect))

  // if refresh needed
  if (refreshDisplay)
  {
    // clear screen
    TV.clear_screen() ;
    // draw the tiles
    drawTiles()  ;
    // draw the cursor
    drawCursor() ;
    // flip the flag back to FALSE
    refreshDisplay = false;
    // chck if game can stil be played
    checkendgame();
    // if is game over .. draw it ...
    if (isGameOver)
    {
      endgame();
    }
    //draw who turn it is
    drawWhoTurnitis();
    // general delay
    delay(500);
  }// end  if (refreshDisplay)
} // end void loop



// **********************************************************************************************************************
// CHECK FOR WIN/END GAME
// **********************************************************************************************************************
void checkendgame()
{
  // loop over row
  for (int row = 0; row < 3; row++) {
    // loop over col
    for (int col = 0; col < 3; col++) {
      // Calc the total of the ROW
      int totalRow = tiles[row][0] + tiles[row][1] + tiles[row][2] ;
      int totalCol = tiles[0][col] + tiles[1][col] + tiles[2][col] ;
      if (totalCol == 9 || totalRow == 9) {
        whoWon = 1;
        isGameOver = 1;
      }
      if (  totalRow == 12 || totalCol == 12) {
        whoWon = 2;
        isGameOver = 1;
      }

      // check diagonals
      if (row != 1 && col != 1) {
        // to know in wich dirctio nto go
        int rowDir =  row == 0 ? 1 : -1;
        int colDir =  col == 0 ? 1 : -1;
        int diagTotal =  tiles[row][col] + tiles[row + rowDir][col + colDir] + tiles[row + rowDir * 2][col + colDir * 2];
        if (diagTotal == 9) {
          whoWon = 1;
          isGameOver = 1;
        }
        if (  diagTotal == 12 ) {
          whoWon = 2;
          isGameOver = 1;
        }
      } //end if notro and col =1
    } // end col
  } //end row
} // end void


// **********************************************************************************************************************
// PLAY MOVE (row,col)
// **********************************************************************************************************************
void playMove(int row, int col) {
  // set the vlaue of the current player place holder
  tiles[row][col] = placeHolder[CurrentPlayer];
  // define to refresh screen
  refreshDisplay = true;
  // flip current player
  CurrentPlayer = CurrentPlayer == 0 ? 1 : 0;
} // end void playmovie

// **********************************************************************************************************************
// Compute points for tile
// **********************************************************************************************************************
int calcTotalPointsSetOfThree(int Sum) {
  int returnPoints = 0;
  int OtherPlayer = CurrentPlayer == 0 ? 1 : 0;
  int CurrentPlayerPlaceHolder = placeHolder[CurrentPlayer];
  int OtherPlayerPlaceHolder = placeHolder[OtherPlayer];

  // if equel player *  2 = 5 points its a win
  if (Sum == CurrentPlayerPlaceHolder * 2) {
    returnPoints = 5;
  }
  // if OtherPlayerPlaceHolder*2 that block the other player = 3 points
  else if (Sum == OtherPlayerPlaceHolder * 2)  {
    returnPoints = 3;
  }
  // if found one mroe tile
  else if (Sum == CurrentPlayerPlaceHolder) {
    returnPoints = 1;
  }
  return returnPoints;
} // end int calcTotalPointsSetOfThree

// **********************************************************************************************************************
// Compute next move
// **********************************************************************************************************************
void computNextMove() {
  // declare high points for tile
  int highpoints = 0;
  int highpointsRow = 4; //we use 4 as WAY bigger then the cube
  int highpointsCol = 4; //we use 4 as WAY bigger then the cube

  // lo0p over row
  for (int row = 0; row < 3; row++) {
    // loop over col
    for (int col = 0; col < 3; col++) {
      // if ZERO - free space
      if (tiles[row][col] == 0) {
        // Calc the total of the ROW
        int totalRow = tiles[row][0] + tiles[row][1] + tiles[row][2] ;
        int totalCol = tiles[0][col] + tiles[1][col] + tiles[2][col] ;
        // declare the othe player for the math
        int OtherPlayer = CurrentPlayer == 0 ? 1 : 0;
        int CurrentPlayerPlaceHolder = placeHolder[CurrentPlayer];
        int OtherPlayerPlaceHolder = placeHolder[OtherPlayer];
        int totalTilePoints = 0;
        // make 2 for self = 1 point **  block 2 of oppisite = 3 **  make 3 (win) = 5
        // add row trotal points
        totalTilePoints = totalTilePoints + calcTotalPointsSetOfThree(totalRow);
        // add col total points
        totalTilePoints = totalTilePoints + calcTotalPointsSetOfThree(totalCol);
        // check center
        if (row == 1 && col == 1)  {
          //ADD point for crners
          totalTilePoints = totalTilePoints + 3 ;
          // on diag
          int diagTotalONE =  tiles[0][0] + tiles[1][1] + tiles[2][2];
          // add diagn poins
          totalTilePoints = totalTilePoints + calcTotalPointsSetOfThree(diagTotalONE);
          // secdiag
          int diagTotalTWO =  tiles[2][0] + tiles[1][1] + tiles[0][2];
          // add diagn poins
          totalTilePoints = totalTilePoints + calcTotalPointsSetOfThree(diagTotalTWO);

        } // end if (row ==1 && col ==1)


        // check diagonals
        if (row != 1 && col != 1)  {
          //ADD point for crners
          // totalTilePoints = totalTilePoints +1 ;
          // to know in wich dirctio nto go
          int rowDir =  row == 0 ? 1 : -1;
          int colDir =  col == 0 ? 1 : -1;
          int diagTotal =  tiles[row][col] + tiles[row + rowDir][col + colDir] + tiles[row + rowDir * 2][col + colDir * 2];
          // add diagn poins
          totalTilePoints = totalTilePoints + calcTotalPointsSetOfThree(diagTotal);
          // add 1 more tests :
        } // END if (row !=1 && col !=1)

        // any of the outer center cubes
        if ((row == 1  && col != 1) || (row != 1  && col == 1))   {
          // if u got the center, pick this one
          if (tiles[1][1] == CurrentPlayerPlaceHolder)  {
            totalTilePoints = totalTilePoints + 2 ;
          }

        } // end if ((row == 1  && col != 1) || (row != 1  && col == 1))



        // check if maxed  high point
        if (highpoints < totalTilePoints)  {
          highpoints =  totalTilePoints;
          highpointsRow = row;
          highpointsCol = col;
        } // end if (highpoints<totalTilePoints)
      } // end (tiles[j][k] =!=0)
    } // end loop over col
  } // end loop over row

  // if found possible row
  if (highpoints > 0)  {
    playMove(highpointsRow, highpointsCol);
  }
  // if not found match .. random choose one of the corners
  else {
    // first check fi (1,1) is avialble
    int fodunfreespace =  0;
    // lop over row
    for (int j = 0; j < 3; j++) {
      // loop over col
      for (int k = 0; k < 3; k++) {
        // set to zero = empty
        if (tiles[j][k] == 0 && fodunfreespace == 0) {
          fodunfreespace = 1;
          playMove(j, k);
          break;
        } // end if(tiles[j][k] == 0)
      }  // end loop over col
    } // end loop over row

    // if not foudn any open space ... end game
    if (fodunfreespace == 0) {
      isGameOver = 1;
      refreshDisplay = true;
    } // end if (fodunfreespace == 0)
  } // end if  (highpoints >0)
} // end void computNextMove


// **********************************************************************************************************************
// DRAW who turn it is
// **********************************************************************************************************************
void drawWhoTurnitis() {
  TV.set_cursor( 5, 88);
  TV.print("YOU - X");
  TV.set_cursor( 70, 88);
  TV.print("COM - O");
  int Xposition = CurrentPlayer == 0 ? 0 : 65;
  TV.draw_rect( Xposition, 85, 50, 10, 1);;
} // end void drawWhoTurnitis

// **********************************************************************************************************************
// DRAW TILE
// **********************************************************************************************************************
void drawTiles() {
  // lop over row
  for (int j = 0; j < 3; j++) {
    // loop over col
    for (int k = 0; k < 3; k++) {
      // draw the box
      TV.draw_rect(tileSize * j + pixtocenter , tileSize * k, tileSize, tileSize, 1);
      // if not ZERO - draw
      if (tiles[j][k] != 0)
      { // goto location
        TV.set_cursor(  tileSize * k + tileSize / 2 - 2 + pixtocenter , tileSize * j + tileSize / 2 - 2);
        // 3 =x // else(4) = O
        if (tiles[j][k] == 3)
        {
          TV.print("X");
        }
        else
        {
          TV.print("O");
        }
      }// end (tiles[j][k] !=0)
    }// end loop over col
  }// end loop over row
} // end void


// **********************************************************************************************************************
// DRAW CUSOR
// **********************************************************************************************************************
void drawCursor() {
  // get x/y
  int x_loc = (cursorColumn) * tileSize + tileSize / 2 + pixtocenter;
  int y_loc = (cursorRow) * tileSize + tileSize / 2;
  TV.draw_circle(  x_loc, y_loc , 6, 1);
  TV.draw_circle(  x_loc, y_loc , 7, 1);
} // end void drawCursor


// **********************************************************************************************************************
// END GAME
// **********************************************************************************************************************
void endgame()  {
  // draw game over
  TV.set_cursor( 2, 88);
  TV.print("GaMe OvEr ");
  //TV.set_cursor( 62, 88);
  if (whoWon == 1)
  {
    TV.print("YOU WIN");
  }
  else if (whoWon == 2)
  {
    TV.print("COM WIN");
  }
  else if (whoWon == 0)
  {
    TV.print("NO WINNER");
  }
  // to prevent the pressed button to over shot this call ...
  delay(1000);
  // loop doiogn nothing till the user hit the select key
  int selectState = 1 ;
  // while the button is LOW loop .. till you press it - simple way to make hte function "wait" for the button press.
  while (selectState)   {
    if (Serial.available()) {
      if (Serial.read() == '5') {
        break;
      } //end if 
    } //end if 
    // statement block - do nothing
    selectState = digitalRead(keySelect) ;
  } //end while 
  // now reset the game :)
  resetgame();
  // to prevent the pressed button to play teh first move ...
  delay(200);
} // end void endgame


// **********************************************************************************************************************
// RESET GAME
// **********************************************************************************************************************
void resetgame()  {
  // clear screen & print first line
  TV.clear_screen() ;
  // set refresh to true
  refreshDisplay = true;
  // loop over row
  for (int j = 0; j < 3; j++) {
    // loop over col
    for (int k = 0; k < 3; k++) {
      // set to zero
      tiles[j][k] = 0;
    } // end loop over col
  } // end loop over row

  // set player
  CurrentPlayer = 0; // player
  // set is game over to no
  isGameOver = 0;
  whoWon = 0;
  // set cursor
  cursorColumn = 1;
  cursorRow = 1;
}  // end void resetgame()


