use rand::Rng;
use std::mem;


#[derive(Debug)]
struct Node {
    val: i32,
    right: Option<Box<Node>>,
    left: Option<Box<Node>>,
}

impl Node {
    fn add(&mut self, x:i32){
        if x > self.val {
            if let Some( rn) = self.right.as_mut() {
                rn.add(x)
            }else {
                self.right = Some(Box::new(Node{val:x,right:None,left:None}));
            }
        } else {
            if let Some(ln) = self.left.as_mut() {
                ln.add(x)
            }else {
                self.left = Some(Box::new(Node{val:x,right:None,left:None}));
            }
        }
    }

    fn get_size(&self) -> usize {
        let mut s:usize = 0;
        s += mem::size_of_val(self);
        if let Some(rn) = self.right.as_ref() {
            s += rn.get_size();
        }
        if let Some(ln) = self.left.as_ref() {
            s += ln.get_size();
        }
        return s;
    }
}

fn main() {

    let mut tree: Node = Node{val:2,right:None,left:None};
    let mut rng = rand::thread_rng();

    let mut i:i32 = 1;
    while i != 0 {
        tree.add(rng.gen::<i32>());
        i-=1;
    }

    println!("Tree occupies {} bytes on the stack", tree.get_size());

    //println!("{:?}", n);

    /* let s = from_utf8(&[240,159,141,137]);
    println!("{}",s.unwrap()); */
    
}
