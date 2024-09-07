cd lab0/
mkdir hello

cd hello
touch empty

cp ../src/hello.c ./
mv hello.c newhello.c


# Task 4
gcc newhello.c -o hello_world
./hello_world
