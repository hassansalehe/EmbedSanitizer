# EmbedSanitizer: A Runtime Race Detection Tool for 32-bit Embedded ARM


## How to Build and Install
###  (a) Prerequisites
This tool has been tested on x86_64 machine with Ubuntu 16.04.  Moreover, make sure that the following are installed in your platform.
* LLVM/Clang, more information on how to install: http://llvm.org/docs/GettingStarted.html
* The cross-compilers utilities: 
```bash
apt-get install -y gcc-multilib g++-multilib 
apt-get install -y libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi
apt-get install -y libncurses5-dev gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
```

### (b) Installation
To build and install, there is Bash script `install.sh` at the main directory of the project. 
```bash
>$  ./install.sh
```
## How to Run
### (a) Compiling your program
The `install.sh` script builds LLVM/Clang with EmbedSanitizer in it and installs it  in `arm` directory. Then you can compile your program from the main directory of the project using the command format below:
```bash
>$  ./arm/bin/clang++ -o <executable_name> <your_program_name.cpp> -fsanitize=thread 
```
The compiler produces an ARM-compatible exexutable.

### (b) Running your program
Once your program is instrumented and compiled by LLVM/Clang, you can run it in a 32-bit ARM platform. Alternatively, you can lauch your program with the Qemu emulator:
```bash
>$ qemu-arm <executable_name>
```

## Experimental Results from the Benchmarks
please refer to `tests/parsec_benchmarks/README.md` for more information on how to run the benchmarks and get results.

## License
Our license derives from that of LLVM/Clang project as we use its source codes. For more information, please read the file `LICENSE.md`.  
Moreover, the benchmarks that we used for evaluation in `tests/parsec_benchmarks` have their own license from the PARSEC Benchmark suite.

## Contact
For any equiries, suggestions or collaborations please contact `hassansalehe<at>gmail.com`
