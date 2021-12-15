#ifndef COMPONENT_H
#define COMPONENT_H

#include "constants.h"
#include "matrix4x4.h"
#include "vertex.h"
#include "vector3d.h"
#include "gltypes.h"
#include <QOpenGLFunctions_4_1_Core>

/**
  TransformComponent inneholder nødvendig data for posisjon og scale.
  Alle objektene våre har denne komponenten.
 */

struct TransformComponent
{
    gsl::Matrix4x4 mMatrix;
    gsl::Vector3D mPosition;
    float mScale = 1;
};

/**
  MeshComponent inneholder data vi trenger for å sette på mesh.
  Alle synlige objekter(/objekter med mesh) har en mesh-komponent.
  Vi setter standard drawtype til å være triangler, siden det er det vi bruker mest.
 */

struct MeshComponent
{
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLenum mDrawType{GL_TRIANGLES};
};

/**
  MaterialComponent inneholder data vi trenger for å bruke materialer.
  Komponenten forteller oss hvilket shaderprogram vi skal bruke,
  og hvilken tekstur-ID objektet har.
 */

struct MaterialComponent
{
    GLuint mShaderProgram{0};
    GLint mTextureUnit{0};
};

/**
  NameComponent inneholder en unik objektID og navn for hvert objekt.
  Alle objekter vi vil liste med navn og ID bruker denne komponenten.
  Kommer godt med når vi lager mange objekter med lik shape.
  Setter og bruker denne komponenten i shape-factory og i UI-en vår.
 */

struct NameComponent
{
    int objectID=0;
    std::string mName;
};

/**
  MovementComponent inneholder data vi trenger for objekter som beveger seg i spillet.
  Player, enemy og kamera bruker denne komponenten til å bevege seg.
 */

struct MovementComponent
{
    gsl::Vector3D mForward{0,0,1};
    float mSpeed{0.05f};
    float mRotateSpeed{0.1f};
    int posX{0};
    int posZ{0};
};

/**
  InputComponent holder på data for alle tastatur og mus-inputs.
  Vi bruker bools til å sjekke om taster blir trykket på.
  Vi lagrer også mouseWheel som float og inter for X og Y mus-posisjon.
 */

struct InputComponent
{
    bool W{false};
    bool A{false};
    bool S{false};
    bool D{false};
    bool UP{false};     //Arrow keys
    bool DOWN{false};
    bool LEFT{false};
    bool RIGHT{false};
    bool Q{false};
    bool E{false};
    bool C{false};
    bool LSHIFT{false};
    bool LCTRL{false};
    bool SPACE{false};
    bool LMB{false};    //Mouse buttons
    bool RMB{false};
    bool MMB{false};
    float MWHEEL{0.f};  //MouseWheel
    int MOUSEX{0};
    int MOUSEY{0};
};

#endif // COMPONENT_H
