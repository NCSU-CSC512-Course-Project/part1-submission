int add(int a, int b) {
  if (a > 0 && b > 0) {
    return a + b;
  }
  return 0;
}

int main(void) {
  int (*add_ptr)(int, int) = &add;

  int result = add(3, 2);

  if (result > 4) {
    if (result == 5) {

      for (int acc = 1; result <= 10;) {
        result += add(acc, acc);
      }
    }
  }
  return result;
}
