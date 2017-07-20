#include "glwidget.h"
#include <QDebug>
#include <QString>

#include <cmath>

#include <iostream>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>

#include "input.h"

#include <iostream>

QOpenGLTexture *texture;

// Create a colored cube

QVector3D cubeVertices[] = {
QVector3D( 0.5f,  0.5f,  0.5f),
QVector3D(-0.5f,  0.5f,  0.5f),
QVector3D(-0.5f, -0.5f,  0.5f),
QVector3D( 0.5f, -0.5f,  0.5f),
QVector3D( 0.5f,  0.5f, -0.5f),
QVector3D(-0.5f,  0.5f, -0.5f),
QVector3D(-0.5f, -0.5f, -0.5f),
QVector3D( 0.5f, -0.5f, -0.5f)};


QVector2D texCoord[] = {
QVector2D( 1.0f,  1.0f),
QVector2D( 0.0f,  1.0f),
QVector2D( 0.0f,  0.0f),
QVector2D( 0.0f,  0.0f),
QVector2D( 1.0f,  0.0f),
QVector2D( 1.0f,  1.0f)};


static GLuint cubeIndices[] = {
0,1,2, 2,3,0,
5,4,7, 7,6,5,
0,4,5, 5,1,0,
3,2,6, 6,7,3,
1,5,6, 6,2,1,
0,3,7, 7,4,0
};


GLWidget::GLWidget(int step, QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0) {
  step_ = step;
  LoadCube();
}

void GLWidget::LoadCube() {
    vertices = (QVector3D*) malloc (36 * sizeof (QVector3D));
    tangents = (QVector3D*) malloc (36 * sizeof (QVector3D));
    normals = (QVector3D*) malloc (36 * sizeof (QVector3D));
    Texture = (QVector2D*) malloc (36 * sizeof (QVector2D));
    flatNormals = (QVector3D*) malloc (36 * sizeof (QVector3D));
    meanNormals = (QVector3D*) malloc (36 * sizeof (QVector3D));

    std::vector<QVector3D> faceNorms;
    for (int i = 0; i < (int) 36; i += 3) {
        QVector3D u(cubeVertices[cubeIndices[i]] - cubeVertices[cubeIndices[i+2]]);
        QVector3D v(cubeVertices[cubeIndices[i+1]] - cubeVertices[cubeIndices[i]]);
        QVector3D prod = u.crossProduct(u,v);
        faceNorms.push_back(prod);
    }

    for (int i = 0; i < 36;++i) {
        flatNormals[i] = faceNorms[i/3];
        flatNormals[i].normalize();
    }

    QVector3D *mean = (QVector3D*) malloc (8 * sizeof (QVector3D));
    for (int i = 0; i < 8;++i) {
        mean[i] = QVector3D(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < (int) 36; ++j)
                mean[cubeIndices[j]] += faceNorms[j/3];
    }
    for (int i = 0; i < 8; ++i)
        mean[i].normalize();

    for (int i = 0; i < 36; ++i) {
        vertices[i] = cubeVertices[cubeIndices[i]];
        Texture[i] = texCoord[(i % 6)];
        normals[i] = cubeVertices[cubeIndices[i]];
        normals[i].normalize();
        meanNormals[i] = mean[cubeIndices[i]];
    }
    free(mean);

    QVector3D *tan = (QVector3D*) malloc (8 * sizeof (QVector3D));
    for (int i = 0; i < (int) 36; i += 3) {
        int x, y, z;
        QVector3D u(cubeVertices[x = cubeIndices[i]] - cubeVertices[z = cubeIndices[i+2]]);
        QVector3D v(cubeVertices[y = cubeIndices[i+1]] - cubeVertices[cubeIndices[i]]);

        float u1 = Texture[i].x();
        float v1 = Texture[i].y();
        float u2 = Texture[i+1].x();
        float v2 = Texture[i+1].y();
        float u3 = Texture[i+2].x();
        float v3 = Texture[i+2].y();

        QVector3D resp = ((v2-v1)*u-(v1-v3)*v)/((u1-u3)*(v2-v1)-(v1-v3)*(u2-u1));

        tan[x] += resp;
        tan[y] += resp;
        tan[z] += resp;
    }
    for (int i = 0; i < 8; ++i) {
        tan[i].normalize();
    }
    for (int i = 0; i < 36; ++i) {
        tangents[i] = tan[cubeIndices[i]];
    }
    free(tan);

  vertexCount_ = 36;
  sg_vertexes_ = vertices;
  sg_texture_ = Texture;
  sg_tangent_ = tangents;
  normalCount_ = 36;
  sg_normals_ = normals;
}

