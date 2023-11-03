int add(int a, int b) {
  return a + b; 
}


int main(void) {
  int (*add_ptr)(int, int) = &add;
  
  int result = (*add_ptr)(2, 2);

  if (result == 4) {
    for (int acc = 1; result <= 10;) {
      result += acc;
    }
  }

  return result;
}
