make
#./imgurfetch -g key.txt 3lV6s
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./imgurfetch -c config
