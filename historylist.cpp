#include "historylist.h"

historyList::historyList()
{
    current_index = 0;
    max_index = 0;
    history.push_back(QList<MeshObject*>());
}

historyList::~historyList()
{
    while(max_index != 0){
        for(QList<MeshObject*>::size_type i = 0; i < history[max_index].size();i++){
            if(history[max_index][i]->hasReferences()){
                history[max_index][i]->removeReference();
            }else{
                delete history[max_index][i];
                history[max_index][i]=NULL;
            }
        }
        history.pop_back();
        --max_index;
    }
}

void historyList::add(QList <MeshObject*> item)
{
    if(current_index != max_index){ // not on the end of history
        cutRedos();
    }
    history.push_back(item);
    current_index++;
    max_index++;
}

QList <MeshObject*> historyList::current()
{
    return history[current_index];
}

QList <MeshObject*> historyList::undo()
{
    if(current_index == 0 || current_index == 1){
        return current();
    }else{
        --current_index;
        return current();
    }
}

QList <MeshObject*> historyList::redo()
{
    if(current_index < max_index) ++current_index;
    return current();
}

void historyList::deleteRow(QList <QList <MeshObject*> >::size_type index)
{
    for(QList<MeshObject*>::size_type i = 0; i < history[index].size();i++){
        if(history[index][i]->hasReferences()){
            history[index][i]->removeReference();
        }else{
            delete history[index][i];
            history[index][i]=NULL;
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
