module rigid_disk
    implicit none

    type disk
        real  :: radius
        real, dimension(2) :: pos
    end type disk

    type boundary
        real, dimension(2) :: upper, lower, width, half_width
    end type boundary

contains

    real function length_sq(vec)
        implicit none
        real, dimension(2), intent(in) :: vec
        length_sq = vec(1)**2 + vec(2)**2
    end function length_sq

    subroutine restrict_position(pos, bc)
        implicit none
        real, dimension(2), intent(inout) :: pos
        type(boundary), intent(in) :: bc

        if (pos(1) .LT. bc%lower(1)) then
            pos(1) = pos(1) + bc%width(1)
        else if (bc%upper(1) .LT. pos(1)) then
            pos(1) = pos(1) - bc%width(1)
        end if

        if (pos(2) .LT. bc%lower(2)) then
            pos(2) = pos(2) + bc%width(2)
        else if (bc%upper(2) .LT. pos(2)) then
            pos(2) = pos(2) - bc%width(2)
        end if
    end subroutine restrict_position

    subroutine restrict_direction(vec, bc)
        implicit none
        real, dimension(2), intent(inout) :: vec
        type(boundary), intent(in) :: bc

        if (vec(1) .LT. -bc%half_width(1)) then
            vec(1) = vec(1) + bc%width(1)
        else if (bc%half_width(1) .LT. vec(1)) then
            vec(1) = vec(1) - bc%width(1)
        end if

        if (vec(2) .LT. -bc%half_width(2)) then
            vec(2) = vec(2) + bc%width(2)
        else if (bc%half_width(2) .LT. vec(2)) then
            vec(2) = vec(2) - bc%width(2)
        end if
    end subroutine restrict_direction

    logical function overlaps(lhs, rhs, bc)
        implicit none
        type(boundary), intent(in) :: bc
        type(disk),     intent(in) :: lhs, rhs
        real, dimension(2) :: vec
        vec = lhs%pos - rhs%pos
        call restrict_direction(vec, bc)

        overlaps = length_sq(vec) .LT. (lhs%radius + rhs%radius)**2
    end function overlaps

end module rigid_disk

program main
    use rigid_disk
    implicit none

    real,    parameter                    :: radius    = 1.0
    real,    parameter                    :: delta     = 0.3
    integer, parameter                    :: num_disks = 100
    integer                               :: i,j,t, idx
    integer                               :: seedsize
    integer, allocatable, dimension(:)    :: seed, indices
    real,    dimension(2)                 :: lower, upper
    type(boundary)                        :: bc
    type(disk), allocatable, dimension(:) :: disks
    type(disk)                            :: new_disk
    real                                  :: rand
    logical                               :: collides
    real, dimension(2)                    :: vec

    call random_seed(size=seedsize)
    allocate(seed(seedsize))
    do i=1, seedsize
      call system_clock(count=seed(i))
    end do
    call random_seed(put=seed)

    lower = (/ 0.0,  0.0/)
    upper = (/30.0, 30.0/)
    bc%lower      = lower
    bc%upper      = upper
    bc%width      = upper - lower
    bc%half_width = (upper - lower) / 2


    allocate(disks(num_disks))
    do i=1, num_disks
        do
            new_disk%radius = radius
            call random_number(rand)
            new_disk%pos(1) = rand * (upper(1) - lower(1)) + lower(1)
            call random_number(rand)
            new_disk%pos(2) = rand * (upper(2) - lower(2)) + lower(2)

            collides = .false.
            do j=1, i-1
                if (overlaps(new_disk, disks(j), bc)) then
                    collides = .true.
                    exit
                end if
            end do
            if (.not. collides) then
                disks(i) = new_disk
                exit
            end if
        end do
    end do

    allocate(indices(num_disks))
    do i=1, num_disks
        indices(i) = i
    end do

    do t=1, 10000
        call print_disks(disks, num_disks)
        call shuffle(indices, num_disks)

        do i=1, num_disks
            idx = indices(i)

            new_disk = disks(idx)
            call random_number(rand)
            new_disk%pos(1) = new_disk%pos(1) + (rand * 2 - 1.0) * delta
            call random_number(rand)
            new_disk%pos(2) = new_disk%pos(2) + (rand * 2 - 1.0) * delta

            call restrict_position(new_disk%pos, bc)

            collides = .false.
            do j=1, num_disks
                if(j == idx) cycle

                if(overlaps(new_disk, disks(j), bc)) then
                    collides = .true.
                end if
            end do

            if(.not. collides) then
                disks(idx) = new_disk
            end if
        end do
    end do

    call print_disks(disks, num_disks)

    deallocate(seed)
    deallocate(indices)
    deallocate(disks)

    stop

contains

    subroutine print_disks(disks, n)
        type(disk) :: disks(:)
        integer, intent(in) :: n

        write(*,'(I0,/)') num_disks
        do i=1, num_disks
            write(*,'("H     ", f8.3, f8.3, " 0.0")') disks(i)%pos(1), disks(i)%pos(2)
        end do
    end subroutine print_disks

    ! Fisher-Yates-Durstenfeld shuffle algorithm
    subroutine shuffle(array, n)
        integer :: array(:)
        integer, intent(in) :: n
        integer :: i, j, tmp
        real    :: rnd

        do i=1, n-1
            call random_number(rnd)
            j = rnd * (n+1 - i) + i
            tmp = array(j)
            array(j) = array(i)
            array(i) = tmp
        end do
    end subroutine shuffle
end program main
