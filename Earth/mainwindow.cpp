#include "mainwindow.hpp"

#include <QEvent>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) : QOpenGLWidget(parent) {}

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();

    controller = std::make_unique<Controller>();
    program = std::make_unique<QOpenGLShaderProgram>(this);

    const char* vertex_path = ":/shaders/vertex.vsh";
    const char* fragment_path = ":/shaders/fragment.fsh";

    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_path);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_path);
    program->link();

    posAttr = program->attributeLocation("posAttr");
    norAttr = program->attributeLocation("norAttr");
    texAttr = program->attributeLocation("texAttr");
    tanAttr = program->attributeLocation("tanAttr");
    bitanAttr = program->attributeLocation("bitanAttr");

    Q_ASSERT(posAttr != -1);
    Q_ASSERT(norAttr != -1);
    Q_ASSERT(texAttr != -1);
    Q_ASSERT(tanAttr != -1);
    Q_ASSERT(bitanAttr != -1);

    initTextures();
    initSphere(1.0f);
    //initCube(0.5f, 10);

    glClearColor(0.0f, 0.0f, 0.0f, 0.8f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    timer.start(30, this);
}

void MainWindow::paintGL()
{
    glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, (GLfloat)width() / height(), 0.1f, 100.0f);

    QMatrix4x4 viewMatrix;
    viewMatrix.translate(0.0, 0.0, -1.5);
    viewMatrix.setToIdentity();
    viewMatrix.rotate(controller->getRotation());
    viewMatrix.scale(0.7f, 0.7f, 0.7f);

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0, 0.0, -1.25);
    modelMatrix.rotate(controller->getRotation());
    modelMatrix.scale(0.7f, 0.7f, 0.7f);

    program->setUniformValue("viewMatrix", viewMatrix);
    program->setUniformValue("modelMatrix", modelMatrix);
    program->setUniformValue("projectionMatrix", projectionMatrix);
    program->setUniformValue("normalMatrix", modelMatrix.normalMatrix());
    program->setUniformValue("lightPos", QVector3D(0, 0, 2.0));
    program->setUniformValue("viewPos", QVector3D(0.0, 0.0, 0.0));

    texture->bind(0);
    program->setUniformValue("texture", 0);
    map->bind(1);
    program->setUniformValue("map", 1);

    vertexBuffer.bind();

    int offset = 0;
    program->enableAttributeArray(posAttr);
    program->setAttributeBuffer(posAttr, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    program->enableAttributeArray(norAttr);
    program->setAttributeBuffer(norAttr, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    program->enableAttributeArray(texAttr);
    program->setAttributeBuffer(texAttr, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);
    program->enableAttributeArray(tanAttr);
    program->setAttributeBuffer(tanAttr, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    program->enableAttributeArray(bitanAttr);
    program->setAttributeBuffer(bitanAttr, GL_FLOAT, offset, 3, sizeof(VertexData));

    indexBuffer.bind();
    context()->functions()->glDrawElements(GL_TRIANGLES, indexBuffer.size(),
                                           GL_UNSIGNED_INT, nullptr);

    program->disableAttributeArray(posAttr);
    program->disableAttributeArray(norAttr);
    program->disableAttributeArray(texAttr);
    program->disableAttributeArray(tanAttr);
    program->disableAttributeArray(bitanAttr);
    program->release();

    ++frames;
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    controller->mousePressEvent(e);
    QWidget::mousePressEvent(e);
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    controller->mouseMoveEvent(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    controller->mouseMoveEvent(e);
    update();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    update();
}

void MainWindow::initTextures()
{
    const char* tex_path = ":/textures/textures.jpg";
    const char* map_path = ":/textures/map.jpg";

    texture = std::make_unique<QOpenGLTexture>(QImage(tex_path).mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    map = std::make_unique<QOpenGLTexture>(QImage(map_path).mirrored());
    map->setMinificationFilter(QOpenGLTexture::Nearest);
    map->setMagnificationFilter(QOpenGLTexture::Linear);
    map->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWindow::initCube(float halfWidth, int N)
{
    Q_ASSERT(halfWidth > 0);
    Q_ASSERT(N > 1);

    auto vertex_step = halfWidth * 2 / float(N - 1);
    auto texture_step = 1.0f / float(N - 1);

    std::vector<VertexData> vertices;

    for (auto z = -1; z <= 1; z += 2)
    {
        for (auto j = 0; j < N; j++)
        {
            for (auto i = 0; i < N; i++)
            {
                QVector3D pos(-halfWidth * z + i * vertex_step * z,
                              -halfWidth + j * vertex_step,
                               halfWidth * z);
                QVector3D nor(0.0f, 0.0f, float(z));
                QVector2D tex(i * texture_step, j * texture_step);
                QVector3D tan(float(z), 0.0f, 0.0f);
                QVector3D bitan(0.0f, 1.0f, 0.0f);

                vertices.push_back(VertexData(pos, nor, tex, tan, bitan));
            }
        }
    }

    for (auto x = -1; x <= 1; x += 2)
    {
        for (auto k = 0; k < N; k++)
        {
            for (auto j = 0; j < N; j++)
            {
                QVector3D pos(halfWidth * x,
                             -halfWidth + j * vertex_step,
                             -halfWidth * x + k * vertex_step * x);
                QVector3D nor(float(x), 0.0f, 0.0f);
                QVector2D tex(j * texture_step, k * texture_step);
                QVector3D tan(0.0f, 1.0f, 0.0f);
                QVector3D bitan(0.0f, 0.0f, float(x));

                vertices.push_back(VertexData(pos, nor, tex, tan, bitan));
            }
        }
    }

    for (auto y = -1; y <= 1; y += 2)
    {
        for (auto i = 0; i < N; i++)
        {
            for (auto k = 0; k < N; k++)
            {
                QVector3D pos(-halfWidth + i * vertex_step,
                               halfWidth * y,
                              -halfWidth * y + k * vertex_step * y);
                QVector3D nor(0.0f, float(y), 0.0f);
                QVector2D tex(k * texture_step, i * texture_step);
                QVector3D tan(0.0f, float(y), 0.0f);
                QVector3D bitan(1.0f, 0.0f, 0.0f);

                vertices.push_back(VertexData(pos, nor, tex, tan, bitan));
            }
        }
    }

    std::vector<GLuint> indices;

    for (int i = 0; i < 6 * N * N; i += N * N)
    {
        for (int j = 0; j < (N - 1) * (N - 1); j += N)
        {
            for (int k = 0; k < (N - 1); k++)
            {
                indices.push_back(i + j + k + N);
                indices.push_back(i + j + k + 0);
                indices.push_back(i + j + k + N + 1);
                indices.push_back(i + j + k + N + 1);
                indices.push_back(i + j + k + 0);
                indices.push_back(i + j + k + 1);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(VertexData)));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));
}

void MainWindow::initSphere(float rad)
{
    uint lats = 100;
    uint longs = 100;

    float lat_step = 2 * M_PI / float(lats);
    float long_step = M_PI / float(longs);

    std::vector<VertexData> vertices;
    for (uint i = 0; i <= longs; ++i)
    {
        float stack_angle = M_PI / 2 - float(i) * long_step;
        float xy = rad * cos(stack_angle);
        float z = rad * sin(stack_angle);

        for (uint j = 0; j <= lats; ++j)
        {
            float sector_angle = float(j) * lat_step;
            float x = xy * cos(sector_angle);
            float y = xy * sin(sector_angle);

            QVector3D pos(x, y, z);
            QVector3D nor(x / rad, y / rad, z / rad);
            QVector2D tex(float(j) / lats, float(i) / longs);
            QVector3D tan(1, 1, 1);
            QVector3D bitan(y / rad - z / rad, z / rad - x / rad, x / rad - y / rad);

            vertices.push_back(VertexData(pos, nor, tex, tan, bitan));
        }
    }

    std::vector<GLuint> indices;
    for (uint i = 0; i < lats; ++i)
    {
        uint k1 = i * (longs + 1);
        uint k2 = k1 + longs + 1;

        for (uint j = 0; j < longs; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (lats - 1))
            {
                indices.push_back(k1 + 1u);
                indices.push_back(k2);
                indices.push_back(k2 + 1u);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(VertexData)));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));
}
