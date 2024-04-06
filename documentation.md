The code is compiled using the commands:

gcc producer.c -pthread -lrt -o producer 
gcc consumer.c -pthread -lrt -o consumer 
./producer & ./consumer &
