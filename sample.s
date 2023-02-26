/* Input (1/1)*/

escan(%d, &num1):
escan(%f, &num2):
escan(%c, &ch1):
escan(%s ,&str):

/*error (1/1)*/
escan(%d &txt):
escan(%d, txt):
escan(%d, &txt :
escan(%d, &txt)



/* Output (3/3)*/
impri("Hello Word!"):
impri(%d,num1):
impri("Welcome! ",%s,name):


/*error (3/3)*/
impri("idk") 
impri(%d, sum:
impri("A Cup of Coffee",%s name):



/* Declaration (3/3)*/
ent prod = 7:
perso ch ='c':
flot sum = 500.34:
cuer name = firstname:
ent age, year, month:


/*error (3/3)*/
ent prod = :
perso ch 'c':
flot = 500.34:
cuer name = firstname
ent age; year, month:


/* Assignment (3/3)*/
val = 7:
b = a:
total = como + numero:
d = ((a*b)/80)+95**2:
c = (a+d+6)/3:

/*error (3/3)*/
val 7:
b = a
total = como numero:
d = ((a*b)/80)+95**2
c = (a+d+6/3:


/* Iterative (2/2)*/
por(ent a = 0; a<5; a++){
    impri("This number is", %d, val):
    count = b+a:
}

por(ent i=0; i<10; i++){
    por(ent j=0; j<10; j++){
        impri("omg 1111"):
    }
}

/*error (2/2)*/
por(ent a = 0 a<5; a++){
    impri("two numbers are equal"):
}

por(ent i=0; i<10; i){
    por(ent j=0; j<10; j++){
        impri("omg 1111"):
    }
}


/* Conditional (3/3)*/
/* if */
si(num1 == num2){
    impri("Two numbers are equal"):
}

/* if else */
si(num1>num2 y num1>num3){
    impri("Max: num1"):
}mas{
    impri("Min: num1"):
}

/*nested if else*/
si(no((num <= 45 y num > 0) ni num == 1)){
    si(num > 5){
        impri("hi"):
    }mas{
        impri("hello"):
    }
}mas{
    impri("hi"):
}

/*error (3/3)*/
si(num == num2)
    impri("two numbers are equal"):
}

si(num1>num2 y num1 num3){
    impri("Max: num1"):
}mas{
    impri("Min: num1"):
}


si(no((num <= 45 y num > 0) ni num == )){
    si(num > ){
        impri("hi"):
    }mas{
        impri("hello"):
    }
}mas{
    impri("hi"):
}
