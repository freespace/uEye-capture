uEye-capture
============
`uEye-capture` is a small c program for capturing still images from the [uEye usb cameras](http://en.ids-imaging.com/store/produkte/kameras/usb-2-0-kameras/ueye-se.html). These are rebranded and [sold by Thorlabs](http://www.thorlabs.de/newgrouppage9.cfm?objectgroup_id=2916), and it is one of these cameras that I have and use this program with.

Usage
=====
`capture`
---------
The main program, `capture` operates in two modes depending on whether a commandline argument is given.

* When invoked without arguments, the `capture` will take a still frame snapshot and save it to `snapshot.bmp`.
* When invoked with a port number as the only commandline argument, the `capture` programm will listen on the specified port for [`zmq`](http://zeromq.org/) connections.
 * When a string is received, `capture` will take a still frame snapshot, and attempt to save it to the file specified by the string. i.e. the string needs to be a valid system path.
 * Upon successfully saving the snapshot, the program responds with `ok`.
 * **Important**: `zmq` is not transport neutral, and you cannot simply use something like `nc` to talk to the program in network mode. The supplied `client` program should be used intead.

When in network mode `capture` will exit if it receives `@exit`.
 
`bench`
-------
The `bench` program can be used to get a rought idea of how many frames per second can be saved. The program will time how long it takes do perform 100 snapshots and writing the results to `/dev/null`.
 
License
=======
The MIT License (MIT)

Copyright (c) 2014 Shuning Bian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
