import random
import copy

class Boundary:
    """Periodic Boundary Condition"""

    def __init__(self, lw, up):
        self.lower = lw
        self.upper = up
        self.width = [up[0] - lw[0], up[1] - lw[1]]
        self.half_width = [self.width[0] / 2, self.width[1] / 2]

    def restrict_position(self, vec):
        if (vec[0] < self.lower[0]):
            vec[0] += self.width[0]
        elif (self.upper[0] < vec[0]):
            vec[0] -= self.width[0]
        if (vec[1] < self.lower[1]):
            vec[1] += self.width[1]
        elif (self.upper[1] < vec[1]):
            vec[1] -= self.width[1]
        return vec

    def restrict_direction(self, vec):
        if (vec[0] < -self.half_width[0]):
            vec[0] += self.width[0]
        elif (self.half_width[0] < vec[0]):
            vec[0] -= self.width[0]
        if (vec[1] < -self.half_width[1]):
            vec[1] += self.width[1]
        elif (self.half_width[1] < vec[1]):
            vec[1] -= self.width[1]
        return vec


class Disk:
    """ rigid disk """
    def __init__(self, r, p):
        self.radius   = r
        self.position = p

def overlaps(lhs, rhs, bc):
    dr = bc.restrict_direction([lhs.position[0] - rhs.position[0],
                                lhs.position[1] - rhs.position[1]])
    return ((dr[0] ** 2 + dr[1] ** 2) < (lhs.radius + rhs.radius) ** 2)

def print_disks(disks):
    print(len(disks))
    print("")
    for disk in disks:
        print("H     {} {} 0.0".format(disk.position[0], disk.position[1]))

N      = 100
radius = 1.0
delta  = 0.3

lower = [ 0.0,  0.0]
upper = [30.0, 30.0]
bc    = Boundary(lower, upper)

random.seed(123456789)

disks = [];
for i in range(N):
    while True:
        new_disk = Disk(radius,
            [random.random() * (upper[0] - lower[0]) + lower[0],
             random.random() * (upper[1] - lower[1]) + lower[1]]
        )
        collides = False
        for disk in disks:
            if overlaps(disk, new_disk, bc):
                collides = True
                break
        if not collides:
            disks.append(new_disk)
            break

indices = [i for i in range(N)]
for t in range(100):
    print_disks(disks)
    random.shuffle(indices)
    for i in indices:
        target = copy.deepcopy(disks[i]) # do not forget deep-copy!
        target.position[0] += (random.random() * 2.0 - 1.0) * delta
        target.position[1] += (random.random() * 2.0 - 1.0) * delta
        target.position = bc.restrict_position(target.position)


        collides = False
        for j in range(N):
            if i == j:
                continue
            if overlaps(target, disks[j], bc):
                collides = True
                break
        if not collides:
            disks[i] = target
print_disks(disks)
