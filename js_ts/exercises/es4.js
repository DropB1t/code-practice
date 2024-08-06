const prompt = require('prompt-sync')();

// 1) Funzione che trasforma da una base all’altra

// const inBase = (num, b) => {
//     if (num == 0) return 0;
//     else {
//         inBase(Math.floor(num / b), b);
//         process.stdout.write((num % b).toString());
//     }
// }
// inBase(Number(prompt("Inserisci un numero > ")), Number(prompt("Inserisci la base in cui vorresti rappresentare il numero > ")));

// 2) Funzione che stampa la rappresentazione in complemento a 2 per un numero intero

// const binaryNumArray = (a, num) => {
//     if (num == 0) return a;
//     else {
//         a.push((num % 2));
//         binaryNumArray(a, Math.floor(num / 2));
//     }
//     return a;
// }

// const c2 = a => {
//     for (let i = 0; i < a.length; i++) {
//         a[i] = a[i] == 1 ? 0 : 1;
//     }
//     if (a[0] == 0) {
//         a[0] = 1;
//         return a;
//     }
//     for (let i = 0; i < a.length; i++) {
//         if (a[i] == 1) {
//             a[i] = 0;
//         }
//         else if (i == a.length - 1 && a[i] == 1) {
//             a[i] = 0;
//             a.push(1)
//             return a;
//         }
//         else {
//             a[i] = 1;
//             return a;
//         }
//     }
// }

// let num = [], n = Number(prompt("Inserisci un numero intero > "));;

// num = binaryNumArray(num, Math.abs(n));
// if (n < 0) {
//     num = c2(num);
// }
// console.log(...num.reverse());

// 3) Dati due interi a e b, rimuovere da a i bit settati a 1 in b

// const binaryNumArray = (a, num) => {
//     if (num == 0) return a;
//     else {
//         a.push((num % 2));
//         binaryNumArray(a, Math.floor(num / 2));
//     }
//     return a;
// }

// let a = [], b = [];
// a = binaryNumArray(a, 5667);
// b = binaryNumArray(b, 3);

// console.log(...a.reverse());
// console.log(...b.reverse());

// for (let i = 0; i < a.length; i++) {
//     if (b[i] != null && b[i] == 1) {
//         a[i] = 0;
//     }
// }

// console.log(...a);

// 4) Contare i bit settati a 1 in un numero a

// const binaryNumArray = (a, num) => {
//     if (num == 0) return a;
//     else {
//         a.push((num % 2));
//         binaryNumArray(a, Math.floor(num / 2));
//     }
//     return a;
// }

// let num = [], n = Number(prompt("Inserisci un numero > ")), sum = 0;
// num = binaryNumArray(num, Math.abs(n));

// num.forEach(a => {
//     if (a == 1)
//         sum++;
// });
// console.log(...num.reverse());
// console.log("Numero di bit settati a 1 > " + sum);

// 5) Scambiare i valori di due variabili a e b utilizzando lo XOR

// let xorChange = (a, b) => {
//     a = a ^ b;
//     b = b ^ a;
//     a = a ^ b;
//     return [a, b];
// }

// let a = 65, b = 52;

// console.log(`A = ${a} B = ${b}`);
// n = xorChange(a, b);
// console.log(`A = ${n[0]} B = ${n[1]}`);

// 6) Programma che legge una bitmap 8×8 e restituisce un array di 8 interi a 8 bit che la codifica

let toInteger = r => {
    let n = 0;
    r.reverse();
    for (let i = 0; i < r.length; i++) {
        if (r[i] == 1)
            n += 2 ** i;
    }
    r.reverse();
    return n;
}

let processBitMap = map => {
    let a = [];
    bitmap.forEach(integer => {
        a.push(toInteger(integer));
    });
    return a;
}

let bitmap =
    [[1, 0, 1, 0, 1, 1, 0, 0],
    [1, 0, 0, 1, 1, 1, 0, 0],
    [0, 0, 1, 0, 1, 1, 0, 1],
    [0, 1, 1, 0, 1, 0, 0, 0],
    [1, 1, 1, 0, 0, 1, 0, 1],
    [1, 0, 1, 0, 0, 1, 1, 1],
    [1, 1, 1, 0, 0, 0, 1, 0],
    [0, 0, 1, 1, 1, 0, 0, 0]];

let a = [];
a = processBitMap(bitmap);

bitmap.forEach(element => {
    console.log(...element);
});
console.log("\nBitmap processato :");
console.log(...a);


// 7) Ricerca lineare - funzione che cerca un elemento in un array e restituisce la sua posizione

// let sequentialSearch = (a, k) => {
//     for (let i = 0; i < a.length; i++) {
//         if (a[i] == k)
//             return i;
//     }
//     return -1;
// }

// let a = [4, 5, 6, 7, 23, 54, 76, 45, 7, 98];
// let num = Number(prompt("Inserisci il numero da cercare > "));
// let position = sequentialSearch(a, num);
// if (position != -1)
//     console.log("Il numero si trova nella posizione " + position);
// else
//     console.log("Il numero non si trova nel vettore");


// 8) Ricerca binaria - funzione che cerca un elemento in un array ordinato e restituisce la sua posizione

// let binarySearch = (a, k) => {
//     let inf = 0, sup = a.length - 1;
//     while (inf <= sup) {
//         let center = Math.floor((inf + sup) / 2);
//         if (a[center] == k)
//             return center;
//         else if (k > a[center])
//             inf = center + 1;
//         else if (k < a[center])
//             sup = center - 1;
//     }

//     return -1;
// }

// let a = [4, 5, 9, 15, 23, 37, 54, 64];
// let num = Number(prompt("Inserisci il numero da cercare > "));
// let position = binarySearch(a, num);

// if (position != -1)
//     console.log("Il numero si trova nella posizione " + position);
// else
//     console.log("Il numero non si trova nel vettore");