We describe how to compile and run these benchmarks.

## How to Run
For each benchmarks, three versions of executable are roduced:
* 32-bit ARM binary without instrumentation
* 32-bit ARM binary with EmbedSanitizer instrumentation for race detection
* 64-bit binary for x86_64 machines, instrumented by ThreadSanitizer

To run these benchmark versions and get results use the bash script `run.sh`:

```bash
>$ ./run.sh # reports the slowdowns and where races were detected.
```
The bash script writes the results in `BenchmarkReports.txt` file
