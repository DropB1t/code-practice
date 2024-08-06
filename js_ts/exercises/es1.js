// console.log();
// console.log();
// console.log();

//ES 1

// function sommAlterna(a) {
//     if (a.length == 0) return 0;
//     let sum = 0;
//     for (index in a)
//         if (index % 2 == 0)
//             sum += a[index];
//         else
//             sum -= a[index];
//     return sum;
// }

// console.log(sommAlterna([1, 2, 3, 4]));
// console.log(sommAlterna([1, -1, 1, -1]));
// console.log(sommAlterna([]));

//ES 2
// function check(a, b) {
//     for (i in a)
//         if (a[i] != b[i])
//             return false

//     return true;
// }

// function subSeq(ago, pagliaio) {
//     if (pagliaio.length < ago.length) return false;
//     let bool = false;
//     for (index in pagliaio) {
//         if (ago[0] == pagliaio[index] && (pagliaio.length - index) >= ago.length)
//             bool = check(ago, pagliaio.slice(index, index + ago.length));
//         if (bool)
//             break;
//     }
//     return bool;
// }

// console.log(subSeq([1], [4, 12, 4, 1, 4]));
// console.log(subSeq([1, 2], [4, 7, 2, 1, 4]));
// console.log(subSeq([true, false, true], [true, true, false, true, true]));

//ES 3

// function tribo(n) {
//     if (n == 0 || n == 1) return 0;
//     if (n == 2) return 1;
//     return tribo(n - 1) + tribo(n - 2) + tribo(n - 3);
// }

// console.log(tribo(1));
// console.log(tribo(10));
// console.log(tribo(22));

//ES 4

// function serieParziale(f, n) {
//     let sum = 0;
//     for (let i = 0; i <= n; i++)
//         sum += f(i);
//     return sum;
// }

// console.log(serieParziale(x => 1, 5));
// console.log(serieParziale(x => (x % 2 == 0 ? 1 : -1), 9));

//ES 5

// function serieParziale2(f) {
//     return fun1 = (n) => {
//         let sum = 0;
//         for (let i = 0; i <= n; i++)
//             sum += f(i);
//         return sum;
//     }
// }

// let g = serieParziale2((x => 1));
// console.log(g(5));
// let h = serieParziale2(x => (x % 2 == 0 ? 1 : -1));
// console.log(h(9));

//ES 6

// function mkmap(s) {
//     if (s.length == 0) return {};
//     let obj = {};
//     for (el of s) {
//         if (!(el in obj))
//             obj[el] = 1;
//     }
//     return obj;
// }

// console.log(mkmap(["pippo", "pluto"]));
// console.log(mkmap([]));
// console.log(mkmap(["pippo", "pippo"]));

//ES 7

// Si scriva una funzione pota(t,k) che, dato un albero binario t costruito come visto a lezione con nodi {val:v, sx:ts, dx:td},
// restituisca un nuovo albero identico a t salvo che per il fatto che tutti i nodi aventi v>k (e gli eventuali sottoalberi radicati in tali nodi)
// vengono omessi.

// function pota(t, k) {
//     if (t == undefined) return {};
//     if (t.val > k)
//         return {};
//     else
//         return { val: t.val, sx: pota(t.sx, k), dx: pota(t.dx, k) }
// }

// var T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };

// console.log(pota(T, 4));
// console.log(pota(T, 7));
// console.log(pota(T, 11));

//ES 8

function count(t) {
    if (t == undefined) return 0;
    return 1 + count(t.sx) + count(t.dx);
}

function pota2(t, k) {
    if (t == undefined || t.val == undefined) return 0;

    // if (t.val > k) return {};
    // let c = 0;
    // if (t.sx != undefined && t.sx.val > k) {
    //     c += count(t.sx);
    //     delete t.sx;
    // }
    // if (t.dx != undefined && t.dx.val > k) {
    //     c += count(t.dx);
    //     delete t.dx;
    // }
    // return c + pota2(t.sx, k) + pota2(t.dx, k);

    if (t.val > k) {
        let c = count(t.sx) + count(t.dx);
        delete t.val;
        delete t.sx;
        delete t.dx;
        return 1 + c;
    }
    else
        return pota2(t.sx, k) + pota2(t.dx, k);
}

var T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };
console.log(pota2(T, 4));
console.log(T);

T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };
console.log(pota2(T, 7));
console.log(T);

T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };
console.log(pota2(T, 11));
console.log(T);

//ES 9

function pota3(t) {
    if (t == undefined || t.val == undefined) return;

    if (t.sx != undefined && t.sx.sx == undefined && t.sx.dx == undefined) {
        t.val += t.sx.val;
        delete t.sx;
    }
    if (t.dx != undefined && t.dx.sx == undefined && t.dx.dx == undefined) {
        t.val += t.dx.val;
        delete t.dx;
    }
    return pota3(t.sx) + pota3(t.dx);
}

