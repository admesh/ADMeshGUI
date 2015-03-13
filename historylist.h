#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <vector>
#include "data.h"
#include "meshobject.h"

using namespace std;

class historyList
{
public:
    /*!
     * \brief Default constructor.
     *
     * Pushes one emtpy item at index 0 - better further manipulation.
     */
    historyList();

    /*!
     * \brief Destructor
     *
     * Cleans whole history.
     */
    ~historyList();

    /*!
     * \brief Add item into history
     * \param item Objects in current state to add to history.
     */
    void add(vector <MeshObject*> item);

    /*!
     * \brief Delete history item given by index
     * \param index Index of item to delete.
     */
    void deleteRow(vector <vector <MeshObject*> >::size_type index);

    /*!
     * \brief Returns item under current_index
     * \return Current item.
     */
    vector <MeshObject*> current();

    /*!
     * \brief Returns item one position back in history.
     * \return Previous item.
     */
    vector <MeshObject*> undo();

    /*!
     * \brief Returns item one position further in history.
     * \return Next item.
     */
    vector <MeshObject*> redo();

    /*!
     * \brief Delete oldest entries in history.
     *
     * Counted references of MeshObjects taken into account.
     */
    void cutOldest();

    /*!
     * \brief Delete "youngest" entries in history (which were undoed).
     *
     * Called during adding phase if current_index is not last. Newly added item replaces all items which were undoed.
     * Counted references of MeshObjects taken into account.
     */
    void cutRedos();
private:
    vector <vector <MeshObject*> > history;                     ///< Editation history list
    vector <vector <MeshObject*> >::size_type current_index;    ///< Current index position
    vector <vector <MeshObject*> >::size_type max_index;        ///< Maximum index position
};

#endif // HISTORYLIST_H
