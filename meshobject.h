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
     * \brief Get vector [min x, min y, max z] for camera purposes.
     * \return vector of mininaml values.
     */
    QVector3D getMin();

    /*!
     * \brief Get info about mesh.
     * \return string containing formated info.
     */
    QString getInfo();

    /*!
     * \brief Save mesh as STL.
     * \param filename Output STL filename.
     * \param type File type - ASCII or binary
     */
    void saveAs(char* filename, int type);

    /*!
     * \brief Export mesh as OFF, VRML, DXF or OBJ.
     * \param filename Exported filename.
     * \param type File File type.
     */
    void exportSTL(char* filename, int type);

    /*!
     * \brief Scale stl file.
     * \param factor Scaling factor.
     */
    void scale(float factor);

    /*!
     * \brief Scale stl file by versor.
     * \param versor Scaling versor.
     */
    void scale(float versor[3]);

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
     * \brief Repair stl file. Repair differs with parameters.
     * \param fixall_flag Fix all errors in STL file.
     * \param tolerance_flag Used tolerance for checking faces.
     * \param tolerance Y Given tolerance.
     * \param increment_flag Used tolerance increment in iterations.
     * \param increment Given increment.
     * \param nearby_flag Check for nearby faces.
     * \param iterations Number of iterations used for checking.
     * \param remove_unconnected_flag Remove unconnected vertices.
     * \param fill_holes_flag Fill holes.
     * \param normal_directions_flag Repair normal directions. All out.
     * \param normal_values_flag Repair normal values. Normalization.
     * \param reverse_all_flag Reverse all faces.
     */
    void repair(int fixall_flag, int exact_flag, int tolerance_flag, float tolerance, int increment_flag, float increment, int nearby_flag, int iterations, int remove_unconnected_flag, int fill_holes_flag, int normal_directions_flag, int normal_values_flag, int reverse_all_flag);

    /*!
     * \brief Destructor. Closes stl structure. Deletes vbo;
     */
    virtual ~MeshObject();

private:
    stl_file* stl;  ///< Stored STL file
    GLuint vbo;     ///< Vertex buffer object used to store STL file for drawing
};

#endif // MESHOBJECT_H
