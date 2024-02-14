# Super-Mario

## Build Instructions

To be able to run this project, follow these steps:

### 1. Download OpenCV

Download the OpenCV library from [https://github.com/opencv/opencv/releases/tag/4.5.1](https://github.com/opencv/opencv/releases/tag/4.5.1) and place it under Super-Mario folder.

### 2. Update Visual Studio Project Properties

In Visual Studio, open the project properties and follow these steps:

- Navigate to Debugging -> Environment.
- Add the following to the PATH variable: `PATH=$(PATH);../OpenCV_451/bin $(LocalDebuggerEnvironment)`

Now, your project is configured to use OpenCV, and you can build and run it successfully.

## Game Instructions

To play the Super Mario game, use the following controls:

- To move right, press the 'd' key.
- To move left, press the 'a' key.
- To duck, press the 's' key.
- To jump, press the 'w' key.
- To throw a hand grenade, press the spacebar.

Adjust your strategy, overcome obstacles, and enjoy the adventure in the world of Super Mario!
