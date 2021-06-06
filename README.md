# PROG4
link to githud repo:  https://github.com/noachvdk/PROG4

Threading times in the levelparser with shape:
With threading 5.734ms
Without threading 8.094ms

You have 2 different files that are being parsed.
1) the shape.txt file in which you describe the amount of rows and then the shape.
2) the layout.txt file in which you describe the levels

I chose to do it this way so you can easily change/add/remove levels without messing up the shape of the actual level.
and this way you only need to load in the level once which works well for qbert.

Sound is played through the service locator.
The soundsystem uses an eventque and runs on a seperate thread. 

The input manager has some fixed inputs to quit the application.
Every scene has a vector of inputactions which you can set. This way you can change what inputactions per scene.
The player component handles the inputactions.

For the deltatime you will need the TimeManager singleton.
I chose this way to avoid having to pass the deltatime everywhere and instead make it more easily accesible.

The observer/subject pattern is also implemented.
