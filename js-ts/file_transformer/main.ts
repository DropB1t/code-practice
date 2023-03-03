import * as fs from 'fs';

interface Dictionary {
    [Key: string]: number
}

class FileTransformer {
    protected f_name: string
    protected d_words:Dictionary
    constructor(name:string){
        this.f_name = name
        this.d_words = this.dictionary()
    }

    private dictionary():Dictionary{
        let obj:Dictionary = {}
        try {
            const data = fs.readFileSync(this.f_name, 'utf8')
            let worlds : Array<string> = (data.match(/[^\n\r?!.,"';: ]+/g) || [])
            //console.table(worlds)
            worlds.forEach((el)=> el in obj ? obj[el]++ : obj[el] = 1);
        } catch (err) {
            //console.error(err)
        }
        return obj
    }

    contaParole():number{
        if(!fs.existsSync(this.f_name))
            return -1;
        try {
            let count = 0
            for(let el in this.d_words){
                count += this.d_words[el]
            }
            return count
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    contaSpazi():number{
        if(!fs.existsSync(this.f_name))
            return -1;
        try {
            const data = fs.readFileSync(this.f_name, 'utf8')
            let exp = /[ ]/g
            let matches :number = (data.match(exp) || []).length
            return matches
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    contaRighe():number{
        if(!fs.existsSync(this.f_name))
            return -1
        try {
            const data = fs.readFileSync(this.f_name, 'utf8')
            let matches : Array<string> = data.split("\n")
            return matches.length
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    occorrenze(s:string):number{
        if(!fs.existsSync(this.f_name))
            return -1
        return s in this.d_words ? this.d_words[s] : 0
    }
    cifra(n:number):number{
        if(!fs.existsSync(this.f_name))
            return -1
        try {
            let buffer = [...fs.readFileSync(this.f_name,'utf8')],count = 0
            for(let i = 0; i<buffer.length;i++){
                buffer[i] = String.fromCharCode((buffer[i].charCodeAt(0)+n)%1024)
                count++
            }
            fs.writeFileSync(this.f_name+"-"+n+".cy", buffer.join(''), 'utf8')
            return count
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    decifra(f:string):number{
        if(!fs.existsSync(f))
            return -1
        let n = Number(f.slice(f.lastIndexOf('-')+1,-3))
        try {
            let buffer = [...fs.readFileSync(f,'utf8')],count = 0
            for(let i = 0; i<buffer.length;i++){
                buffer[i] = String.fromCharCode((buffer[i].charCodeAt(0)-n)%1024)
                count++
            }
            fs.writeFileSync(f+".decy", buffer.join(''), 'utf8')
            return count
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    comprimi():number{
        if(!fs.existsSync(this.f_name))
            return -1
        try {
            let buffer = [...fs.readFileSync(this.f_name,'utf8')],count = 1, str = ""
            for(let i = 0; i<buffer.length;i++){
                if(buffer[i] != buffer[i+1]){
                    str += (count == 1 ? "" : count) + buffer[i]
                    count = 0
                }
                count++
            }
            fs.writeFileSync(this.f_name+".comp", str, 'utf8')
            return str.length
        } catch (err) {
            console.error(err)
            return -1
        }
    }
    decomprimi(f:string):number{
        if(!fs.existsSync(f))
            return -1
        try {
            let buffer = [...fs.readFileSync(f,'utf8')],count = 1, num = "", str = ""
            for(let i = 0; i<buffer.length;i++){
                if(!"1234567890".includes(buffer[i])){
                    if(num.length==0)
                        str += buffer[i]
                    else
                        str += buffer[i].repeat(Number(num))
                    num = ""
                }else
                    num  += buffer[i]
            }
            fs.writeFileSync(f.slice(0,-5)+".decomp", str, 'utf8')
            return str.length
        } catch (err) {
            console.error(err)
            return -1
        }
    }
}

let ft = new FileTransformer("cantoInf1.txt")
console.log(ft.contaParole()); //1004
console.log(ft.contaSpazi()); //924
console.log(ft.contaRighe()); //144
console.log(ft.occorrenze("fui")); //4
console.log(ft.occorrenze("sono")); //0
console.log(ft.cifra(5)); //5149
console.log(ft.decifra("cantoInf1.txt-5.cy")); //5149
console.log(ft.cifra(256)); //5149
console.log(ft.decifra("cantoInf1.txt-256.cy")); //5149

let ft1 = new FileTransformer("testC.txt")
console.log(ft1.contaParole()); //2
console.log(ft1.contaSpazi()); //10
console.log(ft1.contaRighe()); //9
console.log(ft1.comprimi()); //37
console.log(ft1.decomprimi("testC.txt.comp")); //75