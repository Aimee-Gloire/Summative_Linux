# Question 2 — Assembly Sensor Line Counter

Files:

- sensor_count.asm  (NASM source)
- sensor_count      (executable produced locally)
- sensor_log.txt    (input)

Build / Run:

1. nasm -f elf64 sensor_count.asm -o sensor_count.o
2. ld sensor_count.o -o sensor_count
3. echo -e "23\n45\n\n67\n" > sensor_log.txt
4. ./sensor_count

Notes:

- Counts every newline as a reading, including empty lines and trailing newline.
- Uses Linux syscalls (open/read/close/write).
- Designed to be safe (no buffer overruns).
