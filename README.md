# Partou
A Uni-directional Ray-Tracer with Path Tracing and Monte-Carlo method.


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

## Renders

![Cornell's box with causatics](.gallery/Cornell_with_causatics.png "Cornell's box with causatics")

<details>
<summary>
 <b>Standard Cornell's box</b>
</summary>
 
![Cornell's box using MCMC and mixture pdf](.gallery/Cornells_box_using_MCMC_and_mixture_pdf.png "Cornell's box using MCMC and mixture pdf")
</details>

<details>
<summary>
 <b>Different Materials</b>
</summary>
 
![Suzzane with Glass material](.gallery/glass_with_total_internal_suz.png "Suzzane with Glass material")
![Suzzane with Lambertian material](.gallery/lamb_suz.png "Suzzane with Lambertian material")
![Suzzane with Metal material](.gallery/metal_smooth_fuz_suz.png "Suzzane with Metal material")
</details>

<details>
<summary>
 <b>Miscellaneous</b>
</summary>
 
![Suzzane next to a glass sphere](.gallery/suz_with_sphere.png "Suzzane next to a glass sphere")
![Flat Stanford Bunny](.gallery/bun.png "Flat Stanford Bunny")
![Flat Suzzane](.gallery/suzanne.png "Flat Suzzane")
![Smooth Suzzane](.gallery/suzzane_trans_smooth_msaa.png "Smooth Suzzane")
![Sphere and Triangle Rendering](.gallery/sphere_triange_rendering.png)
</details>

