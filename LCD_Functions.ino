void displayLCD(){
  if(LCDData != prevLCDData)
  {  for(int y = 0; y < 4; y++)
    {
      for(int x = 0; x < 20; x++)
      {
        if(LCDData != prevLCDData)
        {
          LCD.setCursor(x,y);
          LCD.print(LCDData[y][x]);
          prevLCDData[y][x] = LCDData[y][x];
        }
      }
    }
  }
}

void addToLCD(int cursorX, int cursorY, String text)
{
  for(int i = cursorX; i < (cursorX + (int)text.length()) && i < 20; i++)
  {
    LCDData[cursorY][i] = text[i - cursorX];
  }
}



void rewriteLCD()
{
//  LCD.noCursor();
  clearLCDData();
  addToLCD(0,0,"Battery Percentages");
  int line = 1;
  for(int i = 0; i < 3; i++)
  {
    if(activePipes[i])
    {
      if(timeout[i])
      {
        addToLCD(0, line, (String)(i) + ": " + convertToPercentage(data[i]/numOfCells) + "% " + "Lost");
      }
      else
      {
//        addToLCD((index%2)*10, floor(index/2) + 1, (String)(i+1) + ": " + convertToPercentage(data[i]) + "%");
        addToLCD(0, line, (String)(i) + ": " + convertToPercentage(data[i]/numOfCells) + "% " + data[i]/numOfCells + "V P/CELL ");
      }
      line++;
    }
  }
}

void clearLCDData()
{
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 20; x++)
    {
      LCDData[y][x] = ' ';
    }
  }
}
