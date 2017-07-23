// A program to use arduino as analog input/output device through a simple serial interface.
// Probably someone else already wrote something better, but I didn't find it.
// Makes a nice quick, cheap, crappy data acquisition board

// I'm using a library called CmdMessenger which seems to be standard for sending commands over serial
// The command syntax is "commandnum, arg1, arg2, arg3;"
// i.e. to set a voltage on pin 10 you could send "0, 10, 128;"

// The reply also follows this syntax, unfortunately, so you need to parse out the number.
// Leaves room to expand the script to do something more advanced later.

// Notice all voltage levels are represented by the digital level
// Also note that the "analog outputs" actually generate PWM signals, at quite low frequency, so filter them if necessary.

// Author: Tyler Hennen
// 2017-07-23

#include <CmdMessenger.h>

int analogWriteValue;
int analogReadValue;
int analogWriteChannel;
int analogReadChannel;

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// This is the list of recognized commands. These can be commands that can either be sent or received.
enum
{
  AO                   , // Command to set analog out
  AI                   , // Command to read analog input
  Reply                , // Command to report status
};

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(AO, AnalogOutput);
  cmdMessenger.attach(AI, AnalogInput);
}

// Callback function that changes the analog output level
void AnalogOutput()
{
  analogWriteChannel = cmdMessenger.readInt16Arg();
  analogWriteValue = cmdMessenger.readInt16Arg();
  analogWrite(analogWriteChannel, analogWriteValue);
  
  // Don't bother replying to this.  Who wants to read that anyway?
  //cmdMessenger.sendCmdStart(Reply);
  //cmdMessenger.sendCmdArg("Wrote some voltage to analog out");
  //cmdMessenger.sendCmdArg(analogWriteValue);
  //cmdMessenger.sendCmdEnd();
}

// Callback function that reads the analog input
void AnalogInput()
{
  analogReadChannel = cmdMessenger.readInt16Arg();
  analogReadValue = analogRead(analogReadChannel);
  
  cmdMessenger.sendCmdStart(Reply);
  cmdMessenger.sendCmdArg(analogReadValue);
  cmdMessenger.sendCmdEnd();
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(Reply,"Command without attached callback");
}

// Setup function
void setup()
{
  // Listen on serial connection for messages from the PC
  Serial.begin(9600); 

  // Adds newline to every command
  cmdMessenger.printLfCr();   

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();

  // Set all digital pins for output
  //pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

// Loop function
void loop() 
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}


