/*impri("this number is", %d, num3: Error */


max = ent + 9 :

escan(c, &ch1) :


/*input*/
escan(%d, &num1) : 
escan(%f, &num2) : 
escan(%c, &ch1) : 
escan(%s, &str) :

/*output*/
impri("hi") : 
impri(%d, num1) : 
impri("this number is", %d, num3) : 
impri("hi", %d, %c, num2, ch2) :

/*Declaration*/
bool found:
flot num3:
ent num2:
cuer str:
doble n:
perso ch:
ent age, year, month:
ent age=7:
ent age,year = 0, day:
ent day, age = year+1:

/* Assignment */
val = 7:
temp = 45.54:
ch = 'c':
str = "hello":
a = x + 3:
d = a + b + 2:

/* Iterative */
por(a = 0; a<5; a++){
    impri("this number is", %d, val):
    val = 0:
}

por(i=0; i<10; i++){
    por(j=0; j<10; j++){
        impri("this number is", %d, num3):
    }
}


/* Conditional*/
si(num == num2){
    impri("two numbers are equal"):
}

si(num1>num2 y num1>num3){
    impri("num1 max"):
}mas{
    impri("num1 min"):
}

si(num <= 100 y
num <=0){
    si(num > 5){
        impri("hi"):
    }mas{
        impri("hello"):
    }
}mas{
    impri("hi"):
}
