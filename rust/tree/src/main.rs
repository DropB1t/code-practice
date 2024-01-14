
use deepsize::DeepSizeOf;
use rand::Rng;

#[derive(PartialEq, DeepSizeOf)]
struct Node<'a> {
    val: &'a str,
    l: Option<Box<Node<'a>>>,
    r: Option<Box<Node<'a>>>,
}
impl<'a> Node<'a> {
    pub fn insert(&mut self, new_val: &'a str) {
        if self.val == new_val {
            return
        }
        let target_node = if new_val < self.val { &mut self.l } else { &mut self.r };
        match target_node {
            &mut Some(ref mut subnode) => subnode.insert(new_val),
            &mut None => {
                let new_node = Node { val: new_val, l: None, r: None };
                let boxed_node = Some(Box::new(new_node));
                *target_node = boxed_node;
            }
        }
    }
}
fn main () {
    let mut rng = rand::thread_rng();

    let mut x = Node { val: "m", l: None, r: None };

    let mut i: i32 = 500;
    while i != 0 {
        let value = rng.gen_range(-500..=500).to_string();
        x.insert(&value);
        i -= 1;
    }

    println!("Deep Size occupies {} bytes", x.deep_size_of());

}
