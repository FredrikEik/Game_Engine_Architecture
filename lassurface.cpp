#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include <vector>
#include "lassurface.h"
#include "contourlines.h"


LasSurface::LasSurface(std::string filnavn)
{
    readLasFile(filnavn);
}

LasSurface::~LasSurface()
{

}

void LasSurface::readLasFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    int n = 0;
    int linesRead = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    if (inn.is_open())
    {
        inn >> n;
        inn >> x >> z >> y;             // leser inn hvor mange linjer det er, og første linje. Første linjen med kordinater brukes som standard
        xMaximum = x;                   // for x,y,z maximum og minimum
        xMinimum = x;
        yMaximum = y;
        yMinimum = y;
        zMaximum = z;
        zMinimum = z;
        lasVertices.reserve(n);
        lasVertex.set_xyz(x,y,z);
        lasVertices.push_back(lasVertex);
        for (int i=0; i<n; i++)                 //for loop som går gjennom hele dokumentet
        {
            inn >> x >> z >> y;
            linesRead++;
            lasVertex.set_xyz(x,y,z);                   //leser inn data og push_backer det
            lasVertices.push_back(lasVertex);
            if (x > xMaximum)
                xMaximum = x;
            else if(x < xMinimum)
                xMinimum = x;
            if (y > yMaximum)                           // endrer på maximum og minimum verdier hvis det kommer en som er større/mindre enn forrige
                yMaximum = y;
            else if(y < yMinimum)
                yMinimum = y;
            if(z > zMaximum)
                zMaximum = z;
            else if(z < zMinimum)
                zMinimum = z;
        }
        inn.close();
    }

    minMaxScale();                                          //skalerer til en minimum- og maximum-grense


    int VerticesCounter=0;
    int step =10;
    float squaresDirection = (step-1);
    float Amountsquares = squaresDirection*squaresDirection;
    float xOffset = (xmax-xmin)/step;
    float zOffset = (zmax-zmin)/step;
    double squareMinZ =zmin;
    double squareMaxZ;
    double squareMinX = xmin;
    double squareMaxX;

    for (double j=squareMinZ; j<=zmax-zOffset; j+=zOffset-0.001f){     //looper gjennom hver square i z retning
        squareMaxZ = j+zOffset;

        for (double i =squareMinX; i<= xmax-xOffset; i+=xOffset-0.001f){       //looper gjennom hver square i x retning
            numberofPointsInside=0;
            float tempX=0;
            float tempY=0;
            float tempZ=0;
            squareMaxX = i +xOffset;
            VerticesCounter++;                          //det er problemer med for løkkene slik at vertices counteren ikke blir riktig på noen steps

            for ( double k = 0 ; k<lasVertices.size(); k++){        //sjekker om punktene er innenfor squaren
                if ( lasVertices[k].getXYZ().getX() < squareMaxX &&
                     lasVertices[k].getXYZ().getX() > i &&
                     lasVertices[k].getXYZ().getZ() < squareMaxZ &&
                     lasVertices[k].getXYZ().getZ() > j){
                    numberofPointsInside++;

                    tempY = tempY+ lasVertices[k].getXYZ().y;


                }
            }
            tempX = squareMaxX - (xOffset/2);                               //finner midtpunktet i squaren
            tempZ =  squareMaxZ - (zOffset/2);
            if(numberofPointsInside==0){
                tempY = (ymin+(ymax-ymin))/2;                               //setter en standar y verdi om det ikke er noen punkter i squaren
            }
            else if(numberofPointsInside > 0)  {

                tempY = tempY/numberofPointsInside;                         // tar gjennomsnitt av alle y verdiene
            }
            vertex.set_xyz(tempX, tempY, tempZ);

            //qDebug () << numberofPointsInside;
            //qDebug() << vertex.getXYZ().x << vertex.getXYZ().y << vertex.getXYZ().z;
           //qDebug() << lasVertices[VerticesCounter].getXYZ().x << lasVertices[VerticesCounter].getXYZ().y << lasVertices[VerticesCounter].getXYZ().z;
            tempVertices.push_back(vertex);                                 //push_backer vertexen

        }
    }

    //qDebug() <<"Number of Vertices" << VerticesCounter;
    //qDebug() << "Amount of squares" << Amountsquares;
    float j=1;
    float sqOffset = 0;
        for(int i =0; i< Amountsquares; i++){                       // lager de nye punktene om til trekanter/squares.
            if (VerticesCounter-i < step ){
                i=Amountsquares;
            }
            if(i == squaresDirection*j){
                sqOffset++;
                j++;                                               // sjekker om alle squarene på en rekke er fylt ut.               
            }


        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[i+sqOffset].getXYZ().x,
                                                 tempVertices[i+sqOffset].getXYZ().y,
                                                 tempVertices[i+sqOffset].getXYZ().z,
                                                 0,0,0,
                                                 0,0));

        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[(i+step)+sqOffset].getXYZ().x,
                                                tempVertices[(i+step)+sqOffset].getXYZ().y,
                                                tempVertices[(i+step)+sqOffset].getXYZ().z,
                                                0,0,0,
                                                1,0));
        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[(i+1)+sqOffset].getXYZ().x,
                                                tempVertices[(i+1)+sqOffset].getXYZ().y,
                                                tempVertices[(i+1)+sqOffset].getXYZ().z,
                                                0,0,0,
                                                0,1));



        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[(i+(step+1))+sqOffset].getXYZ().x,
                                                tempVertices[(i+(step+1))+sqOffset].getXYZ().y,
                                                tempVertices[(i+(step+1))+sqOffset].getXYZ().z,
                                                0,0,0,
                                                1,1));

        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[(i+1)+sqOffset].getXYZ().x,
                                                tempVertices[(i+1)+sqOffset].getXYZ().y,
                                                tempVertices[(i+1)+sqOffset].getXYZ().z,
                                                0,0,0,
                                                0,1));
        getMeshComponent()->mVertices.push_back(Vertex(tempVertices[(i+step)+sqOffset].getXYZ().x,
                                                tempVertices[(i+step)+sqOffset].getXYZ().y,
                                                tempVertices[(i+step)+sqOffset].getXYZ().z,
                                                0,0,0,
                                                1,0));
    }

    //              sortering algoritme, ikke i bruk
    //        for (l = 0; l < n-1; l++)
    //        {
    //            min_index = l;
    //            for (j=l+1 ; j<n; j++)
    //            {
    //                if (a[j] < a[min_index]){
    //                 min_index = j;
    //                }
    //            }



}


void LasSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}

void LasSurface::minMaxScale()          //skalerer mellom to verdier, gjør dette med alle verdiene for å få lettere tall og jobbe med.
{
    for(int index = 0; index < lasVertices.size(); index++){

        float scaledX = xmin+(((lasVertices[index].getXYZ().getX()-xMinimum)*(xmax-xmin))/(xMaximum-xMinimum));
        float scaledY = ymin+(((lasVertices[index].getXYZ().getY()-yMinimum)*(ymax-ymin))/(yMaximum-yMinimum));
        float scaledZ = zmin+(((lasVertices[index].getXYZ().getZ()-zMinimum)*(zmax-zmin))/(zMaximum-zMinimum));


        lasVertices[index].set_xyz(scaledX, scaledY, scaledZ);
    }
}

void LasSurface::createContourLines()
{
   for (float contourHeight = contourMinimum; contourHeight < contourMaximum; contourHeight+=contourStep)
   {
       for (int i = 0; i<getMeshComponent()->mVertices.size(); i+=6){
           Vertex a = getMeshComponent()->mVertices[i];
           Vertex b = getMeshComponent()->mVertices[i+1];
           Vertex c = getMeshComponent()->mVertices[i+3];
           Vertex d = getMeshComponent()->mVertices[i+2];

           bool A = false;
           bool B = false;
           bool C = false;
           bool D = false;

           if(a.getXYZ().y > contourHeight){
               A = 1;
           }
           if(b.getXYZ().y > contourHeight){
               B = 1;
           }
           if(c.getXYZ().y > contourHeight){
               C = 1;
           }
           if(d.getXYZ().y > contourHeight){
               D = 1;
           }
           Vertex ab = (a+b)/2;
           ab.set_y(contourHeight);
           Vertex bc = (b+c)/2;
           bc.set_y(contourHeight);
           Vertex cd = (c+d)/2;
           cd.set_y(contourHeight);
           Vertex da = (d+a)/2;
           da.set_y(contourHeight);
           Vertex bd = (b+d)/2;
           bd.set_y(contourHeight);
           Vertex bbd = (b+bd)/2;
           bbd.set_y(contourHeight);
           Vertex dbd = (d+bd)/2;
           dbd.set_y(contourHeight);

           int state = getstate(A,B,C,D);

           switch (state)
           {
           case 0:
               break;
           case 1:
               //da to cd
               contourPoints.push_back(da);
               contourPoints.push_back(dbd);
               contourPoints.push_back(dbd);
               contourPoints.push_back(cd);
               break;
           case 2:
               //bc to cd
               contourPoints.push_back(bc);
               contourPoints.push_back(cd);
               break;
           case 3:
               contourPoints.push_back(da);
               contourPoints.push_back(bd);
               contourPoints.push_back(bd);
               contourPoints.push_back(bc);
               break;
           case 4:
               contourPoints.push_back(ab);
               contourPoints.push_back(bbd);
               contourPoints.push_back(bbd);
               contourPoints.push_back(bc);
               break;
           case 5:
               contourPoints.push_back(da);
               contourPoints.push_back(ab);
               contourPoints.push_back(cd);
               contourPoints.push_back(bc);
               break;
           case 6:
               contourPoints.push_back(ab);
               contourPoints.push_back(bd);
               contourPoints.push_back(bd);
               contourPoints.push_back(cd);
               break;
           case 7:
               contourPoints.push_back(da);
               contourPoints.push_back(ab);
               break;
           case 8:
               contourPoints.push_back(ab);
               contourPoints.push_back(da);
               break;
           case 9:
               contourPoints.push_back(cd);
               contourPoints.push_back(bd);
               contourPoints.push_back(bd);
               contourPoints.push_back(ab);
               break;
           case 10:
               contourPoints.push_back(da);
               contourPoints.push_back(dbd);
               contourPoints.push_back(dbd);
               contourPoints.push_back(cd);

               contourPoints.push_back(ab);
               contourPoints.push_back(bbd);
               contourPoints.push_back(bbd);
               contourPoints.push_back(bc);
               break;
           case 11:
               contourPoints.push_back(bc);
               contourPoints.push_back(bbd);
               contourPoints.push_back(bbd);
               contourPoints.push_back(ab);
               break;
           case 12:
               contourPoints.push_back(da);
               contourPoints.push_back(bd);
               contourPoints.push_back(bd);
               contourPoints.push_back(bc);
               break;
           case 13:
               contourPoints.push_back(cd);
               contourPoints.push_back(bc);
               break;
           case 14:
               contourPoints.push_back(cd);
               contourPoints.push_back(dbd);
               contourPoints.push_back(dbd);
               contourPoints.push_back(da);
               break;
           case 15:
               break;
           }
       }
   }
}

int LasSurface::getstate(bool A, bool B, bool C, bool D)
{
    return 8*A+4*B+2*C+1*D;
}

void LasSurface::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER,
                  getMeshComponent()->mVertices.size()*sizeof( Vertex ),
                  getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void LasSurface::draw()
{
    glBindVertexArray(getMeshComponent()->mVAO);
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void LasSurface::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}


