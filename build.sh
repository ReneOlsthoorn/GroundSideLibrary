# run it in a MSYS2 MINGW64 shell (using ./ build.sh)
gcc -O2 -DNDEBUG -s -shared -o GroundSideLibrary.dll dllmain.c sidelib.c lodepng.c \
-Wl,--out-implib,libGroundSideLibrary.a -Wl,--strip-all -Wl,--as-needed
