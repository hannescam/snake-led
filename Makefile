local:
  g++ main.cpp -o snake.o -Wall -O3 -g -Wextra -Wmemset-elt-size -Wno-unused-parameter -I ../rpi-rgb-led-matrix/include/ -c
  g++ snake.o -o snake -L ../rpi-rgb-led-matrix/lib/ -lCppLinuxSerial -lrgbmatrix -lrt -lm -lpthread
clean:
  rm snake.o
  rm snake
install:
  cp snake /bin/ 
