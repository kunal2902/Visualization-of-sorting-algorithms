all:
    g++ -I src/include -L src/lib -o main Sorting Visualizer.cpp -lmingw32 -lSDL2main -lSDL2