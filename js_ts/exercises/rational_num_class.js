const util = require('util');

class Rational {
    constructor(num, denom = 1) {
        if (denom == 0) {
            return undefined
        }
        [this.num, this.denom] = mcd(num, denom)
    }

    plus(rational) {
        let num = (this.num / this.denom) + (rational.num / rational.denom)
        if (this.denom >= rational.denom)
            return operation_output(this.denom, num)
        else
            return operation_output(rational.denom, num)
    }

    minus(rational) {
        let num = (this.num / this.denom) - (rational.num / rational.denom)
        if (this.denom >= rational.denom)
            return operation_output(this.denom, num)
        else
            return operation_output(rational.denom, num)
    }

    times(rational) {
        let num = (this.num / this.denom) * (rational.num / rational.denom)
        if (this.denom >= rational.denom)
            return operation_output(this.denom, num)
        else
            return operation_output(rational.denom, num)
    }

    div(rational) {
        let num = (this.num / this.denom) / (rational.num / rational.denom)
        if (this.denom >= rational.denom)
            return operation_output(this.denom, num)
        else
            return operation_output(rational.denom, num)
    }

    [util.inspect.custom](_depth, _opts) {
        return this.num + "/" + this.denom;
    }
}
Rational.prototype.valueOf = function () {
    return this.num / this.denom;
};

function operation_output(denom, num) {
    for (let i = 2; i <= Math.abs(denom); i++)
        if (Number.isInteger(num * i))
            return (num * i) + "/" + i
}

function mcd(num, denom) {
    let mcd = []
    if (num > denom)
        mcd = find_dividers(num, denom)
    else
        mcd = find_dividers(denom, num)
    if (mcd.length != 0) {
        mcd.sort()
        num = num / mcd[mcd.length - 1]
        denom = denom / mcd[mcd.length - 1]
    }
    return [num, denom]
}

function find_dividers(a, b) {
    let arr = []
    for (let i = 2; i <= Math.abs(a); i++)
        if (a % i == 0 && b % i == 0)
            arr.push(i)
    return arr
}

let a = new Rational(3, 4)
console.log(a)
var b = new Rational(7, 8)
console.log(b)
console.log(a.plus(b))
console.log(a.minus(b))
console.log(b.minus(a))
console.log(b.times(new Rational(2, 1)))
console.log(a.div(b))
console.log(new Rational(31415, 10000))

console.log(1 + new Rational(1, 3))