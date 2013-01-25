/*monitor thread to check satisfactory progress*/
void* monitor_progress() {
  int last_step = step;
  while(true) {
    sleep(2);
    if(last_step == step) {
      /*if there has been no change after execution, assume satisfactory progress has
	not been made and report a deadlock*/
      fprintf(stderr, "Potential deadlock detected!\nTool will now exit with error code: %d\n\n",
	      5);
      print_followed();
      exit(5);
    } else {
      last_step = step;
    }
  }
}
