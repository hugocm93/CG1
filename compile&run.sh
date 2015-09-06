#/bin/bash

rm CG1
cd source
gcc *.c -liup -liupgl -lm -lGL -lGLU 
mv a.out ../CG1
cd ..

./CG1


