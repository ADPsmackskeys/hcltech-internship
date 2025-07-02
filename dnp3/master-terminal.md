```bash
# Clone the opendnp3 repo
$ git clone https://github.com/dnp3/opendnp3
$ cd opendnp3/
$ cd cpp/examples/master-gprs # Change `main.cpp` as provided in this folder

# Create the build folder for CMake
$ cd ../../../
$ mkdir -p build
$ cd build/
$ cmake .. -DDNP3_EXAMPLES=ON -DDNP3_TLS=ON
$ make -j $(nproc)
$ cd cpp/examples/master-gprs # Add `config.json` as provided in this folder

# Run the file
$ ./master-gprs-demo
```
