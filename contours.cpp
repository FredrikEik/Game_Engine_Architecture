#include "contours.h"
#include "surface.h"

Contours::Contours()
{

}

Contours::Contours(GameObject* mSurface, float height)
{
    /*for(int i = 0; i < mSurface->getMeshComp()->mIndices.size()-6; i = i+6)
    {
        bool bottomRight = 0;
        bool topRight = 0;
        bool bottomLeft = 0;
        bool topLeft = 0;
        if(mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz().y >= height)
        {
            bottomRight = 1;
        }
        if(mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz().y >= height)
        {
            topRight = 1;
        }
        if(mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz().y >= height)
        {
            bottomLeft = 1;
        }
        if(mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz().y >= height)
        {
            topLeft = 1;
        }

        int state = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;
        //qDebug() << state;

        Vertex v;

        switch(state)
        {
        case 1: //BottomLeft
        {

        v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
        v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
        getMeshComp()->mVertices.push_back(v);

        v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                 (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2 )/2);
        v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
        getMeshComp()->mVertices.push_back(v);
        getMeshComp()->mVertices.push_back(v);


        v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
        v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
        getMeshComp()->mVertices.push_back(v);
        break;
        }
        case 2://BottomRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 3://AllBottom
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 4://TopRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2)/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);


            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 5://Topleft Bottomright
       {
            //BottomRight
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            //TopLeft

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 6://AllRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 7://TopLeft
        {
            //TopLeft

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 8://TopLeft
        {

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;
        }
        case 9://AllRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 10://TopRight BottomLeft
        {
            //BottomLeft
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2 )/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            //TopRight
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);


            v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2)/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);


            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 11://TopRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2)/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);


            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 12://AllBottom
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 13://BottomRight
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+1]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;

        }
        case 14: //BottomLeft
        {
            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+5]].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2 )/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i+2]].get_xyz() + mSurface->getMeshComp()->mVertices[mSurface->getMeshComp()->mIndices[i]].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height + 0.1f, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;
        }

    }

    }*/

    for(int i = 0; i < mSurface->getMeshComp()->mVertices.size()-6; i = i+6)
        {
            bool bottomRight = 0;
            bool topRight = 0;
            bool bottomLeft = 0;
            bool topLeft = 0;
            if(mSurface->getMeshComp()->mVertices[i].get_xyz().y >= height)
            {
                bottomRight = 1;
            }
            if(mSurface->getMeshComp()->mVertices[i+2].get_xyz().y >= height)
            {
                topRight = 1;
            }
            if(mSurface->getMeshComp()->mVertices[i+1].get_xyz().y >= height)
            {
                bottomLeft = 1;
            }
            if(mSurface->getMeshComp()->mVertices[i+3].get_xyz().y >= height)
            {
                topLeft = 1;
            }

            int state = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;
            //qDebug() << state;

            Vertex v;


            switch(state)
            {
            case 1: //BottomLeft
            {

            v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);

            v.set_xyz((((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                     (mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2 )/2);
            v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            getMeshComp()->mVertices.push_back(v);


            v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
            v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
            getMeshComp()->mVertices.push_back(v);
            break;
            }
            case 2://BottomRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 3://AllBottom
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 4://TopRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                         (mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2)/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);


                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 5://Topleft Bottomright
           {
                //BottomRight
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                //TopLeft

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 6://AllRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 7://TopLeft
            {
                //TopLeft

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 8://TopLeft
            {

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+3].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;
            }
            case 9://AllRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 10://TopRight BottomLeft
            {
                //BottomLeft
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                         (mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2 )/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                //TopRight
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);


                v.set_xyz((((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                         (mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2)/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);


                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 11://TopRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                         (mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2)/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);


                v.set_xyz((mSurface->getMeshComp()->mVertices[i+2].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 12://AllBottom
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 13://BottomRight
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+2].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                v.set_xyz((mSurface->getMeshComp()->mVertices[i].get_xyz() + mSurface->getMeshComp()->mVertices[i+1].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;

            }
            case 14: //BottomLeft
            {
                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i].get_xyz())/2) +
                         (mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2 )/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                getMeshComp()->mVertices.push_back(v);

                v.set_xyz((mSurface->getMeshComp()->mVertices[i+1].get_xyz() + mSurface->getMeshComp()->mVertices[i+3].get_xyz())/2);
                v.set_xyz(v.get_xyz().x, height, v.get_xyz().z);
                getMeshComp()->mVertices.push_back(v);
                break;
            }

        }

        }
}
Contours::~Contours()
{

}


void Contours::init()
{
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

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
void Contours::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    glDrawArrays(GL_LINES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);
}
