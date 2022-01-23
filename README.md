- Build project
    ``` sh
    $ mkdir build
    $ cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
    $ cmake --build build -t all
    ```
- Run project
    ``` sh
    $ cd build
    $ ./partou output.ppm
    $ open output.ppm  # or use a ppm viewer
    ```
