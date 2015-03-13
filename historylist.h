#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <vector>
#include "data.h"
#include "meshobject.h"

using namespace std;

class historyList
{
public:
    historyList();
    ~historyList();
    void add(vector <MeshObject*> item);
    void deleteRow(vector <vector <MeshObject*> >::size_type index);
    vector <MeshObject*> current();
    vector <MeshObject*> undo();
    vector <MeshObject*> redo();
    void cutOldest();
    void cutRedos();
private:
    vector <vector <MeshObject*> > history;
    vector <vector <MeshObject*> >::size_type current_index;
    vector <vector <MeshObject*> >::size_type max_index;
};

#endif // HISTORYLIST_H
