#Doggie
A psychology visual experimental platform

![main](https://github.com/Code-Guy/Doggie/blob/master/snapshots/(%60~BPP1(267%25B%5B2%7BF@PS40G.png?raw=true)

###Introduction
We integrated some common psychology visual experimental elements(Stimulus Object, SObj) into one platform: Audio, Demo, Face, Model, Optical Flow, Optical Grating, Picture, Shape, Simple Face, Text, Video. User can observe and config these SObjs on our platform, and save all of these into a project file, which can be opened and edited whenever necessary.

###Feature
 - Plugin-based framework. There are 3 parts of the project: Platform, Utility and Plugins. Platform is responsible to show the main window, managers the scene with lots of SObj; Utility has many useful tool classes and functions, it also implements the SObj base class and SObj interface class. There classes can be invoked in both Platform and Plugins; Plugins implements the real SObj derived classes, which are the core functional part of the whole system.
 - Support all common operations of classical software: copy, paste, cut, group, ungroup, translate, rotate, scale, select, drag, drop...
 - Dynamic property panel generating
 - Dynamic explicitly dll calling
 - Video frame extracting by vlc and displaying with opengl
 - History(undo/redo) mechanism supported by command design pattern
 - MSAA by multi-sample texture and fbo blitting
 - Pixel precision picking
 
###Framework
 Qt + OGL + Assimp + freetype + glm + vlc
 
###More Examples
![overlook](https://github.com/Code-Guy/Doggie/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113205329.png?raw=true)

![overlook](https://github.com/Code-Guy/Doggie/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113205634.png?raw=true)

![overlook](https://github.com/Code-Guy/Doggie/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113205557.png?raw=true)
