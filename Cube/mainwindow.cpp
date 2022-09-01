#include "mainwindow.hpp"

static const GLuint vertex_position_offset  = 0;
static const GLuint vertex_normal_offset    = 3;
static const GLuint vertex_color_offset     = 6;
static const GLuint stride                  = 9;

static const char* phong_vertex = ":/shaders/phong.vsh";
static const char* phong_fragment = ":/shaders/phong.fsh";
static const char* gouraud_vertex = ":/shaders/gouraud.vsh";
static const char* gouraud_fragment = ":/shaders/gouraud.fsh";

// initialization //

void MainWindow::initShaders()
{
    program->removeAllShaders();

    if (controller->phongShaderIsActive())
    {
        program->addShaderFromSourceFile(QOpenGLShader::Vertex, phong_vertex);
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, phong_fragment);
    }
    else
    {
        program->addShaderFromSourceFile(QOpenGLShader::Vertex, gouraud_vertex);
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, gouraud_fragment);
    }

    if (!program->link() || !program->bind())
    {
        close();
    }
}

void MainWindow::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    const QVector4D clearColor{0.25f, 0.25f, 0.375f, 1.0f};
    glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w());

    const GLfloat halfWidth = 0.5f;
    const GLuint numParts = 10;
    cube.reshape(halfWidth, numParts);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(cube.get_vertices().data(),
                          cube.get_vertices_size() * sizeof(GLfloat));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(cube.get_indices().data(),
                         cube.get_indices_size() * sizeof(GLuint));

    controller = std::make_unique<Controller>();
    program = std::make_unique<QOpenGLShaderProgram>(this);

    initShaders();

    int posAttr = program->attributeLocation("vertex_position");
    int norAttr = program->attributeLocation("vertex_normal");
    int colAttr = program->attributeLocation("vertex_color");

    program->setAttributeBuffer(posAttr, GL_FLOAT, vertex_position_offset, 3,
                                stride * sizeof(GLfloat));
    program->setAttributeBuffer(norAttr, GL_FLOAT, vertex_normal_offset, 3,
                                stride * sizeof(GLfloat));
    program->setAttributeBuffer(colAttr, GL_FLOAT, vertex_color_offset, 3,
                                stride * sizeof(GLfloat));

    timer.start();
}

// rendering //

void MainWindow::render()
{
    // new color
    controller->update();
    bool newColorSelected = controller->newColorSelected();

    vertexBuffer.bind();
    if (newColorSelected)
    {
        cube.changeColor(controller->getColor());
        vertexBuffer.allocate(cube.get_vertices().data(),
                              cube.get_vertices_size() * sizeof(GLfloat));
    }
    indexBuffer.bind();

    // new num cubes
    numCubes += controller->newNumCubes();
    if (numCubes <= 0)
    {
        numCubes = 1;
    }
    else if (numCubes > 200)
    {
        numCubes = 200;
    }

    // rendering
    glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    initShaders();

    QMatrix4x4 view_matrix;
    view_matrix.translate(0.0F, 0.0F, -5.0F);
    view_matrix.rotate(controller->getRotation());
    program->setUniformValue("view_matrix", view_matrix);

    QMatrix4x4 projection_matrix;
    projection_matrix.perspective(60.0f, (GLfloat)width() / height(), 0.1f, 100.0f);
    program->setUniformValue("projection_matrix", projection_matrix);

    // morphing

    if (controller->morphingIsActive())
    {
        GLfloat time = static_cast<GLfloat>(morphingCounter);
        GLfloat factor = (sin(time / 30.0f) + 1.0f) / 2.0f;
        program->setUniformValue("factor", factor);
    }
    else
    {
        program->setUniformValue("factor", 1.0f);
    }

    program->setUniformValue("light_position", QVector3D{6.5f, 0.0f, 1.5f});

    int posAttr = program->attributeLocation("vertex_position");
    int norAttr = program->attributeLocation("vertex_normal");
    int colAttr = program->attributeLocation("vertex_color");

    program->setAttributeBuffer(posAttr, GL_FLOAT, vertex_position_offset * sizeof(GLfloat),
                                3, stride * sizeof(GLfloat));
    program->setAttributeBuffer(norAttr, GL_FLOAT, vertex_normal_offset * sizeof(GLfloat),
                                3, stride * sizeof(GLfloat));
    program->setAttributeBuffer(colAttr, GL_FLOAT, vertex_color_offset * sizeof(GLfloat),
                                3, stride * sizeof(GLfloat));

    program->enableAttributeArray(posAttr);
    program->enableAttributeArray(norAttr);
    program->enableAttributeArray(colAttr);

    for (int i = 0; i < numCubes; ++i)
    {
        for (int j = 0; j < numCubes; ++j)
        {
            vertexBuffer.bind();
            indexBuffer.bind();

            QMatrix4x4 modelMatrix;
            modelMatrix.translate(-1.5f + i * 2.0f, -1.5f + j * 2.0f, 0.0f);
            program->setUniformValue("model_matrix", modelMatrix);

            glDrawElements(GL_QUAD_STRIP, cube.get_vertices_size(),
                           GL_UNSIGNED_INT, nullptr);
        }
    }

    program->disableAttributeArray(colAttr);
    program->disableAttributeArray(norAttr);
    program->disableAttributeArray(posAttr);

    program->release();

    ++morphingCounter;

    showFPS();
}

// events //

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    controller->mousePressEvent(e);
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    controller->mouseMoveEvent(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    controller->mouseReleaseEvent(e);
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    controller->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    controller->keyReleaseEvent(e);
}

// fps counter //

void MainWindow::showFPS()
{
    ++frames;
    currentTime = float(timer.elapsed()) / 1000;
    if(currentTime - lastTime > 1.0f)
    {
        fps = frames;
        lastTime = currentTime;
        frames = 0;
    }
    std::cout << "FPS: " << fps << std::endl;
}
