make
#./imgurfetch -g key.txt 3lV6s
valgrind --leak-check=full --track-origins=yes ./imgurfetch -c config
