int add(int a, int b) {
  if (a > 0 && b > 0) {
    return a + b;
  }
  return 0;
}

int main(void) {
  int (*add_ptr)(int, int) = &add;

  int result = (*add_ptr)(2, 2) + 1;

  if (result > 4) {
    if (result == 5) {

      for (int acc = 1; result <= 10;) {
        result += acc;
      }
    }
  }
  return result;
}
