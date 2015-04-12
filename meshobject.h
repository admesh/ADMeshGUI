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
     * \brief Constructor. Initializes new stl structure.
     */
    MeshObject();

    /*!
     * \brief Constructor. Initializes with given stl structure.
     *
     * \param item Given stl_file item.
     */
    MeshObject(stl_file* item);

    /*!
     * \brief Copy constructor. Creates deep copy of MeshObject.
     */
    MeshObject(const MeshObject& m);

    /*!
     * \brief Read in given file.
     *
     * Checks if file exist/is proper STL file.
     * Generates vertex buffer object.
     *
     * \param filename Input STL file.
     * \return True upon successful load.
     */
    bool loadGeometry(QString filename);

    /*!
     * \brief Append "_merged" to filename.
     */
    void mergedFilename();

    /*!
     * \brief Set filename to "filename_part_index.stl".
     *
     * \param index Given index.
     */
    void setSplitName(int index);

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
     * \brief Get maximal diameter of last object in list. Maximal absolute value from all axes.
     *
     * \return diameter
     */
    float getDiameter();

    /*!
     * \brief Get info about mesh.
     * \return array containing info.
     */
    float* getInfo();

    QString getName();

    /*!
     * \brief Save mesh as STL.
     * \param filename Output STL filename.
     * \param type File type - ASCII or binary
     */
    void saveAs(QString filename, int type);

    /*!
     * \brief Save mesh in its default format.
     */
    void save();

    /*!
     * \brief True if stored filename is valid.
     */
    bool hasValidName();

    /*!
     * \brief True if current state of mesh is saved.
     */
    bool isSaved();

    /*!
     * \brief Export mesh as OFF, VRML, DXF or OBJ.
     * \param filename Exported filename.
     * \param type File File type.
     */
    void exportSTL(QString, int type);

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
     * \brief Reverse all facets orientation.
     */
    void reverseAll();

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
     * \brief Set selected.
     */
    void setSelected();

    /*!
     * \brief Set deselected.
     */
    void setDeselected();

    /*!
     * \brief Toggle selected.
     */
    void toggleSelected();

    /*!
     * \brief Is selected?
     *
     * \return true upon selected
     */
    bool isSelected();

    /*!
     * \brief Is active?
     *
     * \return true upon selected and visible
     */
    bool isActive();

    /*!
     * \brief Hide item
     */
    void setHidden();

    /*!
     * \brief Reveal hidden item
     */
    void setVisible();

    /*!
     * \brief Is hidden?
     *
     * \return true upon hidden
     */
    bool isHidden();

    /*!
     * \brief Destructor. Closes stl structure. Deletes vbo.
     */
    virtual ~MeshObject();

    /*!
     * \brief Has references?
     *
     * return true upon references > 0
     */
    bool hasReferences();

    /*!
     * \brief Raise references count by 1.
     */
    void addReference();

    /*!
     * \brief Decrease references count by 1.
     */
    void removeReference();

    /*!
     * \brief Get pointer to stl_file structure stored.
     *
     * return stored stl file structure
     */
    stl_file* getStlPointer();

    unsigned long getSize();

private:
    void countSize();

    stl_file* stl;  ///< Stored STL file
    GLuint vbo;     ///< Vertex buffer object used to store STL file for drawing
    bool selected;    ///< Set active/inactive
    bool hidden;    ///< Is hidden/visible
    bool saved;     ///< Is saved/unsaved
    QString file;     ///< File name
    unsigned int references;    ///< Count if references - how many history items use this object.
    unsigned long size;
};

#endif // MESHOBJECT_H
