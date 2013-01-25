int no_follow()
{
    mutex_lock(&nofollow);
    if(FOLLOW_TO_END && step >= schedule_length()) {
        if(!spoken) {
            fprintf(stderr, "Program has run to the end of the schedule and is resuming normal execution ...\n");
            spoken++;
        }
        mutex_unlock(&nofollow);
        return 1;
    }
    mutex_unlock(&nofollow);
    return 0;
}
