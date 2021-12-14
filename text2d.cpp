#include "text2d.h"

Text2D::Text2D(const char * text, int x, int y)
{
    bHudElement = true;
    mTexture = 6;
    mX = x;
    mY = y;
    getTransformComp()->mMatrix.setToIdentity();
    printText2D(text, x, y, 10); // text is the text, x and y si position on screen and size is the font size
    for (int i = 0; i < vertices.size(); i++)
    {
        Vertex temp = Vertex(gsl::Vector3D(vertices[i].x, vertices[i].y, 0.f), gsl::Vector3D(0.f, 0.f, 0.f), UVs[i]);
        getMeshComp()->mVertices.push_back(temp);
        getMeshComp()->mIndices.push_back(i);
    }
    TransformComp->mTrueScaleMatrix = TransformComp->mMatrix;
    TransformComp->Scal = gsl::Vector3D(0.01, 0.01, 0.01);// downsizing and putting it in the down left corner for when x an y is 0
    TransformComp->mMatrix.scale(TransformComp->Scal);
    TransformComp->mMatrix.translateX(-100.f);
    TransformComp->mMatrix.translateY(-100.f);
    TransformComp->mTrueScaleMatrix.translateX(-100.f);
    TransformComp->mTrueScaleMatrix.translateY(-100.f);
}

Text2D::~Text2D()
{

}

void Text2D::init()
{
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

void Text2D::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    //glUniformMatrix4fv( MeshComp->mMatrixUniform, 1, GL_FALSE, transformComp->mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);
}

void Text2D::printText2D(const char * text, int x, int y, int size)
{
    unsigned int length = strlen(text);

    for ( unsigned int i=0 ; i<length ; i++ ){

        gsl::Vector2D vertex_up_left    = gsl::Vector2D( x+i*size     , y+size );
        gsl::Vector2D vertex_up_right   = gsl::Vector2D( x+i*size+size, y+size );
        gsl::Vector2D vertex_down_right = gsl::Vector2D( x+i*size+size, y      );
        gsl::Vector2D vertex_down_left  = gsl::Vector2D( x+i*size     , y      );

        vertices.push_back(vertex_up_left   );
        vertices.push_back(vertex_down_left );
        vertices.push_back(vertex_up_right  );

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

        char character = text[i];
        int c = character - 32;
        float uv_x = (c % 8) / 8.0f;
        float uv_y = (c / 8) / 8.0f;

        gsl::Vector2D uv_up_left    = gsl::Vector2D( uv_x           , 1.0f - uv_y );
        gsl::Vector2D uv_up_right   = gsl::Vector2D( uv_x+1.0f/8.0f, 1.0f - uv_y );
        gsl::Vector2D uv_down_right = gsl::Vector2D( uv_x+1.0f/8.0f, 1.0f - (uv_y + 1.0f/8.0f) );
        gsl::Vector2D uv_down_left  = gsl::Vector2D( uv_x           , 1.0f - (uv_y + 1.0f/8.0f) );

        UVs.push_back(uv_up_left   );
        UVs.push_back(uv_down_left );
        UVs.push_back(uv_up_right  );

        UVs.push_back(uv_down_right);
        UVs.push_back(uv_up_right);
        UVs.push_back(uv_down_left);
    }
}

void Text2D::updateText(const char * text)
{
    getMeshComp()->mIndices.clear();
    getMeshComp()->mVertices.clear();
    vertices.clear();
    UVs.clear();
    printText2D(text, mX, mY, 10);
    for (int i = 0; i < vertices.size(); i++)
    {
        Vertex temp = Vertex(gsl::Vector3D(vertices[i].x, vertices[i].y, 0.f), gsl::Vector3D(0.f, 0.f, 0.f), UVs[i]);
        getMeshComp()->mVertices.push_back(temp);
        getMeshComp()->mIndices.push_back(i);
    }
}
