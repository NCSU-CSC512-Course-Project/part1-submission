
int test_fun(int x) {
  return x;
}

int main(void) {
  if (1) {
    return (*test_fun)(10);
  }
  return (*other)(0);
}
