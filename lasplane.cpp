#include "lasplane.h"
#include "Shaders/shader.h"

LasPlane::LasPlane(std::string filename)
    :VisualObject()
{
    filepath = filename;
}

void LasPlane::constructPlane()
{
    qDebug() << "Started constructing plane!";

    std::vector<QVector3D> tempLasPos{},  positions{};
    QVector3D max{}, min{}, tempPos{}, tempNormal{};
    float resoluton{30}, temp{};

    readFile(filepath, max, min, tempLasPos);

    qDebug() << "File read!";

    std::map<int, std::pair<int, float>> &squareHeights = (*new std::map<int, std::pair<int, float>>);//(std::pair<int, float>(0,0), (int)((max.z() * max.x()) / resoluton));
    long long squarePosition{};

    for(auto &it : tempLasPos)
    {
        long long positionX = (it.x()-min.x())/resoluton;
        long long positionZ = (it.z()-min.z())/resoluton;
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
        if(columns >= columnSize)
        {
            rows++;
            columns = 0;
        }
        temp = it.second.second / (float)it.second.first;
        tempPos = QVector3D(rows*resoluton, temp - min.y(), columns*resoluton);
        positions.push_back(tempPos);

        tempNormal = QVector3D(0,1,0);

        m_vertices.push_back(Vertex(tempPos, tempNormal));

        columns++;
    }

    qDebug() << "Vertices created!";

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

        m_indices.push_back(i);
        m_indices.push_back(i+1 + columnSize);
        m_indices.push_back(i+columnSize);

        m_indices.push_back(i);
        m_indices.push_back(i+1);
        m_indices.push_back(i+columnSize+1);
        //qDebug()<<"pushing vert at "<<tempPos;
    }

    qDebug() << "Indices created!";
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

    qDebug() << "LAS plane constructed!";

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
