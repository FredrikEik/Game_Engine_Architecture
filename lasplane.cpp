#include "lasplane.h"
#include "Shaders/shader.h"
#include <map>

LasPlane::LasPlane(std::string filename)
    :VisualObject()
{
    filepath = filename;
    //qDebug() << "basfasdfsdgf!";
}

//void LasPlane::constructPlane()
//{
//    if(!m_heightMap)
//        qDebug() << "ERROR: Height map does not exist!";

    //Create vertex positions
//    int bytesize = m_heightMap->bytesPrPixel();
//    for(int i{0}; i<rows; i++) //Z-axis
//    {
//        for(int j{0}; j<columns; j++) //X-axis
//        {
//            height = m_heightMap->bitmap()[((i*rows)+j)*bytesize]/25.f; //Retrieves every 3rd rgb value.


//            //qDebug() << "X: " << j << "Height: " << height << "Z: " << i;
//            tempPosition.push_back(QVector3D(j, height, i));
//        }
//    }

//    qDebug() << "Map vertices created!";

//    QVector3D tempPos{}, tempNorm{};


//    //Calcualte vertex normals
//    for(int i{0}; i<rows; i++)
//    {
//        for(int j{0}; j<columns; j++)
//        {
//            tempPos = tempPosition.at((i*columns)+j);
//            tempNorm = calculateNormal(tempPosition.at((i*columns)+j), (i*columns)+j);
//            m_vertices.push_back(Vertex(tempPos.x(), tempPos.y(), tempPos.z(), tempNorm.x(), tempNorm.y(), tempNorm.z()));
//        }
//    }

//    qDebug() << "Map normals calculated!";

//    //Create triangles
//    for(int i{0}; i<rows-1; i++)
//    {
//        for(int j{0}; j<columns-1; j++)
//        {
//            //Bottom triangle
//            m_indices.push_back(j+(i*columns));
//            m_indices.push_back(j+1+(i*columns));
//            m_indices.push_back(j+((i+1)*columns)+1);

//            //Top triangle
//            m_indices.push_back(j+((i+1)*columns)+1);
//            m_indices.push_back(j+((i+1)*columns));
//            m_indices.push_back(j+(i*columns));
//        }
//    }

//    qDebug() << "Map triangles drawn!";

//    tempPosition.clear();
//}

