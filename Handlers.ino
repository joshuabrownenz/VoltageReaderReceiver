void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(outputB) == LOW)
    {
      deltaEncoder-- ; // Could be -5 or -10
    }
    else {
      deltaEncoder++ ; // Could be +5 or +10
    }
    dataChanged = true;
  }
  // Keep track of when we were here last (no more than every 5ms)
  lastInterruptTime = interruptTime;
}

void checkEncoder()
{
  if(digitalRead(switchPin) && beenPressed)
  {
    beenPressed = false;
    
    bool hold = pressTime + holdTime < millis();
    if(hold)
    {
      noToneAC();
      toneAC(400, 10, 150, true); 
    }
    onClick(hold);

  }
  else if(!digitalRead(switchPin) && !beenPressed)
  {
    noToneAC();
    toneAC(400, 10, 150, true); 
    pressTime = millis();
    beenPressed = true;
  }
  
}

void onClick(bool hold)
{
  if(hold)
  {
    
  }
  else
  {
    if(menu == 0 || menu == 1)
    {
      menu = 1;
      int connections = 0;
      for(int i = 0; i < 3; i++)
      {
        if(activePipes[i])
          connections++;
      }
      if(menuItem == connections)
      {
         menuItem = 0;
         menu = 0;
         return;
      }      
      menuItem++;
      rewriteLCD();
    }
  }
}