use std::{cmp::min, str::from_utf8};

struct Number {
    value: i32,
}

struct Pair<T,S> {
    left: T,
    right: S,
}

impl Number {
    fn is_positive(self) -> bool {
        self.value > 0
    }
}

fn main() {
    let pair: (char, i32) = ('a', 17);

    let x = vec![1, 2, 3, 4, 5, 6, 7, 8]
        .iter()
        .map(|x| x + 3)
        .fold(0, |x, y| x + y);

    println!("{}", (x - pair.1 + dice_spin()));

    {
        let x = 32;
        println!("{}", x);
    }

    let _least = min(1, 2);

    let _x = "amos".len();

    let num = Number {
        value: 32,
    };

    print_number(Number {
        value: 32,
    });

    println!("{}", num.is_positive());

    let pair = Pair{ left: 2, right: "lol" };

    println!("{} | {}", pair.right, pair.left);

    let s = from_utf8(&[240,159,141,137]);
    println!("{}",s.unwrap());
    
}

fn dice_spin() -> i32 {
    4
}

fn print_number(n: Number) {
    match n.value {
        1 => println!("one"),
        2 => println!("two"),
        _ => println!("{}", n.value)
    }
}