// var T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };
// pota3(T);
// console.log(T);

// T = { val: 4, sx: { val: 3, sx: { val: 6, sx: { val: 1 } }, dx: { val: 1 } }, dx: { val: 1, sx: { val: 5, sx: { val: 1 }, dx: { val: 2 } } } };
// pota3(T);
// console.log(T);

//ES 10

// function suffiso(s) {
//     let j = 0;
//     for (var i = s.length - 1; i >= 0; i--) {
//         if ("aeoui".includes(s[i]))
//             j++;
//         if (j == 2)
//             break;
//     }
//     return s.slice(i, s.length);
// }

// function mogol(r, p) {
//     if (r.length == 0) return [];
//     let suf = suffiso(p);
//     let arr = [];
//     for (el of r) {
//         if (suffiso(el) == suf)
//             arr.push(el);
//     }
//     return arr;
// }

// console.log(mogol(["cuore", "amore", "sale"], "sudore"));
// console.log(mogol(["cuore", "amore", "sale"], "mare"));
// console.log(mogol(["cuore", "amore", "sale"], "male"));

//ES 11 ?

//ES 12

// function zip(o) {
//     if (Object.keys(o).length == 0) return [];
//     let arr = [];
//     for (idx in o)
//         arr.push([idx, o[idx]]);
//     return arr;
// }

// console.log(zip({ a: 1, b: true }));
// console.log(zip({ b: true, a: 1 }));
// console.log(zip({}));

//ES 13

// function unzip(a) {
//     if (a.length == 0) return {};
//     let obj = {};
//     for (el of a)
//         obj[el[0]] = el[1];
//     return obj;
// }

// console.log(unzip([['a', 1], ['b', true]]));
// console.log(unzip([['b', true], ['a', 1]]));
// console.log(unzip([]));

//ES 14

// function visita(t, f) {
//     if (t == undefined || t.val == undefined) return;
//     t.val = f(t.val);
//     visita(t.sx, f);
//     visita(t.dx, f);
// }

// var T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };

// console.log(visita(T, x => x % 2));
// console.log(T);

//ES 15

// function visita2(t, f) {
//     if (t == undefined || t.val == undefined) return {};
//     return { val: f(t.val), sx: visita2(t.sx, f), dx: visita2(t.dx, f) };
// }

// var T = { val: 1, sx: { val: 8, sx: { val: 7 }, dx: { val: 1 } }, dx: { val: 3, sx: { val: 5 } } };

// console.log(visita2(T, x => x % 2));
// console.log(T);

//ES 16

// function funprop(f, p = (x => x == x)) {
//     return s = (a, b) => {
//         let arr = [];
//         for (let i = a; i <= b; i++) {
//             if (p(f(i)))
//                 arr.push(i);
//         }
//         return arr;
//     }
// }

// console.log(funprop(n => 2 * n, n => n % 2 == 0)(4, 6));
// console.log(funprop(n => 2 * n, n => n > 10)(4, 8));
// console.log(funprop(n => n, n => n % 2 == 1)(10, 20));
// console.log(funprop(n => n * n)(4, 6));

//ES 17

// function selApply(f, p) {
//     return s = (a) => {
//         for (idx in a) {
//             if (p(a[idx]))
//                 a[idx] = f(a[idx]);
//         }
//     }
// }

// let a = [1, 2, 3, 4];
// selApply(n => n * n, n => n % 2 == 0)(a);
// console.log(a);
// a = [1, 2, 3, 4];
// selApply(n => 0, n => n > 2)(a);
// console.log(a);
// a = [1, 2, 3, 4];
// selApply(n => n, n => n > 0)(a);
// console.log(a);

//ES 18

// function isHeap(t) {
//     if (t.figli == undefined) return true;
//     let bool = false;
//     t.figli.forEach(el => {
//         bool = t.val > el.val && isHeap(el);
//     });
//     return bool;
// }

// let T1 = { val: 9, figli: [{ val: 8, figli: [{ val: 7 }, { val: 1 }, { val: 3 }] }, { val: 3, figli: [{ val: 5 }] }] };
// let T2 = { val: 9, figli: [{ val: 8, figli: [{ val: 7 }, { val: 1 }, { val: 3 }] }, { val: 6, figli: [{ val: 5 }] }] };

// console.log(isHeap(T1));
// console.log(isHeap(T2));

//ES 19

// function semplifica(ao, ak) {
//     let count = 0;
//     ao.forEach(obj => {
//         for (el in obj)
//             if (!ak.includes(el)) {
//                 delete obj[el];
//                 count++;
//             }
//     });
//     return count;
// }

// let o = { a: 12, q: 3 }, ao = [{ a: 1, b: 3 }, { a: 5, c: 7 }, { q: 4, d: true }, o];

// console.log(semplifica(ao, ["q", "c"]));
// console.log(ao);
// console.log(o);