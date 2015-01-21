#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <QGLFunctions>
#include <QGLShaderProgram>
#include "data.h"

/*!
 * \brief Encapsulation of stl_file structure of ADMesh
 *
 * Stores one STL file and according vertex buffer object.
 * stl_file structure allows directly only deprecated style of OpenGL drawing.
 * This class enables to use modern drawing style via vertex buffer object.
 */
class MeshObject : protected QGLFunctions
{
public:
    /*!
     * \brief Constructor. Initializes stl structure.
     */
    MeshObject();

    /*!
     * \brief Read in given file.
     *
     * Checks if file exist/is proper STL file.
     * Generates vertex buffer object.
     *
     * \param filename Input STL file.
     * \return True upon successful load.
     */
    bool loadGeometry(char* filename);

    /*!
     * \brief Update geometry (update VBO)
     *
     * Copies vertex and normal coordinates from stl_file structure into VBO.
     */
    void updateGeometry();

    /*!
     * \brief Draw mesh.
     * \param program Shader program used for drawing.
     */
    void drawGeometry(QGLShaderProgram *program);

    /*!
     * \brief Scale stl file.
     * \param factor Scaling factor.
     */
    void scale(float factor);

    /*!
     * \brief Mirror model by XY plane.
     */
    void mirrorXY();

    /*!
     * \brief Mirror model by YZ plane.
     */
    void mirrorYZ();

    /*!
     * \brief Mirror model by XZ plane.
     */
    void mirrorXZ();

    /*!
     * \brief Rotate stl file along X axis.
     * \param angle Angle of rotation.
     */
    void rotateX(float angle);

    /*!
     * \brief Rotate stl file along Y axis.
     * \param angle Angle of rotation.
     */
    void rotateY(float angle);

    /*!
     * \brief Rotate stl file along Z axis.
     * \param angle Angle of rotation.
     */
    void rotateZ(float angle);

    /*!
     * \brief Translate stl file. All axes together.
     * \param relative Relative translation on/off.
     * \param x_trans X axis translation factor.
     * \param y_trans Y axis translation factor.
     * \param z_trans Z axis translation factor.
     */
    void translate(bool relative, float x_trans, float y_trans, float z_trans);

    /*!
     * \brief Destructor. Closes stl structure. Deletes vbo;
     */
    virtual ~MeshObject();

private:
    stl_file* stl;  ///< Stored STL file
    GLuint vbo;     ///< Vertex buffer object used to store STL file for drawing
};

#endif // MESHOBJECT_H
