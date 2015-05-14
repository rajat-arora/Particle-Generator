/*
 * rajat arora-1205519
 * This assignment was runned on eclipse for gnome
 * For some weird reason I am missing some files and get /usr/bin/ld: /tmp/ccgXpq30.o: undefined reference to symbol '_ZSt17__throw_bad_allocv@@GLIBCXX_3.4'
* and //usr/lib/x86_64-linux-gnu/libstdc++.so.6: error adding symbols: DSO missing from command line
* when I compile using make.  I think it's because I am using a beta version of gonome
*It works fine in eclipse though
*If you wanna try eclipse. Open eclipse on ubuntu or gonme, new c++ project, click on project properties, then click on c++/build
*then settings and cross g++ linker then libraries
* from there then add GLU,glut,GL
*and click ok
*and then copy my source code and run the file :)
 * Things implemented
 * 1.Particle Lifespan -> random life's
 * 2.Particle Spin
 * 3.Particle Floor collision
 * 4.Friction mode: "f" key
 * 5.Scene rotation: arrow keys
 * 6.Start/stop sim: space bar
 * 7. Reset: "r" key
 * 8. Backface culling,double buffer, perspective projection
 * 9. Lighting
 * 10.Particle cam- "c" key
 * 17. Wind - "w" key
 */
