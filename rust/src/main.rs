extern crate rand;
use rand::StdRng;
use rand::Rng;
use std::ops::Add;
use std::ops::Sub;
use std::ops::Mul;
use std::ops::Div;

#[derive(Debug, Copy, Clone)]
struct Coordinate {
    x : f64,
    y : f64,
}

impl Coordinate {
    fn length_sq(&self) -> f64 {
        self.x * self.x + self.y * self.y
    }
}

impl Add for Coordinate {
    type Output = Coordinate;
    fn add(self, other: Coordinate) -> Coordinate {
        Coordinate{ x: self.x + other.x, y: self.y + other.y }
    }
}

impl Sub for Coordinate {
    type Output = Coordinate;
    fn sub(self, other: Coordinate) -> Coordinate {
        Coordinate{ x: self.x - other.x, y: self.y - other.y }
    }
}

impl Mul<f64> for Coordinate {
    type Output = Coordinate;
    fn mul(self, other: f64) -> Coordinate {
        Coordinate{ x: self.x * other, y: self.y * other }
    }
}

impl Mul<Coordinate> for f64 {
    type Output = Coordinate;
    fn mul(self, other: Coordinate) -> Coordinate {
        Coordinate{ x: self * other.x, y: self * other.y }
    }
}

impl Div<f64> for Coordinate {
    type Output = Coordinate;
    fn div(self, other: f64) -> Coordinate {
        Coordinate{ x: self.x / other, y: self.y / other }
    }
}

#[derive(Debug)]
struct Boundary {
    upper : Coordinate,
    lower : Coordinate,
    width : Coordinate,
    halfw : Coordinate,
}

impl Boundary {
    fn new(l : Coordinate, u : Coordinate) -> Boundary {
        Boundary{upper : u, lower : l, width : u - l, halfw : u - l / 2.}
    }

    fn restrict_position(&self, mut p : Coordinate) -> Coordinate {
        if      p.x < self.lower.x {p.x += self.width.x}
        else if self.upper.x < p.x {p.x -= self.width.x}
        if      p.y < self.lower.y {p.y += self.width.y}
        else if self.upper.y < p.y {p.y -= self.width.y}
        p
    }

    fn restrict_direction(&self, mut d : Coordinate) -> Coordinate {
        if      d.x < -self.halfw.x {d.x += self.width.x}
        else if self.halfw.x < d.x  {d.x -= self.width.x}
        if      d.y < -self.halfw.y {d.y += self.width.y}
        else if self.halfw.y < d.y  {d.y -= self.width.y}
        d
    }
}

#[derive(Debug, Clone)]
struct Circle {
    position : Coordinate,
    radius   : f64,
}

fn overlaps(lhs : &Circle, rhs : &Circle, bd : &Boundary) -> bool {
    bd.restrict_direction(lhs.position - rhs.position).length_sq() <
        (lhs.radius + rhs.radius) * (lhs.radius + rhs.radius)
}

struct System {
    circles  : std::vec::Vec<Circle>,
    boundary : Boundary,
}

impl System {
    fn new(lower : Coordinate, upper : Coordinate) -> Self {
        System{
            circles  : std::vec::Vec::new(),
            boundary : Boundary::new(lower, upper)
        }
    }
    fn attempt_move(&mut self, idx : usize, dp: Coordinate) -> bool {
        let mut target = self.circles[idx].clone();
        target.position = self.boundary.restrict_position(target.position + dp);

        for i in 0 .. self.circles.len() {
            if i == idx {continue;}
            if overlaps(&target, &self.circles[i], &self.boundary) {
                return false;
            }
        }
        self.circles[idx] = target;
        true
    }
    fn attempt_push(&mut self, circle : Circle) -> bool {
        for c in &self.circles {
            if overlaps(&circle, c, &self.boundary) {
                return false;
            }
        }
        self.circles.push(circle);
        true
    }

    fn output_xyz(&self) {
        println!("{}", self.circles.len());
        println!("");
        for c in &self.circles {
            println!("H     {:8.3} {:8.3} {:8.3}", c.position.x, c.position.y, 0.0);
        }
    }
}

fn main() {
    let mut rng = StdRng::new().expect("StdRng::new() error.");
    let radius  = 1.0; // S = 3.1416; 900 * 0.63 ~ 3.14 * 180

    let lower_bound = Coordinate{x: 0.0,y: 0.0};
    let upper_bound = Coordinate{x:30.0,y:30.0};
    let mut system  = System::new(lower_bound, upper_bound);
    let num_circles = 100 as usize;

    for _ in 0..num_circles {
        loop {
            let new_x      = rng.gen_range(lower_bound.x, upper_bound.x);
            let new_y      = rng.gen_range(lower_bound.y, upper_bound.y);
            let new_circle = Circle{position : Coordinate{x:new_x, y:new_y}, radius};
            if system.attempt_push(new_circle) {
                break;
            }
        }
    }

    let mut idxs : std::vec::Vec<usize> = std::vec::Vec::new();
    for i in 0..num_circles {
        idxs.push(i);
    }

    let delta = 0.3;
    for _ in 0..10000 {
        system.output_xyz();
        rng.shuffle(&mut idxs);
        for i in idxs.iter() {
            let dx = rng.gen_range(-delta, delta);
            let dy = rng.gen_range(-delta, delta);
            system.attempt_move(*i, Coordinate{x:dx, y:dy});
        }
    }
    system.output_xyz();
}
