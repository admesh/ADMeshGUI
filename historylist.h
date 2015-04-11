#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <QList>
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
     * \brief Set limit size of buffer.
     *
     * \param limit Limit size of buffer in bytes.
     */
    void setLimitSize(unsigned long long limit);

    /*!
     * \brief Add item into history
     * \param item Objects in current state to add to history.
     */
    void add(QList <MeshObject*> item, unsigned long size);

    /*!
     * \brief Delete history item given by index
     * \param index Index of item to delete.
     */
    void deleteRow(QList <QList <MeshObject*> >::size_type index);

    /*!
     * \brief Returns item under current_index
     * \return Current item.
     */
    QList <MeshObject*> current();

    /*!
     * \brief Returns item one position back in history.
     * \return Previous item.
     */
    QList <MeshObject*> undo();

    /*!
     * \brief Returns item one position further in history.
     * \return Next item.
     */
    QList <MeshObject*> redo();

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
    QList <QList <MeshObject*> > history;                     ///< Editation history list
    QList <QList <MeshObject*> >::size_type current_index;    ///< Current index position
    QList <QList <MeshObject*> >::size_type max_index;        ///< Maximum index position
    unsigned long long historySize;
    unsigned long long sizeLimit;
};

#endif // HISTORYLIST_H