void LasPlane::constructPlane()
{
    qDebug() << "Started constructing plane!";

    std::vector<QVector3D> tempLasPos{},  positions{};
    QVector3D max{}, min{}, tempPos{}, tempNormal{};
    float resoluton{10}, temp{};

    readFile(filepath, max, min, tempLasPos);

    qDebug() << "File read!";

    //std::vector<int> test(4, 2000);

    //qDebug()<<min;
    //qDebug()<<max;

    //first == number of points in square, second == total height.
//    std::vector<std::pair<int, float>> &squareHeights = (*new std::vector<std::pair<int, float>>);//(std::pair<int, float>(0,0), (int)((max.z() * max.x()) / resoluton));
    std::map<int, std::pair<int, float>> &squareHeights = (*new std::map<int, std::pair<int, float>>);//(std::pair<int, float>(0,0), (int)((max.z() * max.x()) / resoluton));

    long long squarePosition{};
//    squareHeights.resize((((max.z()-min.z())/resoluton) * ((max.x()-min.z())/resoluton)));
//    squareHeights.reserve((max.z() * max.x()) / resoluton);
//    ((max.z() * max.x()) / resoluton);


    for(auto &it : tempLasPos)
    {
//        tempCollum = (max.x() - it.x()) / resoluton;
//        tempRow = (max.z() - it.z()) / resoluton;
//        squarePosition = (it.x() * it.z()) / resoluton;
        long long positionX = (it.x()-min.x())/resoluton;
//        positionX = positionX >= 0 ? positionX : 0;

        long long positionZ = (it.z()-min.z())/resoluton;
//        positionZ = positionZ >= 0 ? positionZ : 0;
        squarePosition = std::lround(positionX*positionZ);

        if(squareHeights.find(squarePosition) == squareHeights.end())
        {
            squareHeights.insert(std::pair<int, std::pair<int, float>>(squarePosition, {1, it.y()}));

        }
        else
        {
            squareHeights.at(squarePosition).second += it.y();
            squareHeights.at(squarePosition).first++;
        }
    }

    m_vertices.reserve(squareHeights.size());

    int columnSize = std::sqrt(squareHeights.size());
    int columns{};
    int rows{};
    for(const auto& it: squareHeights)
    {
//        if(squareHeights.at(i).first == 0)
//            continue;
        if(columns >= columnSize)
        {
            rows++;
            columns = 0;
        }
        temp = it.second.second / (float)it.second.first;
//        tempPos = QVector3D(tempLasPos.at(i).x(), temp, tempLasPos.at(i).z());
//        tempPos = QVector3D(rows*resoluton, temp-min.y(), columns*resoluton);
        tempPos = QVector3D(rows*resoluton, temp - min.y(), columns*resoluton);
        positions.push_back(tempPos);

        tempNormal = QVector3D(0,1,0);

        m_vertices.push_back(Vertex(tempPos, tempNormal));

        columns++;
    }

    columns=0;
    rows=1;
    for(int i{0}; i < (long long)(positions.size() - columnSize - 1); i++)
    {
        if(((i+1) % (columnSize*rows)) == 0)
        {
            rows++;
            continue;
        }
        tempPos = positions.at(i);

//        tempNormal = calculateNormal(tempPos, i, positions);

        //Bottom triangle
//        m_indices.push_back(columns+(rows*columnSize));
//        m_indices.push_back(columns+1+(rows*columnSize));
//        m_indices.push_back(columns+((rows+1)*columnSize)+1);

//        //Top triangle
//        m_indices.push_back(columns+((rows+1)*columnSize)+1);
//        m_indices.push_back(columns+((rows+1)*columnSize));
//        m_indices.push_back(columns+(rows*columnSize));

//        m_indices.push_back(i);
//        m_indices.push_back(i+1);
//        m_indices.push_back(i+columnSize);

//        m_indices.push_back(i+1);
//        m_indices.push_back(i+columnSize+1);
//        m_indices.push_back(i+columnSize);

        m_indices.push_back(i);
        m_indices.push_back(i+1 + columnSize);
        m_indices.push_back(i+columnSize);

        m_indices.push_back(i);
        m_indices.push_back(i+1);
        m_indices.push_back(i+columnSize+1);
        qDebug()<<"pushing vert at "<<tempPos;
    }
}

void LasPlane::readFile(const std::string &filename, QVector3D &max, QVector3D &min, std::vector<QVector3D> &tempLasPos)
{
    //Get file from path and make sure it is okay!
    std::ifstream file(filename);
    assert(file && "LAS file failed!");

    float x{}, y{}, z{};
    unsigned int lines{};

    //First number is number of lines in file. (?)
    file >> lines;
    tempLasPos.reserve(lines);
    bool firstLoop = true;
    while(file)
    {
        file >> x;
        file >> z;
        file >> y;
        if(firstLoop)
        {
            min = QVector3D(x,y,z);
        }
        //Find max
        if(x > max.x()) max.setX(x);
        if(y > max.y()) max.setY(y);
        if(z > max.z()) max.setZ(z);

        //Find min
        if(x < min.x()) min.setX(x);
        if(y < min.y()) min.setY(y);
        if(z < min.z()) min.setZ(z);

        tempLasPos.push_back(QVector3D(x, y, z));
    }

    file.close();
}

