interface Shape{
    x:number;
    y:number;
    area():number; 
    getName(): string;
    translate(x:number, y:number): void;
  }

class Circle implements Shape{
    x: number;
    y: number;
    r:number
    constructor(x:number,y:number,r:number){
        this.x = x
        this.y = y
        this.r = r > 0 ? r : 1
    }

    area(): number {
        return Math.PI*(this.r**2)
    }
    getName(): string {
        return this.constructor.name
    }
    translate(x: number, y: number): void {
        this.x = x
        this.y = y
    }
    
}
class Square implements Shape{
    x: number;
    y: number;
    l:number
    constructor(x:number,y:number,l:number){
        this.x = x
        this.y = y
        this.l = l > 0 ? l : 1
    }

    area(): number {
        return this.l**2
    }
    getName(): string {
        return this.constructor.name
    }
    translate(x: number, y: number): void {
        this.x = x
        this.y = y
    }
    
}

// let c1 = new Circle(0,0,2)
// console.log(c1)
// console.log(c1.area())
// console.log(c1.translate(1,-0.5))
// console.log(c1)
// console.log(c1.getName())

// let c2 = new Circle(10,1,0.5)
// console.log(c2)
// console.log(c2.area())
// console.log(c2.translate(100,-100))
// console.log(c2)

// let q1 = new Square(1,0,10)
// console.log(q1)
// console.log(q1.area())
// console.log(q1.translate(-4.8,-3.5))
// console.log(q1)
// console.log(q1.getName())

// let q2 = new Square(0,0,0)
// console.log(q2)
// console.log(q2.area())
// console.log(q2.translate(1,1))
// console.log(q2)



function sommaArray<T>(a:T[], somma:(v1:T,v2:T)=>T):T{
    if(a.length < 2)
        throw new Error("Non ci sono abbatanza elementi nel vettore sui quali eseguire la funzione somma")
    let v:T = somma(a[0],a[1])
    for(let i = 2; i<a.length;i++)
        v = somma(v,a[i])
    return v
}

// function sumNumbers (x:number,y:number):number{return x+y}
// console.log(sommaArray([3,6,4,2],sumNumbers))

//console.log(sommaArray([3,6,4,"34"],sumNumbers))

// function sumStrings (x:string,y:string):string{return x+" "+y}
// console.log(sommaArray(["o","che","bel","castello"],sumStrings))

import * as fs from 'fs';
class Studente {
    nome:string
    cognome:string
    citta:string
    votoLaurea:number
    constructor(nome:string,cognome:string,citta:string,votoLaurea:number){
        this.nome = nome
        this.cognome = cognome
        this.citta = citta
        this.votoLaurea= votoLaurea
    }
}

function leggiStudenti(file:string):Array<Studente>{
    if(!fs.existsSync(file)){return []}
    try {
        const data = fs.readFileSync(file, 'utf8')
        let matches:string[] = data.split("\n")
        let arr:Array<Studente> = matches.map((el)=>{
            let student = el.split(',')
            return new Studente(student[0]?student[0]:"",student[1]?student[1]:"",student[2]?student[2]:"",student[3] ? Number(student[3]):NaN)
        })
        return arr
    } catch (err) {
        console.error(err)
        return []
    }
}

// console.log(leggiStudenti("studenti1.txt"))
// console.log(leggiStudenti("studenti2.txt"))

class EccezioneDisponibilità extends Error {
    constructor(message:string = "Se la disponibilità residua è inferiore alla richiesta") {
        super(message);
        this.name = "EccezioneDisponibilità";
    }
}

class Libro{
    titolo:string
    isbn:string
    autore:string
    disponibilità:number
    constructor(titolo:string,isbn:string,autore:string,disponibilità:number=0){
        this.titolo = titolo
        this.isbn = isbn
        this.autore = autore
        this.disponibilità = disponibilità
    }
    sell(ncopie:number=1):void{
        if(ncopie<=0)
            return
        if(this.disponibilità < ncopie)
            throw new EccezioneDisponibilità()
        this.disponibilità -= ncopie
    }
    stock(ncopie:number=1):void{
        if(ncopie<=0)
            return
        this.disponibilità += ncopie
        return
    }
}

// let l:Libro = new Libro("Il buio oltre la siepe","978-8807881558","Harper Lee");
// console.log(l.stock(10))
// console.log(l)
// console.log(l.sell(11))


// let l1:Libro = new Libro("Fondazione","978-8804729198","Isaac Asimov",100)
// console.log(l1.sell(90))
// console.log(l1)
// console.log(l1.stock(2000))
// console.log(l1)
// console.log(l1.sell(2010))
// console.log(l1)



class Graph {
    n:number
    edges:Array<Array<number>>
    constructor(edges:Array<Array<number>>){
        this.n = edges.length
        this.edges = edges
    }

    complement():Graph{
        let arr:Array<Array<number>> = []
        for(let i = 0; i<this.edges.length;i++){
            let adj:number[] = []
            for(let j=0;j<this.edges.length;j++)
                if(!this.edges[i].includes(j))
                    adj.push(j)
            arr[i] = adj.sort()
        }
        return new Graph(arr)
    }

    getSecondNeighbours(idx:number):number[]{
        if(idx >= this.edges.length)
            return []
        let arr:number[] = []
        for(let i = 0; i<this.edges[idx].length;i++){
            for(let j=0;j<this.edges[this.edges[idx][i]].length;j++)
                if(!arr.includes(this.edges[this.edges[idx][i]][j]))
                    arr.push(this.edges[this.edges[idx][i]][j])
        }
        return arr.sort()
    }

}

// let g:Graph=new Graph([[0,3],[1,2,3],[4],[],[0,1,2]]);
// console.log(g)
// console.log(g.complement())
// console.log(g.getSecondNeighbours(0))
// console.log(g.getSecondNeighbours(1))
// console.log(g.getSecondNeighbours(2))
// console.log(g.getSecondNeighbours(3))
// console.log(g.getSecondNeighbours(4))

// let g1=new Graph([[],[],[]])
// console.log(g1)
// console.log(g1.complement())
// console.log(g1.getSecondNeighbours(2))



class GenericFileToArray<T>{
    arr: T[]
    constructor(nomeFile:string,parse:(s:string)=>T){
        const data = fs.readFileSync(nomeFile, 'utf8')
        let matches:string[] = data.split("\n")
        let arr:T[] = matches.map((el)=>parse(el))
        this.arr = arr
    }
    toString(){
        
    }
    cerca(o:T,equals:(a:T,b:T)=>T){

    }
}