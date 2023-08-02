# SDSF

SDSF is a C++ oriented framework designed to make making 2D DS games as easy as possible. No more using pure C, now you can use a C++-based interface to make DS games!

In its current state, it should be possible to make simple games (which is enough for most people), but making more complex games will require you use the libnds API more, which isn't optimal. Eventually, this framework should be able to make more complex games, but as of right now it can't.

Also, this can only make 2D games, please use [Nitro Engine](https://github.com/AntonioND/nitro-engine) if you want 3D.

# Getting started

Check out `templates/` to see how to get started on your own project, and check out `examples/` to see SDSF in action.

# Documentation

Make sure to have [Doxygen](https://www.doxygen.nl/download.html) downloaded, then run `doxygen` in the root directory of SDSF.

Eventually, I would like to make a wiki for this project with tutorials on how to do stuff, but as of right now this project still needs a little more work before the wiki can be given the attention it needs. 

# Opening issues

If you have an issue, just open an issue explaining your issue. If you can, please include a short example which reproduces your issue, thanks!

# Contributing

If you want to contribute, thank you! Just open a pull request explaining what you added, and we'll see from there.

# TODO
1. Clean up code (and refactor the ResourceCreator portion)
2. Make sure that `BlocksDS` works, and port templates and examples to `BlocksDS`.
3. Make better templates.