QVector3D LasPlane::calculateNormal(QVector3D point3D, unsigned int index, const std::vector<QVector3D> &tempPosition)
{
    std::vector<QVector3D> normalsToCalculate;
    unsigned int x, z, rows, columns;
    QVector3D vectorV, vectorW;

    x = static_cast<unsigned int>(point3D.x());
    z = static_cast<unsigned int>(point3D.z());

//    columns = tempPosition.size() / 2;
    columns = std::sqrt(tempPosition.size());

    rows = columns;

    if((x == 0 && z == 0) || (x == columns-1 && z == rows-1)) //Corner with 2 triangles
    {
        for(int i{0}; i<4; i++) //Default values for vertices "outside" the grid
            normalsToCalculate.push_back(QVector3D(0.f, 1.f, 0.f));

        if(x == 0 && z == 0)
        {
            vectorV = tempPosition.at(index + 1) - point3D;
            vectorW = tempPosition.at(index + columns + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index + columns) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW)); //Inverted because reusing vector: v = w

        }else{
            vectorV = tempPosition.at(index - 1) - point3D;
            vectorW = tempPosition.at(index - columns - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index - columns);
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

        }
    }
    else if((x == columns-1 && z == 0) || (x == 0 && z == rows-1)) //Corner with 1 triangle
    {
        for(int i{0}; i<5; i++) //Default values for vertices "outside" the grid
            normalsToCalculate.push_back(QVector3D(0.f, 1.f, 0.f));

        if(x == columns-1 && z == 0)
        {
            vectorV = tempPosition.at(index + columns) - point3D;
            vectorW = tempPosition.at(index - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }else{
            vectorV = tempPosition.at(index - columns) - point3D;
            vectorW = tempPosition.at(index + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }
    }
    else if(x == 0 || x == columns-1) //Vertical sides
    {
        for(int i{0}; i<3; i++) //Default values for vertices "outside" the grid
            normalsToCalculate.push_back(QVector3D(0.f, 1.f, 0.f));

        if(x == 0)
        {
            vectorV = tempPosition.at(index - columns) - point3D;
            vectorW = tempPosition.at(index + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index + columns + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

            vectorW = tempPosition.at(index + columns) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }else{
            vectorV = tempPosition.at(index + columns) - point3D;
            vectorW = tempPosition.at(index - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index + columns - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

            vectorW = tempPosition.at(index - columns) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }

    }
    else if(z == 0 || z == rows-1) //Horizontal sides
    {
        for(int i{0}; i<3; i++) //Default values for vertices "outside" the grid
            normalsToCalculate.push_back(QVector3D(0.f, 1.f, 0.f));

        if(z == 0)
        {
            vectorV = tempPosition.at(index + 1) - point3D;
            vectorW = tempPosition.at(index + columns + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index + columns) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

            vectorW = tempPosition.at(index - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }else{
            vectorV = tempPosition.at(index - 1) - point3D;
            vectorW = tempPosition.at(index - columns - 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

            vectorV = tempPosition.at(index - columns) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

            vectorW = tempPosition.at(index + 1) - point3D;
            normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        }
    }
    else //Everything else - 6 triangles
    {
        vectorV = tempPosition.at(index + 1) - point3D;
        vectorW = tempPosition.at(index + columns + 1) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        vectorV = tempPosition.at(index + columns) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

        vectorW = tempPosition.at(index - 1) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        vectorV = tempPosition.at(index - 1) - point3D;
        vectorW = tempPosition.at(index - columns - 1) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));

        vectorV = tempPosition.at(index - columns) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorV, vectorW));

        vectorW = tempPosition.at(index + 1) - point3D;
        normalsToCalculate.push_back(QVector3D::crossProduct(vectorW, vectorV));
    }
    QVector3D calc{};
    for(auto it = normalsToCalculate.begin(); it != normalsToCalculate.end(); it++)
    {
        calc += (*it);
    }
    calc.normalize();

    normalsToCalculate.clear();

    return calc;
}

void LasPlane::init(GLint matrixUniform)
{
    constructPlane();

    m_matrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors (normals)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV.
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Indices Buffer Object - IBO
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void LasPlane::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray(m_VAO);

    glUniformMatrix4fv(m_shader->getModelMatrixUniform(), 1, GL_FALSE, m_modelMatrix.constData());

    glUniform3fv(m_shader->getObjectColorUniform(), 1, &m_objectColor[0]);
    glUniform1f(m_shader->getSpecularStrengthUniform(), m_specularStrenght);
    glUniform1i(m_shader->getSpecularExponentUniform(), m_specularExponent);
    glUniform1f(m_shader->getObjectAlphaUniform(), m_objectAlpha);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
