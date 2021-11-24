#include "surface.h"
#include "cmath" //må ha denne for M_PI
#include "iostream"
#include "fstream"

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
            //qDebug() << "vertexes" << i; //it does read the correct amount of vertexes, i just can't see them
        }
        qDebug() << "created surface!";
        inn.close();
    }
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

    initializeOpenGLFunctions();

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
}
