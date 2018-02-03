all:
	gcc -std=c99 -Ofast -flto -march=native -mtune=native main.c boundary.c disk.c position.c -lm -DDSFMT_MEXP=19937 dsfmt/dSFMT.c -o mcmc
