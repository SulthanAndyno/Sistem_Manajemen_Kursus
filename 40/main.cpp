#include <iostream>
#include <string>
using namespace std;

//struct
struct Kursus {
    string idKursus;
    string namaKursus;
    int durasiJam;
    bool sertifikat;
};

struct Peserta {
    string idPeserta;
    string nama;
    char jenisKelamin;
    int saldo;
};

struct elmChild;
struct elmRelasi;

struct elmParent {
    Kursus info;
    elmParent *next;
    elmRelasi *childList; 
};

struct elmChild {
    Peserta info;
    elmChild *next;
};

struct elmRelasi {
    elmChild *child;
    elmRelasi *next;
};

struct ListParent { elmParent *first; };
struct ListChild  { elmChild  *first;  };

void createListParent(ListParent &L){ L.first = nullptr; }
void createListChild(ListChild &L){ L.first = nullptr; }

elmParent* findParentNode(ListParent &L, const string &id){
    elmParent *p = L.first;
    while(p){
        if(p->info.idKursus == id) return p;
        p = p->next;
    }
    return nullptr;
}

elmChild* findChildNode(ListChild &L, const string &id){
    elmChild *c = L.first;
    while(c){
        if(c->info.idPeserta == id) return c;
        c = c->next;
    }
    return nullptr;
}


elmParent* newParent(const Kursus &x){
    elmParent *p = new elmParent;
    p->info = x;
    p->next = nullptr;
    p->childList = nullptr;
    return p;
}

elmChild* newChild(const Peserta &x){
    elmChild *c = new elmChild;
    c->info = x;
    c->next = nullptr;
    return c;
}

elmRelasi* newRelasi(elmChild *child){
    elmRelasi *r = new elmRelasi;
    r->child = child;
    r->next = nullptr;
    return r;
}

//insert functions
void insertParent(ListParent &L, const Kursus &x){
    elmParent *p = newParent(x);
    if(L.first == nullptr){
        L.first = p;
    } else {
        elmParent *q = L.first;
        while(q->next != nullptr) q = q->next;
        q->next = p;
    }
}

void insertChild(ListChild &L, const Peserta &x){
    elmChild *c = newChild(x);
    if(L.first == nullptr){
        L.first = c;
    } else {
        elmChild *q = L.first;
        while(q->next != nullptr) q = q->next;
        q->next = c;
    }
}

void insertRelasi(ListParent &LP, ListChild &LC, const string &idK, const string &idP){
    elmParent *p = findParentNode(LP, idK);
    elmChild *c = findChildNode(LC, idP);
    if(p && c){
        elmRelasi *r = newRelasi(c);
        if(p->childList == nullptr){
            p->childList = r;
        } else {
            elmRelasi *q = p->childList;
            while(q->next != nullptr) q = q->next;
            q->next = r;
        }
    }
}

//delete functions
void deleteRelasiList(elmRelasi *r){
    while(r){
        elmRelasi *temp = r;
        r = r->next;
        delete temp;
    }
}

void deleteParent(ListParent &L, const string &id){
    elmParent *p = L.first;
    elmParent *prev = nullptr;

    while(p && p->info.idKursus != id){
        prev = p;
        p = p->next;
    }
    if(p){
        deleteRelasiList(p->childList);

        if(prev == nullptr) L.first = p->next;
        else prev->next = p->next;

        delete p;
    }
}

void removeChildFromParent(elmRelasi* &head, elmChild *target){
    elmRelasi *r = head, *prev = nullptr;

    while(r){
        if(r->child == target){
            elmRelasi *del = r;

            if(prev == nullptr) head = r->next;
            else prev->next = r->next;

            r = r->next;
            delete del;
        } else {
            prev = r;
            r = r->next;
        }
    }
}

void deleteChild(ListParent &LP, ListChild &LC, const string &id){
    elmChild *c = LC.first;
    elmChild *prev = nullptr;

    while(c && c->info.idPeserta != id){
        prev = c;
        c = c->next;
    }
    if(c){
        elmParent *p = LP.first;
        while(p){
            removeChildFromParent(p->childList, c);
            p = p->next;
        }

        if(prev == nullptr) LC.first = c->next;
        else prev->next = c->next;

        delete c;
    }
}

