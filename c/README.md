dSFMTをダウンロードして`dsfmt/`にソースファイルを置く

```sh
bash$ wget http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/dSFMT-src-2.2.3.tar.gz
bash$ tar xvf dSFMT-src-2.2.3.tar.gz
bash$ cp dSFMT-src-2.2.3/*.[ch] rigid_disk/c/dsfmt
```

コンパイルして実行
```sh
$ make
$ ./mcmc > traj.xyz
```
