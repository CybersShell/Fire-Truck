
# Fire-Truck

This repository will be used for any software associated with the mini firetrucks received from the Huntsville Fire Department through SMAP at UAH.

## PlatformIO

This project requires [PlatformIO](https://platformio.org/).

See [install documentation here](https://docs.platformio.org/en/latest/core/installation/index.html).

## Testing components

Run and upload tests using PlatformIO's VSCode extension. Or upload them using the command `pio run -t upload -e Test[Name]`. Name is the name of the test to run.

In general, when adding tests make sure to add Test to the beginning of the environment name.
Make sure to only include the files for your test. See below for guidelines.

If you add a test, add an env section to `platformio.ini` with a syntax like the following:

```ini

... rest of ini file ...

[env:Test[NameOfTest]]
upload_port = [Your upload port]
platform = atmelavr
framework = arduino
board = uno
lib_deps = 
    [any libraries you need]
# filter everything else except the files needed for the test
build_src_filter = 
    -<*> 
    -<.git/> 
    -<.svn/> 
    +<componentTests/[NameOfTest].cpp>
```

## Setup of circuit

Go over the guide in `docs/manual.pdf` to get the parts for this project and learn how to set them up.

To get setup, go to the PlatformIO tab, run the platformIO environment `FireTruckMaster` and `FireTruckSlave`.
