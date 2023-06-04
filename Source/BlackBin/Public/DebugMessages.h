
#pragma once

#include "Engine/GameEngine.h"
/*----------------------------------------------------Section 1--------------------------------------------------------*/
//Prints a Single Message
#define print(Text) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT(Text),false);}
//ex: print("Hello");
#define printc(Text, Color) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 10.0f, Color, TEXT(Text),false);}
//printc("Hello", FColor::Black);
/*----------------------------------------------------Section 2--------------------------------------------------------*/
//Prints a Single Message using key method
#define printk(Key, Text) if(GEngine){GEngine->AddOnScreenDebugMessage(Key, 10.0f, FColor::Yellow, TEXT(Text));}
#define printkc(Key, Text, Color) if(GEngine){GEngine->AddOnScreenDebugMessage(Key, 10.0f, Color, TEXT(Text));}
//-------------//
//we can use -1, 1 and 2 for the keys, check examples below. 
//1 to replace the new data, avoid filling the screen.
//-1 to not replace, fil the screen in the up direction.
//2 to replace the new data, avoid filling the screen. (still need to practice)
//-------------//
//ex:printk(1, "Hello 1");
//ex:printk(-1, "Hello 2");
//ex:printk(2, "Hello 3");
/*----------------------------------------------------Section 3--------------------------------------------------------*/
//Prints Variables Data
#define printf(Format, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__), false);}
// %f = float value
// %d = int value
// %s = string value
//float pi = 3.14f;
//int32 a = 5;
//FString s(TEXT("ANONYMOUS"));
//ex: printf("Calling printf with the value: %f", pi); //printing a value from a single variable.
//ex: printf("Calling printf with the values: %f, %d, %s", pi, a, *s ); //printing values from multiple variables.
/*----------------------------------------------------Section 3--------------------------------------------------------*/
//Prints Variables Data using key method
#define printfk(Key, Format, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(Key, 10.0f, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__));}
//-------------//
//we can use -1, 1 and 2 for the keys, check examples below. 
//1 to replace the new data, avoid filling the screen.
//-1 to not replace, fil the screen in the up direction.
//2 to replace the new data, avoid filling the screen. (still need to practice)
//-------------//
// %f = float value
// %d = int value
// %s = string value
//float pi = 3.14f;
//int32 a = 5;
//FString s(TEXT("ANONYMOUS"));
//ex: printfk(1, "Calling printf with the value: %f", pi); //printing a value from a single variable.
//ex: printfk(1, "Calling printf with the values: %f, %d, %s", pi, a, *s ); //printing values from multiple variables.
//ex: printfk(-1, "Calling printf with the value: %f", pi); //printing a value from a single variable.
//ex: printfk(-1, "Calling printf with the values: %f, %d, %s", pi, a, *s ); //printing values from multiple variables.
//ex: printfk(2, "Calling printf with the value: %f", pi); //printing a value from a single variable.
//ex: printfk(2, "Calling printf with the values: %f, %d, %s", pi, a, *s ); //printing values from multiple variables.
/*----------------------------------------------------Section 3--------------------------------------------------------*/
