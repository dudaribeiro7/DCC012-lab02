#ifndef SORT_H__
#define SORT_H__


template <typename T>
void bubble_sort(T* array, int size, bool (*compare)(T&, T&))  
{
    // TODO: Implementação do Bubble Sort
    for(int i=size-1; i>=1; i--){
        for(int j=1; j<=i; j++){
            if(compare(array[j+1], array[j])){
                T aux = array[j];
                array[j] = array[j+1];
                array[j+1] = aux;
            }
        }
    }
}

template <typename T>
void insert_sort(T* array, int size,  bool (*compare)(T&, T&))
{
    // TODO: Implementação do Insert Sort 
    T pivo;
    int i;
    for(int j=2; j<=size; j++){
        pivo = array[j];
        i = j-1;
        while((i>0)&&(compare(pivo, array[i]))){
            array[i+1] = array[i];
            i--;
        }
        array[i+1] = pivo;
    }
}


template <typename T>
void selection_sort(T* array, int size, bool (*compare)(T&, T&))
{
    // TODO: Implementação do selection Sort
    int min;
    for(int i=1; i<=size-1; i++){
        min = i;
        for(int j=i+1; j<=size; j++){
            if(compare(array[j], array[min])){
                min = j;
            }
        }
        T aux = array[i];
        array[i] = array[min];
        array[min] = aux;
    }
}


#endif /* SORT_H__ */
