//ES 1

// function produttoria(a, b) {
//     if (a > b) return undefined;
//     let mul = 1;
//     for (let i = a; i <= b; i++)
//         mul *= i;
//     return mul;
// }

// console.log(produttoria(4, 6));
// console.log(produttoria(10, 10));
// console.log(produttoria(10, 11));

//ES 2
// function quaterne(a, b) {
//     if (a >= b || (b - a) - 1 < 4)
//         return 0;
//     let count = 0;
//     for (let i = a + 1; i < b; i++)
//         if (i + 3 < b)
//             count++;
//     return count;
// }

// console.log(quaterne(4, 6));
// console.log(quaterne(4, 10));
// console.log(quaterne(-4, 10));

//ES 3

// function perfetto(a) {
//     if (a <= 0) return undefined;
//     let sum = 0;
//     for (let i = 1; i < a; i++)
//         if (a % i == 0)
//             sum += i;

//     return sum == a ? true : false;
// }

// console.log(perfetto(6));
// console.log(perfetto(10));
// console.log(perfetto(28));

//ES 4

// function range(a, b) {
//     if (a > b) return [];
//     let v = [];
//     for (let i = a; i <= b; i++)
//         v.push(i);
//     return v;
// }

// console.log(range(2, 6));
// console.log(range(10, 10));
// console.log(range(-5, 1));
// console.log(range(10, 4));

//ES 5

// function punultimo(a) {
//     if (!(a.length >= 2)) return undefined;
//     a.sort(function (a, b) {
//         if (a.toLowerCase() < b.toLowerCase()) {
//             return -1;
//         } else if (a.toLowerCase() > b.toLowerCase()) {
//             return 1;
//         } else {
//             return 0;
//         }
//     });
//     return a[a.length - 2];
// }

// console.log(punultimo(["pera", "zucca", "mela"]));
// console.log(punultimo(["dattero", "zucca", "mela"]));
// console.log(punultimo(["zucca"]));

//ES 6

// function ordlun(a) {
//     if (a.length == 0) return [];
//     if (a.length == 1) return a;

//     a.sort(function (a, b) {
//         if (a.length < b.length) {
//             return -1;
//         } else if (a.length > b.length) {
//             return 1;
//         } else {
//             if (a.toLowerCase() < b.toLowerCase()) {
//                 return -1;
//             } else if (a.toLowerCase() > b.toLowerCase()) {
//                 return 1;
//             } else {
//                 return 0;
//             }
//         }
//     });

//     return a;
// }

// console.log(ordlun(["pera", "zucca", "mela"]));
// console.log(ordlun(["dattero", "zucca", "mela"]));
// console.log(ordlun([]));

//ES 7

// function cvtmi(a) {
//     if (a.length == 0) return {};
//     let obj = {};
//     for (let i = 0; i < a.length; i++) {
//         if (a[i] in obj)
//             obj[a[i]]++;
//         else
//             obj[a[i]] = 1;
//     }
//     return obj;
// }

// console.log(cvtmi(["pera", "zucca", "mela"]));
// console.log(cvtmi(["pera", "pera", "pera", "zucca"]));
// console.log(cvtmi([1, 2, 3, 4, 3, 4, 5, 2, 1, 1, 9]));

//ES 8

// function unionemi(a, b) {
//     if (typeof a != "object" || typeof b != "object") return undefined;
//     if (Object.keys(a).length === 0 && Object.keys(b).length === 0) return {};
//     if (Object.keys(a).length === 0) return b;
//     if (Object.keys(b).length === 0) return a;

//     let obj = a;
//     for (let key in b) {
//         if (key in obj)
//             obj[key] += b[key];
//         else
//             obj[key] = b[key];
//     }
//     return obj;
// }

// function intersezionemi(a, b) {
//     if (typeof a != "object" || typeof b != "object") return undefined;
//     if (Object.keys(a).length === 0 || Object.keys(b).length === 0) return {};

//     let obj = {};
//     for (let key in b) {
//         if (key in a)
//             obj[key] = b[key];
//     }
//     return obj;
// }

// console.log(unionemi({ 1: 4, 2: 1 }, { 1: 3, 3: 1 }));
// console.log(intersezionemi({ 1: 4, 2: 1 }, { 1: 3, 3: 1 }));
// console.log(intersezionemi({ 1: 4, 2: 1 }, {}));

//ES 9

// function contaVocali(s) {
//     if (s == "") return 0;
//     let count = 0;
//     for (let i = 0; i < s.length; i++) {
//         if (s[i].toLowerCase() == "a" || s[i].toLowerCase() == "e" || s[i].toLowerCase() == "i" || s[i].toLowerCase() == "o" || s[i].toLowerCase() == "u")
//             count++;
//     }
//     return count;
// }

// console.log(contaVocali("Ai lati d’Italia"));
// console.log(contaVocali("qwerty"));
// console.log(contaVocali("3463234"));
// console.log(contaVocali("Nel mezzo del cammin di nostra vita"));

//ES 10

// function firma(s) {
//     if (s == "") return 0;
//     let str = "";
//     for (let i = 0; i < s.length; i++) {
//         if (s[i].toLowerCase() == "a" || s[i].toLowerCase() == "e" || s[i].toLowerCase() == "i" || s[i].toLowerCase() == "o" || s[i].toLowerCase() == "u" || s[i] == " ")
//             str += "1"
//         else
//             str += "0"
//     }
//     return parseInt(str, 2);
// }

// console.log(firma("Vincenzo Gervasi"));
// console.log(firma("Alina Sirbu"));
// console.log(firma(""));

//ES 11

// function applyobj(o, f) {
//     if (Object.keys(o).length === 0 && o.constructor === Object ) return {};
//     for (let key in o)
//         o[key] = f(o[key]);
//     return o;
// }

