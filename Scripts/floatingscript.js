const amp = 100;

const freq = 1;

function tick()
{
    var y = amp*Math.sin(time*freq*Math.PI*2);

    move(0,y,0);
}

function run()
{

}

function move(x,y,z)
{
    cObject.move(x,y,z);
}
