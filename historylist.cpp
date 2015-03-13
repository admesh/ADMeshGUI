#include "historylist.h"

historyList::historyList()
{
    current_index = -1;
    max_index = -1;
}

historyList::~historyList()
{

}

void historyList::add(vector <MeshObject*> item)
{
    if(current_index != max_index){ // not on the end of history
        cutRedos();
    }
    history.push_back(item);
    current_index++;
    max_index++;
}

vector <MeshObject*> historyList::current()
{
    return history[current_index];
}

vector <MeshObject*> historyList::undo()
{
    if(current_index == 0){
        return current();
    }else{
        --current_index;
        return current();
    }
}

vector <MeshObject*> historyList::redo()
{
    if(current_index < max_index) ++current_index;
    return current();
}

void historyList::deleteRow(vector <vector <MeshObject*> >::size_type index)
{
    for(vector<MeshObject*>::size_type i = 0; i < history[index].size();i++){
        if(history[index][i]->hasReferences()){
            history[index][i]->removeReference();
        }else{
            //history[index].erase(history[index].begin() + i);
            delete history[index][i];
        }
    }
}

void historyList::cutOldest()
{

}

void historyList::cutRedos()
{
    while(max_index != current_index){
        deleteRow(max_index);
        history.pop_back();
        --max_index;
    }
}
