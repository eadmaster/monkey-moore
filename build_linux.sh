 
# https://github.com/rjricken/monkey-moore/issues/1
g++ src/*.cpp -o build/mmoore  `wx-config --cflags --libs` -std=c++11 -Wfatal-errors -Wwrite-strings -fpermissive
