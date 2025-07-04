```bash
# Clone the opendnp3 repo
$ git clone https://github.com/dnp3/opendnp3
$ cd opendnp3/

# Create the build folder for CMake
$ mkdir -p build
$ cd build/
$ cmake .. -DDNP3_EXAMPLES=ON -DDNP3_TLS=ON
$ make -j $(nproc) 
$ cd cpp/examples/outstation

# Run the file
$ ./outstation-demo
```
