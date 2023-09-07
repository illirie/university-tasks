use std::{fs, f32::consts::E, string};

fn day1_1() {
    let file = format!("day1.txt");
    let f = fs::read_to_string(&file)
                        .unwrap_or_else(|_| panic!("Error reading file {}", file))
                        .trim().replace('\r', "");
    let data = f.lines()
                        .map(|x| x.parse::<u32>()
                        .unwrap())
                        .collect::<Vec<u32>>();
    let inc = data.windows(2).filter(|x| x[0] < x[1]).count();
    println!("{}", inc);
}

fn day1_2() {
    let file = format!("day1.txt");
    let f = fs::read_to_string(&file)
                        .unwrap_or_else(|_| panic!("Error reading file {}", file))
                        .trim().replace('\r', "");
    let data = f.lines()
                        .map(|x| x.parse::<u32>()
                        .unwrap())
                        .collect::<Vec<u32>>();
    let inc = data.windows(4).filter(|x| x[0] < x[3]).count();
    println!("{}", inc);
}

fn day2_1() {
    let mut x: i32 = 0;
    let mut y: i32 = 0;
    let file = format!("day2.txt");
    let f = fs::read_to_string(&file)
                        .unwrap_or_else(|_| panic!("Error reading file {}", file))
                        .trim().replace('\r', "");
    for i in f.lines() {
        let dist = i.split(' ').collect::<Vec<&str>>();
        let point: i32 = dist[1].parse::<i32>().unwrap();

        match dist[0] {
            "forward" => x += point,
            "up" => y -= point,
            "down" => y += point,
            _ => {}
        }
    }
    println!("{}", x*y);
}

fn day2_2() {
    let mut x: i128 = 0;
    let mut y: i128 = 0;
    let mut y_temp: i128 = 0;
    let file = format!("day2.txt");
    let f = fs::read_to_string(&file)
                        .unwrap_or_else(|_| panic!("Error reading file {}", file))
                        .trim().replace('\r', "");
    for i in f.lines() {
        let dist = i.split(' ').collect::<Vec<&str>>();
        let point: i128 = dist[1].parse::<i128>().unwrap();
        match dist[0] {
            "forward" => {
                x += point;
                y += y_temp * point;
            },
            "up" => y_temp -= point,
            "down" => y_temp += point,
            _ => {}
        }
    }
    println!("{}", x*y);
}


fn day3_1() {
    let mut summary = [0; 12];
    let file = format!("day3.txt");
    let f = fs::read_to_string(&file)
                        .unwrap_or_else(|_| panic!("Error reading file {}", file))
                        .trim().replace('\r', "");
    let mut lenght = 0;
    for i in f.lines() {
        lenght += 1;
        let chars : Vec<char> = i.chars().collect();
        for j in 0..12 {
            summary[j] += chars[j] as i32 - 48;
        }
    }
    for i in 0..12 {
        if summary[i] <= lenght / 2 {
            summary[i] = 0;
        }
        else {
            summary[i] = 1;
        }
    }
    println!("summary: {:?}", summary);
    let mut gamma = 0;
    let mut epsilon = 0;
    for i in 0..12 {
        if summary[i] == 1 {
            gamma += i32::pow(2, 11 - i as u32);
        }
        else {
            epsilon += i32::pow(2, 11 - i as u32);
        }
    }
    print!("{} {} ", gamma, epsilon);
    print!("{}", gamma*epsilon);
}


fn main() {
    //day1_1();
    //day1_2();
    //day2_1();
    //day2_2();
    day3_1();
}
