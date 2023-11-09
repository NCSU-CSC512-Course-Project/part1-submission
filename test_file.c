#include <stdio.h>
#define LOG(BP) printf("%s\n", BP);

int add(int a, int b) {
  return a + b; 
}

int main(void) {
  int (*add_ptr)(int, int) = &add;
  
  int result = (*add_ptr)(2, 2);

  if (result == 4) {
    LOG("br_1"); 
    for (int acc = 1; result <= 10;) {
      LOG("br_3");
      result += acc;
    }
  }
  LOG("br_2")
  return result;
}


