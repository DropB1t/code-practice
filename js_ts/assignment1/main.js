/*
Scrivere una funzione deframmenta(a), con a array di numeri. La funzione
restituisce una copia dell'array a da cui sono state eliminate le occorrenze
dei numeri quando queste non sono ripetute in sequenza (ovvero in posizioni
contigue dell'array) almeno una volta. 
Ad esempio  dato l'array a = [1,1,2,3,3,3,2,2,4] la chiamata deframmenta(a)
restituisce [1,1,3,3,3,2,2], dove gli elementi in posizione 2 e 4 sono stati
eliminati in quanto non ripetuti in sequenza. 

Esempi:
deframmenta([1,1,2,3,3,3,2,2,4]) -> [1,1,3,3,3,2,2] 
deframmenta([0,0,0,0,0,1,0,1,1]) -> [0,0,0,0,0,1,1]
deframmenta([1,0]) -> [] 
*/

function deframmenta(a){
    let arr = []
    for (let i = 0; i < a.length; i++) {
        if(a[i] == a[i+1] || a[i] == a[i-1])
            arr.push(a[i])
    }
    return arr
}

/* console.log(deframmenta([1,1,2,3,3,3,2,2,4]) )
console.log(deframmenta([0,0,0,0,0,1,0,1,1]))
console.log(deframmenta([1,0]))
 */
/*
Si scriva una funzione arrayfy(f) che, dato come argomento
una funzione f : X -> Y (con X e Y qualunque), restituisca
una funzione che, dato un array A con elementi di tipo X,
restituisca un nuovo array in cui ogni elemento x di A è sostituito
dal valore restituito da f(x).

Esempi:
Sia a=[5,2,4]. Allora arrayfy(x=>0)(a) restituisce [0,0,0]
Sia a=[5,2,4]. Allora arrayfy(x=>2*x)(a) restituisce [10,4,8]
Sia a="pippo".split(""). Allora arrayfy(c=>c=="p")(a) restituisce [true,false,true,true,false]
typeof arrayfy(x=>0) è "function"
*/


function arrayfy(f){
    return function t(a){
        return a.map((el)=>f(el))
    }
}
//console.log(typeof arrayfy(x=>0))
/* let a=[5,2,4]
console.log(arrayfy(x=>0)(a))
console.log(arrayfy(x=>2*x)(a))
let b="pippo".split("")
console.log(arrayfy(c=>c=="p")(b)) */

/*
Si scriva una funzione extend(a,b) che, dati due oggetti a e b,
restituisca un nuovo oggetto che ha tutte le proprietà di a e
tutte quelle di b. Se a e b hanno qualche proprietà con lo stesso nome,
prevale il valore contenuto in a.

Esempi:
Sia a= {nome: "pippo", età: 35} e b={nome: "andrea", altezza: 200}, allora: 
extend(a,b) restituisce {nome: "pippo", età: 35, altezza: 200}
extend(a,{}) restituisce {nome: "pippo", età: 35}
extend({},{}) restituisce {}
*/

function extend(a,b){
    let c = {}
    for(el in a){
        c[el] = a[el]
    }
    for(el in b){
        if(!(el in c))
            c[el] = b[el]
    }
    return c
}

/* let a= {nome: "pippo", età: 35}
let b={nome: "andrea", altezza: 200}
console.log(extend(a,b))
console.log(extend(a,{}))
console.log(extend({},{})) */

/*
Scrivere una funzione isSorted(a),
con a un array di numeri. La funzione restituisce true se
l'array e' ordinato in senso strettamente
crescente, e false altrimenti.

Esempi:
isSorted([-21,-2,0,4,6,210]) -> true
isSorted([2,6,8,8,9,21]) -> false
isSorted([2,6,8,9,10,-42]) -> false
 */

function isSorted(a){
    for (let i = 0; i < a.length-1; i++) {
        if(!(a[i]<a[i+1]))
            return false
    }
    return true
}

/* console.log(isSorted([-21,-2,0,4,6,210]))
console.log(isSorted([2,6,8,8,9,21]))
console.log(isSorted([2,6,8,9,10,-42])) */

