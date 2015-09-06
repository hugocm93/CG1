#/bin/bash

cd source
rm CG1.out
gcc *.c -liup -liupgl -lm -lGL -lGLU 
mv a.out ../CG1
cd ..

./CG1


