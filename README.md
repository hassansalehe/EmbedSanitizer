## EmbedSanitizer: A Runtime Race Detection Tool for 32-bit Embedded ARM
This tool extends ThreadSanitizer to do race detection for 32-bit ARM applications. 
Due to the complexity of the ThreadSanitizer's race detection runtime, EmbedSanitizer
implements its own by applying FastTrack. FastTrack (see: [link](https://users.soe.ucsc.edu/~cormac/papers/pldi09.pdf)) 
is an efficient and precise race detection algorithm which relies on *happens-before* concepts.

### Usage Summary
EmbedSanitizer can be installed as part of LLVM/Clang infrastructure in a development machine; 
presumably an x86_64 platform. Then it can be launched in similar manner to ThreadSanitizer; 
using a compiler flag _-fsanitize=thread_. Once the compiler produces your program's final 
executable with EmbedSanitizer instrumentation, you can run on a target ARM 32-bit platform. 

```bash
# Use the installed EmbedSanitizer in your development machine as part of Clang compiler
# to build your 32-bit ARM applications.
>$ ./clang++ my_program.cpp -o my_program.exe -fsanitize=thread -static <other_compiler_flags>

>$ file my_program.exe # this shows the type of the produced binary.
my_program.exe: ELF 32-bit LSB executable, ARM, EABI5 version 1 (GNU/Linux), statically linked, for GNU/Linux 3.2.0
```
Then take your executable to a target platform and run it there.

```bash
>$  ./my_program.exe # running on a target hardware
```
Then races will be reported, if any, when the program runs. Alternatively, you can use `QEMU` emulator in your development machine
to run your program but it may be very slow.
```bash
>$ qemu-arm ./my _program.exe
```

### Building and Installation
####  (a) Prerequisites
This tool has been tested on x86_64 machine with Ubuntu 16.04.  Moreover, make sure that the following are installed in your platform.
* LLVM/Clang, more information on how to install: http://llvm.org/docs/GettingStarted.html
* The cross-compilers utilities: 
```bash
apt-get install -y gcc-multilib g++-multilib 
apt-get install -y libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi
apt-get install -y libncurses5-dev gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
```

#### (b) Installation
To build and install, there is Bash script `install.sh` at the main directory of the project. 
```bash
>$  ./install.sh
```
### Running
#### (a) Compiling your program
The `install.sh` script builds LLVM/Clang with EmbedSanitizer in it and installs it  in `arm` directory. Then you can compile your program from the main directory of the project using the command format below:
```bash
>$  ./arm/bin/clang++ -o <executable_name> <your_program_name.cpp> -fsanitize=thread 
```
The compiler produces an ARM-compatible executable.

#### (b) Running your program
Once your program is instrumented and compiled by LLVM/Clang, you can run it in a 32-bit ARM platform. Alternatively, you can launch your program with the Qemu emulator:
```bash
>$ qemu-arm <executable_name>
```

### Experimental Results from the Benchmarks
please refer to `tests/parsec_benchmarks/README.md` for more information on how to run the benchmarks and get results.

### License
Our license derives from that of LLVM/Clang project as we use its source codes. For more information, please read the file `LICENSE.md`.  
Moreover, the benchmarks that we used for evaluation in `tests/parsec_benchmarks` have their own license from the PARSEC Benchmark suite.

### Contact
For any inquiries, suggestions or collaborations please contact `hassansalehe<at>gmail.com`
