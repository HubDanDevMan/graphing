# Graphing

A 2D/3D Grapher of mathematical functions (AS OF NOW ONLY COMPLEX FUNCTIONS)

## About the Project

This Project serves no particular purpose and is licensed under the
[AGPL-3.0](https://www.gnu.org/licenses/agpl-3.0.en.html).

## Technical details

This program uses C with the `SDL2` Library. Documentation can be found on [the wiki](https:wiki.libsdl.org)
and tutorials can be found [here](https://wiki.libsdl.org/Tutorials). This project uses modern C with no
external dependencies with the excepttion of the SDL2 library. The ***<math.h>*** and ***<complex.h>*** libraries
that are included in the C standard library handle all the math required. For development use the development library that can be downloaded [here](https://github.com/libsdl-org/SDL/releases/latest).

## Setup

`GCC` is recommended for compiling and linking. The build system used is `GNU MAKE`. To compile the project
invoke `make` in the project directory and to compile and run type `make run`. The required dependencies are
**libSDL2** and probably in the future **libSDLttf**. Make sure you add the header files to your include path
and **.dll**s/**.so**s in your linker path. 

## Project Architecture

`main` initializes the permanent SDL components such as the window and renderer. It calls `graphingMain`
which contains the programm loop (or *game loop*). The **view** is a struct that contains all information
related to the current frame, that is ***position***, ***zoom***, ***spread*** and some others which will
all be explained later. The view is global because the **Complex Functions**, the **Controls** and some
other files need to access/modify the individual view variables. All keyboard shortcuts and their
functionality are declared in _controls.c_.  


## Complex Functions

This program draws so-called complex functions. They take a complex number ***Z*** as an argument and
return another complex number. ***Z*** has its real and imaginary values, of which the x coordinate value
is the real part of ***Z*** and the y coordinate value is the imaginary one. The output of the complex
function is then colored using a **Coloring Function** and placed in the pixelbuffer. In essence,
***Z = x + yi*** and is passed to a function ***f(z)***, whose output is placed into a coloring function  
***c(f(z))*** that colors the pixel at ***(x,y)*** with its output. This is done for every pixel, until
the frame is rendered. Most complex functions are not very interesting, but the really interesting ones
share a property: they are _recursive_ or _iterative_. There are three common outcomes of an iterated
complex function:
-The value remains bounded and converges to a finite value 
-The value approaches infinity (unbounded)
-The value enters a cycle, for example a -> b -> c -> a -> b -> c and so forth
This depends a lot on the iterations count. Some values remain bounded until the iteration count is greater
than e.g. 10 but more than that makes the value reach _chaotic_ behaviour. This can be very interesting to
look at, because this chaotic behaviour creates some unexpected, visually pleasing patterns.

In this project, most complex functions are written with a for loop that returns the value after ***N***
iterations. In the `function.c` file you will find how most of them are similarly structured.


## The View

The view is a structure containing variables/parameters that define what is seen in the frame.
The definition of the view can be found in `view.h` and the defaul values can be seen in `view.c`.
Most view params are self-explanatory with a few exceptions. The **spread** is variable used in coloring
functions that defines how values close to zero should be colored, especially the brightness. In
[this example](https://www.desmos.com/calculator/5ddnht3kkz) the spread ***s*** can be adjusted with the
slider. The higher the spread, the darker the values around zero will be. Decreasing the spread can be useful
when the current frames colored points are very faint or dark, i.e. close to zero.

The shift is like an offset in x in the coloring function. That means that not values close to _zero_ will
be dark, but rather values close to the _shift_. Both the spread, the shift and all other view parameters
can be adjusted using keybindings. This is very handy when changing the _extended_ _view_ _variables_ `var1`
and `var2`. These can be accessed inside complex functions and change a constant without having to recompile
the code. 

## Keybindings

The keys listed below modify the view according to their description and they can be prepended by a number that performs the action repeatedly unless explicitly stated. The keybindings are heavily inspired by vi/Vim:
-**z**: zoom into center of view
-**o**: zoom out of view
-**h**: move view left
-**j**: move view down
-**k**: move view up
-**l**: move view right
-**r**: decrease iteration count (faster, less detail)
-**t**: increase iteration count (slower, more detail)
-**m**: resets all the view parameters to the default values (i.e. return to starting view), number prefix is ignored
-**v**: toggle real|imaginary view, number prefix is ignored
-**c**: change coloring function
-**x**: toggle axis drawing [NOT WORKING :(], number prefix is ignored
-**w**: increase the spread (increase brightness)
-**s**: decrease the spread (decrease brightness)
-**f**: increase `variable1` by 1
-**g**: decrease `variable1` by 1
-**y**: increase `variable2` by 1
-**n**: decrease `variable2` by 1

Example: `123l` moves the view to the ***left*** ***123*** times

The following keys are special and will NOT force a redraw of the frame unless explicitly mentioned:
-**e**: Print view parameters to the console, useful to know where the view or the current spread
-**b**: Save the frame as a bitmap [NOT WORKING :(]
-**u**: undoes the latest modification [NOT WORKING :(]. Should force a redraw that is identical to the previous frame.


