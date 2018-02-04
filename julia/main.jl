# "immutable struct" causes an error: syntax: invalid type name "struct"
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
    if (vec[1] < bc.lower[1])
        vec[1] += bc.width[1]
    elseif (bc.upper[1] < vec[1])
        vec[1] -= bc.width[1]
    end
    if (vec[2] < bc.lower[2])
        vec[2] += bc.width[2]
    elseif (bc.upper[2] < vec[2])
        vec[2] -= bc.width[2]
    end
    return vec
end

function restrict_direction(vec, bc)
    if (vec[1] < -bc.half_width[1])
        vec[1] += bc.width[1]
    elseif (bc.half_width[1] < vec[1])
        vec[1] -= bc.width[1]
    end
    if (vec[2] < -bc.half_width[2])
        vec[2] += bc.width[2]
    elseif (bc.half_width[2] < vec[2])
        vec[2] -= bc.width[2]
    end
    return vec
end

mutable struct Disk
    r :: Float64
    p :: Array{Float64, 1}
end

function overlaps(lhs, rhs, bc)
    dr = restrict_direction(lhs.p - rhs.p, bc)
    d2 = dr[1]^2 + dr[2]^2
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
        target = Disk(disks[i].r, disks[i].p)
        dr     = [(rand(mt) * 2.0 - 1.0) * δ, (rand(mt) * 2.0 - 1.0) * δ]
        target.p = restrict_position(target.p + dr, bc)

        collides = false
        for j=1:N
            if i == j
                continue
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
