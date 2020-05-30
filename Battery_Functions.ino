void checkMinVoltages()
{
  for(int i = 0; i < 3; i++)
  {
    if(activePipes[i])
    {
      if(data[i]/numOfCells < minVoltage)
        alarm = true;
    }
  }
}

void checkTimeout()
{
  bool change = false;
  for(int i = 0; i < 3; i++)
  {
    if(activePipes[i])
    {
      if(!timeout[i])
      {
        if(receiveTimes[i] + 10000 < millis())
        {
          timeout[i] = true;
          change = true;
          toneAC(500, 10, 150, true);
        }
      }
    }
  }
  if(change)
    rewriteLCD();
}

int convertToPercentage(float cellVoltage)
{
  return floor((cellVoltage - minVoltage)/(maxVoltage - minVoltage) * 100);
}
