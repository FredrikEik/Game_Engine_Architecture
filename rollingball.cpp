//#include "rollingball.h"
//#include "QDebug"

//RollingBall::RollingBall(int n)
//{
//    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
//    mPosition.setPosition(-1.0,0.0,1);
//    mScale.scale(0.25,0.25,0.25);
//    gForce = gAcceleration * massKG;
//}
//RollingBall::~RollingBall()
//{

//}
//void RollingBall::move(float dt)
//{
//    std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

//    gsl::Vector3D BallPositimPosition.getPosition();

//    for(unsigned long long i = 0; i < vertices.size() - 2; i+=3)
//    {
//        gsml::Vector3d p1, p2, p3;
//        p1 = gsml::Vector3d(vertices[i].getXYZ());
//        p2 = gsml::Vector3d(vertices[i+1].getXYZ());
//        p3 = gsml::Vector3d(vertices[i+2].getXYZ());

//        gsml::Vector3d BaryCord;
//        BaryCord = BallPosition.barycentricCoordinates(p1, p2, p3);

//        if(BaryCord.x >= 0 && BaryCord.y >= 0 && BaryCord.z >= 0)
//        {
//            gsml::Vector3d p12     = p2-p1;
//            gsml::Vector3d p13     = p3-p1;
//            gsml::Vector3d pNormal = p12^p13;
//            pNormal.normalize();

//            /*gForce.x = abs(gForce.x);
//            gForce.y = abs(gForce.y);
//            gForce.z = abs(gForce.z);*/

//            acceleration = gForce ^ pNormal ^ gsml::Vec3(0,0,pNormal.z);
//            velocity = velocity + acceleration * dt;


//            gsml::Vector3d newPosition = mPosition.getPosition() + velocity;
//            newPosition.z = (p1.z*BaryCord.x + p2.z*BaryCord.y + p3.z*BaryCord.z)+0.25;
//            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z);

//            BallPosition = mPosition.getPosition();
//            qDebug() << "BallPosition: " << BallPosition.x << BallPosition.y << BallPosition.z;
//        }
//    }


//    mMatrix = mPosition * mScale;


//}

//void RollingBall::init(GLint matrixUniform)
//{
//   mMatrixUniform = matrixUniform;
//   initializeOpenGLFunctions();

//   //Vertex Array Object - VAO
//   glGenVertexArrays( 1, &getMeshComponent()->mVAO );
//   glBindVertexArray( mVAO );

//   //Vertex Buffer Object to hold vertices - VBO
//   glGenBuffers( 1, &mVBO );
//   glBindBuffer( GL_ARRAY_BUFFER, mVBO );

//   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

//   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
//   glEnableVertexAttribArray(0);

//   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
//   glEnableVertexAttribArray(1);

//   glBindVertexArray(0);
//}

//void RollingBall::barycentricCoordinates()
//{
//}

//void RollingBall::draw()
//{
//   glBindVertexArray( mVAO );
//   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
//   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
//}
