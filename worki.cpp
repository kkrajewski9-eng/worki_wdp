// Zlozonosc: O(1) dla kazdej operacji

#include "worki.h"
#include <stdlib.h>
#include <assert.h>
#include <vector>

using namespace std;

// Wektory pozwalaja trzymac wskazniki do alokowanej pamieci
static vector<przedmiot*> wszystkie_przedmioty;
static vector<worek*> wszystkie_worki;
static vector<worek*> grupy;

// Grupy - Struktura pomocnicza trzymajaca pod kazdym indeksem 
// zbior przedmiotow i workow nalezacych do worka wskazywanego
// przez grupy[i] -> par. Ulatwia operacje na_odwrot

worek* stworz_grupe(worek* w){
    worek* g = new worek();
    g -> par = w;
    g -> ctr = 0;
    w -> grupa = g;
    grupy.push_back(g);
    return g;
}

// Zakladamy ze biurko tez jest workiem, wiec tworzymy go na poczatku programu
void init(){
    worek* w = new worek();
    w -> par = nullptr;
    w -> ctr = 0;
    w -> num = -1;
    w -> grupa = stworz_grupe(w);
    wszystkie_worki.push_back(w);
}

przedmiot *nowy_przedmiot(){
    if(wszystkie_worki.empty()) init();

    przedmiot* p = new przedmiot(); 
    if(!wszystkie_worki.empty()){
        p -> par = wszystkie_worki[0] -> grupa;
        wszystkie_worki[0] -> grupa -> ctr++;
    }
    wszystkie_przedmioty.push_back(p);
    return p;
}

worek *nowy_worek(){
    if(wszystkie_worki.empty()) init();

    worek* w = new worek();
    if(!wszystkie_worki.empty()){
        w -> par = wszystkie_worki[0] -> grupa;
        w -> ctr = 0;
        w -> num = (int)(wszystkie_worki.size() - 1);
        w -> grupa = stworz_grupe(w);
    }
    wszystkie_worki.push_back(w);
    return w;
}

// Przepinam wskaznik przedmiotu z grupy wskazujacej na biurko 
// na grupe wskazujaca moj worek dodajac jeden do licznika przedmiotow w tej grupie
void wloz(przedmiot *co, worek *gdzie){
    co -> par = gdzie -> grupa;
    gdzie -> grupa -> ctr++;
    return;
}

// Przepinam wskaznik worka z grupy wskazujacej na biurko 
// na grupe ktora wskazuje worek do ktorego wkladam moj worek
void wloz(worek *co, worek *gdzie){
    co -> par = gdzie -> grupa;
    gdzie -> grupa -> ctr += co -> grupa -> ctr;
    return;
}

// Zmniejszam licznik przedmiotow w grupie z ktorej wyjmuje przedmiot
// oraz przepinam wskaznik przedmiotu na grupe biurka
void wyjmij(przedmiot *p){
    p -> par -> ctr--;
    if(!wszystkie_worki.empty()) p -> par = wszystkie_worki[0] -> grupa;
    return;
}

void wyjmij(worek *w){
    // Zmniejszam licznik przedmiotow w grupie z ktorej wyjmuje worek
    // oraz przepinam wskaznik worka na grupe biurka
    w -> par -> ctr -= w -> grupa -> ctr;   // bylo w -> par -> grupa -> ctr   i w wyjmij() tez
    if(!wszystkie_worki.empty()) w -> par = wszystkie_worki[0] -> grupa;
    return;
}

int w_ktorym_worku(przedmiot *p){
    // Zwracam numer worka na ktory patrzy grupa w ktorej jest p
    return p -> par -> par -> num;
}

int w_ktorym_worku(worek *w){
    // Zwracam numer worka na ktory patrzy grupa w ktorej jest w
    return w -> par -> par -> num;
}

int ile_przedmiotow(worek *w){
    // Zwracam licznik przedmiotow w grupie worka w
    return w -> grupa -> ctr;
}

void na_odwrot(worek *w){
    
    // Zmniejszam licznik przedmiotow w grupie biurka o liczbe przedmiotow
    // w grupie worka w
    if(!wszystkie_worki.empty()){ 
        wszystkie_worki[0] -> grupa -> ctr = 
        max(0, wszystkie_worki[0] -> grupa -> ctr - w -> grupa -> ctr);

        // Zamieniam grupy miejscami
        w -> grupa -> par = wszystkie_worki[0];
        swap(w -> par, w -> grupa);
        w -> grupa -> par = w;
        wszystkie_worki[0] -> grupa = w -> par;

        // Ustawiam licznik przedmiotow w grupie biurka na liczbe wszystkich przedmiotow
        wszystkie_worki[0] -> grupa -> ctr = (int)(wszystkie_przedmioty.size());
    }
    return;
}

// Kończy i zwalnia pamięć
void gotowe(){
    for(worek* it : wszystkie_worki) delete it;
    for(przedmiot* it : wszystkie_przedmioty) delete it;
    for(worek* it : grupy) delete it;
    wszystkie_przedmioty.clear();
    wszystkie_worki.clear();
    grupy.clear();
    return;
}
