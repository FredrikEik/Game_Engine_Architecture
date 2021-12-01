#include "rollingball.h"


RollingBall::RollingBall(int n) : OctahedronBall(n)
{
    //gKraft = gAkselerasjon*masseIKG;        //regner ut gkraften til ballen
    gKraft =  gsl::Vector3D(0,-9.80565f*masseIKG,0);
}


RollingBall::~RollingBall()
{

}

void RollingBall::init()
{
    initializeOpenGLFunctions();

      //Vertex Array Object - VAO
      glGenVertexArrays( 1, &getMeshComponent()->mVAO );
      glBindVertexArray( getMeshComponent()->mVAO );

      //Vertex Buffer Object to hold vertices - VBO
      glGenBuffers( 1, &getMeshComponent()->mVBO );
      glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

      glBufferData( GL_ARRAY_BUFFER, getMeshComponent()->mVertices.size()*sizeof(Vertex), getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

      glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
      glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
      glEnableVertexAttribArray(1);

      glBindVertexArray(0);
}

void RollingBall::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void RollingBall::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
      getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);

}

void RollingBall::move(float dt)
{

    std::vector<Vertex>& vertices = dynamic_cast<class LasSurface*>(LasSurface)->getMeshComponent()->mVertices;             //skaffer vertices
    for (int i=0; i < vertices.size()-2; i+=3){
        v0 = gsl::Vector3D(vertices[i].getXYZ());
        v1 = gsl::Vector3D(vertices[i+1].getXYZ());
        v2 = gsl::Vector3D(vertices[i+2].getXYZ());

        gsl::Vector3D ballpos = getTransformComponent()->mMatrix.getPosition();                 //skaffer posisjonen til ballen

        barycentricCord = ballpos.barycentricCoordinates(
                    vertices[i].getXYZ(),vertices[i+1].getXYZ(), vertices[i+2].getXYZ());  //kalkulerer barisentriske kordinater

        if(barycentricCord.x > 0 && barycentricCord.y > 0 && barycentricCord.z > 0 &&
                barycentricCord.x < 1 && barycentricCord.y < 1 && barycentricCord.z < 1){   //sjekker at ballen er innafor trianglene

            normalvektor = (v1-v0)^(v2-v0);                                             //regner ut normalvektoren til planet
            normalvektor.normalize();                                                   //normaliserer normalvektoren
            float planHøyde = v0.y*barycentricCord.x+v1.y*barycentricCord.y+v2.y*barycentricCord.z;              //bruker barysentriske kordinatene til å bestemme nye z posisjonen

            if (ballpos.y < planHøyde + radius*3){
            akselerasjon = gsl::Vector3D(normalvektor.x*normalvektor.y*9.80665f,        //regner ut akselerasjon
                                         normalvektor.y*normalvektor.y*9.80665f,
                                         normalvektor.z*normalvektor.y*9.80665f)+gKraft;
            hastighet = hastighet + akselerasjon * dt;                                  //regner ut hastighet
            nyPosisjon = getTransformComponent()->mMatrix.getPosition() + hastighet*dt;                                         //oppdaterer posisjonen

            getTransformComponent()->mMatrix.setPosition(nyPosisjon.x, planHøyde+radius, nyPosisjon.z);      //setter den nye posisjonen, plusser også på radiusen på z'en til ballen så den ligger oppå planet
            }
            else {
            akselerasjon.y = gKraft.y;
            hastighet = hastighet + akselerasjon *dt;
            nyPosisjon = getTransformComponent()->mMatrix.getPosition() + hastighet*dt;
            getTransformComponent()->mMatrix.setPosition(nyPosisjon.x, nyPosisjon.y,nyPosisjon.z);
            }



                    //work in progress
            //avstand = (temp-vertices[0].getXYZ())^normalvektor;
            //projeksjon = normalvektor^(avstand^normalvektor);
            //distanseFlyttet = hastighet * dt;
            //distanseFlyttetNM = normalvektor^(distanseFlyttet ^normalvektor);
            //mPosition.setPosition(hastighet.x, hastighet.y, nyPosisjon.z+radius);

                    //Test greier
            //qDebug() << pow(normalvektor.z,2)-1;
            //qDebug() << nyPosisjon.z;
            //qDebug() << distanseFlyttet.x << distanseFlyttet.y << distanseFlyttet.z;
            //qDebug() << distanseFlyttetNM.x << distanseFlyttetNM.y << distanseFlyttetNM.z;
            //qDebug() << avstand.x << avstand.y << avstand.z;
            //qDebug() << projeksjon.x << projeksjon.y << projeksjon.z;
            //qDebug() << hastighet.x << hastighet.z << hastighet.y;

            //ting som trengs for akselerasjonsvektor og posisjon
            // *******************************************************
            //normalvektor n-> , avstanden til planet y og projeksjonen y->n->= n->*(y->*n->)
            //Avstanden ballen har flyttet seg langs normalvektoren =ds->n-> = n->*(ds->*n->)
            //Tyngdepunktet til ballen ligger enten over eller under planet (y->*n->)/||y->|| = +-1
            //negativt = under, må flytte tilbake posisjon med d=r-y langs normalvektor til planet,
            // må også flytte samme distansen d langs nye hastighetsvektoren
        }
        else if (barycentricCord.x < 0 && barycentricCord.y < 0 && barycentricCord.z < 0 &&     //sjekker at ballen ikke er innenfor trianglene
                 barycentricCord.x > 1 && barycentricCord.y > 1 && barycentricCord.z > 1) {
            akselerasjon.y = gKraft.y;
            hastighet = hastighet + akselerasjon *dt;
            nyPosisjon = getTransformComponent()->mMatrix.getPosition() + hastighet*dt;
            getTransformComponent()->mMatrix.setPosition(nyPosisjon.x, nyPosisjon.y,nyPosisjon.z);
           qDebug() << "The ball is outside";
        }
    }
}
