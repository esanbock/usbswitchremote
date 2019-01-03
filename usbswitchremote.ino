#include <IRremote.h>
#include <IRremoteInt.h>


// constants won't change. They're used here to set pin numbers:
const int ledPin =  LED_BUILTIN;      // the number of the LED pin
const int PINMIN = 4;
const int PINMAX = 7;
const int PINIR = 11;

const long IR_CableA = 2495;
const long IR_CableB = 447;
const long IR_GameA = 3009533835;
const long IR_GameB = 2733336051;
const long IR_Aux1A = 2583465647;
const long IR_Aux1B = 2307267863;
const long IR_Aux2A = 2231727814;
const long IR_Aux2B = 720141854;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

IRrecv irrecv(PINIR);     // create instance of 'irrecv'
decode_results irDecodeResult;      // create instance of 'decode_results'


void setup() {
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // IR setup
  irrecv.enableIRIn();

  // initialize output pins
  for( int i=PINMIN; i <= PINMAX; i++ )
  {
    pinMode(i,OUTPUT);
    digitalWrite(i, HIGH);
  }
  
}

void loop() 
{
  if( irrecv.decode(&irDecodeResult)) // have we received an IR signal?
  {
    translateIR(irDecodeResult);
    irrecv.resume();
  }
  
  if (stringComplete) 
  {
    stringComplete = false;
    processCommCommand();
  }
}

void fakePush(int pin)
{
    digitalWrite(ledPin, HIGH);
    Serial.print("switching to pin " );
    Serial.print(pin);
    Serial.print("\n");
    digitalWrite(pin, LOW);
    delay(50);
    digitalWrite(pin, HIGH);
    digitalWrite(ledPin, LOW);
}

void translateIR(decode_results& ir)
{
  switch(ir.value)
  {
    case IR_GameA:
    case IR_GameB:
      fakePush(PINMIN);
      break;
    case IR_CableA:
    case IR_CableB:
      fakePush(PINMIN + 1);
      break;
    case IR_Aux2A:
    case IR_Aux2B:
      fakePush(PINMIN + 2);
      break;
    case IR_Aux1A:
    case IR_Aux1B:
      fakePush(PINMIN + 3);
      break;
    default:
      Serial.print("unknown IR:");
      Serial.println(ir.value);
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processCommCommand()
{
    int val = inputString.toInt();
    inputString = "";
    if( val > 0 && val <= PINMAX - (PINMAX - PINMIN) )
    {
      fakePush(val + PINMIN - 1);
    }
    else
    {
      Serial.println("DK");
    }
}
