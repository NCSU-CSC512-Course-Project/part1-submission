int add(int a, int b) { return a + b; }

int sub(int a, int b) { 
  return a - b; }

int main(void) {
  int (*add_ptr)(int, int) = &add;
  int (*sub_ptr)(int, int) = &sub;

  int result = (*add_ptr)(2, 2);
  result = (*sub_ptr)(result, result);
  result = 4;

  if (result == 4) {
    for (int acc = 1; result <= 100;) {
      result += add(result, result);
    }
    result = sub(result, 1);
  }
  return result;
}
