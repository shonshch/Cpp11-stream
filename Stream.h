//
// Created by isr31 on 23/06/2018.
//

#ifndef STREAM_STREAM_H
#define STREAM_STREAM_H

#include <functional>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <map>
#include <list>
#include <set>

using namespace std;
template <typename T>
class Stream{
    function<vector<T*>()> clousre;


public:

    explicit Stream () = default;
    vector<T*> invoke(){
        return clousre();
    }

    void setClousre(function<vector<T*>()> newLambada){
        clousre = newLambada;
    }

    template<typename ContainerType>
    explicit Stream (ContainerType& container){
        clousre = [container]() -> vector<T*>{
            vector<T*> res;
            res.reserve(container.size());
            for (auto curr : container)
                res.push_back(curr);
            return res;
        };
    }
    template <typename W>
    explicit Stream(map<W,T*>& map){
        clousre = [map]() -> vector<T*> {
            vector<T*> res;
            res.reserve(map.size());
            for (auto it = map.begin(); it != map.end() ; ++it){
                res.push_back(it->second);
            }
            return res;
        };
    }

    template<typename ContainerType>
    static Stream of(ContainerType& container){
        return Stream(container);
    }

    template <typename W>
    static Stream of (map<W,T*> map){
        return Stream(map);
    }


    template <typename U>
    U collect(){
        U copyTo;
        vector<T*> copyFrom = clousre();
        std::copy(copyFrom.begin(),copyFrom.end(),inserter(copyTo,copyTo
                .begin()));
        return copyTo;
    }


    void forEach(std::function<void(const T*)> forFunction){
        auto prevector = this->invoke();
        for (auto curr : prevector){
            forFunction(curr);
        }

    }

    T* min(){
        auto preVector = this->invoke();
        return *min_element(preVector.begin(),preVector.end(),
                           [](T* a,
                              T* b)->bool {return  *a < * b;});
    }

    T* max(){
        auto preVector = invoke();
        return *max_element(preVector.begin(),preVector.end(),[](T* a,
        T* b)->bool
        {
            if (*b < *a) {
                return false;
            } else if(*a == *b) return false;
            else return true;
        });
    }

    int count(){
        auto preVector = this->invoke();
        return preVector.size();
    }


    bool anyMatch(const function<bool(const T*)> matchFunction){
        auto preVector = this->invoke();
        return any_of(preVector.begin(),preVector.end(),matchFunction);
    }


    bool allMatch(const function<bool(const T*)> matchFunction){
        auto preVector = this->invoke();
        return all_of(preVector.begin(),preVector.end(),matchFunction);
    }

    bool findIf(const function<bool(const T*)> firstFunction){
        auto preVector = this->invoke();
        return find_if(preVector.begin(),preVector.end(),firstFunction);
    }

    T* reduce(T* initial,function<T*(const T*,const T*)> foldFunction){
        auto preVector = this->invoke();
        return accumulate(preVector.begin(),preVector.end(),initial,
                          foldFunction);
    }

    T* findFirst(function<bool(const T*)> findFunction){
        auto preVector = this->invoke();
        for (auto it : preVector){
            if(findFunction(it))
                return it;
        }
        return nullptr;
    }

    Stream filter(const function<bool(const T*)> filterFunc){
        auto currentLambada = clousre;
        this->clousre= [currentLambada,filterFunc](){
                vector<T*> prevVector = currentLambada();
                vector<T*> newVector;
                copy_if(prevVector.begin(),prevVector.end()
                        ,back_inserter(newVector),filterFunc);
                return newVector;
            };
        return *this;
    }
    template <typename R>
    Stream<R> map(const function<R*(const T*)> map){
        auto prev = clousre;
        function<vector<R*>()> lambada = [prev,map](){
            auto prevector = prev();
            vector<R*> newVector;newVector.reserve(prevector.size());
            transform(prevector.begin(),prevector.end(),back_inserter(newVector)
                    ,map);
            return newVector;
        };
        Stream<R> s;
        s.setClousre(lambada);
        return s;
    }

    Stream distinct(){
        auto prev = clousre;
        this->clousre = [prev]() {
            auto prevector = prev();
            for (auto iterA = prevector.begin(); iterA != prevector.end();
                 iterA++)
            {
                for (auto iterB = iterA + 1; iterB != prevector.end(); iterB++)
                {
                    if(**iterB == **iterA)
                    {
                        prevector.erase(iterB);
                        iterB--;
                    }
                }
            }
            return prevector;
            };
        return *this;
    }

    Stream distinct(const function<bool(const T*,const T*)> compFunc){
        auto prev = clousre;
        this->clousre = [prev,compFunc]() {
            auto prevector = prev();
            for (auto iterA = prevector.begin(); iterA != prevector.end();
                 iterA++)
            {
                for (auto iterB = iterA + 1; iterB != prevector.end(); iterB++)
                {
                    if(compFunc(*iterB,*iterA))
                    {
                        prevector.erase(iterB);
                        iterB--;
                    }
                }
            }
            return prevector;
        };
        return *this;
    }

    Stream sorted(){
        auto prev = clousre;
        this -> clousre = [prev] () {
            auto prevector = prev();
            sort(prevector.begin(),prevector.end(),[](const T* f,const T* l){
                return *f < *l;
            });
            return prevector;
        };
        return *this;
    }

    Stream sorted(const function<bool(const T*,const T*)> sortFunc){
        auto prev = clousre;
        this->clousre = [prev,sortFunc]() {
            auto prevector = prev();
            std::sort(prevector.begin(), prevector.end(), sortFunc);
            return prevector;
        };
        return *this;
    }



};




#endif //STREAM_STREAM_H
