// Check if the batteries are below the min voltage
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

// Check if the transmitters have lost connection
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

//Sound create alarm sounds
void alarm()
{
  if(alarm && alarmWaitTime + 30000 < millis())
  {
    if(alarmTime + 300 < millis())
    {
      toneAC(400, 10, 150, true);
      alarmCount++;
    }
    if(alarmCount>10)
    {
      alarmWaitTime = millis();
      alarm = false;
      alarmCount = 0;
    }
  }
}

int convertToPercentage(float cellVoltage)
{
  return floor((cellVoltage - minVoltage)/(maxVoltage - minVoltage) * 100);
}
