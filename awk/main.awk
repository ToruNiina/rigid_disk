function restrict_position(vec, boundary) {
    if (vec[0] <  boundary["lower",0]) {
        vec[0] += boundary["width",0]
    } else if (boundary["upper",0] < vec[0]) {
        vec[0] -= boundary["width",0]
    }
    if (vec[1] <  boundary["lower",1]) {
        vec[1] += boundary["width",1]
    } else if (boundary["upper",1] < vec[1]) {
        vec[1] -= boundary["width",1]
    }
}
function restrict_direction(vec, boundary) {
    if (vec[0] < -boundary["halfw",0]) {
        vec[0] += boundary["width",0]
    } else if (boundary["upper",0] < vec[0]) {
        vec[0] -= boundary["width",0]
    }
    if (vec[1] < -boundary["halfw",1]) {
        vec[1] += boundary["width",1]
    } else if (boundary["upper",1] < vec[1]) {
        vec[1] -= boundary["width",1]
    }
}

function overlaps(disks, i, j, boundary) {
    dr[0] = disks[i,"position",0] - disks[j,"position",0]
    dr[1] = disks[i,"position",1] - disks[j,"position",1]
    restrict_direction(dr, boundary)
    return (dr[0]**2 + dr[1]**2) < (disks[i,"radius"] + disks[j,"radius"]) ** 2
}

function shuffle(idxs, n) {
    for(i=0; i<n-1; ++i) {
        j = int(rand() * (n-i) + i)
        tmp = idxs[j]
        idxs[j] = idxs[i]
        idxs[i] = tmp
    }
}

function print_disks(disks, n) {
    printf("%d\n\n", n)
    for(i=0; i<n; ++i) {
        printf("H     %8.3f%8.3f   0.000\n",\
               disks[i,"position",0], disks[i,"position",1])
    }
}

BEGIN {
    srand(123456789)
    N      = 100
    radius = 1.0
    delta  = 0.3
    boundary["lower",0] =  0.0; boundary["lower",1] =  0.0
    boundary["upper",0] = 30.0; boundary["upper",1] = 30.0
    boundary["width",0] = 30.0; boundary["width",1] = 30.0
    boundary["halfw",0] = 15.0; boundary["halfw",1] = 15.0
    for(i=0;i<N;++i) {
        while(1) {
            disks[i,"radius"] = radius
            disks[i,"position",0] = boundary["lower",0] +\
                rand() * (boundary["upper",0] - boundary["lower",0])
            disks[i,"position",1] = boundary["lower",0] +\
                rand() * (boundary["upper",1] - boundary["lower",1])
            collides=0
            for(j=0;j<i; ++j) {
                if(overlaps(disks, i, j, boundary)) {
                    collides=1
                    break
                }
            }
            if(collides == 0) {
                break
            }
        }
    }
    for(i=0; i<N; ++i) {
        idxs[i] = i
    }
    for(t=0; t<10000; ++t) {
        print_disks(disks, N)
        shuffle(idxs, N)
        for(i=0; i<N; ++i) {
            idx = idxs[i]
            x_prev = disks[i,"position",0]
            y_prev = disks[i,"position",1]
            nextpos[0] = x_prev + (rand() * 2.0 - 1.0) * delta
            nextpos[1] = y_prev + (rand() * 2.0 - 1.0) * delta
            restrict_position(nextpos, boundary)
            disks[i,"position",0] = nextpos[0]
            disks[i,"position",1] = nextpos[1]

            collides=0
            for(j=0; j<N; ++j) {
                if(i==j) {continue}
                if(overlaps(disks, i, j, boundary)) {
                    collides=1
                    break
                }
            }
            if(collides == 1) {
                disks[i,"position",0] = x_prev
                disks[i,"position",1] = y_prev
            }
        }
    }
    print_disks(disks, N)
}
