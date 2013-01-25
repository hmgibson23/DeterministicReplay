int GLOB = 0;
void* set(int a){
  lock_mutex(mutex);
  GLOB = a;
}

int main() {
  Thread A, B;
  create_thread(A, set, 1);
  create_thread(B, set, 2);

  thread_join(A);
  thread_join(B);

  return 0;
}
