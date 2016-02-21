# Table of Contents #

  1. [Introduction](#Introduction.md)
  1. [Details](#Details.md)
    1. [A simple example](#A_simple_example.md)
    1. [Complex targets](#Complex_targets.md)
    1. [Rules](#Rules.md)
    1. [Events and listeners](#Events_and_listeners.md)

# Introduction #

Lunar's "configuration" is (in style of ant) saved in `build.lua` files. These files exist per buildable directory.

Just like `Makefiles` or `build.xml`-files they can specify multiple targets. Because we use Lua as parser for these files you can easily include "templates" and create specific targets from prototypes.

What Lunar does is providing built-in prototypes for targets, files and such, so you can get a working `build.lua` file without much effort.

This article describes the architecture and gives a short overview over the basics. You should have a basic understanding of Lua to understand the details, but most examples explain themself rather good. If you want to learn about Lua, please go to [their website](http://www.lua.org). This article does not try to be an in-depth reference. Please refer to the manual (included as manpage and PDF file in the downloadable packages) for details.


# Details #

## A simple example ##

Let's start with a basic example to compile a `hello_world.c` file into a `hello_world` executable:

```
myproject = lunar.project:new( "Hello World" )

mytarget  = lunar.target:new( "binary" )
mytarget.input_files:add( "source/hello_world.c" )
mytarget.output_files:add( "bin/hello_world" )

myproject.targets:add( mytarget )
```

... and that's just it! So what does this do? First you create a new project by calling the constructor. If you are familiar with Lua you know that it is a prototype-based language. However it is possible not to create a full copy, but only "references" instead. And that's exactly what our `new` function does: It creates a table with required fields and references to the actual functions.

By convention we use the `:` operator for this purpose and not the `.` operator. We do not really need the reference to `lunar.project` here but it's consistent and it's easier to remember that each and every method has to be called with a `:`.

The only parameter for `new()` is the project name. It has no actual functionality but can be used to obtain a reference to the project variable (`lunar.project:find( "Hello World" )`).

Next we create a new target the same way. The target get's a name as well. You can later use this when you call the `lunar` executable to build a specific target.

After that we add input and output files to the target and add the target to the project.

The input files are somewhat like the dependencies of make while the output files represent make's targets or at least those that produce files with the same name .. and that's just one point of make that's a little bit messy, isn't it?

To build the project you just have to go the directory with your `build.lua` file and type `lunar`. That's it.

You might wonder **why** it works. In fact this example makes use of a lot of implicitness. The simplest is that lunar will try to build the only target if no other targets are present. If you have multiple targets you can either specify a default target (`myproject.targets:set_default( "targetname" )`) or specify the target via the command line.
What's more complex is the way the target itself works. With it's default configuration lunar "knows" how to make executables from .c-files. When you specify a .c file as input and a file without extension a output lunar guesses that you want to make an executable.


## Complex targets ##

The rules that are applied are far more complex so you could as well do something like this:

```

mytarget.input_files:add( "test.c", "util.c", "util.h", "external.o" )
mytarget.output_files:add( "test", "util.o" )

```

Lunar will first notice that it has to create an executable from c files and an object. Then it goes on and sees that it has to create an object as well. It guesses that the object "util.o" should be created from "util.c" and "util.h". To have optimal performance it then splits up the target into multiple tasks. The first one would be compiling "util.c" and "util.h" to "util.o" and compiling "test.c" to a temporary object. These steps can and will be done in parallel. The next task is to link the temporary object, "util.o" and "external.o" into the executable "test" after that the target is done (since all output files have been updated).

What I did not mention above is that, when executing tasks (which are like atomic targets with one only output file), Lunar checks if any of the input files can be rebuilt using existing rules. After expanding all recursive dependencies Lunar checks if any of the input files is newer than the output file. If there are no newer files the building step is skipped.

## Rules ##

You can of course specify your own rules and disable the built-in rules if this is too much implicitness for you:

```

lunar.rules:clear()
myrule = lunar.rules:new( "Compile using GCC" )

myrule.filter = function( filter, target )
  -- since we call using `:` filter is like a
  -- this in other languages
  -- do something using target
  -- return true if you think you can build
  -- the target with myrule, false otherwise
end

myrule.action = function( filter, target )
  -- do something using target
  -- return true if the action (build) succeeded
  -- false otherwise
end

```

This example first clears the rules list, before the new entry is created.


## Events and listeners ##

Lunar uses a simple publisher-subscriber-model for all output. Listeners wait for events to happen and then print a fitting message. You can use this for implementing your own output.

You can induce events anytime using `myevent:induce()`. After that all listeners for the event will be executed using `somelistener:action( myevent )`.

Let's illustrate this with an example:

```

myevent = lunar.events:new( "outofmem" )
mylistener = lunar.listeners:new( "outofmem" )
mylistener.events:add( myevent )
mylistener.action = function( listener, event )
  print( event.timestamp )
  print( "Out of memory" )
  -- do some logging?
end

mytarget = lunar.targets:new( "test" )
mytarget.input_files:add( "pretty_big_file" )
mytarget.output_files:add( "even_bigger_file" )
mytarget.action = function( target )
  -- iffiletobig
  lunar.events:find( "outofmem" ):induce()
end

```

This is about it.