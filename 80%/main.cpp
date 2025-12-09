// Progress 2 (~80%) - Sistem Manajemen Kursus & Peserta (Multi Linked List - TIPE A)
// - Parent: Single Linked List (SLL)
// - Child : Double Linked List (DLL)


#include <iostream>
#include <string>
using namespace std;


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
    int saldo; // INT sesuai preferensi
};

struct elmChild;
struct elmRelasi;

struct elmParent {
    Kursus info;
    elmParent *next;      // SLL for parents
    elmRelasi *childList; // head of relasi list (each relasi points to an elmChild)
};

struct elmChild {
    Peserta info;
    elmChild *next;
    elmChild *prev;       // DLL for children
};

struct elmRelasi {
    elmChild *child;      // pointer to child node in ListChild
    elmRelasi *next;      // next relasi in this parent's relasi list
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

bool isChildRelasiPresent(elmParent *parentNode, elmChild *childNode){
    elmRelasi *r = parentNode->childList;
    while(r){
        if(r->child == childNode) return true;
        r = r->next;
    }
    return false;
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
    c->prev = nullptr;
    return c;
}

elmRelasi* newRelasi(elmChild *c){
    elmRelasi *r = new elmRelasi;
    r->child = c;
    r->next = nullptr;
    return r;
}


void deleteRelasiList(elmRelasi *r){
    while(r){
        elmRelasi *tmp = r;
        r = r->next;
        delete tmp;
    }
}

//INSERT FUNCTIONS (Progress 1: a,b,c)
//- a: insertParent
//- b: insertChild (DLL)
//- c: insertRelasi

void insertParent(ListParent &L, const Kursus &x){
    if(findParentNode(L, x.idKursus)){
        cout << "[Error] Kursus sudah ada.\n";
        return;
    }
    elmParent *p = newParent(x);
    if(!L.first) L.first = p;
    else{
        elmParent *q = L.first;
        while(q->next) q = q->next;
        q->next = p;
    }
    cout << "[OK] Kursus ditambahkan: " << x.idKursus << " - " << x.namaKursus << "\n";
}

void insertChild(ListChild &L, const Peserta &x){
    if(findChildNode(L, x.idPeserta)){
        cout << "[Error] Peserta sudah ada.\n";
        return;
    }
    elmChild *c = newChild(x);
    if(!L.first){
        L.first = c;
    } else {
        elmChild *q = L.first;
        while(q->next) q = q->next;
        q->next = c;
        c->prev = q; // DLL backward link
    }
    cout << "[OK] Peserta ditambahkan: " << x.idPeserta << " - " << x.nama << "\n";
}

void insertRelasi(ListParent &LP, ListChild &LC, const string &idK, const string &idP){
    elmParent *p = findParentNode(LP, idK);
    if(!p){ cout << "[Error] Kursus tidak ditemukan.\n"; return; }
    elmChild *c = findChildNode(LC, idP);
    if(!c){ cout << "[Error] Peserta tidak ditemukan.\n"; return; }
    if(isChildRelasiPresent(p, c)){ cout << "[Info] Relasi sudah ada.\n"; return; }
    elmRelasi *r = newRelasi(c);
    if(!p->childList) p->childList = r;
    else{
        elmRelasi *q = p->childList;
        while(q->next) q = q->next;
        q->next = r;
    }
    cout << "[OK] Relasi ditambahkan: Peserta " << idP << " -> Kursus " << idK << "\n";
}


// DELETE FUNCTIONS (Progress 1: d,e,f)
// - d: deleteParent
// - e: deleteChild (DLL-aware)
// - f: deleteRelasi


void removeRelasiPointing(elmRelasi* &head, elmChild *target){
    elmRelasi *r = head, *prev = nullptr;
    while(r){
        if(r->child == target){
            elmRelasi *del = r;
            if(!prev) head = r->next;
            else prev->next = r->next;
            r = r->next;
            delete del;
        } else {
            prev = r;
            r = r->next;
        }
    }
}

void deleteParent(ListParent &L, const string &id){
    if(!L.first){ cout << "[Error] Tidak ada kursus.\n"; return; }
    elmParent *p = L.first, *prev = nullptr;
    while(p && p->info.idKursus != id){
        prev = p;
        p = p->next;
    }
    if(!p){ cout << "[Error] Kursus tidak ditemukan.\n"; return; }
    if(p->childList) deleteRelasiList(p->childList);
    if(!prev) L.first = p->next;
    else prev->next = p->next;
    delete p;
    cout << "[OK] Kursus " << id << " dihapus.\n";
}

void deleteChild(ListParent &LP, ListChild &LC, const string &id){
    if(!LC.first){ cout << "[Error] Tidak ada peserta.\n"; return; }
    elmChild *c = LC.first;
    while(c && c->info.idPeserta != id) c = c->next;
    if(!c){ cout << "[Error] Peserta tidak ditemukan.\n"; return; }

    // Hapus semua relasi yang menunjuk ke child ini
    elmParent *p = LP.first;
    while(p){
        removeRelasiPointing(p->childList, c);
        p = p->next;
    }

    // Hapus dari DLL
    if(c->prev) c->prev->next = c->next;
    else LC.first = c->next;
    if(c->next) c->next->prev = c->prev;

    delete c;
    cout << "[OK] Peserta " << id << " dihapus beserta relasinya.\n";
}

void deleteRelasi(ListParent &LP, const string &idK, const string &idP){
    elmParent *p = findParentNode(LP, idK);
    if(!p){ cout << "[Error] Kursus tidak ditemukan.\n"; return; }
    if(!p->childList){ cout << "[Error] Tidak ada relasi pada kursus itu.\n"; return; }
    elmRelasi *r = p->childList, *prev = nullptr;
    while(r && r->child->info.idPeserta != idP){
        prev = r; r = r->next;
    }
    if(!r){ cout << "[Error] Relasi tidak ditemukan.\n"; return; }
    if(!prev) p->childList = r->next;
    else prev->next = r->next;
    delete r;
    cout << "[OK] Relasi Peserta " << idP << " dari Kursus " << idK << " dihapus.\n";
}


//FIND FUNCTIONS (Progress 2: g,h,i)
//- g: findParent
//- h: findChild
//- i: findRelasi


bool findParent(ListParent &L, const string &id){ return findParentNode(L,id) != nullptr; }
bool findChild(ListChild &L, const string &id){ return findChildNode(L,id) != nullptr; }

bool findRelasi(ListParent &LP, const string &idK, const string &idP){
    elmParent *p = findParentNode(LP, idK);
    if(!p) return false;
    elmRelasi *r = p->childList;
    while(r){
        if(r->child->info.idPeserta == idP) return true;
        r = r->next;
    }
    return false;
}


// SHOW FUNCTIONS (Progress 1+2: j,k,l,m,n)
// - j: showAllParent
// - k: showAllChild
// - l: showChildByParent
// - m: showParentWithChildren
// - n: showChildWithParents


void showAllParent(ListParent &LP){
    cout << "\n=== LIST KURSUS ===\n";
    elmParent *p = LP.first;
    if(!p){ cout << "(Kosong)\n"; return; }
    while(p){
        cout << p->info.idKursus << " | " << p->info.namaKursus << " | " 
             << p->info.durasiJam << " jam | Sertifikat: " << (p->info.sertifikat ? "Ya":"Tidak") << "\n";
        p = p->next;
    }
}

void showAllChild(ListChild &LC){
    cout << "\n=== LIST PESERTA ===\n";
    elmChild *c = LC.first;
    if(!c){ cout << "(Kosong)\n"; return; }
    while(c){
        cout << c->info.idPeserta << " | " << c->info.nama << " | JK:" << c->info.jenisKelamin 
             << " | Saldo: " << c->info.saldo << "\n";
        c = c->next;
    }
}

void showChildByParent(ListParent &LP, const string &idK){
    elmParent *p = findParentNode(LP, idK);
    if(!p){ cout << "[Error] Kursus tidak ditemukan.\n"; return; }
    cout << "\nPeserta pada kursus " << p->info.namaKursus << " (" << p->info.idKursus << "):\n";
    elmRelasi *r = p->childList;
    if(!r){ cout << " - (Belum ada peserta)\n"; return; }
    while(r){
        cout << " * " << r->child->info.idPeserta << " - " << r->child->info.nama << "\n";
        r = r->next;
    }
}

void showParentWithChildren(ListParent &LP){
    cout << "\n=== PARENT BESERTA CHILD (Kursus -> Peserta) ===\n";
    elmParent *p = LP.first;
    if(!p){ cout << "(Tidak ada kursus)\n"; return; }
    while(p){
        cout << p->info.idKursus << " : " << p->info.namaKursus << "\n";
        elmRelasi *r = p->childList;
        if(!r) cout << "  - (Tidak ada peserta)\n";
        while(r){
            cout << "    * " << r->child->info.idPeserta << " - " << r->child->info.nama << "\n";
            r = r->next;
        }
        p = p->next;
    }
}

void showChildWithParents(ListParent &LP, ListChild &LC){
    cout << "\n=== CHILD BESERTA PARENT (Peserta -> Kursus) ===\n";
    elmChild *c = LC.first;
    if(!c){ cout << "(Tidak ada peserta)\n"; return; }
    while(c){
        cout << c->info.idPeserta << " : " << c->info.nama << "\n";
        bool any = false;
        elmParent *p = LP.first;
        while(p){
            elmRelasi *r = p->childList;
            while(r){
                if(r->child == c){
                    cout << "    - " << p->info.idKursus << " : " << p->info.namaKursus << "\n";
                    any = true;
                    break;
                }
                r = r->next;
            }
            p = p->next;
        }
        if(!any) cout << "    - (Tidak terdaftar di kursus mana pun)\n";
        c = c->next;
    }
}


//COUNT FUNCTIONS
//- p: countRelasiPerParent
//- q: countRelasiOfChild
//- r: countChildWithoutRelasi


void countRelasiPerParent(ListParent &LP){
    cout << "\n=== Jumlah relasi per parent (Kursus) ===\n";
    elmParent *p = LP.first;
    if(!p){ cout << "(Tidak ada kursus)\n"; return; }
    while(p){
        int cnt = 0;
        elmRelasi *r = p->childList;
        while(r){ cnt++; r = r->next; }
        cout << p->info.idKursus << " -> " << cnt << " peserta\n";
        p = p->next;
    }
}

int countRelasiOfChild(ListParent &LP, const string &idP){
    int cnt = 0;
    elmParent *p = LP.first;
    while(p){
        elmRelasi *r = p->childList;
        while(r){
            if(r->child->info.idPeserta == idP) cnt++;
            r = r->next;
        }
        p = p->next;
    }
    return cnt;
}

int countChildWithoutRelasi(ListParent &LP, ListChild &LC){
    int cnt = 0;
    elmChild *c = LC.first;
    while(c){
        bool has = false;
        elmParent *p = LP.first;
        while(p && !has){
            elmRelasi *r = p->childList;
            while(r){
                if(r->child == c){ has = true; break; }
                r = r->next;
            }
            p = p->next;
        }
        if(!has) cnt++;
        c = c->next;
    }
    return cnt;
}


void clearAll(ListParent &LP, ListChild &LC){
    // delete parents and their relasi lists
    elmParent *p = LP.first;
    while(p){
        if(p->childList) deleteRelasiList(p->childList);
        elmParent *tmp = p;
        p = p->next;
        delete tmp;
    }
    LP.first = nullptr;
    // delete children (DLL)
    elmChild *c = LC.first;
    while(c){
        elmChild *tmp = c; c = c->next; delete tmp;
    }
    LC.first = nullptr;
}


void printMenu(){
    cout << "\n===== SISTEM MLL: Kursus & Peserta (Tipe A) - Progress 2 =====\n";
    cout << "1  - Insert Kursus (parent)\n";
    cout << "2  - Insert Peserta (child)\n";
    cout << "3  - Insert Relasi\n";
    cout << "4  - Delete Kursus\n";
    cout << "5  - Delete Peserta\n";
    cout << "6  - Delete Relasi\n";
    cout << "7  - Find Kursus\n";
    cout << "8  - Find Peserta\n";
    cout << "9  - Find Relasi\n";
    cout << "10 - Show all Kursus\n";
    cout << "11 - Show all Peserta\n";
    cout << "12 - Show Peserta dari Kursus (l)\n";
    cout << "13 - Show Parent beserta Child (m)\n";
    cout << "14 - Show Child beserta Parent (n)\n";
    cout << "15 - Show Parent yang berelasi dengan Peserta tertentu (o) [Belum di Progress 2]\n";
    cout << "16 - Count relasi per Parent (p)\n";
    cout << "17 - Count relasi of a Child (q)\n";
    cout << "18 - Count Child tanpa relasi (r)\n";
    cout << "19 - Edit Relasi (ganti child pada suatu kursus) (s) [Belum di Progress 2]\n";
    cout << "20 - Clear All Data\n";
    cout << "0  - Exit\n";
    cout << "Pilih opsi: ";
}


//main
int main(){
    ListParent LP; ListChild LC;
    createListParent(LP); createListChild(LC);

    int opt = -1;
    while(true){
        printMenu();
        if(!(cin >> opt)){ cin.clear(); cin.ignore(1000,'\n'); cout << "Input invalid\n"; continue; }
        cin.ignore(1000,'\n');

        switch(opt){
            case 0:
                clearAll(LP, LC);
                cout << "Keluar. Data dibersihkan.\n";
                return 0;

            case 1: {
                Kursus k;
                cout << "ID Kursus: "; getline(cin, k.idKursus);
                cout << "Nama Kursus: "; getline(cin, k.namaKursus);
                cout << "Durasi (jam): "; cin >> k.durasiJam; cin.ignore();
                cout << "Sertifikat? (y/n): ";
                char s; cin >> s; cin.ignore();
                k.sertifikat = (s=='y' || s=='Y');
                insertParent(LP, k);
                break;
            }

            case 2: {
                Peserta p;
                cout << "ID Peserta: "; getline(cin, p.idPeserta);
                cout << "Nama Peserta: "; getline(cin, p.nama);
                cout << "Jenis Kelamin (L/P): "; cin >> p.jenisKelamin; cin.ignore();
                cout << "Saldo: "; cin >> p.saldo; cin.ignore();
                insertChild(LC, p);
                break;
            }

            case 3: {
                string idK, idP;
                cout << "ID Kursus: "; getline(cin, idK);
                cout << "ID Peserta: "; getline(cin, idP);
                insertRelasi(LP, LC, idK, idP);
                break;
            }

            case 4: {
                string id; cout << "ID Kursus to delete: "; getline(cin, id);
                deleteParent(LP, id);
                break;
            }

            case 5: {
                string id; cout << "ID Peserta to delete: "; getline(cin, id);
                deleteChild(LP, LC, id);
                break;
            }

            case 6: {
                string idK, idP;
                cout << "ID Kursus: "; getline(cin, idK);
                cout << "ID Peserta: "; getline(cin, idP);
                deleteRelasi(LP, idK, idP);
                break;
            }

            case 7: {
                string id; cout << "ID Kursus to find: "; getline(cin, id);
                cout << (findParent(LP,id) ? "Ditemukan\n" : "Tidak ditemukan\n");
                break;
            }

            case 8: {
                string id; cout << "ID Peserta to find: "; getline(cin, id);
                cout << (findChild(LC,id) ? "Ditemukan\n" : "Tidak ditemukan\n");
                break;
            }

            case 9: {
                string idK, idP;
                cout << "ID Kursus: "; getline(cin, idK);
                cout << "ID Peserta: "; getline(cin, idP);
                cout << (findRelasi(LP,idK,idP) ? "Relasi ada\n" : "Relasi tidak ada\n");
                break;
            }

            case 10:
                showAllParent(LP);
                break;

            case 11:
                showAllChild(LC);
                break;

            case 12: {
                string id; cout << "ID Kursus: "; getline(cin, id);
                showChildByParent(LP, id);
                break;
            }

            case 13:
                showParentWithChildren(LP);
                break;

            case 14:
                showChildWithParents(LP, LC);
                break;

            case 15:
                cout << "[Info] Fungsi ini belum diimplementasikan di Progress 2.\n";
                break;

            case 16:
                countRelasiPerParent(LP);
                break;

            case 17: {
                string id; cout << "ID Peserta: "; getline(cin, id);
                cout << "Jumlah relasi: " << countRelasiOfChild(LP, id) << "\n";
                break;
            }

            case 18:
                cout << "Jumlah peserta tanpa relasi: " << countChildWithoutRelasi(LP, LC) << "\n";
                break;

            case 19:
                cout << "[Info] Fungsi edit relasi belum diimplementasikan di Progress 2.\n";
                break;

            case 20:
                clearAll(LP, LC);
                createListParent(LP); createListChild(LC);
                cout << "Semua data direset.\n";
                break;

            default:
                cout << "Opsi tidak valid.\n";
        }
    }

    return 0;
}
