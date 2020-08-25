HI! If you are reading this file, you probably came from the .cfg file, or you just stumbled upon this file.
Anyways, this file is supposed to explain the use of the .cfg that comes with this.

The ConfigFile class that is supposed to come with this should be included into whatever application you are using this for.

Feature 1: Sub-sections
These act as pseudo folders to organise your work, if needed.
They are identified by being enclosed by [] (e.g. [FOLDER] )
The sub section name should be called in the ConfigFile class to know what subsection of data you are using.

Hashtag:
If you see "#1", "#2" etc, that means you have multiple sub sections.
These should not be left out, as it marks the next section for edit.

Variable assignment:
All variables that you want to be assigned in the application, should come in the following format
VariableName = VariableValue (e.g speed = 1.0)

Information on how to parse the information to and from the .cfg file is in the .h and .cpp files.
( Namely functions ParseToFile() and ParseFromFile() )

// Disclaimer: Depending on the use of this class for your application, your config file  WILL look different

Example of config file:

// Config file
#1
[Main Settings]
forward = W
left = A
back = S
right = D

#2
[Misc Settings]
shader = 0
lights = 1

