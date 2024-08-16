# markov-video

A tool to create Markov Chain videos.

## Features

- Efficient Markov Chain library.
- Creation of Markov Graphs using LaTeX.
- Creation of videos using Markov Graphs.
- Creation of GIFs using Markov Graphs.

## Usage

```sh
markov-video -m markov_chain.txt -i 10 -V videos_folder -o output.mp4`
```

Where markov_chain.txt is a file that contains the transition matrix probabilities. An example file would be:

```
0, 1, 0
0.3, 0.5, 0.2
0.1, 0.9, 0
```

Please note that standard Markov Chain rules apply. That is, it must be a square matrix and the probabilities in each row must sum to 1. 10 is the number of iterations that the Markov Chain will go through. (For now this options is mandatory, but it will be made optional). This value is in the range of `std::size_t`.

The videos_folder is the folder that contains the video segments. The folder must contain the same number of video segments as the size of the transition matrix. For example, for the transition matrix provided above, you would name the videos as:

```
0.mp4
1.mp4
2.mp4
```

For now only mp4 files are supported. Lastly output.mp4 is simply the name of the output file, and it can be any filetype that supports video channels.

To view all options just run `markov-video` or `markov-video --help`.

## Requirements:

Requirements to use the software:

- `ffmpeg` installed and available on path to overlay Markov Chains on the video clips and to combine the clips.
- `pdflatex` or any other latex compiler to compile the Markov Chains (and the necessary packages used in the .tex files).
- `magick` from ImageMagick to convert the PDFs into PNGs. (This will probably be moved to use the poppler library).

After satisfying these Requirements, simply download the binary files provided, make them executable and run them. Please note that these binaries are compiled for the x86-64 architecture, and Windows and non-Windows binaries are separate since silencing command line output in Windows differs compared to \*nix.

Requirements to compile the software:

- A C++17 compiler to compile the source code. (The code has been tested on clang++ only.)
- `make`. (You can compile by hand if you wish.)

## Build

```sh
git clone https://github.com/Amir1453/markov-video.git --depth 1
cd markov-video
make
```

This will use the Makefile provided to build the software. If you use `g++`, just edit the compiler option in the Makefile. The created binary will be at `./target/debug/markov-video`. To compile with release flags, use `make release`.

Since excessive optimization causes the binaries to be unportable the Makefile does not optimize for CPU type and architecture fully, if you wish to increase the optimization level, you can edit the Makefile to use

```sh
-march=native -mtune=native
```

Afterwards run `make clean` and `make release`. The binary will be at `.target/release/markov-video`.

## Goals

I do not really see this project growing that much in the video making direction, so I will either focus on making the Markov Graphs better (the code that generates them now is pretty bad) or just create a very good Markov Graph library that I might use later on. The current one currently misses some features that would increase performance.

The problem with the graph generation thing is that it is really difficult to make something that works for all kinds of Markov Graphs out there, and it is not really easy to implement. On the other hand the Markov Chain library is much easier to dive into (also seems much more useful) but there has already been at least 2 very good implementations, so I am not sure if I want to reinvent the wheel.

Of course, this project still has tons of things to improve on, and they will be implemented when I have the time.

## Thanks

Thanks to p-ranav for the `argparse` C++17 library which was included in `include/argparse.hpp`.
Thanks to Kat's Sandbox for the idea.
