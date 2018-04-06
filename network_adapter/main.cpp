#include <iostream>
#include "board.h"

int main(int argc, char** argv) {
   std::cout << "dxnet network adapter init." << std::endl;

   dxnet::Board board("eth0", "00::00::00::00::00::00");

   std::cout << board;

   return 0;
}
