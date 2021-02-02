#include <DigiKeyboard.h>
#include <string.h>

#define LED_PORT  0x01
#define DELAY_LED 1000

// Universal Serial Bus HID Usage Tables (https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
#define KEY_PAD_1 0x59
#define KEY_PAD_2 0x5A
#define KEY_PAD_3 0x5B
#define KEY_PAD_4 0x5C
#define KEY_PAD_5 0x5D
#define KEY_PAD_6 0x5E
#define KEY_PAD_7 0x5F
#define KEY_PAD_8 0x60
#define KEY_PAD_9 0x61
#define KEY_PAD_0 0x62

#define DELAY_AFTER_START       5000
#define DELAY_BETWEEN_KEYSTROKE 0
#define DELAY_BETWEEN_CHAR      0
#define DELAY_BETWEEN_COMMAND   1000

// Array for transform numeric of keypad to key HID code 
byte const codeKeyOfKeypad[10] = {KEY_PAD_0, KEY_PAD_1, KEY_PAD_2, KEY_PAD_3, KEY_PAD_4, KEY_PAD_5, KEY_PAD_6, KEY_PAD_7, KEY_PAD_8, KEY_PAD_9};

unsigned int multipliers[4] = {1, 10, 100, 1000}; // It's faster then multiplication


void sendPartKeyCode(unsigned short keyCode)
{
  static unsigned short prevKeyCode = 0;

  if(prevKeyCode == keyCode)
    DigiKeyboard.sendKeyPress(0, MOD_ALT_LEFT);
    
  DigiKeyboard.sendKeyPress(codeKeyOfKeypad[keyCode], MOD_ALT_LEFT);
  DigiKeyboard.delay(DELAY_BETWEEN_KEYSTROKE);

  prevKeyCode = keyCode;
}


// Send ASCII chain of ASCII char
void sendAsciiString(String asciiString)
{
  
  int lenghtOfString = asciiString.length();

  for(unsigned int charPosition=0; charPosition<lenghtOfString; charPosition++)
  {
    char currentChar = asciiString.charAt(charPosition);

    for(int dec=3; dec>=0; dec--)
    {
      unsigned short keyCode = ((unsigned short)currentChar / multipliers[dec]) % 10;   // It's faster then multiplication
      sendPartKeyCode(keyCode);
    }

    DigiKeyboard.sendKeyPress(0, 0);
  }
    DigiKeyboard.delay(DELAY_BETWEEN_CHAR);  
}


void setup() {

  pinMode(LED_PORT, OUTPUT);

  DigiKeyboard.delay(DELAY_AFTER_START);


  digitalWrite(LED_PORT, HIGH);
  
  payLoad();

  digitalWrite(LED_PORT, LOW); 
 
}



void loop() {


}


// Commands put here
void payLoad()
{
//  https://tech-geek.ru/powershell-download-file/
// powershell –c “(new-object System.Net.WebClient).DownloadFile(‘https://cdn.stubdownloader.services.mozilla.com/builds/firefox-stub/en-US/win/b2b7944c6fed1b91a97ca9198cddf6ffb43396b4c28c88348a3b3e23dee2f163/Firefox%20Installer.exe’,’C:\tmp\FirefoxInstaller.exe’)”
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(DELAY_BETWEEN_COMMAND);
  sendAsciiString("powershell.exe");
  DigiKeyboard.delay(DELAY_BETWEEN_COMMAND);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(DELAY_BETWEEN_COMMAND);
  sendAsciiString("$wshell = New-Object -ComObject Wscript.Shell");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(DELAY_BETWEEN_COMMAND);
  sendAsciiString("$Output = $wshell.Popup(\"Hello!\")");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
}
