[![Build](https://github.com/aignacio/nox_freertos/actions/workflows/build.yaml/badge.svg)](https://github.com/aignacio/nox_freertos/actions/workflows/build.yaml)

# NoX - FreeRTOS demo SW

Run with a pre-built container with all the tools, including toolchain and python:
```bash
docker run -it --rm --name mpsoc_sw -v $(pwd):/test -w /test aignacio/mpsoc bash
```

Once the container is up and running, proceed executing all the steps within the container terminal. In order to run the makefile, first generate with cmake through these cmds:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug|Release 
```

Finally build the app with this `make all`: 
```bash
make all -j24
```

## Programming the hardware with the elf file

If you have the NoX SoC HW flashed in a FPGA device and the tile is running the bootloader app, you can easily transfer
the binary with the bootloader script below. Please note that the [bootloader script](scripts/bootloader_elf.py) is
considering the **memory map** address below and the uart speed equal to 8-N-1 / 115200 bits/s.
```bash
python3 scripts/bootloader_elf.py --elf build/nox_freertos.elf --device /dev/ttyUSB1
```
