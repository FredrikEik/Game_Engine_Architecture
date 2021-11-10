function addition(a, b)
{
    return a + b;
}

function callCFunction()
{
    var x = 5;
    var y = 0.5;
    var z = 5;
    cObject.scriptFunction(x, y, z);
    x = -4;
    y = 0.5;
    z = 0;
    cObject.scriptFunction(x, y, z);
    x = 1;
    y = 0.5;
    z = -3;
    cObject.scriptFunction(x, y, z);
    return x;
}