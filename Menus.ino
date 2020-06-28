// Time of letter flash
unsigned long letterFlash;

// Change the letter that defines each wireless module using the rotary encoder
void menuOne()
{
  // If the menu one is not meant to be open
  if(menu != 1)
    return;

  //Flash the character selected
  if(letterFlash + 300 < millis())
  {
    if(menuOneCharacterOn)
    {
      addToLCD(0, menuItem, " ");
      menuOneCharacterOn = false;
    }
    else
    {
      addToLCD(0, menuItem, (String)(char)addressLetters[editingAddress]);
      menuOneCharacterOn = true;
    }
    letterFlash = millis();
  }

  //Change the character to the degree of the spin of the encoder and save to peristant memory
  if(deltaEncoder != 0)
  {
    byte num = addressLetters[editingAddress];
    for(int i = 0; i < abs(deltaEncoder); i++)
    {
      num -= deltaEncoder/abs(deltaEncoder);
      switch(num)
      {
        case 123:
          num = 65;
          break;
        case 91:
          num = 48;
          break;
        case 58:
          num = 97;
          break;
        case 96:
          num = 57;
          break;
        case 46:
          num = 90;
          break;
        case 64:
          num = 122;
          break;
      }
    }
    addressLetters[editingAddress] = num;
    deltaEncoder = 0;
    addToLCD(0, menuItem, (String)(char)num);
    menuOneCharacterOn = true;
    EEPROM.write(editingAddress, num);
    letterFlash = millis();
  }
}
// Clear saved letters
void ResetEEPROM()
{
  addressLetters[0] = 48;
  addressLetters[1] = 49;
  addressLetters[2] = 50;
  EEPROM.write(0,48);
  EEPROM.write(1,49);
  EEPROM.write(2,50);
  rewriteLCD();
}
