We describe how to compile and run these benchmarks.

## How to Run
For each benchmarks, three versions of executable are produced:
* 32-bit ARM binary without instrumentation
* 32-bit ARM binary with EmbedSanitizer instrumentation for race detection
* 64-bit binary for x86_64 machines, instrumented by ThreadSanitizer

To run these benchmark versions and get results use the bash script `run.sh`:

```bash
>$ ./run.sh # reports the slowdowns and where races were detected.
```
The bash script writes the results in `BenchmarkReports.txt` file. Sample output from one
of the benchmarks is shown below.
```c++
blackscholes:
 - ThreadSanitizer races?: no
 - EmbedSanitizer in ARMv7 races?: no
 - ARMv7 slowdown:       ~12.0665224445646295295
 - Unique Addresses: 28686
 - Total Reads: 5324786
 - Total Writes: 409602
 - Unique Locks: 0
 - # of Threads: 3
```

If you want to run the benchmarks on target 32-bit ARM platform, please copy the whole of
`parsec_benchmarks` folder to  the target and run the `run.sh` script.
