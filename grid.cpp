#include "grid.h"


grid::grid(int size, float spacing)
{
    for(int i{-size}; i <= size; ++i){
        if(i != 0){
            mVertices.insert(mVertices.end(),
            {
                Vertex {size*spacing, 0.0, i*spacing,     0.7, 0.7, 0.7},
                Vertex{ -size*spacing, 0.0, i*spacing,     0.7, 0.7, 0.7},
                Vertex{ i*spacing, 0.0,  size*spacing,     0.7, 0.7, 0.7},
                Vertex{ i*spacing, 0.0, -size*spacing,     0.7, 0.7, 0.7}
            });
        }
        else{
            mVertices.insert(mVertices.end(),
            {
                Vertex{  size*spacing, 0.0, i*spacing,     1.0, 0.0, 0.0},
                Vertex{ -size*spacing, 0.0, i*spacing,     1.0, 0.0, 0.0},
                Vertex{ i*spacing, 0.0,  size*spacing,     0.0, 0.0, 1.0},
                Vertex{ i*spacing, 0.0, -size*spacing,     0.0, 0.0, 1.0}
            });
        }


    }
    mMatrix.setToIdentity();
}

grid::grid()
{

}

void grid::init()
{
    mMatrix.setToIdentity();
    initializeOpenGLFunctions();
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex) , mVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0  ));          // array buffer offset
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void grid::draw()
{

        glBindVertexArray( mVAO );
        glDrawArrays(GL_LINES, 0, mVertices.size());

}
