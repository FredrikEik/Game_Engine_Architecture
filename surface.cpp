#include "surface.h"
#include "cmath" //må ha denne for M_PI
#include "iostream"
#include "fstream"
#include "sstream"

Surface::Surface()
{
    //mMatrix.setToIdentity();
    getTransformComp()->mMatrix.setToIdentity();
}

Surface::Surface(std::string filename) //går her når filepath er inputta i renderwindow.cpp new Surface.
{
    writeFile(filename);
    readFile(filename);
    //mMatrix.setToIdentity();
    getTransformComp()->mMatrix.setToIdentity();
}

Surface::~Surface()
{

}

void Surface::writeFile(std::string filename)
{
    /*std::ofstream file;
    file.open(filename.c_str());
    if(file.is_open())
    {
        //qDebug() << "filen  ble åpnet\n\n";
        //int n = 32;
        Vertex vertex;
        //file << n << "\n";

       float xmin = 0.0f, xmax = 1.0f, ymin = 0.0f, ymax = 1.0f, h = 0.25f;
        file << ((xmax - xmin) / h) * ((ymax - ymin) / h) * 6 << "\n"; //6 er hvor mange push backs som gjøres.
        for(auto x = xmin; x < xmax; x += h)
            for(auto y = ymin; y < ymax; y += h)
            {
                float z = sin(M_PI * x) * sin(M_PI * y);
                mVertices.push_back(Vertex{x,y,z,x,y,z});
                file << mVertices.back() << std::endl;

                z = sin(M_PI * (x+h)) * sin(M_PI * y);
                mVertices.push_back(Vertex{x+h,y,z,x,y,z});
                file << mVertices.back() << std::endl;

                z = sin(M_PI * x) * sin(M_PI * (y+h));
                mVertices.push_back(Vertex{x,y+h,z,x,y,z});
                file << mVertices.back() << std::endl;

                mVertices.push_back(Vertex{x,y+h,z,x,y,z});
                file << mVertices.back() << std::endl;

                z = sin(M_PI * (x+h)) * sin(M_PI * y);
                mVertices.push_back(Vertex{x+h,y,z,x,y,z});
                file << mVertices.back() << std::endl;

                z = sin(M_PI * (x+h)) * sin(M_PI * (y+h));
                mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
                file << mVertices.back() << std::endl;
            }

    }
    file.close();*/
}

void Surface::readFile(std::string filename)
{
    //******** UNCOMMENT THIS FOR MY READIFLE (DOESNT SHOW PLANE AT THE MOMENT) ********//
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n;
        Vertex vertex;
        inn >> n;
        getMeshComp()->mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            getMeshComp()->mVertices.push_back(vertex);
            //getMeshComp()->mIndices.push_back();
            //qDebug() << "vertexes" << i; //reads the correct amount of vertexes
            //qDebug() << getTransformComp()->mMatrix.getPosition(); //only shows {0,0,0}
        }
        qDebug() << "created surface!";
        inn.close();
    }

    //Open File
        //    std::string filename = Orf::assetFilePath.toStdString() + fileName + ".obj";


        //******** UNCOMMENT THIS FOR GUDBRAND READIFLE (SHOWS PLANE BUT VERY BROKEN) ********//
        /*std::ifstream fileIn;
        fileIn.open (filename, std::ifstream::in);
        if(!fileIn)
            qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

        //One line at a time-variable
        std::string oneLine;
        //One word at a time-variable
        std::string oneWord;

        std::vector<gsl::Vector3D> tempVertecies;
        std::vector<gsl::Vector3D> tempNormals;
        std::vector<gsl::Vector2D> tempUVs;

        std::getline(fileIn, oneLine);
        std::stringstream sStream;
        sStream << oneLine;
        sStream >> oneWord;
        int num = std::atoi(oneWord.c_str());
        qDebug() << "Vertices: " << num;

        unsigned int temp_index = 0;

        for(int i = 0; i < num; i++)
        {
            std::getline(fileIn, oneLine);
            std::stringstream sStream;
            //Pushing line into stream
            sStream << oneLine;
            //Streaming one word out of line
            oneWord = ""; //resetting the value or else the last value might survive!
            sStream >> oneWord;

            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord) - 6152000.f; // file reads wrong for no reason
            tempVertex.x -= 606000.f;
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord) - 565.f;
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord) - 565.f;
            //qDebug() << "x: " << tempVertex.x << " y: " << tempVertex.y << " z: " << tempVertex.z;

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            Vertex tempVert(tempVertex, gsl::Vector3D(1.f, 0.f, 0.f), gsl::Vector2D(0.f, 0.f));
            if(getMeshComp())
            {
                getMeshComp()->mVertices.push_back(tempVert);
                getMeshComp()->mIndices.push_back(temp_index++);
            }
        }*/

}

void Surface::init()
{
    /*mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);*/

    /*initializeOpenGLFunctions();

       glGenVertexArrays( 1, &getMeshComp()->mVAO );
       glBindVertexArray( getMeshComp()->mVAO );


       glGenBuffers( 1, &getMeshComp()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComp()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComp()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComp()->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);*/

       initializeOpenGLFunctions();

       //Vertex Array Object - VAO
       glGenVertexArrays( 1, &getMeshComp()->mVAO );
       glBindVertexArray( getMeshComp()->mVAO );

       //Vertex Buffer Object to hold vertices - VBO
       glGenBuffers( 1, &getMeshComp()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

       glBufferData( GL_ARRAY_BUFFER, getMeshComp()->mVertices.size()*sizeof(Vertex), getMeshComp()->mVertices.data(), GL_STATIC_DRAW );

       // 1rst attribute buffer : vertices
       glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
       glEnableVertexAttribArray(2);

       //Second buffer - holds the indices (Element Array Buffer - EAB):
       glGenBuffers(1, &getMeshComp()->mEAB);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mEAB);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mIndices.size() * sizeof(GLuint), getMeshComp()->mIndices.data(), GL_STATIC_DRAW);

       glBindVertexArray(0);
}

void Surface::draw()
{
    /*glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());*/

    glBindVertexArray( getMeshComp()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);

    /*glBindVertexArray( getMeshComp()->mVAO );
    glDrawElements(GL_TRIANGLES, getMeshComp()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);*/
}