// console.log(applyobj({ pere: 3, mele: 1 }, x => 2 * x));
// console.log(applyobj({ io: "Vincenzo", tu: "Alina" }, e => e.length));
// console.log(applyobj({ io: 8, tu: 5 }, e => e));
// console.log(applyobj({}, e => e + 1));

//ES 12

// function maxprod(a) {
//     if (a.length === 0) return {};
//     let obj = { "idx": 1, "val": -Infinity }
//     for (let i = 0; i < a.length; i++) {
//         if ((i * a[i]) > (obj.val * obj.idx)) {
//             obj.idx = i;
//             obj.val = a[i];
//         }
//     }
//     return obj;
// }

// console.log(maxprod([8, 2, 2, 1]));
// console.log(maxprod([1, 8, 1, 2, 2]));

//ES 13

// function appiattisci(a) {
//     if (!a.length) return [];
//     let arr = [];
//     a.forEach(el => {
//         if (Array.isArray(el))
//             arr.push(...appiattisci(el));
//         else
//             arr.push(el);
//     });
//     return arr;
// }

// console.log(appiattisci([8, [2, 2], 1]));
// console.log(appiattisci([[1], 8, [1, 2], 2, []]));

//ES 14

// function rosa(s) {
//     if (s == "") return undefined;

//     let obj = { x: 0, y: 0 };
//     for (c of s) {
//         switch (c) {
//             case "N":
//                 obj.y = 1;
//                 break;
//             case "S":
//                 obj.y = -1;
//                 break;
//             case "E":
//                 obj.x = 1;
//                 break;
//             case "W":
//                 obj.x = -1;
//                 break;
//         }
//     }
//     return obj;
// }

// console.log(rosa("NE"));
// console.log(rosa("S"));
// console.log(rosa("EN"));
// console.log(rosa("NW"));

//ES 15

// function calc(obj, s) {
//     if (s == "") return obj;
//     for (c of s) {
//         switch (c) {
//             case "N":
//                 obj.y += 1;
//                 break;
//             case "S":
//                 obj.y += -1;
//                 break;
//             case "E":
//                 obj.x += 1;
//                 break;
//             case "W":
//                 obj.x += -1;
//                 break;
//         }
//     }
//     return obj;
// }

// function percorso(a) {
//     if (a.length == 0) return 0;
//     let obj = { x: 0, y: 0 };
//     a.forEach(el => {
//         obj = calc(obj, el);
//     });

//     return Math.sqrt((obj.x ** 2) + (obj.y ** 2));
// }

// console.log(percorso([]));
// console.log(percorso(["N"]));
// console.log(percorso(["N", "S"]));
// console.log(percorso(["N", "E"]));
// console.log(percorso(["N", "N", "NE"]));
// console.log(percorso(["N", "S", "NE"]));

//ES 16

// function componi(f, g) {
//     return x => g(f(x));
// }

// var h = componi(x => 2 * x, x => 2 * x);
// console.log(h(3));

// var h = componi(s => s.length, x => x ** 2 + 1);
// console.log(h("Vincenzo"));

// var h = componi(a => a[0], s => s.length);
// console.log(h(["pere", "banane"]));

//ES 17

// function diff(d1, d2) {
//     let date1 = new Date(d1.anno + "/" + (d1.mese < 10 ? "0" + d1.mese : d1.mese) + "/" + d1.giorno);
//     let date2 = new Date(d2.anno + "/" + (d2.mese < 10 ? "0" + d2.mese : d2.mese) + "/" + d2.giorno);
//     return (date2.getTime() - date1.getTime()) / (1000 * 3600 * 24)
// }

// console.log(diff({ giorno: 1, mese: 1, anno: 2020 }, { giorno: 4, mese: 2, anno: 2020 }));
// console.log(diff({ giorno: 1, mese: 1, anno: 2019 }, { giorno: 1, mese: 1, anno: 2020 }));

//ES 18

// function leaveArr(t) {
//     if (t.figli === undefined) return [t.val];
//     let arr = [];
//     t.figli.forEach(el => {
//         arr.push(...leaveArr(el));
//     });
//     return arr;
// }

// function eredita(t) {
//     if (Object.keys(t).length == 0) return 0;
//     if (Object.keys(t).length == 1 && t.val !== undefined) return t.val;
//     let m = t.val === undefined || t.val === 0 ? 0 : t.val / t.figli.length;
//     t.figli.forEach(el => {
//         el.val += m;
//         if (el.figli !== undefined)
//             eredita(el);
//     });
//     return Math.max(...leaveArr(t));
// }

// console.log(eredita(t = { val: 16, figli: [{ val: 4 }, { val: 2, figli: [{ val: 8 }, { val: 2 }] }] }));
// console.log(eredita({ val: 5 }));

// function eredita(t) {
//     if (Object.keys(t).length === 0) return 0;
//     if (t.val != undefined && t.figli === undefined) return t.val;
//     let max = -Infinity;
//     let m = t.val == undefined || t.val == 0 ? 0 : t.val / t.figli.length;
//     t.figli.forEach(el => {
//         el.val += m;
//         max = Math.max(max, eredita(el));
//     });
//     return max;
// }

// console.log(eredita(t = { val: 16, figli: [{ val: 4 }, { val: 2, figli: [{ val: 8 }, { val: 2 }] }] }));
// console.log(eredita({ val: 5 }));

//ESERCIZIO RICORSIVO

// Realizzare una funzione ricorsiva 
// che controlli che un array di numeri interi positivi 
// dato in input sia ordinato in ordine strettamente crescente.

function ordinato(a) {
    if (a.length <= 1) return true;
    if (a.pop() < a[a.length - 1])
        return true && ordinato(a);
    else
        return false;
}