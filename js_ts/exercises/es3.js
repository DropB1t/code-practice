const util = require('util');

// function fattori(n) {
//     if (n == 0)
//         return [];
//     let arr = [];
//     for (let i = 1; i <= n; i++)
//         if (n % i == 0)
//             arr.push([i, n / i]);
//     return arr;
// }

// console.log(fattori(359))
// console.log(fattori(10))
// console.log(fattori(12))

// function ordnung(a) {

//     let compare = (a, b, i = 0) => {
//         if (a.length == i || b.length == i)
//             return a[i] == b[i] ? a.length >= b.length ? 1 : -1 : a[i] > b[i] ? 1 : -1
//         if (a[i] > b[i])
//             return 1
//         else if (a[i] < b[i])
//             return -1
//         return compare(a, b, i + 1)
//     }

//     a.forEach(el => { el.sort() });
//     a.sort(compare);
//     return a
// }

// console.log(ordnung([[5, 1, 3], [1, 5, 2, 7], [3, 4], [3]]))
// console.log(ordnung([[5, 7, 3], [8, 3, 5], [1], [5]]))


// function strafatto(n) {
//     let arr = [], flag;
//     for (let i = 2; i < n; i++) {
//         if (n % i == 0) {
//             flag = true
//             let temp = [...arr], el = [i, n / i].sort();
//             temp.forEach((v) => {
//                 if (v.length == 2)
//                     v.sort();
//                 if (v[0] == el[0] && v[1] == el[1])
//                     flag = false
//             })
//             if (flag) {
//                 arr.push([i, n / i])
//                 if (strafatto(n / i).length != 0)
//                     arr.push([i, ...strafatto(n / i).flat()])
//             }
//         }
//     }
//     return arr;
// }

//Versione del prof:
function divisors(n) {
    var r = []
    for (let d = 2; d < n; d++)
        if (n % d == 0) r.push(d)
    return r;
}

function strafatto0(n) {
    var r = []
    var ds = divisors(n);
    if (ds.length >= 1) {
        for (let i = 0; i < ds.length; i++) {
            var h = ds[i];
            var ts = strafatto0(n / h);
            for (let j = 0; j < ts.length; j++) {
                r.push([h, ...ts[j]]);
            }
        }
    } else {
        r.push(n > 1 ? [n] : []);
    }
    return r;
}

function strafatto(n) {

    function acmp(aa, bb) {
        if (aa.length != bb.length)
            return aa.length - bb.length
        else
            for (let h = 0; h < aa.length; h++)
                if (aa[h] != bb[h]) return aa[h] - bb[h]
        return 0;
    }

    var fs = strafatto0(n);
    for (d of fs)
        d.sort((a, b) => a - b)
    return fs.sort(acmp).filter((v, i, a) => (i > 0) ? acmp(a[i], a[i - 1]) != 0 : true)
}

console.log(strafatto(359));
console.log(strafatto(10));
console.log(strafatto(12));
console.log(JSON.stringify(strafatto(60)));

// function calc1([op, ...el]) {
//     return el.reduce((a, b) => op == "+" ? a + b : op == "-" ? a - b : op == "*" ? a * b : op == "/" ? a / b : a + b)
// }
// console.log(calc1(["+", 4, 5, 2]))
// console.log(calc1(["*", 4, 5, 2]))
// console.log(calc1(["/", 64, 2, 2, 4]))

// function calcT([op, ...el]) {
//     return el.reduce((a, b) => {
//         if (Array.isArray(a))
//             a = calcT(a);
//         if (Array.isArray(b))
//             b = calcT(b);
//         return op == "+" ? a + b : op == "-" ? a - b : op == "*" ? a * b : op == "/" ? a / b : a + b;
//     })
// }

// console.log(calcT(["+", 4, ["*", 3, 2]]))
// console.log(calcT(["*", 4, 5, 2]))
// console.log(calcT(["/", 64, ["*", ["+", 2, 2], 4]]))

// FUNZIONE MAGICA STO C**ZO function calcT([a, ...b]) { return a == "+" ? b.reduce((acc, v) => acc + calcT(v)) : a == "-" ? b.reduce((acc, v) => acc - calcT(v)) : a == "*" ? b.reduce((acc, v) => acc * calcT(v)) : a == "/" ? b.reduce((acc, v) => acc / calcT(v)) : a }

// String.prototype.titolo = function () {
//     return this.split('').join(' ').toUpperCase();
// }

// console.log("pippo".titolo())
// console.log(`3*2 fa ${3 * 2}`.titolo())
// var s = "Pluto"
// console.log(s.titolo())

// Math.rational = function (n) {
//     if (Number.isInteger(n))
//         return [n, 1];
//     let i = 1;
//     while (i != 100000000) {
//         if (Number.isInteger(n * i))
//             return [n * i, i];
//         i++;
//     }
//     return []
// }

// console.log(Math.rational(4.5))
// console.log(Math.rational(-3))
// console.log(Math.rational(3.1415))

// class PuntoCartesiano {
//     constructor(x, y) {
//         this.x = x;
//         this.y = y;
//     }

//     dist(q) {
//         return Math.sqrt((this.x - q.x) ** 2 + (this.y - q.y) ** 2);
//     }

//     translate(q) {
//         this.x += q;
//         this.y += q;
//     }

//     zero() {
//         this.x = 0;
//         this.y = 0;
//     }

//     [util.inspect.custom](_depth, _opts) {
//         return [this.x, this.y];
//     }
// }

// let p = new PuntoCartesiano(2, 2)

// console.log(p);
// console.log(p.dist(new PuntoCartesiano(2, 0)));
// p.translate(2)
// console.log(p);
// p.zero()
// console.log(p);

// class Studente {
//     constructor(nome, mat, laurea = "", anno = undefined) {
//         this.nome = nome
//         this.matricola = mat
//         this.laurea = laurea
//         this.anno = anno
//         this.carriera = []
//     }

//     passato(esame) {
//         this.carriera.push(esame);
//     }

//     media() {
//         if (this.carriera.length == 0)
//             return undefined
//         let a = 0, b = 0;
//         this.carriera.forEach((el) => a += (el.lode ? 32 * el.cfu : el.voto * el.cfu));
//         this.carriera.forEach((el) => b += el.cfu);
//         return Math.floor(a / b)
//     }

//     [util.inspect.custom](_depth, _opts) {
//         return `Nome Cognome: ${this.nome}\nNumero Matricola: ${this.matricola}\n${this.laurea ? "Corso di Laurea:" + this.laurea + "\n" : ""}${this.anno ? "Anno:" + this.anno + "\n" : ""}`;
//     }
// }

// let studente = new Studente("Yuriy Rymarchuk", 56345)
// console.log(studente);
// studente.passato({ materia: "FdI", cfu: 9, voto: 28, lode: false })
// studente.passato({ materia: "Laboratorio", cfu: 12, voto: 30, lode: true })
// console.log(studente.carriera)
// console.log(studente.media())