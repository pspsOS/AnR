# Task
This document was last edited by Person on *D Mon. YYYY*. <br/>

## Description
This is a documentation for the code supporting the task _Task_. <br>
Created for use by Purdue Space Program Solids. <br><br>
[View][1] the _Task Flow Chart_. <br>

__Required Files:__
* [task.h](../MITCH_2021/IncPSP/task.h)
* [task.c](../MITCH_2021/SrcPSP/task.c)



## task.h
_task.h_ is a header file. A header file is a file with extension _.h_ which contains C function declarations and macro definitions to be shared between several source files. <br>
```c
void setup();
void function2();
``` 
The above is the main contents of _task.h_. This declares all the functions created in _task.c_.

## task.c
_task.c_ is a source file. A source file is a file with extension _.c_ that contains program instructions. <br> <br>
_task.c_ acquires the raw data from the sensors and sends the data to processing via the global interface structs.

```c
float var1[4]      // Array of floats that is 4 long
bool var2;         // bool that keeps track of something
ui8 var3;          // Comments like these are great
```
The above declares the local variables. Listed below are the current variables:
* __var1[4]__ is an array of floats that keeps track of something. An explanation would be defined here.
* __var2__ is a bool that keeps track of something else.
* __var3__ is a ui8 which is a 8 bit unsigned int. If it is relevant to mention that it is used later in [function2](###function2), then linking to it like this is a good way to indicate that fact.

### setup()
```c
void setup() {
	//initialize variables
	for(int i = 0; i < 4; i++)
    	var1[i] = i * 3.4;
    var2 = false;
    var3 = 29;
}
```
Each function should be isolated and described in one or more paragraphs, making its use clear. For example: <br> 
The above function is the one-time setup function for _task.c_. This function only initializes local variables.


### function2()
```c
void function2() {
	var3 = !var3;
}
```
The above function is linked to earlier in the document. You can use links to increase readability.


<!--- You can make an list of links at the end of a file like the one below. This makes it so there isn't a giant link in the middle of your text-->

[1]: https://drive.google.com/file/d/17b3zl4mSmjUOjUfPsQ9OTFR39xYNh1Iw/view?usp=sharing "Task Flow Chart (Read Only)"
