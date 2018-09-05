## Romdisk Naming Test

I've realized that there's some issue with filename that are longer than 15 char in the romdisk under KallistiOS.
There's also an issue with similar names?

This is a serie of test that will print those result using printf.

### How to use

Use `make` to build the program.

Edit `makeCD.sh` and make sure you edit the ip at line 24 (the `arp` command)
Use `./makeCD.sh` to make a .cdi and .iso. This will lauch the dcip-tool automaticly.

You should get result in your terminal.
This program doesn't show anything on screen and will exit immediatly.

### Printout format

`Opening /rd_2/test.png : result -1`
`Opening /rd_2/testtest.png : result 3`

The result is the file handle, -1 means the file isn't found.

You can get my result at `result.txt`