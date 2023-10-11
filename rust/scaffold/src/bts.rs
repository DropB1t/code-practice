use std::{cmp::Ordering::*, mem};

use deepsize::DeepSizeOf;

#[derive(DeepSizeOf)]
pub struct Node<K: Ord> {
    key: K,
    left: BST<K>,
    right: BST<K>,
}

impl<K: Ord> Node<K> {
    pub fn size(&self) -> usize{
        return mem::size_of_val(self);
    }
}

#[derive(DeepSizeOf)]
pub struct BST<K: Ord>(Option<Box<Node<K>>>);

#[allow(dead_code)]
impl<K: Ord> BST<K> {

    pub fn new() -> BST<K> {
        BST(None)
    }

    pub fn insert(&mut self, key: K) -> bool {
        unsafe {
            let mut tree: *mut BST<K> = self;

            while let Some(ref mut node) = (*tree).0 {
                match key.cmp(&node.key) {
                    Less => tree = &mut node.left,
                    Greater => tree = &mut node.right,
                    Equal => return false
                }
            }

            (*tree).0 = Some(Box::new(Node {
                key,
                left: BST(None),
                right: BST(None),
            }));

            true
        }
    }

    pub fn remove(&mut self, key: K) -> bool {
        let mut tree: *mut BST<K> = self;

        unsafe {
            while let Some(ref mut node) = (*tree).0 {
                match key.cmp(&node.key) {
                    Less => tree = &mut node.left,
                    Greater => tree = &mut node.right,
                    Equal => {
                        (*tree).0 = None;
                        return true;
                    }
                }
            }
        }

        false
    }

    pub fn contains(&self, key: K) -> bool {
        let mut tree = self;

        while let Some(ref node) = tree.0 {
            match key.cmp(&node.key) {
                Less => tree = &node.left,
                Greater => tree = &node.right,
                Equal => return true,
            }
        }

        false
    }

    pub fn get_size(&self) -> usize {
        let tree = self;
        let mut s: usize = 0;

        match &tree.0 {
            Some(r) => s +=  r.size() + r.left.get_size() + r.right.get_size(),
            None => (),
        }

        return s;
    }
}

#[test]
fn test_everything() {
    let mut tree = BST::new();

    assert!(!tree.contains(5));

    tree.insert(5);

    assert!(tree.contains(5));

    assert!(tree.insert(6));
    // inserting a key is a no-op if it's already in the tree
    assert!(!tree.insert(6));
    tree.insert(7);
    tree.insert(10);

    assert!(tree.contains(10));
    tree.remove(10);
    assert!(!tree.contains(10));
}
