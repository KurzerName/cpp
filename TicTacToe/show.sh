#!/bin/bash
clear
# Скрипт для компиляции проекта с sfml
#
#g++ workshop.cpp -o startCpp
#./startCpp

g++ ticTacToe.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

if [[ "$1" == "test" ]]; then
  gdb ./sfml-app
else
   ./sfml-app
fi





#echo "Результат чистки:"
#echo ""
#cat test1.txt
