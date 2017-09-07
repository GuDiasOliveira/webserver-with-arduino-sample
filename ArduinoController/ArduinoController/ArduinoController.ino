/*
 Name:		ArduinoController.ino
 Created:	06/09/2017 11:11:38
 Author:	gusta
*/

#include "TimerTask.h"

void write7seg(int number, boolean commonAnode = false);
void clear7seg(boolean commonAnode = false);
boolean set7seg(int number);
void toggleLedR();
void toggleLedY();
void toggleLedG();
void toggleLedW();

const int pinLedR = 2;
const int pinLedY = 3;
const int pinLedG = 4;
const int pinLedW = 5;
const int pinFirst7seg = 6;

TimerTask timerLedR;
TimerTask timerLedY;
TimerTask timerLedG;
TimerTask timerLedW;

const int array7seg[10][7] =
{
	{ 1,1,1,1,1,1,0 },  // 0
	{ 0,1,1,0,0,0,0 },  // 1
	{ 1,1,0,1,1,0,1 },  // 2
	{ 1,1,1,1,0,0,1 },  // 3
	{ 0,1,1,0,0,1,1 },  // 4
	{ 1,0,1,1,0,1,1 },  // 5
	{ 1,0,1,1,1,1,1 },  // 6
	{ 1,1,1,0,0,0,0 },  // 7
	{ 1,1,1,1,1,1,1 },  // 8
	{ 1,1,1,0,0,1,1 }   // 9
};

void write7seg(int number, boolean commonAnode = false)
{
	int pin = pinFirst7seg;
	for (int j = 0; j < 7; j++)
	{
		digitalWrite(pin, array7seg[number][j] ^ commonAnode);
		pin++;
	}
}

void clear7seg(boolean commonAnode = false)
{
	int pin = pinFirst7seg;
	for (int j = 0; j < 7; j++)
	{
		digitalWrite(pin, commonAnode);
		pin++;
	}
}

boolean set7seg(int number)
{
	if (number < 0 || number > 9)
	{
		clear7seg(true);
		return false;
	}
	else
	{
		write7seg(number, true);
		return true;
	}
}

void toggleLedR()
{
	digitalWrite(pinLedR, !digitalRead(pinLedR));
}

void toggleLedY()
{
	digitalWrite(pinLedY, !digitalRead(pinLedY));
}

void toggleLedG()
{
	digitalWrite(pinLedG, !digitalRead(pinLedG));
}

void toggleLedW()
{
	digitalWrite(pinLedW, !digitalRead(pinLedW));
}

// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(pinLedR, OUTPUT);
	pinMode(pinLedY, OUTPUT);
	pinMode(pinLedG, OUTPUT);
	pinMode(pinLedW, OUTPUT);
	for (int j = 0; j < 7; j++)
		pinMode(pinFirst7seg + j, OUTPUT);
	
	Serial.begin(9600);

	set7seg(10);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	// Processing Commands from serial
	if (Serial.available())
	{
		char* command = nullptr;
		char* led = nullptr;
		char* input = new char[70];
		Serial.readStringUntil('\n').toCharArray(input, 70);
		boolean commandSuccess = false;
		for (char *p = strtok(input, " "); p != NULL; p = strtok(NULL, " "))
		{
			if (command == nullptr)
			{
				command = new char[70];
				strcpy(command, p);
			}
			else if (strcmp(strlwr(command), "num") == 0)
			{
				int num = atoi(p);
				set7seg(num);
				commandSuccess = true;
				break;
			}
			else if (strcmp(strlwr(command), "led") == 0)
			{
				if (led == nullptr)
				{
					led = new char[70];
					strcpy(led, p);
				}
				else if (strcmp(strlwr(led), "r") == 0)
				{
					int val = atoi(p);
					if (val <= 0 || val > 9)
					{
						timerLedR.setTask(nullptr);
						digitalWrite(pinLedR, val > 9);
					}
					else
					{
						timerLedR.setTask(toggleLedR);
						timerLedR.setIntervalMillis(1000 / val);
					}
					commandSuccess = true;
					break;
				}
				else if (strcmp(strlwr(led), "y") == 0)
				{
					int val = atoi(p);
					if (val <= 0 || val > 9)
					{
						timerLedY.setTask(nullptr);
						digitalWrite(pinLedY, val > 9);
					}
					else
					{
						timerLedY.setTask(toggleLedY);
						timerLedY.setIntervalMillis(1000 / val);
					}
					commandSuccess = true;
					break;
				}
				else if (strcmp(strlwr(led), "g") == 0)
				{
					int val = atoi(p);
					if (val <= 0 || val > 9)
					{
						timerLedG.setTask(nullptr);
						digitalWrite(pinLedG, val > 9);
					}
					else
					{
						timerLedG.setTask(toggleLedG);
						timerLedG.setIntervalMillis(1000 / val);
					}
					commandSuccess = true;
					break;
				}
				else if (strcmp(strlwr(led), "w") == 0)
				{
					int val = atoi(p);
					if (val <= 0 || val > 9)
					{
						timerLedW.setTask(nullptr);
						digitalWrite(pinLedW, val > 9);
					}
					else
					{
						timerLedW.setTask(toggleLedW);
						timerLedW.setIntervalMillis(1000 / val);
					}
					commandSuccess = true;
					break;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		delete command;
		delete led;
		delete input;
		Serial.println(commandSuccess ? "1" : "0");
		Serial.flush();
	} // end processing commands from serial
	timerLedR.run();
	timerLedY.run();
	timerLedG.run();
	timerLedW.run();
}