
void displayLCD(){
  if(LCDData != prevLCDData)
  {  for(int y = 0; y < 4; y++)
    {
      for(int x = 0; x < 19; x++)
      {
        if(LCDData != prevLCDData)
        {
          LCD.setCursor(x,y);
          LCD.print(LCDData[y][x]);
          prevLCDData[y][x] = LCDData[y][x];
        }
      }
    }
    int line = 0;
    for(int i = 0; i < 3; i++)
    {
      if(activePipes[i])
      {
        line++;
        LCD.setCursor(19,line);
        if(silentPipes[i])
        {
          LCD.write(byte(1));
        }
        else
        {
          LCD.write(byte(0));
        }
      }
    }
  }
}

void addToLCD(int cursorX, int cursorY, String text)
{
  for(int i = cursorX; i < (cursorX + (int)text.length()) && i < 19; i++)
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
        addToLCD(0, line, (String)(char)(addressLetters[i]) + ": " + convertToPercentage(data[i]/numOfCells) + "% " + "Lost");
      }
      else
      {
//        addToLCD((index%2)*10, floor(index/2) + 1, (String)(i+1) + ": " + convertToPercentage(data[i]) + "%");
        addToLCD(0, line, (String)(char)(addressLetters[i]) + ": " + convertToPercentage(data[i]/numOfCells) + "% " + data[i]/numOfCells + "V P/CELL ");
      }
      line++;
    }
  }
}

void clearLCDData()
{
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 19; x++)
    {
      LCDData[y][x] = ' ';
    }
  }
}
