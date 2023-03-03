
/*
Scrivere una funzione Typescript "unique(a, equals)" che utilizza generics.
La funzione prende come parametro un array "a" di oggetti di tipo "T" e una funzione "equals(T,T)"
che confronta due oggetti di tipo T, restituendo true se sono uguali. La funzione "unique"
restituisce un nuovo array contenente tutti gli elementi unici di "a" (quindi senza duplicati),
nell'ordine in cui sono in "a". Nota: la funzione equals non va implementata, viene specificata
dall'utilizzatore della funzione.


Esempio:
unique([3,5,3,2,8,5],((x,y)=>x==y)) -> [3,5,2,8]
unique([0,0,0,0,0,0,0],((x,y)=>x==y)) -> [ 0 ]
unique(['hello','hello', 'how','are','you'],((x,y)=>x==y)) -> [ 'hello', 'how', 'are', 'you' ]
*/

function unique<T>(a:T[], equals:(v1:T,v2:T)=>boolean):T[]{
    let arr:T[] = []
    for (let i = 0; i < a.length; i++) {
        if(!arr.some((el)=>equals(a[i],el))){
            arr.push(a[i])
        }
    }
    return arr
}

console.log(unique([3,5,3,2,8,5],((x,y)=>x==y)))
console.log(unique([0,0,0,0,0,0,0],((x,y)=>x==y)))
console.log(unique(['hello','hello', 'how','are','you'],((x,y)=>x==y)))

function f(v:string,v1?:string) {
    if(v1 == undefined)
        return v1
    return v + v1;
}

/*
Scrivere in Typescript una classe Car che rappresenta un automobile. La classe ha le seguenti prorietà:

- fuelType, rappresenta il tipo di carburante, di tipo Fuel definito
dal seguente enum (che trovate anche nel REPL):

enum Fuel {Diesel, Gasoline, Electric};

- fuelCapacity, di tipo numerico, rappresenta la capacità massima del serbatoio o batteria; 
- consumption, di tipo numerico, rappresenta il consumo medio, in unità di carburante per km;
- fuelLevel, di tipo numerico, rappresenta il livello del carburante 
rimanente nel serbatoio, o il livello della batteria. Prende valori tra 0 e fuelCapacity. 

La classe contiene un costruttore che prende come parametri, 
nell'ordine: fuelType, fuelCapacity, consumption e fuelLevel. 
I primi tre parametri sono obligatori, invece fuelLevel può mancare e ha 0 come valore di default.

La classe implementa anche i seguenti metodi (usate ESATTAMENTE i nomi indicati):
- refillFuel(f) - prende come parametro una quantità 
di carburante/energia f e aggiorna la proprietà fuelLevel, 
incrementandola del valore f; attenzione a rispettare 
la capacità massima del sebatoio/batteria (specificata dalla proprietà fuelCapacity): 
se la capacità è superata, fuelLevel viene settato a fuelCapacity. 

Il parametro f è opzionale: se non viene specificato il metodo fa il 'pieno', 
portando il valore di fuelLevel al valore massimo possibile (cioè alla capcità massima). 
Il metodo restituise il nuovo livello di carburante/energia.

- drive (km) - prende come parametro il numero di km da percorrere, e aggiorna 
fuelLevel considerando il consumo medio; la funzione restituisce il nuovo valore di fuelLevel. 
Se il livello di carburante non è sufficiente per completare il viaggio, si lancia un'eccezione 
di tipo NoFuelException. In caso di eccezione il livello del carburante non viene modificato.

Esempio:
let c1=new Car(Fuel.Diesel,30,0.5) -> 
Car { fuelType: 0, fuelCapacity: 30, consumption: 0.5, fuelLevel: 0 }

c1.drive(10) -> lancia eccezione NoFuelException();

c1.refillFuel(20); ->20

c1.drive(10); -> 15

c1.drive(30);-> 0

c1.refillFuel(); ->30
*/

enum Fuel {Diesel, Gasoline, Electric};

class NoFuelException extends Error {
    constructor(message:string = "Il livello di carburante non è sufficiente per completare il viaggio") {
        super(message);
        this.name = "NoFuelException";
    }
}

class Car{
    fuelType:Fuel
    fuelCapacity:number
    consumption:number
    fuelLevel:number
    constructor(fuelType:Fuel,fuelCapacity:number,consumption:number,fuelLevel:number=0){
        this.fuelType = fuelType
        this.fuelCapacity = fuelCapacity
        this.consumption = consumption
        this.fuelLevel = fuelLevel
    }

    refillFuel(f:number=this.fuelCapacity):number{
        if(this.fuelLevel+f >= this.fuelCapacity)
            this.fuelLevel = this.fuelCapacity
        else
            this.fuelLevel = this.fuelLevel + f
        return this.fuelLevel
    }
    drive (km:number):number{
        let consum = km * this.consumption
        if(consum > this.fuelLevel)
            throw new NoFuelException()
        this.fuelLevel -= consum

        return this.fuelLevel
    }

}

let c1=new Car(Fuel.Diesel,30,0.5)
console.log(c1)
//console.log(c1.drive(10))
console.log(c1.refillFuel(20))
console.log(c1.drive(10))
console.log(c1.drive(30))
console.log(c1.refillFuel())