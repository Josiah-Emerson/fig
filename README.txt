TODO: Implementing GLShaderProgram. Make it so that it can take a GLShader shared pointer as an argument (probalby a virtual func) to attach. 
Add to destructor of GLShader to delete it to free memory. In GLShaderProgram, detach GLShader in destructor from each shared pointers. Do linking stuff

TODO: Projection matrix is ever so slightly off of what glm::perspective returns. Specifically in the first element and 6th i.e. second row second column
TODO: Think about some cool ways to overload vector/matrix funcs to allow for some more useful and different stuff
TODO: Look into how to handle (for just in vec/mat stuff but in the future more heavy calculations) things like loss of precision, when should a small number just become 0, etc..
TODO: Work on openGL renderer
TODO: Implement a Fig enum for datatypes and have imgui handle translating so that layers don't need to know about ImGuiDataType
TODO: Implement Tree and refactor stuff to use that
TODO: Implement actual structures for Fig::Events (and maybe begin actually handling them in app), 
TODO: ImGuiWidgets folder (i.e. reusable widgets like color picker)

TODO: Vision for future is to have array simple structs represent particles and such, and hold the basic info such as position and mass (i.e. data oriented approach)
for doing the physics on them, and then when we render have perhaps a pointer to a render object 

   /* 
   std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10);
   dvec3 vec1 {3929.31038, 9804.72841, 5063.86373};
   dvec3 vec2 {6935.28254, 7699.60976, 3030.65272};
   dvec3 vec3 {6051.64732, 3844.55456, 6890.15868};
   dvec3 vec4 {1741.83093, 7555.56280, 3415.21740};

   dvec3 vec5 {9059.37915, 2793.78649, 3076.63488};
   dvec3 vec6 {3307.81531, 1392.47996, 6361.62214};
   dvec3 vec7 {5227.21071, 1810.54073, 7185.46577};
   // 6488.09236, 1666.50859,  588.44051

   Matrix<double, 4, 3> mat1 { vec1, vec2, vec3, vec4 };
   Matrix<double, 3, 3> mat2 { vec5, vec6, vec7 };
   std::cout << "mat1: " << mat1;
   std::cout << "mat2: " << mat2;

   auto res = mat1 * mat2;
   std::cout << "mat1 * mat2 = " << res;
   */
   // THIS EXAMPLE AS A FLOAT LOSES PRECISION (Notice they are currently double)


Stuff for EulerMaruyama solver ( might be changed up later)
I believe the solver works correctly. See the MSD (Mean Squared Distance) check below. Might be good idea to look into other
ways to check the validity of the solver. Now just need to create some plots of the particle, and check dimensions past 1. Also,
remove the added bits from the solver which were used for investigating things. Also, look into refactoring solver since we no longer subtract to find the distance and allat.

The expected MSD is: 4.4e-05
The average MSD after 10000 iterations for 1 time steps is: 4.36658e-05
The average MSD after 10000 iterations for 5 time steps is: 4.31166e-05
The average MSD after 10000 iterations for 10 time steps is: 4.3788e-05
The average MSD after 10000 iterations for 50 time steps is: 4.40551e-05
The average MSD after 10000 iterations for 100 time steps is: 4.45078e-05
The average MSD after 10000 iterations for 500 time steps is: 4.43106e-05
The average MSD after 10000 iterations for 1000 time steps is: 4.373e-05
The average MSD after 10000 iterations for 5000 time steps is: 4.3849e-05
The average MSD after 10000 iterations for 10000 time steps is: 4.39099e-05
The average MSD after 10000 iterations for 50000 time steps is: 4.46298e-05
