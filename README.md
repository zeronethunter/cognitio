# Cognitio

---

Before starting writing the code: 
1. Check out the whole repository. Make sure you understand what is going on here:
   - All files are supposed to be built inside the *build* directory
   - *cmake* directory contains all files which are included in the main CMakeLists:
     - You can configure options of the project building right inside main CMakelists
     - If you want to disable some static sanitizers while building, check out *cmake/CheckToolsOptions*
     - If you think, that some of the features of the static analyzers has to be disabled, don't hesitate and turn the off in the *cmake/FormatStaticAnalysys.cmake*. But it's better to use NOLINT* directive (but don't use it too much)
   - All git submodules must be added to *third-party* directory (and not in the root one!)
 
2. Check out the structure of the project's directories inside *src/* and start writing code)
   - files in the *src/hello* are just the example of configuring Cmake in our project. This directory must be deleted
   

Writing code:
1. Our style is [GoogleStyle](https://google.github.io/styleguide/cppguide.html)
1. Write docs in comments ([Doxygen](https://www.doxygen.nl/manual/docblocks.html))

TODO (for myself):
   - Makefile & CI configuration
   - Build directories
   - Doxygen
