use rand::Rng;
use std::mem;

#[derive(Debug)]
struct Node {
    val: i32,
    right: Option<Box<Node>>,
    left: Option<Box<Node>>,
}

impl Node {
    fn add(&mut self, x: i32) {
        if x > self.val {
            match &mut self.right {
                Some(r) => (*r).add(x),
                None => {
                    self.right = Some(Box::new(Node {
                        val: x,
                        right: None,
                        left: None,
                    }))
                }
            }
            return;
        }

        match &mut self.left {
            Some(l) => (*l).add(x),
            None => {
                self.left = Some(Box::new(Node {
                    val: x,
                    right: None,
                    left: None,
                }))
            }
        }
    }

    fn get_size(&self) -> usize {
        let mut s: usize = 0;
        s += mem::size_of_val(self);

        match &self.right {
            Some(r) => s += (*r).get_size(),
            None => (),
        }

        match &self.left {
            Some(l) => s += (*l).get_size(),
            None => (),
        }

        return s;
    }
}

fn main() {
    let mut tree: Node = Node {
        val: 0,
        right: None,
        left: None,
    };
    let mut rng = rand::thread_rng();

    let mut i: i32 = 100;
    while i != 0 {
        tree.add(rng.gen_range(-500..=500));
        i -= 1;
    }

    println!("{:?}", tree);
    println!("Tree occupies {} bytes", tree.get_size());
}
