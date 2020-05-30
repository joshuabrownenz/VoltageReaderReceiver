unsigned long letterFlash;
void menuOne()
{
  if(menu != 0)
    return;
  if(letterFlash + 500 < millis())
  {
    if(on)
    {
      addToLCD(0, menuItem, " ");
    }
    else
    {
      
      addToLCD(0, menuItem, 
    }
  }
}
