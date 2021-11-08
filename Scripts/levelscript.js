

function callSpawnObject(objectName, x, y, z)
{
    cObject.spawnObjectAtLocation(objectName, x, y, z);
}

function spawnObjectsFromLevel()
{
    const testLevel = new Map([["Cube",     1, 0, 1],
                           ["Triangle", 2, 0, 2],
                           ["Plane",    3, 0, 3]
                          ]);
    for(let i = 0; i < testLevel.size(); i++)
    {
        callSpawnObject(testLevel[i]);
    }
}