void deleteRelasi(ListParent &LP, const string &idK, const string &idP){
    elmParent *p = findParentNode(LP, idK);
    if(!p) return;

    elmRelasi *r = p->childList;
    elmRelasi *prev = nullptr;

    while(r && r->child->info.idPeserta != idP){
        prev = r;
        r = r->next;
    }

    if(r){
        if(prev == nullptr) p->childList = r->next;
        else prev->next = r->next;
        delete r;
    }
}

//show functions
void showAllParent(ListParent &L){
    elmParent *p = L.first;
    cout << "\n=== LIST KURSUS ===\n";
    if(!p) cout << "(Kosong)\n";
    while(p){
        cout << p->info.idKursus << " - " << p->info.namaKursus << endl;
        p = p->next;
    }
}

void showAllChild(ListChild &L){
    elmChild *c = L.first;
    cout << "\n=== LIST PESERTA ===\n";
    if(!c) cout << "(Kosong)\n";
    while(c){
        cout << c->info.idPeserta << " - " << c->info.nama << endl;
        c = c->next;
    }
}

//opsi menu 
void printMenu(){
    cout << "\n===== MENU PROGRESS 1 =====\n";
    cout << "1  - Insert Kursus (parent)\n";
    cout << "2  - Insert Peserta (child)\n";
    cout << "3  - Insert Relasi\n";
    cout << "4  - Delete Kursus\n";
    cout << "5  - Delete Peserta\n";
    cout << "6  - Delete Relasi\n";
    cout << "7  - Show all Kursus\n";
    cout << "8  - Show all Peserta\n";
    cout << "0  - Exit\n";
    cout << "Pilih opsi: ";
}

//main program
int main(){
    ListParent LP; 
    ListChild LC;
    createListParent(LP);
    createListChild(LC);

    int opt;
    while(true){
        printMenu();
        cout << "\nInput: ";
        cin >> opt;
        cin.ignore();

        if(opt == 0) break;

        if(opt == 1){
            Kursus k;
            cout << "ID Kursus: "; getline(cin, k.idKursus);
            cout << "Nama Kursus: "; getline(cin, k.namaKursus);
            cout << "Durasi (jam): "; cin >> k.durasiJam; cin.ignore();
            cout << "Sertifikat? (y/n): ";
            char s; cin >> s; cin.ignore();
            k.sertifikat = (s=='y' || s=='Y');
            insertParent(LP, k);
        }

        else if(opt == 2){
            Peserta p;
            cout << "ID Peserta: "; getline(cin, p.idPeserta);
            cout << "Nama Peserta: "; getline(cin, p.nama);
            cout << "Jenis Kelamin (L/P): "; cin >> p.jenisKelamin; cin.ignore();
            cout << "Saldo: "; cin >> p.saldo; cin.ignore();
            insertChild(LC, p);
        }

        else if(opt == 3){
            string idK, idP;
            cout << "ID Kursus: "; getline(cin, idK);
            cout << "ID Peserta: "; getline(cin, idP);
            insertRelasi(LP, LC, idK, idP);
        }

        else if(opt == 4){
            string id;
            cout << "ID Kursus yang dihapus: "; getline(cin, id);
            deleteParent(LP, id);
        }

        else if(opt == 5){
            string id;
            cout << "ID Peserta yang dihapus: "; getline(cin, id);
            deleteChild(LP, LC, id);
        }

        else if(opt == 6){
            string idK, idP;
            cout << "ID Kursus: "; getline(cin, idK);
            cout << "ID Peserta: "; getline(cin, idP);
            deleteRelasi(LP, idK, idP);
        }

        else if(opt == 7){
            showAllParent(LP);
        }

        else if(opt == 8){
            showAllChild(LC);
        }

        else {
            cout << "Opsi tidak valid!\n";
        }
    }

    cout << "\nProgram selesai.\n";
    return 0;
}