void GLWidget::changeToFlat() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = flatNormals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::changeToNorm() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = normals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::changeToMean() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = meanNormals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::loadObj (int indSize, int vertCount, uint *inds, QVector3D *verts, QVector3D *norms, QVector3D *tex) {
    m_program->bind();
    m_vertex.bind();
    m_object.bind();

    free (tangents);
    tangents= (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free (Texture);
    Texture = (QVector2D*) malloc (indSize * sizeof (QVector2D));
    free(vertices);
    vertices = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(normals);
    normals = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(flatNormals);
    flatNormals = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(meanNormals);
    meanNormals = (QVector3D*) malloc (indSize * sizeof (QVector3D));

    QVector3D *mean = (QVector3D*) malloc (vertCount * sizeof (QVector3D));
    std::vector<QVector3D> faceNorms;
    for (int i = 0; i < (int) indSize; i += 3) {
        int x, y, z;
        QVector3D u(verts[x = inds[i]] - verts[z = inds[i+2]]);
        QVector3D v(verts[y = inds[i+1]] - verts[inds[i]]);
        QVector3D prod = u.crossProduct(u,v);
        faceNorms.push_back(prod);
        mean[x] += prod.normalized();
        mean[y] += prod.normalized();
        mean[z] += prod.normalized();
    }

    for (int i = 0; i < indSize;++i) {
        flatNormals[i] = faceNorms[i/3];
        flatNormals[i].normalize();
    }

    for (int i = 0; i < vertCount; ++i)
        mean[i].normalize();

    float min_w,min_h, max_w, max_h;
    max_w = max_h = 0.0f;
    min_w = (float) maximumWidth();
    min_h = (float) maximumHeight();

    for (int i = 0; i < indSize; ++i) {
        normals[i] = norms[inds[i]];
        Texture[i] = tex[inds[i]].toVector2D();
        meanNormals[i] = mean[inds[i]];
        vertices[i] = verts[inds[i]];
        if (vertices[i].x() < min_w)
            min_w = vertices[i].x();
        else if (vertices[i].x() > max_w)
            max_w = vertices->x();
        if (vertices[i].y() < min_h)
            min_h = vertices[i].y();
        else if (vertices[i].y() > max_h)
            max_h = vertices->y();
    }
    free (mean);

    QVector3D *tan = (QVector3D*) malloc (vertCount * sizeof (QVector3D));
    for (int i = 0; i < (int) indSize; i += 3) {
        int x, y, z;
        QVector3D u(verts[x = inds[i]] - verts[z = inds[i+2]]);
        QVector3D v(verts[y = inds[i+1]] - verts[inds[i]]);

        float u1 = Texture[i].x();
        float v1 = Texture[i].y();
        float u2 = Texture[i+1].x();
        float v2 = Texture[i+1].y();
        float u3 = Texture[i+2].x();
        float v3 = Texture[i+2].y();

        QVector3D resp = ((v2-v1)*u-(v1-v3)*v)/((u1-u3)*(v2-v1)-(v1-v3)*(u2-u1));

        tan[x] += resp;
        tan[y] += resp;
        tan[z] += resp;
    }
    for (int i = 0; i < vertCount; ++i) {
        tan[i].normalize();
    }
    for (int i = 0; i < indSize; ++i) {
        tangents[i] = tan[inds[i]];
    }
    free(tan);

    vertexCount_ = indSize;
    normalCount_ = indSize;
    sg_vertexes_ = vertices;
    sg_tangent_ = tangents;
    sg_texture_ = Texture;
    sg_normals_ = normals;

    m_vertex.allocate(sg_vertexes_, vertexCount_*sizeof(QVector3D));

    float max = max_w - min_w;
    if (max < max_h - min_h)
        max = max_h - min_h;

    m_transform.translate(0.0f, 0.0f, -actual +max*factor);
    actual = max*factor;

    m_object.release();
    m_vertex.release();

    if (shading == 0)
        changeToFlat();
    else if (shading == 1)
        changeToMean();
    else
        changeToNorm();

    m_object.bind();
    m_tex.bind();
    m_tex.allocate(sg_texture_, vertexCount_*sizeof(QVector2D));
    m_tex.release();
    m_object.release();

    m_object.bind();
    m_tan.bind();
    m_tan.allocate(sg_tangent_, vertexCount_*sizeof(QVector3D));
    m_tan.release();
    m_object.release();

    m_program->release();
}

GLWidget::~GLWidget() {
    cleanup();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(200, 200);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::cleanup() {
    makeCurrent();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void GLWidget::connectUpdate() {
  m_connection = connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  update();
}

void GLWidget::disconnectUpdate() {
  disconnect(m_connection);
}


void initTex (QString url) {
    texture = new QOpenGLTexture (QImage (url).mirrored());

    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::MirroredRepeat);

}

void GLWidget::loadTexture (QString file) {
    delete texture;
    QString url = (file);
    initTex (url);
}

void GLWidget::initializeGL() {

  QString url = ("");

  vLightPosition = QVector3D (0.0f, 4.0f, 5.0f);

  initTex(url);

  m_camera.translate(QVector3D(0.0f, 0.0f, 2.0f));
  // Put the object a little to the front of the camera (the camera is looking
  // at (0.0, 0.0, -1.0))
  actual = factor = -5;
  m_transform.translate(0.0f, 0.0f, -5.0f);
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
  connectUpdate();
  printContextInformation();
  // Set global information
  glEnable(GL_DEPTH_TEST);
  glDepthRange(0,1);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BP_interp_sd.vert");
    shader = new QOpenGLShader(QOpenGLShader::Fragment);
    shader->compileSourceFile(":/shaders/BP_interp_sd.frag");
    m_program->addShader(shader);
    //m_program->bindAttributeLocation("texCoord", 2);
    m_program->link();
    m_program->bind();

    // Cache Uniform Locations
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");
    u_lightPosition = m_program->uniformLocation("lightPosition");
    u_difuseColor= m_program->uniformLocation("diffuseProduct");
    u_ambientColor= m_program->uniformLocation("ambientProduct");
    u_specularColor= m_program->uniformLocation("specularProduct");
    u_shininess= m_program->uniformLocation("shininess");
    u_albedo= m_program->uniformLocation("albedo");
    u_roughness= m_program->uniformLocation("roughness");
    u_bumpness= m_program->uniformLocation("bumpness");
    u_displacement= m_program->uniformLocation("displacement");
    m_normalAttr = m_program->attributeLocation("vNormal");
    m_texAttr = m_program->attributeLocation("texCoord");
    m_tanAttr = m_program->attributeLocation("vTangent");

    // Create Buffer (Do not release until VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes_, vertexCount_*sizeof(QVector3D));

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();

    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(0);

    // Release (unbind) all
    m_object.release();

    m_object.bind();
    m_normal.create();
    m_normal.bind();
    m_normal.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_program->setAttributeBuffer(1, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(1);

    m_object.release();

    m_object.bind();
    m_tex.create();
    m_tex.bind();
    m_tex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_tex.allocate(sg_texture_, vertexCount_*sizeof(QVector2D));

    m_program->setAttributeBuffer(2, GL_FLOAT, 0, 2,0);
    m_program->enableAttributeArray(2);

    m_object.release();

    m_object.bind();
    m_tan.create();
    m_tan.bind();
    m_tan.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_tan.allocate(sg_tangent_, vertexCount_*sizeof(QVector3D));

    m_program->setAttributeBuffer(3, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(3);

    m_object.release();

    m_vertex.release();
    m_normal.release();
    m_tex.release();
    m_tan.release();
    m_program->release();
  }
}

void GLWidget::update() {
  Input::update();
  if (Input::buttonPressed(Qt::LeftButton)) {
    static const float rotSpeed   = 0.75f;
    m_transform.rotate(rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    m_transform.rotate(rotSpeed * Input::mouseDelta().y(), Camera3D::LocalRight);
  }
  // Schedule a redraw
  QOpenGLWidget::update();
}

void GLWidget::switchMappig (QString shad) {
    QString pref = QString (":/shaders/").append(shad);
    switch (textureMapping) {
    case 0:
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, pref.append("_sd.vert"));
        pref = QString (":/shaders/").append(shad);
        shader->compileSourceFile(pref.append("_sd.frag"));
        break;
    case 1:
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, pref.append("_bp.vert"));
        pref = QString (":/shaders/").append(shad);
        shader->compileSourceFile(pref.append("_bp.frag"));
        break;
    case 2:
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, pref.append("_dp.vert"));
        pref = QString (":/shaders/").append(shad);
        shader->compileSourceFile(pref.append("_dp.frag"));
        break;
    default:
        break;
    }
}

void GLWidget::reload_shader () {
    m_program = new QOpenGLShaderProgram();

    if (reflectmodel == 0) {
        switch (shading) {
        case 0:
            switchMappig(QString("BP_interp"));
            break;
        case 1:
            switchMappig(QString("BP_noInterp"));
            break;
        case 2:
            switchMappig(QString("BP_interp"));
            break;
        default:
            break;
        }
    }
    else if (reflectmodel == 1) {
        switch (shading) {
        case 0:
            switchMappig(QString("ON_interp"));
            break;
        case 1:
            switchMappig(QString("ON_noInterp"));
            break;
        case 2:
            switchMappig(QString("ON_interp"));
            break;
        default:
            break;
        }
    }

    m_program->addShader(shader);
    m_program->link();
    m_program->bind();

    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");
    u_lightPosition = m_program->uniformLocation("lightPosition");
    u_difuseColor= m_program->uniformLocation("diffuseProduct");
    u_ambientColor= m_program->uniformLocation("ambientProduct");
    u_specularColor= m_program->uniformLocation("specularProduct");
    u_shininess= m_program->uniformLocation("shininess");
    u_albedo= m_program->uniformLocation("albedo");
    u_roughness= m_program->uniformLocation("roughness");
    u_bumpness= m_program->uniformLocation("bumpness");
    u_displacement= m_program->uniformLocation("displacement");
    m_normalAttr = m_program->attributeLocation("vNormal");
    m_texAttr = m_program->attributeLocation("texCoord");
    m_tanAttr = m_program->attributeLocation("vTangent");

    m_program->release();
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (changed) {
    reload_shader();
    changed = false;
  }

  m_program->bind();

  m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());

  m_program->setUniformValue(u_lightPosition, vLightPosition.toVector4D());
  m_program->setUniformValue(u_cameraToView, m_projection);
  m_program->setUniformValue(u_difuseColor, difusiveColor);
  m_program->setUniformValue(u_ambientColor, ambientColor);
  m_program->setUniformValue(u_specularColor, specularColor);
  m_program->setUniformValue(u_shininess, shininess);
  m_program->setUniformValue(u_albedo, albedo);
  m_program->setUniformValue(u_roughness, roughness);
  m_program->setUniformValue(u_bumpness, bumpness);
  m_program->setUniformValue(u_displacement, displacement);
  m_program->setUniformValue("texture", 0);
  {
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    m_object.release();
  }
  m_program->release();
}

void GLWidget::resizeGL(int w, int h) {
  m_projection.setToIdentity();
  m_projection.perspective(30.0f, w / float(h), 1.0f, 1000.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  Input::registerMouseRelease(event->button());
}

void GLWidget::printContextInformation() {
  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << "Context valid: " << context()->isValid();
  qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
  qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
  qDebug() << "                    RENDERER:    " << (const char*)glGetString(GL_RENDERER);
  qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
  qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}
