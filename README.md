# Burrows-Wheeler transform and run-length encoding

Essentially lossless compression, primarily for text files.

## Compilation/installation

### Windows

I don't know. I'm on Windows but I use WSL. But you should be able to figure it out, it can't be that complicated.

Hopefully.


### Linux/MacOS

If you have `make` and `git` installed, you should be able to just `git clone`, then navigate to the repo folder and
execute `make` without arguments. And it should compile without issues. Then you can run it by writing `./bwt --help`.

For Linux users, if you want to execute it without the leading `./`, you can add it to `$HOME/.local/bin` or
`$HOME/bin`. For MacOS users, I have no idea but you should be able to figure it out too.