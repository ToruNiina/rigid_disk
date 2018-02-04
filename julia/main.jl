immutable Boundary
    lower      :: Array{Float64,1}
    upper      :: Array{Float64,1}
    width      :: Array{Float64,1}
    half_width :: Array{Float64,1}

    function Boundary(lower, upper)
        return new(lower, upper, upper - lower, (upper - lower) / 2)
    end
end

function restrict_position(vec, bc)
    vec[1] += if (vec[1] < bc.lower[1])  bc.width[1]
          elseif (bc.lower[1] < vec[1]) -bc.width[1]
          else 0.0 end;
    vec[2] += if (vec[2] < bc.lower[2])  bc.width[2]
          elseif (bc.lower[2] < vec[2]) -bc.width[2]
          else 0.0 end;
    return vec
end

function restrict_direction(vec, bc)
    vec[1] += if (vec[1] < -bc.half_width[1])  bc.width[1]
          elseif (bc.half_width[1]  < vec[1]) -bc.width[1]
          else 0.0 end;
    vec[2] += if (vec[2] < -bc.half_width[2])  bc.width[2]
          elseif (bc.half_width[2]  < vec[2]) -bc.width[2]
          else 0.0 end;
    return vec
end

mutable struct Disk
    r :: Float64
    p :: Array{Float64, 1}
end

function overlaps(lhs, rhs, bc)
    dr = lhs.p - rhs.p
    d2 = dot(dr, dr)
    return d2 < (lhs.r + rhs.r) ^ 2
end

function print(disks)
    println(length(disks))
    println("")
    for disk in disks
        println("H     ", disk.p[1], " ", disk.p[2], " 0.0")
    end
end

const N = 100
const σ = 1.0
const δ = 0.3

const lower = [ 0.0,  0.0]
const upper = [30.0, 30.0]
const bc    = Boundary(lower, upper)

mt = MersenneTwister(123456789)

disks = Disk[];
for i = 1:N
    while true
        new_disk = Disk(σ,
            [rand(mt) * (upper[1] - lower[1]) + lower[1],
             rand(mt) * (upper[2] - lower[2]) + lower[2]]
        )
        collides = false
        for disk in disks
            if overlaps(disk, new_disk, bc)
                collides = true
                break
            end
        end
        if !collides
            push!(disks, new_disk)
            break
        end
    end
end

indices = range(1,N)
for t=1:10
    print(disks)
    shuffle(mt, indices)
    for i in indices
        target = disks[i]
        dr     = [rand(mt), rand(mt)]
        target.p += dr

        collides = false
        for j=1:N
            if i == j
                break
            end
            if overlaps(target, disks[j], bc)
                collides = true
                break
            end
        end
        if !collides
            disks[i] = target
        end
    end
end
print(disks)
