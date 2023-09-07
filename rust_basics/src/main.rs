fn bubble_sort(x: &mut [i32], n: usize) -> &mut [i32]{
    for i in 0..n-1 {
        for j in 0..n-i-1 {
            if x[j+1] < x[j] {
                x.swap(j, j+1);
            }
        }
    }
    x
}


fn main() {
    let mut x = [0, 5, 8, 7, 4, 17, 1, -5];
    let y = bubble_sort(&mut x, 8);
    for i in y {
        print!("{} ", i);
    }
}