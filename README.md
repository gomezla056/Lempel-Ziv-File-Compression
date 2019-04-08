# Lempel-Ziv-File-Compression

This is a parameterized version of the Lempel-Ziv file compression algorithm created on a Linux machine. The algorithm compresses the files along with optional arguments in case the user wants to experiment with saving more or less memory. Afterwards, the user is presented with how much data was compressed and how much time it took to complete the program. The algorithm also decompresses the compressed files back to its original state. The output of decompressing shows the user how much time it took to recover the compressed file back to its original state.

## How to use

Run `make all` on your terminal. You should be presented with the executables `LZ` and `EXPAND`. 

### LZ

`LZ` takes in a file along with optional arguments `N`, `L`, and `S` to compress and saves it.

`N` is the number of bits to encode the window offset. The range is from 9 to 14.

`L` is the number of bits to encode that matches length L. The range is from 3 to 4.

`S` is the number of bits to encode the length of a literal string. The range is from 1 to 5.

By default, `N` is set to 11, `L` is set to 4, and `S` is set to 3. However, if you would like to use the optional arguments, you can add them by the following example on a Linux terminal.

```
./LZ -N=10 -L=3 -S=2 myfile.txt > myfile.comp
```

The arguments can be set in any order. However, if the file you wish to compress is not outputted to another file name of your choice, the program will not execute properly.

### EXPAND

`EXPAND`, on the other hand, decompresses the file you originally compressed. The only necessary argument you will need is the compressed file's name. For example,

```
./EXPAND myfile.comp > myfile.rec
```

However, you may remove the redirection and just have the compressed file. This will output all the content on your terminal.

### Example

```
$ LZ -N=14 -L=3 -S=2 book1 > book1.comp
N: 14
L: 3
S: 2
output =  367000
total =  768771
compression savings = 0.522615
Compression processing time = 1.03389
$ EXPAND book1.comp > book1.rec
Expand processing time = 0.021823

```