/*
Si scriva una funzione angolo(d) che, ricevuto come argomento 
un oggetto della classe Date di JavaScript, restituisca l'angolo 
formato dalla lancetta delle ore e quella dei secondi di un tradizionale 
orologio analogico (con quadrante di 12 ore).
Le lancette formano due angoli: si restituisca il più piccolo, 
espresso come un numero positivo di gradi sessagesimali, e approssimato all'intero più vicino.
Si assuma che le lancette cambino posizione in maniera continua, ogni secondo.

Esempi:
angolo(new Date("2021-04-23 14:00:00")) -> 60
angolo(new Date("2021-04-23 14:15:15")) -> 23
angolo(new Date("2021-04-23 6:50:50")) -> 95
*/

function angolo(d){
    let h = d.getHours()
    let s = d.getSeconds()
    let calc = (h * 30 + s * 0.5) - (s * 6)
    return Math.ceil(Math.abs(Math.min(360-calc,calc)))
}
/* console.log(angolo(new Date("2021-04-23 14:00:00")))
console.log(angolo(new Date("2021-04-23 14:15:15")))
console.log(angolo(new Date("2021-04-23 6:50:50"))) */

/*
Si consideri un oggetto G che rappresenta una grammatica. 
Le proprietà di G hanno chiavi il cui nome consiste di una sola lettera maiuscola (A-Z); 
il valore di queste proprietà è una stringa qualunque.

Si scriva una funzione produci(G,s) che, data una grammatica G e una stringa qualunque s, 
restituisca la stringa ottenuta sostituendo ogni lettera maiuscola L contenuta in s con 
la corrispondente stringa associata a L in G, se esiste, o * altrimenti. 
Tutti i caratteri di G che non sono lettere maiuscole vengono lasciati intatti.
Si scriva poi una funzione multip(G,s,k) che ripete il processo 
di produci k volte (con k intero non negativo), espandendo ripetutamente la stringa prodotta.

Esempio:
Sia G= {A:"aAB", B:"bb", C:"33"}, allora:
produci(G,"CAz") -> "33aABz"
produci(G,"BAH!") -> "bbaAB*!"
multip(G,"CAz",3) -> "33aaaABbbbbz"
*/

function produci(G,s){
    let p = ""
    for (let i = 0; i < s.length; i++) {
        if(s[i] == s[i].toUpperCase() && /[a-zA-Z]/.test(s[i]))
            if(s[i] in G)
                p+=G[s[i]]
            else
                p+="*"
        else
            p+=s[i]
    }
    return p
}
function multip(G,s,k){
    let mp = produci(G,s)
    for (let i = 0; i < k-1; i++) {
        mp = produci(G,mp)
    }
    return mp
}

/* let G= {A:"aAB", B:"bb", C:"33"}
console.log(produci(G,"CAz"))
console.log(produci(G,"BAH!"))
console.log(multip(G,"CAz",3)) */

/*
Si scriva una funzione contali(C) che, ricevuto come argomento una classe C, 
restituisca una classe C2 identica a C ma con in più la capacità di 
contare quante istanze di oggetti della classe sono stati creati.
In particolare, C2 deve esporre una proprietà statica di sola lettura quanti, 
il cui valore sia il numero totale di istanze create fino a quel momento.

Esempio:
var A2 = contali(Array)
var S2 = contali(String)

var a=new A2()          // creo un primo "Array contato"
var b=new A2(4,7,21)    // creo e inizializzo un secondo "Array contato"
b.push(42)              // le istanze di A2 sono Array
var c=new S2("pippo")   // creo e inizializzo 1 "Stringa contata"
console.log(c.length)   // le istanze di S2 sono String

console.log(A2.quanti)  // stampa 2
console.log(S2.quanti)  // stampa 1

A2.quanti=0             // ignorato
console.log(A2.quanti)  // stampa ancora 2
console.log(a instanceof Array) // stampa true
*/

function contali(C){
    return class T extends C{
        static quanti = 0
        constructor(){
            super()
            T.quanti++
        }
        static quanti(){
            return this.quanti
        }
    }
}
var A2 = contali(Array)
var S2 = contali(String)
var a=new A2() 
var b=new A2(4,7,21) 
console.log(A2.quanti)
var c=new S2("pippo")
console.log(S2.quanti)
A2.quanti=0 
console.log(A2.quanti) 
console.log(a instanceof Array)