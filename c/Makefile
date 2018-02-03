all:
	gcc -std=c99 -O3 -flto -march=native main.c boundary.c disk.c position.c -lm -DDSFMT_MEXP=19937 dsfmt/dSFMT.c -o mcmc
