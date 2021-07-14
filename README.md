# protobuf-demo

Examples to use [protobuf](https://github.com/protocolbuffers/protobuf).

## Install dependencies

It's recommended to use [vcpkg](https://github.com/microsoft/vcpkg) to install dependencies.

Take MacOS 64-bit as example, you need to run:

```bash
vcpkg install --feature-flags=manifests --triplet x64-osx
```

Choose the correct triplet for your OS, see [here](https://github.com/microsoft/vcpkg/tree/master/triplets) for all available triplets.

## Build examples

If you have installed dependencies using vcpkg, you can simply run:

```bash
cmake -B ./build -DVCPKG_TRIPLET=x64-osx
cmake --build ./build
```

Then all executables will be generated under `build/` directory.

On MacOS, you can also run the wrapped scripts:

```bash
./scripts/configure-osx.sh
./scripts/build-osx.sh
```

> **NOTE**
>
> You must ensure the `VCPKG_TRIPLET` is the correct triplet for your OS. If you don't use vcpkg to install dependencies, you will need to specify `-DCMAKE_PREFIX_PATH` or ensure your dependencies are installed in system path.

## Code format

`clang-format` is required. Run:

```bash
cmake --build ./build --target format
```

You can also run the wrapped script:

```bash
./scripts/format.sh
```
